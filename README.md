## GLenjhin

Some C++/OpenGL rendering practice. It's pronounced "glen jin".

Compiled with MSVC, C++17. Using OpenGL 4.6 Core w/ no extensions.

Supports Windows 64-bit only.

## Features

- Primitive meshes
- Blinn-Phong shading
- Real-time soft shadows
  - Directional lights
- HDR and post processing effects pipeline
  - Bloom
  - Tonemapping
  - Fullscreen Gaussian blur
  - Fog
- Custom memory/binary stream implementation
- Custom image library

## Dependencies

- [GLFW 3.3.5](https://www.glfw.org/)
- [Glad](https://github.com/Dav1dde/glad) - OpenGL 4.6 Core, no extensions
- [Dear ImGui](https://github.com/ocornut/imgui)
