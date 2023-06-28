<div align=center>
  <h1>💫 Shader Sandbox ✧˖°.</h1>
  C++ CUDA &amp; OpenCL implementations for a shader sandbox
</div>

## Requirements
1. [CUDA Toolkit](https://developer.nvidia.com/cuda-downloads) (also contains OpenCL)

### Dependencies

2. [lodepng](https://github.com/lvandeve/lodepng) PNG-Library
   - Install with [vcpkg](https://vcpkg.io/en/): `vcpkg install lodepng`
3. [GLFW](https://github.com/glfw/glfw) & [GLAD](https://github.com/dav1dde/glad)
   - `vcpkg install glad glfw3`
  
> **Note**:  The dependencies do not need to be installed with `vcpkg`. They can also be manually linked in each project.

## Usefule References
These can help while coding:
- [C Math Funcions](https://en.cppreference.com/w/c/numeric/math)
- [OpenCL Error Codes](https://github.com/KhronosGroup/OpenCL-Headers/blob/main/CL/cl.h#L195-L271)
