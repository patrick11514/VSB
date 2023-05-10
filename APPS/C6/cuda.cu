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

__global__ void kernel_overlap( CudaImg one, CudaImg two, int x, int y )
{
    // X,Y coordinates and check image dimensions
    int l_y = blockDim.y * blockIdx.y + threadIdx.y;
    int l_x = blockDim.x * blockIdx.x + threadIdx.x;
    if ( l_y >= one.m_size.y ) return;
    if ( l_x >= one.m_size.x ) return;

    if (l_x < x || l_y < y || l_x > (x + two.m_size.x) || l_y > (y + two.m_size.y)) return;

    if (two.m_p_uchar4[(l_y - y) * two.m_size.x + (l_x - x)].w == 0) return;

    one.m_p_uchar3[l_y * one.m_size.x + l_x].x = two.m_p_uchar4[(l_y - y) * two.m_size.x + (l_x - x)].x;
    one.m_p_uchar3[l_y * one.m_size.x + l_x].y = two.m_p_uchar4[(l_y - y) * two.m_size.x + (l_x - x)].y;
    one.m_p_uchar3[l_y * one.m_size.x + l_x].z = two.m_p_uchar4[(l_y - y) * two.m_size.x + (l_x - x)].z;
}

void cu_create_overlap( CudaImg one, CudaImg two, int x, int y )
{
    cudaError_t l_cerr;

    int l_block_size = 32;
    dim3 l_blocks( ( one.m_size.x + l_block_size - 1 ) / l_block_size,
                   ( one.m_size.y + l_block_size - 1 ) / l_block_size );
    dim3 l_threads( l_block_size, l_block_size );
    kernel_overlap<<< l_blocks, l_threads >>>( one, two, x, y );

    if ( ( l_cerr = cudaGetLastError() ) != cudaSuccess )
        printf( "CUDA Error [%d] - '%s'\n", __LINE__, cudaGetErrorString( l_cerr ) );

    cudaDeviceSynchronize();
}

// ==============================

__global__ void kernel_resize( CudaImg input, CudaImg output )
{
    // X,Y coordinates and check image dimensions
    int l_y = blockDim.y * blockIdx.y + threadIdx.y;
    int l_x = blockDim.x * blockIdx.x + threadIdx.x;
    if ( l_y >= output.m_size.y ) return;
    if ( l_x >= output.m_size.x ) return;

    //int xRatio = input.m_size.x / width;
    //int yRatio = input.m_size.y / height;

    //int pos = (l_y / yRatio) * output.m_size.x + (l_x / xRatio);

    //printf("%d\n", pos);

    //output.m_p_uchar4[pos].x = (output.m_p_uchar4[pos].x + input.m_p_uchar4[l_y * input.m_size.x + l_x].x) /2;
    //output.m_p_uchar4[pos].y = (output.m_p_uchar4[pos].y + input.m_p_uchar4[l_y * input.m_size.x + l_x].y) /2;
    //output.m_p_uchar4[pos].z = (output.m_p_uchar4[pos].z + input.m_p_uchar4[l_y * input.m_size.x + l_x].z) /2;
    //output.m_p_uchar4[pos].w = (output.m_p_uchar4[pos].w + input.m_p_uchar4[l_y * input.m_size.x + l_x].w) /2;
}

void cu_create_resize( CudaImg input, CudaImg output )
{
    cudaError_t l_cerr;

    int l_block_size = 32;
    dim3 l_blocks( ( output.m_size.x + l_block_size - 1 ) / l_block_size,
                   ( output.m_size.y + l_block_size - 1 ) / l_block_size );
    dim3 l_threads( l_block_size, l_block_size );
    kernel_resize<<< l_blocks, l_threads >>>( input, output );

    if ( ( l_cerr = cudaGetLastError() ) != cudaSuccess )
        printf( "CUDA Error [%d] - '%s'\n", __LINE__, cudaGetErrorString( l_cerr ) );

    cudaDeviceSynchronize();
}