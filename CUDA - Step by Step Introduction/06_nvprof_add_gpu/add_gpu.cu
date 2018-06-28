#include <iostream>
#include <math.h>
#include <stdio.h>

const int VARIANT_NR = 3;

// Kernel function to add the elements of two arrays

// Variant 1
__global__
void add1(int n, float *x, float *y)
{
  for (int i = 0; i < n; i++)
    y[i] = x[i] + y[i];
}


// Variant 2
__global__
void add2(int n, float *x, float *y)
{
  int index = threadIdx.x;
  int stride = blockDim.x;
  //std::cout << "Hello from thread " << threadIdx.x <<
  //            " in block " << blockDim.x << "\n";
  //printf("threadIdx.x=%d, threadIdx.y=%d, blockDim.x=%d, blockDim.y=%d\n",
  // threadIdx.x, threadIdx.y, blockDim.x, blockDim.y);
  for (int i = index; i < n; i += stride)
      y[i] = x[i] + y[i];
}


// Variant 3
__global__
void add3(int n, float *x, float *y)
{
  int index = blockIdx.x * blockDim.x + threadIdx.x;
  int stride = blockDim.x * gridDim.x;

  /*
  printf("threadIdx.x=%d, threadIdx.y=%d, blockIdx.x=%d, blockIdy.y=%d " \
         "blockDim.x=%d, blockDim.y=%d, gridDim.x=%d\n",
         threadIdx.x, threadIdx.y,
         blockIdx.x, blockIdx.y,
         blockDim.x, blockDim.y, gridDim.x);
  */
  
  for (int i = index; i < n; i += stride)
    y[i] = x[i] + y[i];

}


int main(void)
{
  int N = 1<<20;
  float *x, *y;

  // Allocate Unified Memory – accessible from CPU or GPU
  cudaMallocManaged(&x, N*sizeof(float));
  cudaMallocManaged(&y, N*sizeof(float));

  // initialize x and y arrays on the host
  for (int i = 0; i < N; i++) {
    x[i] = 1.0f;
    y[i] = 2.0f;
  }

  // Choose selected kernel to add
  // two vectors of 1M elements each
  // on GPU
  int NrBlocks, NrThreads;

  if (VARIANT_NR==1)
  {
    printf("Launching kernel add1\n");
    NrBlocks = 1;
    NrThreads = 1;
    printf("NrBlocks=%d, NrThreads=%d\n", NrBlocks, NrThreads);
    add1<<<NrBlocks, NrThreads>>>(N, x, y);
  }

  if (VARIANT_NR==2)
  {
    printf("Launching kernel add2\n");
    NrBlocks = 1;
    NrThreads = 256;
    printf("NrBlocks=%d, NrThreads=%d\n", NrBlocks, NrThreads);
    add2<<<NrBlocks, NrThreads>>>(N, x, y);
  }

  if (VARIANT_NR==3)
  {
    printf("Launching kernel add3\n");
    NrThreads = 32;
    NrBlocks = N/NrThreads;
    if (N%NrThreads!=0)
      NrBlocks++;
    printf("NrBlocks=%d, NrThreads=%d\n", NrBlocks, NrThreads);
    add3<<<NrBlocks, NrThreads>>>(N, x, y);
  }


  // Wait for GPU to finish before accessing on host
  cudaDeviceSynchronize();

  for (int i=0; i<10; i++)
    printf("i=%d: %f\n", i, y[i]);
  for (int i=N-10; i<N; i++)
    printf("i=%d: %f\n", i, y[i]);

  // Check for errors (all values should be 3.0f)
  float maxError = 0.0f;
  for (int i = 0; i < N; i++)
    maxError = fmax(maxError, fabs(y[i]-3.0f));
  std::cout << "Max error: " << maxError << std::endl;

  // Free memory
  cudaFree(x);
  cudaFree(y);

  return 0;
}
