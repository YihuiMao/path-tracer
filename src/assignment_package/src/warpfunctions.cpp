#define _USE_MATH_DEFINES
#include "warpfunctions.h"
#include <math.h>

Point3f WarpFunctions::squareToDiskUniform(const Point2f &sample)
{
    //TODO
    float angle = 2.f * glm::pi<float>() * sample.y;
       float r = glm::sqrt(sample.x);
       return glm::vec3(glm::cos(angle) * r,glm::sin(angle) * r,0.f);
}

Point3f WarpFunctions::squareToDiskConcentric(const Point2f &sample)
{
    //TODO
    //throw std::runtime_error("You haven't yet implemented concentric disk warping!");
    float t, r;
         float a = 2.f * sample.x - 1.f;
         float b = 2.f * sample.y - 1.f;

          if(a <= -b)
         {
             if(a < b)
             {

                 r = -a;
                 t = (glm::pi<float>() / 4.f) * (4 + (b / a));
             }
             else
             {
                 r = -b;
                 if(b != 0)
                 {
                     t = (glm::pi<float>() / 4.f) * (6 - (a / b));
                 }
                 else
                 {
                     t = 0;
                 }
             }
         }
         else
          {
             if(a > b)
             {

                 r = a;
                 t = (glm::pi<float>() / 4.f) * (b / a);
             }
             else
             {

                 r = b;
                 t = (glm::pi<float>() / 4.f) * (2.f - (a / b));
             }
         }


         return glm::vec3(r * glm::cos(t), r * glm::sin(t), 0.f);

}

float WarpFunctions::squareToDiskPDF(const Point3f &sample)
{
    //TODO
    //return 0;
    return InvPi;
}

Point3f WarpFunctions::squareToSphereUniform(const Point2f &sample)
{
    //TODO
   // throw std::runtime_error("You haven't yet implemented uniform sphere warping!");
    float z = 1.f - 2.f * sample.x;
    float x = glm::cos(2.f * glm::pi<float>() * sample.y) * glm::sqrt(1.f - z * z);
    float y = glm::sin(2.f * glm::pi<float>() * sample.y) * glm::sqrt(1.f - z * z);
    return glm::vec3(x, y, z);


}

float WarpFunctions::squareToSphereUniformPDF(const Point3f &sample)
{
    //TODO
    //return 0;
     return Inv4Pi;
}

Point3f WarpFunctions::squareToSphereCapUniform(const Point2f &sample, float thetaMin)
{
    //TODO
   // throw std::runtime_error("You haven't yet implemented sphere cap warping!");
    float phi = 2 * glm::pi<float>() * sample.y;
           float theta = glm::acos(sample.x) * (180.f - thetaMin) / 90.f;
           return glm::vec3(glm::sin(theta) * glm::cos(phi),
                            glm::sin(theta) * glm::sin(phi),
                            glm::cos(theta));
}

float WarpFunctions::squareToSphereCapUniformPDF(const Point3f &sample, float thetaMin)
{
    //TODO
    //return 0;
     return Inv2Pi * (1.f / (1.f - glm::cos(glm::radians(180.f - thetaMin))));
}

Point3f WarpFunctions::squareToHemisphereUniform(const Point2f &sample)
{
    //TODO
   // throw std::runtime_error("You haven't yet implemented uniform hemisphere warping!");
    float z = sample.x;
    float x = glm::cos(2.f * glm::pi<float>() * sample.y) * glm::sqrt(1.f - z * z);
    float y = glm::sin(2.f * glm::pi<float>() * sample.y) * glm::sqrt(1.f - z * z);
    return glm::vec3(x, y, z);
}

float WarpFunctions::squareToHemisphereUniformPDF(const Point3f &sample)
{
    //TODO
    //return 0;
    return Inv2Pi;
}

Point3f WarpFunctions::squareToHemisphereCosine(const Point2f &sample)
{
    //TODO
  //  throw std::runtime_error("You haven't yet implemented cosine-weighted hemisphere warping!");
    glm::vec3 S;
    S = squareToDiskConcentric(sample);
    S.z = glm::sqrt(1 - S.x * S.x - S.y * S.y);
    return S;
}

float WarpFunctions::squareToHemisphereCosinePDF(const Point3f &sample)
{
    //TODO
    //return 0;
    return sample.z * InvPi;
}
