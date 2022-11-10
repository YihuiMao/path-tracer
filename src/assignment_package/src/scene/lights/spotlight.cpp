
#include "spotlight.h"
#include <math.h>
float SpotLight::Fall_off(const Vector3f &w) const
{
    Vector3f t = glm::normalize(transform.invWT() * w);
    if (t.z < cTW)
        return 0;
    if (t.z >= cFS)
        return 1;
    return powf((t.z - cTW) / (cFS - cTW),4.f);
}


Color3f SpotLight::Sample_Li(const Intersection &ref, const Point2f &xi,
                                     Vector3f *wi, Float *pdf) const
{
    *wi = glm::normalize(transform.position() - ref.point);
    *pdf = 1.f;
    Color3f t=emitL * Fall_off(-*wi);
    float l=glm::length2(transform.position()-ref.point);
    return t / l;
}





