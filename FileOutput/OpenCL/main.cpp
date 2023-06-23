//std
#include <stdio.h>
#include <fstream>
#include <vector>

// cl
#include <CL/cl.h>

// vcpkg
#include "lodepng.h"

constexpr int WIDTH = 640;		// number of pixels on x-axis;
constexpr int HEIGHT = 360;		// number of pixels on y-axis;
constexpr int DEPTH = 24;		// color depth in bits;

#define SIZE		HEIGHT * WIDTH	// total image pixels
#define BITS		SIZE * DEPTH	// total image size in bits
#define BYTES		BITS / 8		// total image size in bytes
#define CHANNELS	DEPTH / 8		// number of bytes per color

/// <summary>
/// Try and create a device. Prefferably GPU over CPU
/// </summary>
cl_device_id create_device(cl_int& err)
{
	cl_platform_id platform;
	cl_device_id device_id;

	// Identify platform
	err = clGetPlatformIDs(1, &platform, NULL);

	// Try to access a GPU
	err = clGetDeviceIDs(platform, CL_DEVICE_TYPE_GPU, 1, &device_id, NULL);

	// If no GPU could be found, use CUP
	if (err == CL_DEVICE_NOT_FOUND)
		err = clGetDeviceIDs(platform, CL_DEVICE_TYPE_CPU, 1, &device_id, NULL);

	return device_id;
}

/// <summary>
/// Create program from a file and compile it
/// </summary>
cl_program build_program(cl_context context, cl_device_id device_id, const char* filename, cl_int& err)
{
	// Load file
	std::ifstream file(filename);

	// Seek to end of file.
	file.seekg(0, std::ios_base::end);

	// get file-size
	size_t program_size = file.tellg();

	// go to beginning
	file.seekg(0);

	// read file into memory
	std::vector<char> program_buffer(program_size);
	file.read(program_buffer.data(), program_size);

	const char* ptr = program_buffer.data();

	// create new program with buffer
	cl_program program = clCreateProgramWithSource(context, 1, (const char**)&ptr, &program_size, &err);

	// build program
	err = clBuildProgram(program, 1, &device_id, NULL, NULL, NULL);

	if (err >= 0)
		return program;

	// if build is not successful, print the build-info to the console

	size_t log_size;
	// first get size of log-text
	clGetProgramBuildInfo(program, device_id, CL_PROGRAM_BUILD_LOG, 0, NULL, &log_size);

	// get that memory
	std::vector<char> program_log(log_size);
	clGetProgramBuildInfo(program, device_id, CL_PROGRAM_BUILD_LOG, log_size + 1, program_log.data(), NULL);

	// finally print it to the console
	printf("%s\n", program_log.data());

	return program;
}

int main()
{
	// to check for errorss
	int err;

	cl_device_id device_id = create_device(err);

	cl_context context = clCreateContext(NULL, 1, &device_id, NULL, NULL, &err);

	cl_program program = build_program(context, device_id, "kernel.cl", err);

	// create vector the size of the devices memory
	std::vector<unsigned char> out(BYTES);

	// create output buffer in which the kernel writes and the host copies out of
	cl_mem out_buffer = clCreateBuffer(context, CL_MEM_WRITE_ONLY | CL_MEM_COPY_HOST_PTR, BYTES, out.data(), &err);

	cl_command_queue command_queue = clCreateCommandQueueWithProperties(context, device_id, 0, &err);

	cl_kernel kernel = clCreateKernel(program, "apply_shader", &err);

	// set output-buffer as argument (index 0)
	err = clSetKernelArg(kernel, 0, sizeof(cl_mem), &out_buffer);

	size_t global_size = BYTES;
	size_t local_size = 64; // divide work ino blocks of 64

	// this executes the kernel code
	err = clEnqueueNDRangeKernel(command_queue, kernel, 1, NULL, &global_size, &local_size, 0, NULL, NULL);

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
