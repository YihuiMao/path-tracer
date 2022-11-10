#pragma once
#include "integrator.h"
float BalanceHeuristic(int nf, Float fPdf, int ng, Float gPdf);
float PowerHeuristic(int nf, Float fPdf, int ng, Float gPdf);

class DirectLightingIntegrator : public Integrator
{
public:
    DirectLightingIntegrator(Bounds2i bounds, Scene* s, std::shared_ptr<Sampler> sampler, int recursionLimit)
        : Integrator(bounds, s, sampler, recursionLimit)
    {}

    virtual Color3f Li(const Ray &ray, const Scene &scene, std::shared_ptr<Sampler> sampler, int depth) const;
};
