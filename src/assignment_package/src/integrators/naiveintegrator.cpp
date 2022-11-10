#include "naiveintegrator.h"

Color3f NaiveIntegrator::Li(const Ray &ray, const Scene &scene, std::shared_ptr<Sampler> sampler, int depth) const
{
    //TODO
    Color3f colL(0.f), colB(1.f);
    Ray temp(ray);

   int b=0;

    while(true){

           Intersection inst;
           bool ist = scene.Intersect(temp, &inst);

           if(ist)
           {
               Color3f col = inst.Le(-temp.direction);


               if(!(col.x*col.y*col.z == 0.f))
               {
                   colL += colB * col;
                   break;
               }
           }


           if(!ist || b >= recursionLimit)
           {
               break;
           }


           if(!inst.ProduceBSDF())
           {
               break;
           }



           Vector3f wi;
           float pdf;
           BxDFType flags;
           Color3f f = inst.bsdf->Sample_f(-temp.direction, &wi, sampler->Get2D(), &pdf, BSDF_ALL, &flags);

           if(f.x + f.y + f.z < FLT_EPSILON || pdf == 0.f)
           {
               break;
           }
           Vector3f t=f * glm::abs(glm::dot(wi, inst.bsdf->normal)) / pdf;
           colB *= t;

           temp = inst.SpawnRay(wi);


              b++;
        }
       return colL;
}
