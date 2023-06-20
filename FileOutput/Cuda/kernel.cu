#include "cuda_runtime.h"				// for funcs like cudaMalloc and defines like __global__
#include "device_launch_parameters.h"	// for paramteres like threadIdx

constexpr int WIDTH = 640;		// number of pixels on x-axis;
constexpr int HEIGHT = 360;		// number of pixels on y-axis;
constexpr int DEPTH = 24;		// color depth in bits;

#define SIZE		HEIGHT * WIDTH	// total image pixels
#define BITS		SIZE * DEPTH	// total image size in bits
#define BYTES		BITS / 8		// total image size in bytes
#define CHANNELS	DEPTH / 8		// number of bytes per color

typedef char color[CHANNELS];

__device__ void shader(color& out, int x, int y)
{
	// calculate color (same for each channel)
	char color = 255 / 3 * 2;

	// apply same color on each channel
	for (int i = 0; i < CHANNELS; ++i)
	{
		out[i] = color;
	}
}

__global__ void applyShader(color* data)
{
	int i = threadIdx.x;

	shader(data[i], i, i);
}

int main()
{
	color* cudaData = nullptr;

	cudaMalloc(&cudaData, BYTES);
	
	applyShader<<<1, BYTES>>>(cudaData);

	color* outData = new color[BYTES];

	cudaMemcpy(outData, cudaData, BYTES, cudaMemcpyDeviceToHost);
}
