#include <stdio.h>

const int N_in=86;
const int RADIUS=3;

const int N_out=N_in-2*RADIUS;

const int NR_BLOCKS  = 8;
const int NR_THREADS = 10;



__global__ void stencil_1d(int *in, int *out) {

 // 1. define shared memory
 __shared__ int temp[NR_THREADS + 2 * RADIUS];

 // 2. compute global (in-array) and
 //    local array (temp-array) indices
 int gindex = threadIdx.x + blockIdx.x * blockDim.x + RADIUS;
 int lindex = threadIdx.x + RADIUS;

 // 3. Read one more input element from
 // input array <in> into shared memory
 temp[lindex] = in[gindex];

 // 4. Make sure, also the "border" elements
 //    are read in
 if (threadIdx.x < RADIUS) {
   temp[lindex - RADIUS]     = in[gindex - RADIUS];
   temp[lindex + NR_THREADS] = in[gindex + NR_THREADS];
 }

// 5. Make sure, data is already copied
//    completely to <temp> buffer, in order
//    to compute stencil in next step
 __syncthreads();

 // 6. Compute stencil result
 int result = 0;
 for (int offset = -RADIUS ; offset <= RADIUS ; offset++)
  result += temp[lindex + offset];

 // 7. Store the result
 //printf("Thread %d in block %d (gindex=%d) has computed result=%d\n",
 //        threadIdx.x, blockIdx.x, gindex, result);
 out[gindex - RADIUS] = result;
}



int main(void) {

  // 1. Prepare host and device memory structures
  int *in, *out;     // host memory
  int *d_in, *d_out; // device memory
  int size_in  = N_in  * sizeof(int);
  int size_out = N_out * sizeof(int);


  // 2. Alloc space for device memory
  cudaMalloc((void **)&d_in, size_in);
  cudaMalloc((void **)&d_out, size_out);


  // 3. Alloc space for host memory
  in  = (int *)malloc(size_in);
  out = (int *)malloc(size_out);
  for (int i=0; i<N_in; i++)
    in[i] = 1;

  for (int i=0; i<N_out; i++)
    out[i] = 0;



  // 4. Copy host memory to device memory
  cudaMemcpy(d_in,   in, size_in,  cudaMemcpyHostToDevice);
  cudaMemcpy(d_out, out, size_out, cudaMemcpyHostToDevice);


  // 5. Launch stencil_1d() kernel on GPU
  printf("NR_BLOCKS:  %d\n", NR_BLOCKS);
  printf("NR_THREADS: %d\n", NR_THREADS);
  stencil_1d<<<NR_BLOCKS,NR_THREADS>>>(d_in, d_out);


  // 6. Copy result back from device to host
  cudaMemcpy(out, d_out, size_out, cudaMemcpyDeviceToHost);


  // 7. Show result (output) array
  printf("\nHere is the result:\n");
  for (int i=0; i<N_out; i++)
  {
    printf("%d ", out[i]);
  }
  printf("\n");

  // 8. Cleanup host and device memory
  free(in); free(out);
  cudaFree(d_in); cudaFree(d_out);


  // 9. Finished!
  return 0;
}
