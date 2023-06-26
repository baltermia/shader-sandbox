#include "ReadWriteFile.h"

// std
#include <fstream>

size_t Common::read_file(std::string filename, std::vector<char>* dest)
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

	*dest = program_buffer;

	return program_size;
}