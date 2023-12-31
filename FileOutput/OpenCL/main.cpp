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

constexpr int WIDTH = 512;		// number of pixels on x-axis;
constexpr int HEIGHT = 512;		// number of pixels on y-axis;
constexpr int DEPTH = 24;		// color depth in bits;

#define SIZE		HEIGHT * WIDTH	// total image pixels
#define BITS		SIZE * DEPTH	// total image size in bits
#define BYTES		BITS / 8		// total image size in bytes
#define CHANNELS	DEPTH / 8		// number of bytes per color

int main()
{
	// to check for errorss
	int err;

	cl_device_id device_id = create_device(err);

	cl_context context = clCreateContext(NULL, 1, &device_id, NULL, NULL, &err);

	cl_program program = build_program(context, device_id, PROGRAM_FILE, err);

	// create vector the size of the devices memory
	std::vector<unsigned char> out(BYTES);

	// create output buffer in which the kernel writes and the host copies out of
	cl_mem out_buffer = clCreateBuffer(context, CL_MEM_WRITE_ONLY | CL_MEM_COPY_HOST_PTR, BYTES, out.data(), &err);

	cl_mem const_width = clCreateBuffer(context, CL_MEM_READ_ONLY, sizeof(cl_int), NULL, &err);
	cl_mem const_height = clCreateBuffer(context, CL_MEM_READ_ONLY, sizeof(cl_int), NULL, &err);
	cl_mem const_channels = clCreateBuffer(context, CL_MEM_READ_ONLY, sizeof(cl_int), NULL, &err);

	cl_command_queue command_queue = clCreateCommandQueueWithProperties(context, device_id, 0, &err);

	cl_int width = WIDTH;
	cl_int height = HEIGHT;
	cl_int channels = CHANNELS;
	
	err = clEnqueueWriteBuffer(command_queue, const_width, CL_TRUE, 0, sizeof(cl_int), &width, 0, NULL, NULL);
	err = clEnqueueWriteBuffer(command_queue, const_height, CL_TRUE, 0, sizeof(cl_int), &height, 0, NULL, NULL);
	err = clEnqueueWriteBuffer(command_queue, const_channels, CL_TRUE, 0, sizeof(cl_int), &channels, 0, NULL, NULL);

	cl_kernel kernel = clCreateKernel(program, KERNEL_FUNC, &err);

	// set output-buffer as argument (index 0)
	err = clSetKernelArg(kernel, 0, sizeof(cl_mem), &out_buffer);
	err = clSetKernelArg(kernel, 1, sizeof(cl_mem), &const_width);
	err = clSetKernelArg(kernel, 2, sizeof(cl_mem), &const_height);
	err = clSetKernelArg(kernel, 3, sizeof(cl_mem), &const_channels);

	size_t global_size[] = { WIDTH, HEIGHT };
	size_t local_size[] = { 16, 16 }; // divide work ino blocks of 64

	// this executes the kernel code
	err = clEnqueueNDRangeKernel(command_queue, kernel, 2, NULL, global_size, local_size, 0, NULL, NULL);

	// read device memory into heap memory
	err = clEnqueueReadBuffer(command_queue, out_buffer, CL_TRUE, 0, BYTES, out.data(), 0, NULL, NULL);

	// run commands
	clFlush(command_queue);

	// Deallocate
	clFinish(command_queue);
	clReleaseKernel(kernel);
	clReleaseMemObject(out_buffer);
	clReleaseCommandQueue(command_queue);
	clReleaseProgram(program);
	clReleaseContext(context);

	unsigned int error = lodepng::encode("Result.png", out, WIDTH, HEIGHT, LCT_RGB);
}
