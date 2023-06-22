#include "Bitmap.h"
#include <fstream>

Bitmap::Bitmap(std::string path, uint32_t width, uint32_t height, char* data, uint16_t depth)
	: m_path(path),
	  m_data(data),
	  m_header(new BitmapHeader(width, height, depth))
{ }

Bitmap::Bitmap(std::string path)
	: m_path(path),
	  m_data(nullptr),
	  m_header(nullptr)
{ }

Bitmap::~Bitmap()
{
	delete[] m_data;
	delete m_header;
}

void Bitmap::setData(char* data)
{
	m_data = data;
}

const char* Bitmap::data() const
{
	return m_data;
}

const BitmapHeader* Bitmap::header() const
{
	return m_header;
}

bool Bitmap::read()
{
	std::ifstream file(m_path);

	// Seek to end of file.
	file.seekg(0, std::ios_base::end);

	// get file-size
	size_t size = file.tellg();
	size_t header_size = sizeof(BitmapHeader);
	size_t data_size = size - header_size;

	// go to beginning
	file.seekg(0);

	// read header data
	char* header_data = new char[header_size];
	file.read(header_data, header_size);
	delete m_header;
	m_header = reinterpret_cast<BitmapHeader*>(header_data);

	// read content data
	delete m_data;
	m_data = new char[data_size];
	file.read(m_data, data_size);

	file.close();
}