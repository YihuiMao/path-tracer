
Progressive rendering and render region
---------------------------------------
We also have added functionality to progressively render scenes based on the regions you select. By default, if you hit render, you would see the OpenGL preview change to the render of the scene.

![](./img/guiImage.png)

The preview scene will show up if you move your camera and then set it to render.

The render region works with the progressive renderer, similar to what exists in Maya.
![](./img/renderRegionI.png)

Click and drag region in the GL preview and hit render.

![](./img/renderRegionII.png)

![](./img/renderRegionIII.png)


 Overview
------------
* a na&#239;ve Monte Carlo path tracer
* path tracer with direct lighting intergrator to estimate the direct lighting in a scene by sampling points
on light surfaces, and a collection of `BxDF`s to handle specular reflective and
transmissive materials, 
* an updated DirectLightingIntegratorthat improves the way it samples the direct on complex surfaces such as the new microfacet materials we've provided.
*  path tracer with full lighting intergrator, which will compute both the direct lighting 
and global illumination at each ray intersection to produce a more converged image in a shorter period of time.

result
------------------
100 sample 1 recusrion cos weight

![](./img/100Sample_1recursion_cos.png)

100 sample 5 recursrion cos weight

![](./img/100Sample_5recusion_cos.png)

100 sample 1 recusrion uniform

![](./img/100sample_1recursion_uniform.png)

100 sample 5 recusrion uniform

![](./img/100sample_5recursion_uniform.png)

* navie integrator image

![](./img/smooth_render.png)



The Light Transport Equation
--------------
#### L<sub>o</sub>(p, &#969;<sub>o</sub>) = L<sub>e</sub>(p, &#969;<sub>o</sub>) + &#8747;<sub><sub>S</sub></sub> f(p, &#969;<sub>o</sub>, &#969;<sub>i</sub>) L<sub>i</sub>(p, &#969;<sub>i</sub>) V(p', p) |dot(&#969;<sub>i</sub>, N)| _d_&#969;<sub>i</sub>

* __L<sub>o</sub>__ is the light that exits point _p_ along ray &#969;<sub>o</sub>.
* __L<sub>e</sub>__ is the light inherently emitted by the surface at point _p_
along ray &#969;<sub>o</sub>.
* __&#8747;<sub><sub>S</sub></sub>__ is the integral over the sphere of ray
directions from which light can reach point _p_. &#969;<sub>o</sub> and
&#969;<sub>i</sub> are within this domain. In general, we tend to only care
about one half of this sphere, determining the relevant half based on the
reflectance or transmittance of the BSDF.
* __f__ is the Bidirectional Scattering Distribution Function of the material at
point _p_, which evaluates the proportion of energy received from
&#969;<sub>i</sub> at point _p_ that is reflected along &#969;<sub>o</sub>.
* __L<sub>i</sub>__ is the light energy that reaches point _p_ from the ray
&#969;<sub>i</sub>. This is the recursive term of the LTE.
* __V__ is a simple visibility test that determines if the surface point _p_' from
which &#969;<sub>i</sub> originates is visible to _p_. It returns 1 if there is
no obstruction, and 0 is there is something between _p_ and _p_'. This is really
only included in the LTE when one generates &#969;<sub>i</sub> by randomly
choosing a point of origin in the scene rather than generating a ray and finding
its intersection with the scene.
* The __absolute-value dot product__ term accounts for Lambert's Law of Cosines.














