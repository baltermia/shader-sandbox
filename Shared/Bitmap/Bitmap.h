#ifndef BITMAP_H
#define BITMAP_H

// std
#include <string>
#include <vector>
#include <memory>

// lib
#include "Header.hpp"

namespace Shared
{
	class Bitmap
	{
		static const std::string s_extension;

		std::string m_path;
		std::vector<char> m_data;
		std::shared_ptr<BitmapHeader> m_header;

	public:
		/// <summary>
		/// Initializes a new Bitmap object with optional data
		/// </summary>
		/// <param name="path">The path in the file-system</param>
		/// <param name="data">Optional data that will be written as the bitmaps conent</param>
		Bitmap(std::string path, uint32_t width, uint32_t height, std::vector<char> data, uint16_t depth = 24);
		Bitmap(std::string path);

		/// <summary>
		/// Sets the content-data. This does not write the data to the file system.
		/// </summary>
		void setData(std::vector<char>& data);
		/// <summary>
		/// The content-data of the bitmap file
		/// </summary>
		const std::vector<char>& data() const;

		/// <summary>
		/// The header data of the bitmap file, generated from the given data
		/// </summary>
		std::shared_ptr<const BitmapHeader> header() const;

		/// <summary>
		/// Tries to read data from the given path
		/// </summary>
		/// <returns></returns>
		void read();

		/// <summary>
		/// Tries to write the bitmap data to the given path
		/// </summary>
		void write() const;

		/// <summary>
		/// Releases any data. The Bitmap-object cannot be used anymore
		/// </summary>
		void release();
	};
}

#endif
