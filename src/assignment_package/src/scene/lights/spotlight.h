
#ifndef SPOTLIGHT_H
#define SPOTLIGHT_H
#include "light.h"

class SpotLight : public Light
{
public:
    SpotLight(const Transform &t, const Color3f& Le, float totalWidth, float falloffStart)
        :Light(t), emitL(Le),cTW(std::cos(totalWidth*(3.14159265359f/180.f))),
                             cFS(std::cos(falloffStart*(3.14159265359f/180.f)))
    {}

    ~SpotLight(){}

    const Color3f emitL;
    const float cTW;
    const float cFS;

    virtual Color3f L(const Intersection &isect, const Vector3f &w) const{ return Color3f(0.f);};

    virtual Color3f Sample_Li(const Intersection &ref, const Point2f &xi,
                                         Vector3f *wi, Float *pdf) const;

    virtual float Pdf_Li(const Intersection &ref, const Vector3f &wi) const{return 0;};

    float Fall_off(const Vector3f &w) const;


};

#endif // SPOTLIGHT_H

