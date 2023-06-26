#ifndef CONFIGURECL_H
#define CONFIGURECL_H

// cl
#include "CL\cl.h"

//std
#include <vector>
#include <fstream>

#define RETURN_IF_CL_ERROR(return_value) if (is_error(err)) return return_value;

namespace ConfigureCL
{
	/// <summary>
	/// Checks whether the given opencl error value indicates success or an error.
	/// </summary>
	bool is_error(cl_int& cl_err);

	/// <summary>
	/// Try and create a device. Prefferably GPU over CPU
	/// </summary>
	cl_device_id create_device(cl_int& err);

	/// <summary>
	/// Create program from a file and compile it
	/// </summary>
	cl_program build_program(cl_context context, cl_device_id device_id, std::string filename, cl_int& err);
}

#endif
