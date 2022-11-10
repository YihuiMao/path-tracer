#include "fulllightingintegrator.h"
#include "directlightingintegrator.h"




Color3f DirectLight(const Ray &ray, const Scene &scene, std::shared_ptr<Sampler> sampler)
{
    Color3f L(0.f);

    Intersection T;
    if(!scene.Intersect(ray, &T))
    {
        return L;
    }

    Color3f ls = T.Le(- ray.direction);
    if(!IsBlack(ls))
    {
        L += ls;
        return L;
    }

    T.ProduceBSDF();
    Normal3f nor = T.bsdf->normal;

    int numL = int(scene.lights.size());

    if (numL == 0) {
        return Color3f(0.f);
    }
    int t = sampler->Get1D() * numL;
    int n = std::min(t, numL - 1);

    Vector3f wi(0.f);
    float pdf = 0.f;
    Color3f Li = scene.lights[n]->Sample_Li(T, sampler->Get2D(), &wi, &pdf);

    Intersection Ist;
    if(!scene.Intersect(Ray(T.point + nor
                            * 0.0001f, wi), &Ist))
    {
        Li = Color3f(0.f);
    }
    else
    {
        const AreaLight* tempAreaLight=Ist.objectHit->GetAreaLight();
        if(scene.lights[n].get() != tempAreaLight)
        {
            Li = Color3f(0.f);
        }
        else
        {
            Color3f f = T.bsdf->f(- ray.direction, wi);
            float mPDF = T.bsdf->Pdf(- ray.direction, wi);
            if(pdf <= 0.f || mPDF <= 0.f)
            {
                Li = Color3f(0.f);
            }
            else
            {

                float w = PowerHeuristic(1, pdf, 1, mPDF);
                float abs=AbsDot(wi, nor);
                Color3f tempL=f * Li * abs * w / (pdf / numL);
                L += tempL;
            }
        }
    }


    float matPDF = 0.f;
    Vector3f mWi(0.f);
    BxDFType type;
    Color3f mF = T.bsdf->Sample_f(- ray.direction, &mWi, sampler->Get2D(), &matPDF, BSDF_ALL, &type);
    if(!IsBlack(mF) && matPDF != 0.f)
    {
        Ray tempRay = T.SpawnRay(mWi);
        Intersection insectBRDF;
        if(scene.Intersect(tempRay, &insectBRDF))
        {
            const AreaLight* tempAreaLight1=insectBRDF.objectHit->GetAreaLight();
            if( scene.lights[n].get()== tempAreaLight1)
            {
                float tmppdf = tempAreaLight1->Pdf_Li(T, mWi);
                Color3f tempLi=mF * Li * AbsDot(mWi, nor)
                                  * PowerHeuristic(1, matPDF, 1, tmppdf) / (tmppdf / numL);
                L += tempLi;
            }
        }
    }
    return L;
}

Color3f FullLightingIntegrator::Li(const Ray &ray, const Scene &scene, std::shared_ptr<Sampler> sampler, int depth) const
{

    Color3f L(0.f);
    Color3f Bta(1.f);
    Ray rayGlob(ray);
    bool SpecFlag = false;
     for(int numBounce =depth; numBounce>0;--numBounce)
    {
        Intersection isect;
        if(!scene.Intersect(rayGlob, &isect))
        {
            break;
        }

        if(isect.objectHit->GetAreaLight())
        {
            if(numBounce == depth || SpecFlag)
            {
                L += Bta * isect.Le(- rayGlob.direction);

            }
            break;
        }

        isect.ProduceBSDF();
        SpecFlag = false;
        float S_pdf;
        Vector3f S_wi;
        BxDFType S_type;
        Color3f fSpec = isect.bsdf->Sample_f(- rayGlob.direction, &S_wi, sampler->Get2D(), &S_pdf, BSDF_ALL, &S_type);
        if(S_type & BxDFType::BSDF_SPECULAR)
        {
            float a=AbsDot(S_wi, isect.bsdf->normal);
            Color3f t=fSpec* a / S_pdf;
            Bta *= t;
            SpecFlag = true;
            rayGlob = isect.SpawnRay(S_wi);
            --numBounce;
            continue;
        }

        Color3f temp_L=Bta * DirectLight(rayGlob, scene, sampler);
        L += temp_L;

        float pdf_Glob;
        BxDFType typeGlob;
        Vector3f wi_Glob;

        Color3f t_G = isect.bsdf->Sample_f(- rayGlob.direction, &wi_Glob, sampler->Get2D(), &pdf_Glob, BSDF_ALL, &typeGlob);
        if(IsBlack(t_G) || pdf_Glob == 0.f)
        {
            break;
        }
        float temp_dot=AbsDot(wi_Glob, isect.bsdf->normal);
        Color3f temp_G=t_G * temp_dot  / pdf_Glob;
        Bta *= temp_G;
        rayGlob = isect.SpawnRay(wi_Glob);

        float max = Bta[0];
        for(int i = 1; i < 3; i++)
        {
            if(Bta[i] > max)
            {
                max = Bta[i];
            }
        }
        if(max < (1.f - sampler->Get1D()))
        {
            break;
        }
        else
        {
           float t_v= 1.f/max;
            Bta *= t_v;
        }

    }
    return L;
}
