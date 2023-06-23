# History

This is a showcase of the history of all of the algorithms.

## First Gradients

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

---

Just moments later though, a 1D gradient was achieved though:

<img src=Gradient.png />

Updated Color algorithm:

```c
char color = convert_char(0xFF * x / *width);
```

---

As well as a 2D-Gradient:

<img src="2D Gradient.png" />

```c
char color = convert_char(0xFF * x / *width * y / *height);
```