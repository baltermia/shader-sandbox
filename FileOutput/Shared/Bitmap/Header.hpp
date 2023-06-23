#ifndef HEADER_H
#define HEADER_H

// bitmap header docs: https://docs.fileformat.com/image/bmp/
// https://dev.to/muiz6/c-how-to-write-a-bitmap-image-from-scratch-1k6m

// std
#include <cstdint>
#include <fstream>

namespace Shared
{
	struct BitmapInformationHeader
	{
		BitmapInformationHeader() { }

		BitmapInformationHeader(uint32_t width, uint32_t height, uint16_t depth = 24)
			: width(width),
			height(height),
			depth(depth)
		{ }

		uint32_t headerSize = sizeof(BitmapInformationHeader);
		uint32_t width = 0;
		uint32_t height = 0;
		uint16_t planes = 1;
		uint16_t depth = 0;
		uint32_t compressionMethod = 0;
		uint32_t rawBitmapDataSize = 0;
		int32_t horizontalResolution = 3780;
		int32_t verticalResolution = 3780;
		uint32_t colorTableEntries = 0;
		uint32_t importantColors = 0;

		void write_to_file(std::ofstream& fout) const
		{
			fout.write((char*)&headerSize, sizeof(uint32_t));
			fout.write((char*)&width, sizeof(int32_t));
			fout.write((char*)&height, sizeof(int32_t));
			fout.write((char*)&planes, sizeof(uint16_t));
			fout.write((char*)&depth, sizeof(uint16_t));
			fout.write((char*)&compressionMethod, sizeof(uint32_t));
			fout.write((char*)&rawBitmapDataSize, sizeof(uint32_t));
			fout.write((char*)&horizontalResolution, sizeof(int32_t));
			fout.write((char*)&verticalResolution, sizeof(int32_t));
			fout.write((char*)&colorTableEntries, sizeof(uint32_t));
			fout.write((char*)&importantColors, sizeof(uint32_t));
		}
	};

	struct BitmapFileHeader
	{
		BitmapFileHeader() { }

		BitmapFileHeader(uint32_t contentSize)
			: fileSize(offset + contentSize)
		{ }

		BitmapFileHeader(uint32_t width, uint32_t height, uint16_t depth = 24)
			: fileSize(offset + width * height * depth / 8)
		{ }

		char signature[2] = { 'B', 'M' };
		uint32_t fileSize = 0;
		char reserved[4] = { 0 };
		uint32_t offset = sizeof(BitmapFileHeader) + sizeof(BitmapInformationHeader);

		void write_to_file(std::ofstream& fout) const
		{
			fout.write(signature, 2);
			fout.write((char*)&fileSize, sizeof(uint32_t));
			fout.write(reserved, 4);
			fout.write((char*)&offset, sizeof(uint32_t));
		}
	};

	struct BitmapHeader
	{
		BitmapHeader() { }

		BitmapHeader(uint32_t width, uint32_t height, uint16_t depth = 24)
			: fileHeader(width, height, depth),
			informationheader(width, height, depth)
		{ }

		BitmapFileHeader fileHeader;
		BitmapInformationHeader informationheader;

		void write_to_file(std::ofstream& fout) const
		{
			fileHeader.write_to_file(fout);
			informationheader.write_to_file(fout);
		}
	};

}

#endif
