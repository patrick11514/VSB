// ***********************************************************************
//
// Demo program for education in subject
// Computer Architectures and Paralel Systems.
// Petr Olivka, dep. of Computer Science, FEI, VSB-TU Ostrava, 2020/11
// email:petr.olivka@vsb.cz
//
// Example of CUDA Technology Usage without unified memory.
//
// Manipulation with prepared image.
//
// ***********************************************************************

#include <stdio.h>
#include <cuda_device_runtime_api.h>
#include <cuda_runtime.h>

#include "cuda_img.h"

// Every threads identifies its position in grid and in block and modify image
__global__ void kernel_animation( CudaImg t_cuda_img )
{
    // X,Y coordinates 
    int l_y = blockDim.y * blockIdx.y + threadIdx.y;
    int l_x = blockDim.x * blockIdx.x + threadIdx.x;
    if ( l_x >= t_cuda_img.m_size.x ) return;
    if ( l_y >= t_cuda_img.m_size.y ) return;

    // Point [l_x,l_y] selection from image
    uchar3 l_bgr, l_tmp = t_cuda_img.m_p_uchar3[ l_y * t_cuda_img.m_size.x + l_x ];

    // color rotation
    l_bgr.x = l_tmp.y;
    l_bgr.y = l_tmp.z;
    l_bgr.z = l_tmp.x;

    // Store point [l_x,l_y] back to image
    t_cuda_img.m_p_uchar3[ l_y * t_cuda_img.m_size.x + l_x ] = l_bgr;
}

void cu_run_animation( CudaImg t_img, uint2 t_block_size )
{
    cudaError_t l_cerr;

    CudaImg l_cuda_pic;
    l_cuda_pic.m_size = t_img.m_size;

    // Memory allocation in GPU device
    l_cerr = cudaMalloc( &l_cuda_pic.m_p_void, l_cuda_pic.m_size.x * l_cuda_pic.m_size.y * sizeof( uchar3 ) );
    if ( l_cerr != cudaSuccess )
        printf( "CUDA Error [%d] - '%s'\n", __LINE__, cudaGetErrorString( l_cerr ) );

    // Copy data to GPU device
    l_cerr = cudaMemcpy( l_cuda_pic.m_p_void, t_img.m_p_void, l_cuda_pic.m_size.x * l_cuda_pic.m_size.y * sizeof( uchar3 ), cudaMemcpyHostToDevice );
    if ( l_cerr != cudaSuccess )
        printf( "CUDA Error [%d] - '%s'\n", __LINE__, cudaGetErrorString( l_cerr ) );

    // Grid creation with computed organization
    dim3 l_grid( ( l_cuda_pic.m_size.x + t_block_size.x - 1 ) / t_block_size.x,
                 ( l_cuda_pic.m_size.y + t_block_size.y - 1 ) / t_block_size.y );
    kernel_animation<<< l_grid, dim3( t_block_size.x, t_block_size.y ) >>>( l_cuda_pic );

    if ( ( l_cerr = cudaGetLastError() ) != cudaSuccess )
        printf( "CUDA Error [%d] - '%s'\n", __LINE__, cudaGetErrorString( l_cerr ) );

    // Copy data from GPU device to PC
    l_cerr = cudaMemcpy( t_img.m_p_void, l_cuda_pic.m_p_void, t_img.m_size.x * t_img.m_size.y * sizeof( uchar3 ), cudaMemcpyDeviceToHost );
    if ( l_cerr != cudaSuccess )
        printf( "CUDA Error [%d] - '%s'\n", __LINE__, cudaGetErrorString( l_cerr ) );

    // Free memory
    cudaFree( l_cuda_pic.m_p_void );

    cudaDeviceSynchronize();
}
