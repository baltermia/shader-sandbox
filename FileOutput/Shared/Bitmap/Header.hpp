#ifndef HEADER_H
#define HEADER_H

// bitmap header docs: https://docs.fileformat.com/image/bmp/
// https://dev.to/muiz6/c-how-to-write-a-bitmap-image-from-scratch-1k6m

#include <cstdint>

struct BitmapInformationHeader
{
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
};

struct BitmapFileHeader
{
	BitmapFileHeader(uint32_t contentSize)
		: fileSize(offset + contentSize)
	{ }

	BitmapFileHeader(uint32_t width, uint32_t height, uint16_t depth = 24)
		: fileSize(offset + width * height * depth / 8)
	{ }

	char signature[2] = { 'B', 'M' };
	uint32_t fileSize;
	char reserved[4] = { 0 };
	uint32_t offset = sizeof(BitmapFileHeader) + sizeof(BitmapInformationHeader);
};

struct BitmapHeader
{
	BitmapHeader(uint32_t width, uint32_t height, uint16_t depth = 24)
		: fileHeader(width, height, depth),
		informationheader(width, height, depth)
	{ }

	BitmapFileHeader fileHeader;
	BitmapInformationHeader informationheader;
};

#endif