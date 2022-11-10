#include "diffusearealight.h"

Color3f DiffuseAreaLight::L(const Intersection &isect, const Vector3f &w) const
{
    //TODO

    if(glm::dot(isect.normalGeometric, w) > 0.f){
      return emittedLight;
    }
    else{
     return Color3f(0.f);
    }
}




Color3f DiffuseAreaLight::Sample_Li(const Intersection &ref, const Point2f &xi,
                                     Vector3f *wi, Float *pdf) const
{

    Point3f p = shape->Sample(ref, xi, pdf).point;


    float l=glm::length(p - ref.point);
    if(*pdf == 0.f || l < FLT_EPSILON)
    {
        return Color3f(0.f);
    }


    *wi = glm::normalize(p - ref.point);

    return this->L(shape->Sample(ref, xi, pdf), -*wi);
}

float DS(const Point3f &p1, const Point3f &p2) {
    return glm::length2(p1 - p2);
}


float DiffuseAreaLight::Pdf_Li(const Intersection &ref, const Vector3f &wi) const{


        Intersection T;
        if(!shape->Intersect(ref.SpawnRay(wi), &T))
        {
            return 0.f;
        }

        else{
            float ds= DS(ref.point, T.point);
            float as=AbsDot(T.normalGeometric, -wi) * shape->Area();
            return ds / as;
        }

}
