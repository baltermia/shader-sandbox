#pragma once

#include "..\pch.h"

#include <string>
#include <vector>

class Bitmap
{
	static const std::string s_extension;

	const char* m_data;
	char* m_header;

public:
	/// <summary>
	/// Initializes a new Bitmap object with optional data
	/// </summary>
	/// <param name="path">The path in the file-system</param>
	/// <param name="data">Optional data that will be written as the bitmaps conent</param>
	Bitmap(std::string path, const char* data = nullptr);

	~Bitmap();

	/// <summary>
	/// Sets the content-data. This does not write the data to the file system.
	/// </summary>
	void setData(const char* data);
	/// <summary>
	/// The content-data of the bitmap file
	/// </summary>
	const char* data() const;

	/// <summary>
	/// The header data of the bitmap file, generated from the given data
	/// </summary>
	const char* header() const;

	/// <summary>
	/// Tries to read data from the given path
	/// </summary>
	/// <returns></returns>
	bool read();

	/// <summary>
	/// Tries to write the bitmap data to the given path
	/// </summary>
	bool write();

	/// <summary>
	/// Releases any data, same as if the destructor would be called. The Bitmap-object cannot be used anymore
	/// </summary>
	void release();
};
