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
	cl_uint2 size { 512, 512 };
	size_t depth = 32;
	size_t bytes = size.x * size.y * 4;

	// to check for errorss
	cl_int err = 0;

	cl_device_id device_id = create_device(err);

	cl_context context = clCreateContext(NULL, 1, &device_id, NULL, NULL, &err);

	cl_program program = build_program(context, device_id, PROGRAM_FILE, err);

	// image properties
	cl_image_format image_format{ CL_RGBA, CL_UNORM_INT8 }; // == uchar, normalized is important, 4 channels
	cl_image_desc image_desc{};
	image_desc.image_type = CL_MEM_OBJECT_IMAGE2D;
	image_desc.image_width = size.x;
	image_desc.image_height = size.y;
	image_desc.image_depth = depth;

	// this allocates memory on the device
	cl_mem image = clCreateImage(context, CL_MEM_WRITE_ONLY, &image_format, &image_desc, NULL, &err);
	
	cl_command_queue command_queue = clCreateCommandQueueWithProperties(context, device_id, 0, &err);

	cl_kernel kernel = clCreateKernel(program, KERNEL_FUNC, &err);

	// provide image
	err = clSetKernelArg(kernel, 0, sizeof(cl_mem), &image);

	// global size are the images dimensions
	size_t global_size[] = { size.x, size.y};
	size_t local_size[] = { 16, 16 }; // blocks 

	// this executes the kernel code
	err = clEnqueueNDRangeKernel(command_queue, kernel, 2, NULL, global_size, local_size, 0, NULL, NULL);

	size_t origin[3] = { 0 };
	size_t region[3] = { size.x, size.y, 1 };

	// reserve vector
	std::vector<cl_uchar> output_buffer(bytes);
	
	// read device memory
	err = clEnqueueReadImage(command_queue, image, CL_TRUE, origin, region, 0, 0, output_buffer.data(), 0, NULL, NULL);

	// run commands
	clFlush(command_queue);

	// Deallocate
	clFinish(command_queue);
	clReleaseKernel(kernel);
	clReleaseMemObject(image);
	clReleaseCommandQueue(command_queue);
	clReleaseProgram(program);
	clReleaseContext(context);
	
	// create png
	unsigned int lode_err = lodepng::encode("Result.png", output_buffer.data(), size.x, size.y, LCT_RGBA);
}
