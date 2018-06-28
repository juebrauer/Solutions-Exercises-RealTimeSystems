// file: matrix_multiply_gmem.cu
//
// shows how to multiply two matrices A,B
// by making use of a GPU
//


#include <iostream>
#include <stdio.h>
using namespace std;

namespace help {
    template <class T>
    inline void print_array1d(T a, int n){
        for(int i=0;i<n;++i){
            std::cout<<a[i]<<' ';
        }
        std::cout<<'\n';
    }

    template <class T>
    inline void print_array2d(string str, T a, int n, int m){
        std::cout << str << ":\n";
        for(int i=0;i<n;++i){
            for(int j=0; j<m; ++j){
                std::cout<<a[j+i*m]<<' ';
            }
            std::cout<<'\n';
        }
    }

} // namespace help


// Matrices are stored in row-major order:
// M(row, col) = *(M.elements + row * M.width + col)

typedef struct {
    int width;
    int height;
    float* elements;
} Matrix;

// Thread block size
#define BLOCK_SIZE 32


// Matrix multiplication kernel called by MatMul()
__global__ void MatMulKernel(Matrix A, Matrix B, Matrix C)
{
    // Each thread computes one element of C
    // by accumulating results into Cvalue
    float Cvalue = 0;
    int row = blockIdx.y * blockDim.y + threadIdx.y;
    int col = blockIdx.x * blockDim.x + threadIdx.x;
    if(row>=C.height||col>=C.width)
        return;
    for (int e = 0; e < A.width; ++e)
        Cvalue += A.elements[row * A.width + e]
                * B.elements[e * B.width + col];
    C.elements[row * C.width + col] = Cvalue;
}


// Matrix multiplication - Host code
// Matrix dimensions are assumed to be multiples of BLOCK_SIZE
void MatMul(const Matrix A, const Matrix B, Matrix C)
{
    // 1. Copy matrix A to device memory
    Matrix d_A;
    d_A.width = A.width; d_A.height = A.height;
    size_t size = A.width * A.height * sizeof(float);
    cudaMalloc(&d_A.elements, size);
    cudaMemcpy(d_A.elements, A.elements, size,
               cudaMemcpyHostToDevice);

    // 2. Copy matrix B to device memory
    Matrix d_B;
    d_B.width = B.width; d_B.height = B.height;
    size = B.width * B.height * sizeof(float);
    cudaMalloc(&d_B.elements, size);
    cudaMemcpy(d_B.elements, B.elements, size,
               cudaMemcpyHostToDevice);

    // 3. Allocate device memory for matrix C
    Matrix d_C;
    d_C.width = C.width; d_C.height = C.height;
    size = C.width * C.height * sizeof(float);
    cudaMalloc(&d_C.elements, size);

    // 4. Setup configuration

    dim3 dimBlock(BLOCK_SIZE, BLOCK_SIZE);

    int gdimx = (B.width+dimBlock.x-1) / dimBlock.x;
    int gdimy = (A.height+dimBlock.y-1) / dimBlock.y;
    dim3 dimGrid(gdimx, gdimy);

    printf("dimBlock(%d,%d)\n", BLOCK_SIZE,BLOCK_SIZE);
    printf("dimGrid(%d,%d)\n", gdimx, gdimy );

    // 5. Launch the kernel
    MatMulKernel<<<dimGrid, dimBlock>>>(d_A, d_B, d_C);

    // 6. Copy result matrix from device memory to host memory
    cudaMemcpy(C.elements, d_C.elements, size,
               cudaMemcpyDeviceToHost);

    // 7. Free device memory
    cudaFree(d_A.elements);
    cudaFree(d_B.elements);
    cudaFree(d_C.elements);
}



int main(){

    // 1. prepare memory for host matrix A
    Matrix A, B, C;
    A.height=100; A.width=100;
    size_t N_A=A.width * A.height;
    size_t sizeA = A.width * A.height * sizeof(float);
    A.elements=(float *) malloc(sizeA);

    // 2. prepare memory for host matrix B
    B.height=A.width; B.width=100;
    size_t N_B=B.width * B.height;
    size_t sizeB = B.width * B.height * sizeof(float);
    B.elements=(float *) malloc(sizeB);

    // 3. prepare memory for host matrix C
    C.height=A.height; C.width=B.width;
    size_t N_C=C.width * C.height;
    // size_t sizeC = C.width * C.height * sizeof(float);
    // C.elements=(float *) malloc(sizeC);
    C.elements = new float[N_C]();

    // 4. initialize matrix A and B
    for(int i=0;i<N_A;++i){
        A.elements[i]=2.0;
    }
    for(int i=0;i<N_B;++i){
        B.elements[i]=5.0f;
    }

    // 5. print input matrices (if matrices are small)
    if ((A.height<=20) && (A.width<=20) && (B.width<=20))
    {
      help::print_array2d<float *>("A", A.elements,A.height, A.width);
      help::print_array2d<float *>("B", B.elements,B.height, B.width);
      help::print_array2d<float *>("C", C.elements,C.height, C.width);
    }

    // 6. do the matrix multiplication on the GPU
    MatMul(A, B, C);

    // 7. print multiplication result matrix C (if matrix is small))
    if ((C.height<=20) && (C.width<=20))
      help::print_array2d<float *>("C", C.elements,C.height, C.width);

    // 8. free host memory
    free(A.elements);free(B.elements);
    delete[] C.elements;

} // main
