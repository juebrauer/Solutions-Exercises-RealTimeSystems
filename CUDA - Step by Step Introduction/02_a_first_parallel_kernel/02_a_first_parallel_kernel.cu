#include <stdio.h>

__global__ void add(int *a, int *b, int *c) {
  //c[blockIdx.x] = a[blockIdx.x] + b[blockIdx.x];
  c[threadIdx.x] = a[threadIdx.x] + b[threadIdx.x];
}

#define N 512
int main(void) {
  int *a, *b, *c;       // host copies of a, b, c
  int *d_a, *d_b, *d_c; // device copies of a, b, c
  int size = N * sizeof(int);

  // Alloc space for device copies of a, b, c
  cudaMalloc((void **)&d_a, size);
  cudaMalloc((void **)&d_b, size);
  cudaMalloc((void **)&d_c, size);

  // Alloc space for host copies of a, b, c and setup input values
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

  // Launch add() kernel on GPU with N blocks
  //add<<<N,1>>>(d_a, d_b, d_c);

  // Launch add() kernel on GPU with N threads
  add<<<1,N>>>(d_a, d_b, d_c);

  // Copy result back to host
  cudaMemcpy(c, d_c, size, cudaMemcpyDeviceToHost);

  printf("Resulting vector is:\n");
  for (int i=0; i<N; i++)
  {
    printf("%d ", c[i]);
  }
  printf("\n");

  // Cleanup
  free(a); free(b); free(c);
  cudaFree(d_a); cudaFree(d_b); cudaFree(d_c);
  return 0;
}
