// ***********************************************************************
//
// Demo program for education in subject
// Computer Architectures and Parallel Systems.
// Petr Olivka, dep. of Computer Science, FEI, VSB-TU Ostrava, 2020/11
// email:petr.olivka@vsb.cz
//
// Uniform memory allocator for class Mat
//
// ***********************************************************************

#pragma once

#include <cuda_runtime.h>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core_c.h>
#include <opencv2/core/mat.hpp>

#if CV_MAJOR_VERSION >= 4
typedef cv::AccessFlag AccessFlag_v_3_4;
#else
typedef int AccessFlag_v_3_4;
#endif

class UniformAllocator : public cv::MatAllocator {
public:
	cv::UMatData* allocate(int dims, const int* sizes, int type,
                       void* data0, size_t* step,
                       AccessFlag_v_3_4 /*flags*/, cv::UMatUsageFlags /*usageFlags*/) const override;

    bool allocate(cv::UMatData* u,  AccessFlag_v_3_4 /*accessFlags*/, cv::UMatUsageFlags /*usageFlags*/) const override;

    void deallocate(cv::UMatData* u) const override;

};
