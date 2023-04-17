// ***********************************************************************
//
// Demo program for education in subject
// Computer Architectures and Parallel Systems.
// Petr Olivka, dep. of Computer Science, FEI, VSB-TU Ostrava, 2020/11
// email:petr.olivka@vsb.cz
//
// Example of CUDA Technology Usage without unified memory.
//
// Simple animation.
// Image manipulation is performed by OpenCV library. 
//
// ***********************************************************************

#include <stdio.h>
#include <sys/time.h>
#include <cuda_device_runtime_api.h>
#include <cuda_runtime.h>
#include <opencv2/opencv.hpp>

#include "uni_mem_allocator.h"
#include "cuda_img.h"
#include "animation.h"

// Function prototype from .cu file

int main( int t_numarg, char **t_arg )
{
	// Uniform Memory allocator for Mat
	UniformAllocator allocator;
	cv::Mat::setDefaultAllocator( &allocator );

	Animation l_animation;

	// Output image
	cv::Mat l_animation_cv_img( 777, 888, CV_8UC3 );
	// Ball image
	cv::Mat l_ball_cv_img = cv::imread( "ball.png", cv::IMREAD_UNCHANGED ); // CV_LOAD_IMAGE_UNCHANGED );

	printf( "ball channels %d\n", l_ball_cv_img.channels() );

	// Data for CUDA
	CudaImg l_animation_cuda_img, l_ball_cuda_img;

	l_animation_cuda_img.m_size.x = l_animation_cv_img.cols;
	l_animation_cuda_img.m_size.y = l_animation_cv_img.rows;
	l_animation_cuda_img.m_p_uchar3 = ( uchar3 * ) l_animation_cv_img.data;

	l_ball_cuda_img.m_size.x = l_ball_cv_img.cols;
	l_ball_cuda_img.m_size.y = l_ball_cv_img.rows;
	l_ball_cuda_img.m_p_uchar4 = ( uchar4 * ) l_ball_cv_img.data;

	// Prepare data for animation
	l_animation.start( l_animation_cuda_img, l_ball_cuda_img );

	// simulation of a bouncing ball
	// 1 pixel ~ 1 mm
	float l_bump_up = 0.8;
	float l_g = 9.81;
	float l_v_z = 0;	// vertical speed m/s
	float l_h_z = 0.001 * ( l_animation_cv_img.rows + l_ball_cv_img.rows ); // height
	float l_direction = -1.0;
	int l_dir_changed = 0;
	int l_iterations = 0;
	int l_run_simulation = 1;

	timeval l_start_time, l_cur_time, l_old_time, l_delta_time;
	gettimeofday( &l_old_time, NULL );
	l_start_time = l_old_time;

	while ( l_run_simulation )
	{
		cv::waitKey( 1 );

		// time measuring
		gettimeofday( &l_cur_time, NULL );
		timersub( &l_cur_time, &l_old_time, &l_delta_time );
		if ( l_delta_time.tv_usec < 1000 ) continue; // too short time
		l_old_time = l_cur_time;
		float l_delta_sec = ( float ) l_delta_time.tv_usec / 1E6; // time in seconds

		l_iterations++;
		// height and speed computation
		l_h_z = l_h_z + l_v_z * l_delta_sec;
		l_v_z = l_v_z - l_g * l_delta_sec;

		// direction changed?
		if ( l_direction < 0 && l_h_z <= 0 )
		{
			// decrease speed of a bounced ball
			l_v_z = -l_v_z * l_bump_up;
			//l_h_z = 0;

			l_direction = 1.0;
			l_dir_changed++;
		}
		else if ( l_direction > 0 && l_v_z <= 0 )
		{
			// simulation limit is jump 2 mm (2 pixels)
			if ( l_h_z < 0.002 ) l_run_simulation = l_h_z = 0;

			l_direction = -1.0;
			l_dir_changed++;
		}

		// bottom position of ball
		int l_z = l_animation_cv_img.rows - l_h_z * 1000;
		// position of the ball image
		l_z -= l_ball_cv_img.rows;

		l_animation.next( l_animation_cuda_img, { l_animation_cv_img.cols - 3 * l_ball_cv_img.rows / 2, l_z } );

		cv::imshow( "Bouncing Ball", l_animation_cv_img );
	}

	l_animation.stop();

	gettimeofday( &l_cur_time, NULL );
	timersub( &l_cur_time, &l_start_time, &l_delta_time );
	int l_delta_ms = l_delta_time.tv_sec * 1000 + l_delta_time.tv_usec / 1000;

	printf( "Ball stopped after %d iterations\n", l_iterations );
	printf( "The whole simulation time %d ms.\n", l_delta_ms );
	printf( "The ball direction changed %d times.\n", l_dir_changed );

	cv::waitKey( 0 );
}

