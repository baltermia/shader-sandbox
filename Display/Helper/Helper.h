#pragma once

// gl
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// std
#include <string>
#include <fstream>

namespace help
{
	std::string read_file_into_string(const std::string& filename);

	bool check_compilation_success(unsigned int shader_id);

	bool try_load_shader(const std::string& file_name, unsigned int* out_shader_id, GLenum shader_type);

	bool check_linking_success(unsigned int program_id);

    template<typename T>
    struct Vec3
    {
        T x, y, z;
    };
}

typedef help::Vec3<float> vec3f;
typedef help::Vec3<int> vec3i;
typedef help::Vec3<unsigned int> vec3ui;