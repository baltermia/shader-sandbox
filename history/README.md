# History

This is a showcase of the history of all of the algorithms.

## First

The first algorithm, where the aim was to achieve a gradient, looked as follows:

<img src=First.png />

Kernel-Code:

```c
__kernel void apply_shader(__global unsigned char* data,
						   __constant int* width,
						   __constant int* height,
						   __constant int* channels)
{
	int x = get_global_id(0);
	int y = get_global_id(1);

	int i = (x + y * *height) * *channels;

	char color = convert_char(0xFF * (*width / x) * (*height / y));

	for (int j = 0; j < *channels; ++j)
		data[i + j] = color;
}
```