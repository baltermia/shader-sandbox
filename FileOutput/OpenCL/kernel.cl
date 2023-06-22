__kernel void apply_shader(__global char* data)
{
	int i = get_global_id(0);

	__global char* curr = &data[i * 3];

	*++curr = convert_char(0xAAAAAA);
	*++curr = convert_char(0xAAAAAA);
	*curr = convert_char(0xAAAAAA);
}
