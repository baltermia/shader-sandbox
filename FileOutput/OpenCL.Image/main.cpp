//std
#include <stdio.h>
#include <fstream>
#include <vector>

// cl
#include <CL/cl.h>

// vcpkg
#include "lodepng.h"

// lib
#include "ConfigureCL/ConfigureCL.h"

using namespace ConfigureCL;

#define PROGRAM_FILE	"kernel.cl"
#define KERNEL_FUNC		"apply_shader"

int main()
{
	cl_uint2 size{ 512, 512 };
	size_t depth = 32;

	// to check for errorss
	cl_int err = 0;

	cl_device_id device_id = create_device(err);

	cl_context context = clCreateContext(NULL, 1, &device_id, NULL, NULL, &err);

	cl_program program = build_program(context, device_id, PROGRAM_FILE, err);

	cl_image_format image_format{ CL_RGBA, CL_FLOAT };
	cl_image_desc image_desc{};
	image_desc.image_type = CL_MEM_OBJECT_IMAGE2D;
	image_desc.image_width = size.x;
	image_desc.image_height = size.y;
	image_desc.image_depth = 32;

	cl_mem image = clCreateImage(context, CL_MEM_WRITE_ONLY, &image_format, &image_desc, NULL, &err);
	
	cl_command_queue command_queue = clCreateCommandQueueWithProperties(context, device_id, 0, &err);

	cl_kernel kernel = clCreateKernel(program, KERNEL_FUNC, &err);

	err = clSetKernelArg(kernel, 0, sizeof(cl_mem), &image);

	size_t global_size[] = { size.x, size.y};
	size_t local_size[] = { 16, 16 };

	// this executes the kernel code
	err = clEnqueueNDRangeKernel(command_queue, kernel, 2, NULL, global_size, local_size, 0, NULL, NULL);

	size_t region = size.x * size.y;
	std::vector<cl_float4> output_buffer(region);
	size_t bytes = region * sizeof(cl_float) * depth / 8;
	err = clEnqueueReadImage(command_queue, image, CL_TRUE, 0, &bytes, 0, 0, output_buffer.data(), 0, NULL, NULL);

	// run commands
	clFlush(command_queue);

	// Deallocate
	clFinish(command_queue);
	clReleaseKernel(kernel);
	clReleaseMemObject(image);
	clReleaseCommandQueue(command_queue);
	clReleaseProgram(program);
	clReleaseContext(context);

	std::vector<unsigned char> out_chars(region * 4);

	for (size_t i = 0; i < region; ++i) 
	{
		out_chars[i * 4]	 =	(char)(output_buffer[i].x * 255);
		out_chars[i * 4 + 1] =	(char)(output_buffer[i].y * 255);
		out_chars[i * 4 + 2] =	(char)(output_buffer[i].z * 255);
		out_chars[i * 4 + 3] =	(char)(output_buffer[i].w * 255);
	}
	
	unsigned int lode_err = lodepng::encode("Result.png", out_chars, size.x, size.y, LCT_RGBA);
}
