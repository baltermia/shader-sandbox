__kernel void apply_shader(__write_only image2d_t image)
{
	int2 coords = { get_global_id(0), get_global_id(1) };
	int2 size = { get_global_size(0), get_global_size(1) };

	float color = (1.0f / size.x * coords.x) * (1.0f / size.y * coords.y);

	write_imagef(image, coords, (float4)((float3)(color), 1));
}
