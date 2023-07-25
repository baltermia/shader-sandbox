#version 460 core

uniform float seconds;
uniform uvec2 window_size;

out vec4 FragColor;

void main()
{
	FragColor = vec4(vec3(1), 1);
}
