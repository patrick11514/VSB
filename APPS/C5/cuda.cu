// ***********************************************************************
//
// Demo program for education in subject
// Computer Architectures and Parallel Systems.
// Petr Olivka, dep. of Computer Science, FEI, VSB-TU Ostrava, 2020/11
// email:petr.olivka@vsb.cz
//
// Example of CUDA Technology Usage wit unified memory.
// Image transformation from RGB to BW schema. 
//
// ***********************************************************************

#include <stdio.h>
#include <cuda_device_runtime_api.h>
#include <cuda_runtime.h>

#include "cuda_img.h"

__global__ void kernel_mirror( CudaImg from, CudaImg to )
{
    // X,Y coordinates and check image dimensions
    int l_y = blockDim.y * blockIdx.y + threadIdx.y;
    int l_x = blockDim.x * blockIdx.x + threadIdx.x;
    if ( l_y >= from.m_size.y ) return;
    if ( l_x >= from.m_size.x ) return;

    to.m_p_uchar3[l_y * to.m_size.x + (to.m_size.x - l_x)] = from.m_p_uchar3[l_y * from.m_size.x + l_x];
}

void cu_create_mirror( CudaImg from, CudaImg to )
{
    cudaError_t l_cerr;

    int l_block_size = 32;
    dim3 l_blocks( ( from.m_size.x + l_block_size - 1 ) / l_block_size,
                   ( from.m_size.y + l_block_size - 1 ) / l_block_size );
    dim3 l_threads( l_block_size, l_block_size );
    kernel_mirror<<< l_blocks, l_threads >>>( from, to );

    if ( ( l_cerr = cudaGetLastError() ) != cudaSuccess )
        printf( "CUDA Error [%d] - '%s'\n", __LINE__, cudaGetErrorString( l_cerr ) );

    cudaDeviceSynchronize();
}

// ==============================

__global__ void kernel_dark( CudaImg from, CudaImg to )
{
    // X,Y coordinates and check image dimensions
    int l_y = blockDim.y * blockIdx.y + threadIdx.y;
    int l_x = blockDim.x * blockIdx.x + threadIdx.x;
    if ( l_y >= from.m_size.y ) return;
    if ( l_x >= from.m_size.x ) return;

    to.m_p_uchar3[l_y * to.m_size.x + l_x].x = from.m_p_uchar3[l_y * from.m_size.x + l_x].x * ((float) l_y/from.m_size.y);
    to.m_p_uchar3[l_y * to.m_size.x + l_x].y = from.m_p_uchar3[l_y * from.m_size.x + l_x].y * ((float) l_y/from.m_size.y);
    to.m_p_uchar3[l_y * to.m_size.x + l_x].z = from.m_p_uchar3[l_y * from.m_size.x + l_x].z * ((float) l_y/from.m_size.y);
}

void cu_create_dark( CudaImg from, CudaImg to )
{
    cudaError_t l_cerr;

    int l_block_size = 32;
    dim3 l_blocks( ( from.m_size.x + l_block_size - 1 ) / l_block_size,
                   ( from.m_size.y + l_block_size - 1 ) / l_block_size );
    dim3 l_threads( l_block_size, l_block_size );
    kernel_dark<<< l_blocks, l_threads >>>( from, to );

    if ( ( l_cerr = cudaGetLastError() ) != cudaSuccess )
        printf( "CUDA Error [%d] - '%s'\n", __LINE__, cudaGetErrorString( l_cerr ) );

    cudaDeviceSynchronize();
}