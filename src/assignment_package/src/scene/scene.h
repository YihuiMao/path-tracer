#pragma once
#include <QList>
#include <raytracing/film.h>
#include <scene/camera.h>
#include <scene/lights/light.h>
#include "thincamera.h"
class Primitive;
class Material;
class Light;
class Shape;
class Scene
{
public:
    Scene();
    QList<std::shared_ptr<Primitive>> primitives;
    QList<std::shared_ptr<Material>> materials;
    QList<std::shared_ptr<Light>> lights;
    Camera camera;
    Film film;

    Camera* cPtr;
    thincamera tlCamera;
    void SetCamera(const Camera &c);
    void SetThinCamera(const thincamera &c);
    void CreateTestScene();
    void Clear();

    bool Intersect(const Ray& ray, Intersection* isect) const;
};
