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

void cu_create_mirror( CudaImg from, CudaImg to );

void cu_create_dark( CudaImg from, CudaImg to );

int main( int t_numarg, char **t_arg )
{
    // Uniform Memory allocator for Mat
    UniformAllocator allocator;
    cv::Mat::setDefaultAllocator( &allocator );

    // some argument?
    if ( t_numarg > 1 )
    {
        // Load image
        cv::Mat from_cv_img = cv::imread( t_arg[ 1 ], cv::IMREAD_COLOR ); // CV_LOAD_IMAGE_UNCHANGED );

        if ( !from_cv_img.data )
            printf( "Unable to read file '%s'\n", t_arg[ 1 ] );

        else
        {
            // insert loaded image
            CudaImg from;
            from.m_size.x = from_cv_img.cols;
            from.m_size.y = from_cv_img.rows;
            from.m_p_uchar3 = ( uchar3 * ) from_cv_img.data;

            cv::Mat cv_to_img(from_cv_img.rows, from_cv_img.cols, CV_8UC3 );

            CudaImg to;
            to.m_size.x = cv_to_img.cols;
            to.m_size.y = cv_to_img.rows;
            to.m_p_uchar3 = ( uchar3 * ) cv_to_img.data;

            //cu_create_mirror(from, to);
            cu_create_dark(from, to);

            cv::imshow( "Image", cv_to_img );
        }
    }

    cv::waitKey( 0 );
}