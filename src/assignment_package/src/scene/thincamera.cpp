#include "thincamera.h"
#include "warpfunctions.h"

thincamera::thincamera()
    :Camera(), FL(29.5f), LR(1.5f), spl(Sampler(100, 0))
{}

thincamera::thincamera(unsigned int w, unsigned int h)
    :Camera(w, h), FL(29.5f), LR(1.5f), spl(Sampler(100, 0))
{}

thincamera::thincamera(unsigned int w, unsigned int h, const Vector3f &e, const Vector3f &r, const Vector3f &worldUp)
    :Camera(w, h, e, r, worldUp), FL(29.5f), LR(1.5f), spl(Sampler(100, 0))
{
    this->RecomputeAttributes();
}

thincamera::thincamera(const thincamera &c): spl(Sampler(100, 0))

{
    fovy = c.fovy;
    width = c.width;
    height = c.height;
    near_clip = c.near_clip;
    far_clip = c.far_clip;
    aspect = c.aspect;
    eye = c.eye;
    ref = c.ref;
    look = c.look;
    up = c.up;
    right = c.right;
    world_up = c.world_up;
    V = c.V;
    H = c.H;
    FL = 29.5f;
    LR = 1.5f;
    this->RecomputeAttributes();
}

void thincamera::RecomputeAttributes()
{
    look = glm::normalize(ref - eye);
    right = glm::normalize(glm::cross(look, world_up));
    up = glm::cross(right, look);

    float tan_fovy = tan(glm::radians(fovy/2));
    float len = this->FL;
    aspect = width/(float)height;
    V = up*len*tan_fovy;
    H = right*len*aspect*tan_fovy;
}



Ray thincamera::RaycastNDC(float ndc_x, float ndc_y)
{
    Point3f focalRef = eye + this->look * this->FL;
    glm::vec3 pFocal = focalRef + ndc_x*H + ndc_y*V;
    Point2f pLenSample = spl.Get2D();
    Point3f pLens = LR * WarpFunctions::squareToDiskConcentric(pLenSample);
    pLens = eye + up * pLens.y + right * pLens.x;
    Ray result(pLens, glm::normalize(pFocal - pLens));


    return result;
}




