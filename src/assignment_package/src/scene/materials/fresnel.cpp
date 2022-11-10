#include "fresnel.h"

Color3f FresnelDielectric::Evaluate(float cosThetaI) const
{
    //TODO

    cosThetaI = glm::clamp(cosThetaI, -1.f, 1.f);

        // Potentially swap indices of refraction.
        float I = etaI;
        float T = etaT;

        if(cosThetaI <= 0.f)
        {
            float temp=I;
            I=T;
            T=temp;
            cosThetaI = std::abs(cosThetaI);
        }


        float s = I / T * std::sqrt(std::max(0.f, 1.f
                           - cosThetaI * cosThetaI));

        // Handle total internal reflection.
        if(s >= 1.f)
        {
           return Color3f(1.f);
        }

        float cosThetaT = std::sqrt(std::max(0.f, 1.f - s * s));

        float multTI=T * cosThetaI;
        float multII=I * cosThetaI;
        float multTT=T * cosThetaT;
        float multIT=I * cosThetaT;
        float l = ((multTI) - (multIT)) / ((multTI) + (multIT));
        float p = ((multII) - (multTT)) / ((multII) + (multTT));

        return Color3f((l *l + p * p) / 2.f);
}


