#include <iostream>
#include <fstream>
#include <ostream>
#include <math.h>
#include <stdint.h>

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

    int index_1d = x*BYTES_PER_PIXEL + y*WIDTH*BYTES_PER_PIXEL + z;

    pic[index_1d] = (uint8_t) blockIdx.x;
    pic[index_1d+1] = (uint8_t) blockIdx.y;
    pic[index_1d+2] = (uint8_t) blockIdx.z;
}

bool write_ppm_file(const std::string& filename, const uint8_t* data, int width=WIDTH, int height=HEIGHT) {
    std::ofstream outfile(filename, std::ios::out | std::ios::binary);

    if (!outfile.is_open()) {
        std::cerr << "Error: Could not open file " << filename << " for writing." << std::endl;
        return false;
    }

    outfile << "P6\n"; 

    outfile << width << " " << height << "\n"; 
    outfile << "255\n"; 

    size_t data_size = (size_t)width * height * 3;
    
    outfile.write(reinterpret_cast<const char*>(data), data_size);

    if (outfile.fail()) {
        std::cerr << "Error: Failed writing data to " << filename << std::endl;
        outfile.close();
        return false;
    }

    outfile.close();
    std::cout << "Successfully wrote " << width << "x" << height << " PPM file to " << filename << std::endl;
    return true;
}

int main(void)
{
    int N = 1<<20;
    uint8_t *pic;



    cudaMallocManaged(&pic, 3*WIDTH*HEIGHT*sizeof(uint8_t));
    
    dim3 threadsPerBlock(16, 16, 3);
    dim3 numBlocks(
            (WIDTH + threadsPerBlock.x - 1)/threadsPerBlock.x,
            (HEIGHT + threadsPerBlock.y - 1)/threadsPerBlock.y,
            (BYTES_PER_PIXEL + threadsPerBlock.z - 1)/threadsPerBlock.z);


    // Run kernel on 1M elements on the GPU
    add<<<numBlocks, threadsPerBlock>>>(pic);
    
    write_ppm_file("test.ppm", pic);

    // Wait for GPU to finish before accessing on host
    cudaDeviceSynchronize();

    // Free memory
    cudaFree(pic);
    return 0;
}

