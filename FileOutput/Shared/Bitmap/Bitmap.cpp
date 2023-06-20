#include "Bitmap.h"
#include "Header.cpp"

Bitmap::Bitmap(std::string path, const char* data)
	: m_path(path),
	  m_data(data),
	  m_header(nullptr)
{ }

Bitmap::~Bitmap()
{
	delete[] m_data;
	delete[] m_header;
}
