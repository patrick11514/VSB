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

void cu_create_overlap( CudaImg one, CudaImg two, int x, int y );
void cu_create_resize( CudaImg input, CudaImg output );

int main( int t_numarg, char **t_arg )
{
    // Uniform Memory allocator for Mat
    UniformAllocator allocator;
    cv::Mat::setDefaultAllocator( &allocator );

    // some argument?
    if ( t_numarg > 2 )
    {
        // Load images
        cv::Mat one_cv = cv::imread( t_arg[ 1 ], cv::IMREAD_COLOR );

        cv::Mat two_cv = cv::imread( t_arg[ 2 ], cv::IMREAD_UNCHANGED  ); 

        if ( !one_cv.data || !two_cv.data )
            printf( "Unable to read file(s) '%s' or '%s'\n", t_arg[ 1 ], t_arg[ 2 ] );

        else
        {
            // insert loaded image
            CudaImg one;
            one.m_size.x = one_cv.cols;
            one.m_size.y = one_cv.rows;
            one.m_p_uchar3 = ( uchar3 * ) one_cv.data;

            CudaImg two;
            two.m_size.x = two_cv.cols;
            two.m_size.y = two_cv.rows;
            two.m_p_uchar4 = ( uchar4 * ) two_cv.data;

            int resizedWidth = two_cv.cols / 2;
            int resizedHeight = two_cv.rows / 2;

            cv::Mat resized_cv(resizedHeight, resizedWidth, CV_8UC4 );

            CudaImg resized;
            resized.m_size.x = resized_cv.cols;
            resized.m_size.y = resized_cv.rows;
            resized.m_p_uchar4 = ( uchar4 * ) resized_cv.data;

            cu_create_overlap(one, two, 300, 10);

            //cu_create_resize(two, resized );

            //cv::imshow( "Image", resized_cv );
            //cv::imshow( "Image2", two_cv );

            cv::imwrite("output.jpg", one_cv);
        }
    }

    cv::waitKey( 0 );
}