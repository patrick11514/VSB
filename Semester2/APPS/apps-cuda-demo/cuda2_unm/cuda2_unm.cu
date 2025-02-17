/// ***********************************************************************
//
// Demo program for education in subject
// Computer Architectures and Parallel Systems
// Petr Olivka, dep. of Computer Science, FEI, VSB-TU Ostrava, 2020/11
// email:petr.olivka@vsb.cz
//
// Example of CUDA Technology usage with unified memory.
//
// Multiplication of elements in float array.
//
// ***********************************************************************

#include <stdio.h>
#include <cuda_device_runtime_api.h>
#include <cuda_runtime.h>

// Demo kernel for array elements multiplication.
// Every thread selects one element and multiply it. 
__global__ void kernel_mult( float *t_array, int t_length, float t_mult )
{
    int inx = blockDim.x * blockIdx.x + threadIdx.x;
    // if grid is greater then length of array...
    if ( inx >= t_length ) return;

    t_array[ inx ] *= t_mult;
}

void cu_run_mult( float *t_array, int t_length, float t_mult )
{
    cudaError_t l_cerr;
    int l_threads = 128;
    int l_blocks = ( t_length + l_threads - 1 ) / l_threads;

    // Grid creation
    kernel_mult<<< l_blocks, l_threads >>>( t_array, t_length, t_mult );

    if ( ( l_cerr = cudaGetLastError() ) != cudaSuccess )
        printf( "CUDA Error [%d] - '%s'\n", __LINE__, cudaGetErrorString( l_cerr ) );

    cudaDeviceSynchronize();
}
