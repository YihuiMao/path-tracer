#include "integrator.h"

void Integrator::run()
{
    Render();
}

void Integrator::Render()
{

    std::vector<Point2i> tilePixels = bounds.GetPoints();

    for(Point2i pixel : tilePixels)
    {

        Color3f pixelColor(0.f);

        std::vector<Point2f> pixelSamples = sampler->GenerateStratifiedSamples();
        for(Point2f sample : pixelSamples)
        {
            sample = sample + Point2f(pixel);

            Ray ray = camera->Raycast(sample);
            Color3f L = Li(ray, *scene, sampler, recursionLimit);
            pixelColor += L;
        }

        pixelColor /= pixelSamples.size();
        film->SetPixelColor(pixel, glm::clamp(pixelColor, 0.f, 1.f));
    }

}


void Integrator::ClampBounds()
{
    Point2i max = bounds.Max();
    max = Point2i(std::min(max.x, film->bounds.Max().x), std::min(max.y, film->bounds.Max().y));
    bounds = Bounds2i(bounds.Min(), max);
}
