#include <iostream>
#include <fstream>
#include <ostream>
#include <math.h>
 
const uint32_t WIDTH = 1280;
const uint32_t HEIGHT = 720;
const uint32_t BYTES_PER_PIXEL = 3;
const uint32_t SIZE = HEIGHT*WIDTH*BYTES_PER_PIXEL;


// Kernel function to add the elements of two arrays
__global__
void add(uint8_t *pic)
{
    int x = blockIdx.x * blockDim.x + threadIdx.x;
    int y = blockIdx.y * blockDim.y + threadIdx.y;
    int z = blockIdx.z * blockDim.z + threadIdx.z;

    int index_1d = x + y*WIDTH*BYTES_PER_PIXEL + 
}
 
int main(void)
{
    int N = 1<<20;
    uint8_t *pic;



    cudaMallocManaged(&pic, 3*width*height*sizeof(float));
    
    // initialize x and y arrays on the host
    for (int i = 0; i < N; i++) {
        x[i] = 1.0f;
        y[i] = 2.0f;
    }
    
    // Run kernel on 1M elements on the GPU
    add<<<1, 1>>>(N, x, y);
    
    // Wait for GPU to finish before accessing on host
    cudaDeviceSynchronize();
    
    // Check for errors (all values should be 3.0f)
    float maxError = 0.0f;
    for (int i = 0; i < N; i++) {
    maxError = fmax(maxError, fabs(y[i]-3.0f));
    }
    std::cout << "Max error: " << maxError << std::endl;
    
    // Free memory
    cudaFree(x);
    cudaFree(y);
    return 0;
}

