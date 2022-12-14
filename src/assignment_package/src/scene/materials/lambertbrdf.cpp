#include "lambertbrdf.h"
#include <warpfunctions.h>

Color3f LambertBRDF::f(const Vector3f &wo, const Vector3f &wi) const
{
    //TODO

    return R * InvPi;
}

Color3f LambertBRDF::Sample_f(const Vector3f &wo, Vector3f *wi, const Point2f &u,
                        Float *pdf, BxDFType *sampledType) const
{
    //TODO


    //*wi = WarpFunctions::squareToHemisphereCosine(u);
         *wi = WarpFunctions::squareToHemisphereUniform(u);
        if(wo.z < 0)
        {
            wi->z *= -1;
        }
        *pdf = this->Pdf(wo, *wi);
        return this->f(wo, *wi);
}

float LambertBRDF::Pdf(const Vector3f &wo, const Vector3f &wi) const
{
    //TODO
    if(SameHemisphere(wo, wi)){
        return Inv2Pi;
    }
    else{
        return 0;
    }
}
