#include "shape.h"
#include <QDateTime>

pcg32 Shape::colorRNG = pcg32(QDateTime::currentMSecsSinceEpoch());


void Shape::InitializeIntersection(Intersection *isect, float t, Point3f pLocal) const
{
    isect->point = Point3f(transform.T() * glm::vec4(pLocal, 1));
    ComputeTBN(pLocal, &(isect->normalGeometric), &(isect->tangent), &(isect->bitangent));
    isect->uv = GetUVCoordinates(pLocal);
    isect->t = t;
}



 float Shape::Pdf(const Intersection &ref, const Vector3f &wi) const{

 }

Intersection Shape::Sample(const Intersection &ref, const Point2f &xi, float *pdf) const
{
    //TODO

    Intersection ist = this->Sample(xi, pdf);

   *pdf = glm::length2(ref.point- ist.point) /
                (AbsDot(ist.normalGeometric,
                -glm::normalize(ist.point -
                 ref.point)) * this->Area());


   return ist;
}
