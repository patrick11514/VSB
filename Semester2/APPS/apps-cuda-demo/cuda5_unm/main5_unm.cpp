// ***********************************************************************
//
// Demo program for education in subject
// Computer Architectures and Parallel Systems.
// Petr Olivka, dep. of Computer Science, FEI, VSB-TU Ostrava, 2020/11
// email:petr.olivka@vsb.cz
//
// Example of CUDA Technology Usage with unified memory.
//
// Image transformation from RGB to BW schema. 
// Image manipulation is performed by OpenCV library. 
//
// ***********************************************************************

#include <stdio.h>
#include <cuda_device_runtime_api.h>
#include <cuda_runtime.h>
#include <opencv2/opencv.hpp>

#include "uni_mem_allocator.h"
#include "cuda_img.h"

// Function prototype from .cu file
void cu_create_chessboard( CudaImg t_color_cuda_img, int t_square_size );
void cu_create_alphaimg( CudaImg t_color_cuda_img, uchar3 t_color );
void cu_insertimage( CudaImg t_big_cuda_img, CudaImg t_small_cuda_img, int2 t_position );

int main( int t_numarg, char **t_arg )
{
    // Uniform Memory allocator for Mat
    UniformAllocator allocator;
    cv::Mat::setDefaultAllocator( &allocator );

    cv::Mat l_chessboard_cv_img( 511, 515, CV_8UC3 );

    CudaImg l_chessboard_cuda_img;
    l_chessboard_cuda_img.m_size.x = l_chessboard_cv_img.cols;
    l_chessboard_cuda_img.m_size.y = l_chessboard_cv_img.rows;
    l_chessboard_cuda_img.m_p_uchar3 = ( uchar3 * ) l_chessboard_cv_img.data;

    cu_create_chessboard( l_chessboard_cuda_img, 21 );

    cv::imshow( "Chess Board", l_chessboard_cv_img );

    cv::Mat l_alphaimg_cv_img( 211, 191, CV_8UC4 );

    CudaImg l_alphaimg_cuda_img;
    l_alphaimg_cuda_img.m_size.x = l_alphaimg_cv_img.cols;
    l_alphaimg_cuda_img.m_size.y = l_alphaimg_cv_img.rows;
    l_alphaimg_cuda_img.m_p_uchar4 = ( uchar4 * ) l_alphaimg_cv_img.data;

    cu_create_alphaimg( l_alphaimg_cuda_img, { 0, 0, 255 } );

    cv::imshow( "Alpha channel", l_alphaimg_cv_img );

    cu_insertimage( l_chessboard_cuda_img, l_alphaimg_cuda_img, { 11, 23 } );

    cv::imshow( "Result I", l_chessboard_cv_img );

    // some argument?
    if ( t_numarg > 1 )
    {
        // Load image
        cv::Mat l_bgra_cv_img = cv::imread( t_arg[ 1 ], cv::IMREAD_UNCHANGED ); // CV_LOAD_IMAGE_UNCHANGED );

        if ( !l_bgra_cv_img.data )
            printf( "Unable to read file '%s'\n", t_arg[ 1 ] );

        else if ( l_bgra_cv_img.channels() != 4 )
            printf( "Image does not contain alpha channel!\n" );

        else
        {
            // insert loaded image
            CudaImg l_bgra_cuda_img;
            l_bgra_cuda_img.m_size.x = l_bgra_cv_img.cols;
            l_bgra_cuda_img.m_size.y = l_bgra_cv_img.rows;
            l_bgra_cuda_img.m_p_uchar4 = ( uchar4 * ) l_bgra_cv_img.data;

            cu_insertimage( l_chessboard_cuda_img, l_bgra_cuda_img, { ( int ) l_chessboard_cuda_img.m_size.x / 2, ( int ) l_chessboard_cuda_img.m_size.y / 2 } );

            cv::imshow( "Result II", l_chessboard_cv_img );
        }
    }

    cv::waitKey( 0 );
}

