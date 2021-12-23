## GLenjhin

Some C++/OpenGL rendering practice. It's pronounced "glen jin".

Compiled with MSVC v143, C++17. Using OpenGL 4.6 Core. The rendering pipeline is very object-oriented, which should make the rendering pipeline flexible at the cost of some performance.

It's recommended to compile in Release mode for the fastest performance.

Supports Windows 64-bit only.

![render](https://user-images.githubusercontent.com/32236920/146665178-380484a5-ffc8-4386-9079-fbb721208ac4.jpg)

## Dependencies

- [GLFW 3.3.5](https://www.glfw.org/)
- [Glad](https://github.com/Dav1dde/glad) - OpenGL 4.6 Core, no extensions
- [Dear ImGui 1.85](https://github.com/ocornut/imgui)
- [Assimp 5.1.3](https://github.com/assimp/assimp)
- [xxHash 0.8.1](https://github.com/Cyan4973/xxHash)

## Features

- Lighting
  - Physically-Based Rendering (PBR): Lambertian (diffuse term) / GGX + Cook-Torrance (specular term) / Schlick (fresnel approximation)
    - Albedo, Normalmaps, MSA (Metallic/Smoothness/AO), Parallax Occlusion Mapping
  - Real-time shadows with 4x4 PCF (soft filtering)
  - Directional lights
- HDR and post processing effects pipeline
  - FXAA (Fast Approximate Anti-Aliasing)
  - Motion Blur (via motion vectors)
  - Deferred SSAO (Screen Space Ambient Occlusion)
  - Bloom
  - Sun Shafts (God Rays)
  - Tonemapping
  - Fullscreen Gaussian blur
  - Fog
- Shaders
  - Skybox rendered from fullscreen triangle via equirectangular projection
- I/O
  - Custom memory/binary stream implementation with bit-level serialization/deserialization support
  - Custom image library
    - PNG support (WIP; RGB/RGBA, 8-bit support only)
    - Custom DEFLATE/zlib implementation
  - Custom asset cache implementation powered by [xxHash](https://github.com/Cyan4973/xxHash). Subsequent app startups will be faster if cache is present.
  - Model import pipeline powered by [Assimp](https://github.com/assimp/assimp)
- Primitive meshes generated at runtime (mostly trigonometry): cubes, spheres, capsules, cylinders, quads
