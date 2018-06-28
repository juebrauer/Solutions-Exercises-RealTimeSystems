#include <stdio.h>

#define N (1024*2048)
#define THREADS_PER_BLOCK 512


__global__ void add(int *a, int *b, int *c, int n) {
 int index = threadIdx.x + blockIdx.x * blockDim.x;
 c[index] = a[index]+b[index];
}

int main(void) {

  int NrBlocks = N/THREADS_PER_BLOCK;
  printf("Launching kernel with:\n");
  printf("\t NrBlocks: %d\n", NrBlocks);
  printf("\t THREADS_PER_BLOCK: %d\n", THREADS_PER_BLOCK);

  int *a, *b, *c;       // host copies of a, b, c
  int *d_a, *d_b, *d_c; // device copies of a, b, c
  int size = N * sizeof(int);

  // Alloc space for device copies of a, b, c
  cudaMalloc((void **)&d_a, size);
  cudaMalloc((void **)&d_b, size);
  cudaMalloc((void **)&d_c, size);

  // Alloc space for host copies of a, b, c and setup input values
  printf("Preparing vectors of size %d\n", N);
  a = (int *)malloc(size);
  b = (int *)malloc(size);
  c = (int *)malloc(size);
  for (int i=0; i<N; i++)
  {
    a[i] = 1;
    b[i] = 4;
    c[i] = 0;
  }

  // Copy inputs to device
  cudaMemcpy(d_a, a, size, cudaMemcpyHostToDevice);
  cudaMemcpy(d_b, b, size, cudaMemcpyHostToDevice);


  // Launch add() kernel on GPU with several
  // blocks and THREADS_PER_BLOCK many threads
  // per block
  add<<<NrBlocks,THREADS_PER_BLOCK>>>(d_a, d_b, d_c, N);


  // Copy result back to host
  cudaMemcpy(c, d_c, size, cudaMemcpyDeviceToHost);

  printf("\nFirst 10 elements of resulting vector are:\n");
  for (int i=0; i<10; i++)
  {
    printf("%d ", c[i]);
  }
  printf("\nLast 10 elements of resulting vector are:\n");
  for (int i=N-10; i<N; i++)
  {
    printf("%d ", c[i]);
  }
  printf("\n");

  // Cleanup
  free(a); free(b); free(c);
  cudaFree(d_a); cudaFree(d_b); cudaFree(d_c);
  return 0;
}
