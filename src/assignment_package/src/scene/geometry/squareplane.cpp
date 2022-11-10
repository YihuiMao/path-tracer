#include "squareplane.h"

float SquarePlane::Area() const
{
    //TODO
    //return 0.f;
    glm::vec3 scale = transform.getScale();
    return scale.x * scale.y;
}

bool SquarePlane::Intersect(const Ray &ray, Intersection *isect) const
{

    Ray r_loc = ray.GetTransformedCopy(transform.invT());

    float t = glm::dot(glm::vec3(0,0,1), (glm::vec3(0.5f, 0.5f, 0) - r_loc.origin)) / glm::dot(glm::vec3(0,0,1), r_loc.direction);
    Point3f P = Point3f(t * r_loc.direction + r_loc.origin);
    if(t > 0 && P.x >= -0.5f && P.x <= 0.5f && P.y >= -0.5f && P.y <= 0.5f)
    {
        InitializeIntersection(isect, t, P);
        return true;
    }
    return false;
}

void SquarePlane::ComputeTBN(const Point3f &P, Normal3f *nor, Vector3f *tan, Vector3f *bit) const
{
    *nor = glm::normalize(transform.invTransT() * Normal3f(0,0,1));
    CoordinateSystem(*nor, tan, bit);
}


Point2f SquarePlane::GetUVCoordinates(const Point3f &point) const
{
    return Point2f(point.x + 0.5f, point.y + 0.5f);
}

Intersection SquarePlane::Sample(const Point2f &xi, Float *pdf) const
{
    Intersection isect;

    isect.normalGeometric = glm::normalize(transform.invTransT() * Normal3f(0,0,1));
    isect.point = glm::vec3(transform.T() * glm::vec4(xi.x - 0.5f, xi.y - 0.5f, 0.f, 1.f));
    *pdf = (1.f / this->Area());

    return isect;
}
