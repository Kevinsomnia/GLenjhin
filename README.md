## GLenjhin

Some C++/OpenGL rendering practice. It's pronounced "glen jin".

Compiled with MSVC, C++17. Using OpenGL 4.6 Core w/ no extensions.

Supports Windows 64-bit only.

## Features

- Primitive meshes
- Lighting
  - Physically-Based Rendering (PBR) for deferred rendering: GGX (diffuse term) / Cook-Torrance (specular term) / Schlick (fresnel approximation)
  - Blinn-Phong lighting model for forward rendering.
  - Real-time soft shadows (4x4 PCF)
  - Directional lights
- HDR and post processing effects pipeline
  - Bloom
  - Tonemapping
  - Fullscreen Gaussian blur
  - Fog
- Custom memory/binary stream implementation
- Custom image library
  - PNG loader (WIP; RGB/RGBA, 8-bit support only)

## Dependencies

- [GLFW 3.3.5](https://www.glfw.org/)
- [Glad](https://github.com/Dav1dde/glad) - OpenGL 4.6 Core, no extensions
- [Dear ImGui](https://github.com/ocornut/imgui)
