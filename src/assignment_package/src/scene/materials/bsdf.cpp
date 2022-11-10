
#include "bsdf.h"
#include <warpfunctions.h>

BSDF::BSDF(const Intersection& isect, float eta /*= 1*/)
//TODO: Properly set worldToTangent and tangentToWorld
    : worldToTangent(/*COMPUTE ME*/),
      tangentToWorld(/*COMPUTE ME*/),
      normal(isect.normalGeometric),
      eta(eta),
      numBxDFs(0),
      bxdfs{nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr}
{
    tangentToWorld = glm::mat3(isect.tangent, isect.bitangent, isect.normalGeometric);
    worldToTangent = glm::inverse(tangentToWorld);
}


void BSDF::UpdateTangentSpaceMatrices(const Normal3f& n, const Vector3f& t, const Vector3f b)
{
    //TODO: Update worldToTangent and tangentToWorld based on the normal, tangent, and bitangent
}


Color3f BSDF::f(const Vector3f &woW, const Vector3f &wiW, BxDFType flags /*= BSDF_ALL*/) const
{
    //TODO


     float a1 = glm::dot(wiW, normal);
     float a2 = glm::dot(woW, normal);
     bool rf = a1*a2;
     Color3f c(0.f, 0.f, 0.f);
     for(int i = 0; i < numBxDFs; i++)
     {
         if(bxdfs[i]->MatchesFlags(flags) &&
                 ((rf && (bxdfs[i]->type & BSDF_REFLECTION)) ||
                  (!rf && (bxdfs[i]->type & BSDF_TRANSMISSION))))
         {
             c += bxdfs[i]->f( worldToTangent * woW, worldToTangent * wiW);
         }
     }
     return  c;
}

Color3f BSDF::Sample_f(const Vector3f &woW, Vector3f *wiW, const Point2f &xi,
                       float *pdf, BxDFType type, BxDFType *sampledType) const
{
    //TODO
    // Choose which BxDF to sample:

    if(this->BxDFsMatchingFlags(type) == 0)
    {
        *pdf = 0;
        return Color3f(0.f);
    }
    int t1 = (int)std::floor(xi[0] * this->BxDFsMatchingFlags(type));
    int t2 = this->BxDFsMatchingFlags(type) - 1;
    int min= std::min(t1,t2);
    BxDF *b = nullptr;
    int count = min;
    int i=0;

    while(i < numBxDFs){
        if(bxdfs[i]->MatchesFlags(type) && count-- == 0)
        {
            b = bxdfs[i];
            break;
        }
        i++;
     }

    BxDFType B_type=b->type;

    // Sample Chosen BxDF:
    Vector3f wiT, woT = worldToTangent * woW;
    *pdf = 0;
    if(sampledType)
    {
        *sampledType = B_type;
    }
    Color3f f = b->Sample_f(woT, &wiT,
                   glm::vec2(xi[0] *
                   this->BxDFsMatchingFlags(type)
                   - min, xi[1]), pdf, sampledType);

    if(*pdf == 0)
    {
        return Color3f(0.f, 0.f, 0.f);
    }
    *wiW = tangentToWorld * wiT;

    if(!(B_type & BSDF_SPECULAR) && this->BxDFsMatchingFlags(type) > 1)
    {
        for(int i = 0; i < numBxDFs; i++)
        {
            if(bxdfs[i] != b && bxdfs[i]->MatchesFlags(type))
            {
                *pdf += bxdfs[i]->Pdf(woT, wiT);
            }
        }
    }
    if(this->BxDFsMatchingFlags(type) > 1)
    {
        *pdf /= this->BxDFsMatchingFlags(type);
    }


    if(!(B_type & BSDF_SPECULAR) && this->BxDFsMatchingFlags(type) > 1)
    {
        int t1 = glm::dot(*wiW, normal);
        int t2 = glm::dot(woW, normal);
        bool rf;
        if(t1*t2>0){
            rf=true;
        }
        else{
            rf=false;
        }
        f = Color3f(0.f, 0.f, 0.f);
        for(int i = 0; i < numBxDFs; i++)
        {
            if(bxdfs[i]->MatchesFlags(type) &&
                    ((rf && (bxdfs[i]->type & BSDF_REFLECTION)) ||
                    (!rf && (bxdfs[i]->type & BSDF_TRANSMISSION))))
            {
                f += bxdfs[i]->f(woT, wiT);
            }
        }
    }
    return f;
}


float BSDF::Pdf(const Vector3f &woW, const Vector3f &wiW, BxDFType flags) const
{
    //TODO

    if(this->BxDFsMatchingFlags(flags) == 0)
    {
        return 0.f;
    }

    float temp = 0.f;
    int i = 0;

    while(i<numBxDFs){
        if(bxdfs[i]->MatchesFlags(flags))
        {
            temp += bxdfs[i]->Pdf(worldToTangent * woW, worldToTangent * wiW);
        }
        i++;
    }

    return temp;
}




Color3f BxDF::Sample_f(const Vector3f &wo, Vector3f *wi, const Point2f &xi,
                       Float *pdf, BxDFType *sampledType) const
{
    //TODO


    float z = xi.x;
    float x = glm::cos(2.f * glm::pi<float>() * xi.y) * glm::sqrt(1.f - z * z);
    float y = glm::sin(2.f * glm::pi<float>() * xi.y) * glm::sqrt(1.f - z * z);
    *wi = glm::vec3(x, y, z);

    if(wo.z < 0)
    {
        wi->z *= -1;
    }
    *pdf = this->Pdf(wo, *wi);
    return this->f(wo, *wi);
}


float BxDF::Pdf(const Vector3f &wo, const Vector3f &wi) const
{

    return wi.z * InvPi;
}

BSDF::~BSDF()
{
    for(int i = 0; i < numBxDFs; i++)
    {
        delete bxdfs[i];
    }
}
