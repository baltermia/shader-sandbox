#include "ConfigureCL.h"
#include "..\Common\ReadWriteFile.h"

bool ConfigureCL::is_error(cl_int& cl_err)
{
	return cl_err < 0;
}

cl_device_id ConfigureCL::create_device(cl_int& err)
{
	cl_platform_id platform = NULL;
	cl_device_id device_id = NULL;

	// Identify platform
	err = clGetPlatformIDs(1, &platform, NULL);

	RETURN_IF_CL_ERROR(device_id);

	// Try to access a GPU
	err = clGetDeviceIDs(platform, CL_DEVICE_TYPE_GPU, 1, &device_id, NULL);

	RETURN_IF_CL_ERROR(device_id);

	// If no GPU could be found, use CUP
	if (err == CL_DEVICE_NOT_FOUND)
		err = clGetDeviceIDs(platform, CL_DEVICE_TYPE_CPU, 1, &device_id, NULL);

	return device_id;
}

cl_program ConfigureCL::build_program(cl_context context, cl_device_id device_id, std::string filename, cl_int& err)
{
	std::vector<char> program_buffer;
	size_t program_size = Common::read_file(filename, &program_buffer);
	const char* ptr = program_buffer.data();

	// create new program with buffer
	cl_program program = clCreateProgramWithSource(context, 1, (const char**)&ptr, &program_size, &err);

	// build program
	err = clBuildProgram(program, 1, &device_id, NULL, NULL, NULL);

	RETURN_IF_CL_ERROR(program);

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
