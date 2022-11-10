#include "specularbtdf.h"

Color3f SpecularBTDF::f(const Vector3f &wo, const Vector3f &wi) const
{
    return Color3f(0.f);
}


float SpecularBTDF::Pdf(const Vector3f &wo, const Vector3f &wi) const
{
    return 0.f;
}

Color3f SpecularBTDF::Sample_f(const Vector3f &wo, Vector3f *wi, const Point2f &sample, Float *pdf, BxDFType *sampledType) const
{
    //TODO!

    bool entering = CosTheta(wo) > 0;
    Float etaI = entering ? etaA : etaB;
    Float etaT = entering ? etaB : etaA;

      if(!Refract(wo, Faceforward(Normal3f(0.f, 0.f, 1.f), wo), etaI / etaT, wi))
      {
          return Color3f(0.f);
      }

      if(glm::dot(wo, *wi) > 0.f)
      {
          return Color3f(0.f);
      }

      *pdf = 1;
      Color3f ft = T * (Color3f(1.f) - fresnel->Evaluate(std::abs(wi->z)));


      return ft / AbsCosTheta(*wi);
}
