#ifndef READWRITEFILE_H
#define READWRITEFILE_H

// std
#include <string>
#include <vector>

namespace Common
{
	/// <summary>
	/// Reads a whole files content into the destination vector and returns the size
	/// </summary>
	size_t read_file(std::string filename, std::vector<char>* destination);
}

#endif
