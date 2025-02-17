// ***********************************************************************
//
// Demo program for education in subject
// Computer Architectures and Parallel Systems
// Petr Olivka, dep. of Computer Science, FEI, VSB-TU Ostrava, 2020/11
// email:petr.olivka@vsb.cz
//
// Example of CUDA Technology usage without unified memory.
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
    int l_inx = blockDim.x * blockIdx.x + threadIdx.x;
    // if grid is greater then length of array...
    if ( l_inx >= t_length ) return;

    t_array[ l_inx ] *= t_mult;
}

void cu_run_mult( float *t_array, int t_length, float t_mult )
{
    cudaError_t l_cerr;
    int l_threads = 128;
    int l_blocks = ( t_length + l_threads - 1 ) / l_threads;

    // Memory allocation in GPU device
    float *l_cuda_array;
    l_cerr = cudaMalloc( &l_cuda_array, t_length * sizeof( float ) );
    if ( l_cerr != cudaSuccess )
        printf( "CUDA Error [%d] - '%s'\n", __LINE__, cudaGetErrorString( l_cerr ) );

    // Copy data from PC to GPU device
    l_cerr = cudaMemcpy( l_cuda_array, t_array, t_length * sizeof( float ), cudaMemcpyHostToDevice );
    if ( l_cerr != cudaSuccess )
        printf( "CUDA Error [%d] - '%s'\n", __LINE__, cudaGetErrorString( l_cerr ) );

    // Grid creation
    kernel_mult<<< l_blocks, l_threads >>>( l_cuda_array, t_length, t_mult );

    if ( ( l_cerr = cudaGetLastError() ) != cudaSuccess )
        printf( "CUDA Error [%d] - '%s'\n", __LINE__, cudaGetErrorString( l_cerr ) );

    // Copy data from GPU device to PC
    l_cerr = cudaMemcpy( t_array, l_cuda_array, t_length * sizeof( float ), cudaMemcpyDeviceToHost );
    if ( l_cerr != cudaSuccess )
        printf( "CUDA Error [%d] - '%s'\n", __LINE__, cudaGetErrorString( l_cerr ) );

    // Free memory
    cudaFree( l_cuda_array );
}
