#include "directlightingintegrator.h"
#include<cmath>
#include <scene/lights/pointlight.h>
#include <scene/lights/spotlight.h>

float BalanceHeuristic(int nf, Float fPdf, int ng, Float gPdf)
{
    float a = float(nf) * fPdf;
    float b = float(nf) * fPdf + float(ng) * gPdf;
    return a / b;
}

float PowerHeuristic(int nf, Float fPdf, int ng, Float gPdf)
{
    float f_2 = pow(float(nf * fPdf),2.0f);
    float g_2 = pow(float(ng * gPdf),2.0f);

    return f_2 / (f_2 + g_2);
}
 float AbsDot1( Vector3f& a, Vector3f& b)
{
    return glm::abs(glm::dot(a, b));
}

Color3f DirectLightingIntegrator::Li(const Ray &ray, const Scene &scene, std::shared_ptr<Sampler> sampler, int depth) const
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

        int numL = int(scene.lights.size());

        if (numL == 0) {
            return Color3f(0.f);
        }
        int t = sampler->Get1D() * numL;
        int n = std::min(t, numL - 1);
        Ray R(Vector3f(0), Vector3f(0));

         const std::shared_ptr<Light> &light = scene.lights[n];
        Vector3f wi(0.f);
        float pdf = 0.f;
        Color3f Li = scene.lights[n]->Sample_Li(T, sampler->Get2D(), &wi, &pdf);



           if(dynamic_cast<PointLight*>(light.get())||dynamic_cast<SpotLight*>(light.get()))
           {


               if(dynamic_cast<PointLight*>(light.get())||dynamic_cast<SpotLight*>(light.get()) )
               {
                   R = Ray(light.get()->transform.position(), -wi);
               }

               Intersection I;
               if(!scene.Intersect(R, &I))
               {
                   Li = Color3f(0.f);
               }
               else
               {


                   if(pdf > 0.f && T.bsdf->Pdf(- ray.direction, wi) > 0.f)
                   {
                       if(glm::length(I.point - T.point) < 0.01)
                       {

                           Color3f temp= T.bsdf->f(- ray.direction, wi) * AbsDot1(wi, T.normalGeometric);
                           L = L + (temp * Li / (pdf / numL));
                       }
                   }
                   else{
                       Li = Color3f(0.f);
                   }
               }
               return L;
           }

        else
        {

        Intersection Ist;
        if(!scene.Intersect(Ray(T.point + T.normalGeometric
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
                    float abs=AbsDot(wi, T.normalGeometric);
                    Color3f tempL=f * Li * abs * w / (pdf / numL);
                    L += tempL;
                }
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
                    Color3f tempLi=mF * Li * AbsDot(mWi, T.normalGeometric)
                                      * PowerHeuristic(1, matPDF, 1, tmppdf) / (tmppdf / numL);
                    L += tempLi;
                }
            }
        }

        return L;
}
