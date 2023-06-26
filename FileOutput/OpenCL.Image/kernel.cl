__kernel void apply_shader(__write_only image2d_t image)
{
	int2 coords = { get_global_id(0), get_global_id(1) };
	int2 size = { get_global_size(0), get_global_size(1) };

	float4 color = { 0.75f, 0.75f, 0.75f, 0.75f };

	write_imagef(image, coords, color);
}
