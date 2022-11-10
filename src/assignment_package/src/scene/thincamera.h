#ifndef THINCAMERA_H
#define THINCAMERA_H

#include "camera.h"
#include <samplers/sampler.h>
class thincamera:public Camera
{
public:
    thincamera();
    thincamera(unsigned int w, unsigned int h);
    thincamera(unsigned int w, unsigned int h, const Vector3f &e, const Vector3f &r, const Vector3f &worldUp);
    thincamera(const thincamera &c);

    Sampler spl;
    float FL;
    float LR;

    virtual void RecomputeAttributes() override;
    virtual Ray RaycastNDC(float ndc_x, float ndc_y)  override;

};

#endif // THINCAMERA_H
