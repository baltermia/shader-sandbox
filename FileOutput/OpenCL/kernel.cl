__kernel void apply_shader(__global unsigned  char* data)
{
	int i = get_global_id(0);

	__global unsigned char* curr = &data[i * 3];

	*curr = convert_char(0xAAAAAA);
	*++curr = convert_char(0xAAAAAA);
	*++curr = convert_char(0xAAAAAA);
}
