__kernel void apply_shader(__global unsigned char* data,
						   __constant int* width,
						   __constant int* height,
						   __constant int* channels)
{
	int x = get_global_id(0);
	int y = get_global_id(1);

	int i = (x + y * *height) * *channels;

	for (int j = 0; j < *channels; ++j)
		data[i + j] = convert_char(0xAA);
}
