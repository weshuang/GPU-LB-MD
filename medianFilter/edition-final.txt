#include "medianFilter_kernel.h"
#include <cuda.h>
#include <math.h>
#include <cuPrintf.cu>
#include <cuda_runtime.h>

/* You can modify this file as you want. */
__global__ void mfKernel(const RGB* src, RGB* dst, int r, int w, int h);

//#define CCE checkCudaErrors




extern "C" void
medianFilter(
             const uchar *h_src,
             uchar *h_dst,
             int w,
             int h,
             int r)
{
	int block_size=16;
	dim3 blockDim(/*BLOCK_SIZE, BLOCK_SIZE*/16,16);
	dim3 gridDim((int) ceil((double)(w-2*r)/(block_size -2*r)), (int) ceil((double)(h-2*r)/(block_size - 2*r)));

    /* Write your code here */
	RGB* d_dst = NULL;
	RGB* d_src = NULL;

	cudaMalloc( (void**)&d_dst, sizeof(RGB)*w*h);
	cudaMalloc( (void**)&d_src, sizeof(RGB)*w*h);
	cudaMemcpy( d_src, (RGB*)h_src, w*h*3, cudaMemcpyHostToDevice); 

	//call kernel
	printf("[%d %d %d %d]\n", gridDim.x, gridDim.y, blockDim.x, blockDim.y);

	cudaPrintfInit();
	mfKernel<<< gridDim, blockDim>>>( d_src, d_dst, r, w, h); 
	FILE *cuprintout;
	cuprintout=fopen("cudadebugprint.txt","w");
	cudaPrintfDisplay(cuprintout, false);
	cudaPrintfEnd();
	fclose(cuprintout);


	cudaMemcpy( h_dst, (uchar*)d_dst, w*h*3, cudaMemcpyDeviceToHost);

	cudaFree(d_dst); 
	cudaFree(d_src);
}


__global__ void mfKernel(const RGB* src, RGB* dst, int r, int w, int h)
{
	
	//const int px=threadIdx.x+r;
	//const int py=blockIdx.x+r;
	const int px = blockIdx.x * (blockDim.x -2*r) + threadIdx.x;
	const int py = blockIdx.y * (blockDim.y -2*r) + threadIdx.y;
	RGB pixels[9];
	const int pixelamount=9;
	int i,j;
	int par1=3, par2=3, par3=3, par4=3;

	//create shared memory and copy device_memory into it
	__shared__ RGB subpic[16 * 16];
	if(px<w && py<h)
		subpic[threadIdx.y * blockDim.x + threadIdx.x] = src[ py * w + px];

	//if(blockIdx.x<par1 && blockIdx.y<par2 && threadIdx.x<par3 && threadIdx.y<par4)
	//	cuPrintf("(%d %d %d %d) %d %d\n", blockIdx.x, blockIdx.y, threadIdx.x, threadIdx.y, px, py);

	__syncthreads();
	
	//write to pixels
	if(px>=r && px<w-r && py>=r && py<h-r && threadIdx.x>=r && threadIdx.x < blockDim.x-r && threadIdx.y>=r && threadIdx.y < blockDim.y-r)
	{
		i=0;
		for (int dy = -r; dy <= r; ++dy)
		{
			for (int dx = -r; dx <= r; ++dx)
			{        
				pixels[i++] = subpic[ (threadIdx.x+dx) + (threadIdx.y+dy) * blockDim.x];
			}
		}
		//bubble sort, arrange from big to small
		for(i=pixelamount-1; i>0; i--)
		{
			for(j=0; j<i; j++)
			{
				if( computeLuminance(pixels[j].R, pixels[j].G, pixels[j].B) < computeLuminance(pixels[j+1].R, pixels[j+1].G, pixels[j+1].B))
				{
					RGB temp;
					temp=pixels[j];
					pixels[j]=pixels[j+1];
					pixels[j+1]=temp;
				}
			}
		}
		
	}

	if(px>=r && px<w-r && py>=r && py<h-r && threadIdx.x>=r && threadIdx.x < blockDim.x-r && threadIdx.y>=r && threadIdx.y < blockDim.y-r)
		dst[py * w + px] = pixels[((2*r+1)*(2*r+1)-1)/2]; // copy median to output
}