// ***********************************************************************
//
// Demo program for education in subject
// Computer Architectures and Parallel Systems.
// Petr Olivka, dep. of Computer Science, FEI, VSB-TU Ostrava, 2020/11
// email:petr.olivka@vsb.cz
//
// Example of CUDA Technology Usage.
// Global variables usage in threads, the use of printf.
//
// Every thread displays information of its position in block,
// position of block in grid and global position.
//
// ***********************************************************************


#include <stdio.h>
#include <cuda_device_runtime_api.h>
#include <cuda_runtime.h>

// Demo kernel will display all global variables of grid organization.
// Warning! Function printf is available from compute capability 2.x
__global__ void thread_hierarchy()
{
    // Global variables
    // Grid dimension -             gridDim
    // Block position in grid -     blockIdx
    // Block dimension -            blockDim
    // Thread position in block -   threadIdx
    int l_x = threadIdx.x + blockIdx.x * blockDim.x;
    int l_y = threadIdx.y + blockIdx.y * blockDim.y;
    printf( "Block{%d,%d}[%d,%d] Thread{%d,%d}[%d,%d] [%d,%d]\n",
        gridDim.x, gridDim.y, blockIdx.x, blockIdx.y,
        blockDim.x, blockDim.y, threadIdx.x, threadIdx.y, l_x, l_y );
}

void cu_run_cuda( dim3 t_grid_size, dim3 t_block_size )
{
    cudaError_t l_cerr;
    // Following command can increase internal buffer for printf function
    /*l_cerr = cudaDeviceSetLimit( cudaLimitPrintfFifoSize, required_size );
    if ( l_err != cudaSuccess )
        printf( "CUDA Error [%d] - '%s'\n", __LINE__, cudaGetErrorString( l_cerr ) );
    */
    // Thread creation from selected kernel:
    // first parameter dim3 is grid dimension
    // second parameter dim3 is block dimension
    thread_hierarchy<<< t_grid_size, t_block_size >>>();

    if ( ( l_cerr = cudaGetLastError() ) != cudaSuccess )
        printf( "CUDA Error [%d] - '%s'\n", __LINE__, cudaGetErrorString( l_cerr ) );

    // Output from printf is in GPU memory. 
    // To get its contens it is necessary to synchronize device.

    cudaDeviceSynchronize();
}

