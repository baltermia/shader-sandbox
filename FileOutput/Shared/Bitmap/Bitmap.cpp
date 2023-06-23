// lib
#include "Bitmap.h"

// std
#include <fstream>

namespace Shared
{
	Bitmap::Bitmap(std::string path, uint32_t width, uint32_t height, std::vector<char> data, uint16_t depth)
		: m_path(path),
		  m_data(data),
		  m_header(std::make_shared<BitmapHeader>(width, height, depth))
	{ }

	Bitmap::Bitmap(std::string path)
		: m_path(path),
		  m_header(nullptr)
	{ }

	void Bitmap::setData(std::vector<char>& data)
	{
		m_data = data;
	}

	const std::vector<char>& Bitmap::data() const
	{
		return m_data;
	}

	std::shared_ptr<const BitmapHeader> Bitmap::header() const
	{
		return m_header;
	}

	bool Bitmap::read()
	{
		std::ifstream file(m_path, std::ios_base::in);

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
		m_header.reset();
		m_header = std::shared_ptr<BitmapHeader>(reinterpret_cast<BitmapHeader*>(header_data));

		// read content data
		m_data.clear();
		m_data = std::vector<char>(data_size);
		file.read(m_data.data(), data_size);

		file.close();

		return true;
	}

	bool Bitmap::write() const
	{
		return true;
	}

	void Bitmap::release()
	{
		m_data.clear();
		m_header.reset();
	}
}
