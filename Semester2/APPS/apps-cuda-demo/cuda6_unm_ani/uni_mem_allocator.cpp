
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

#include "uni_mem_allocator.h"

cv::UMatData* UniformAllocator::allocate(int dims, const int* sizes, int type,
				   void* data0, size_t* step,
				   AccessFlag_v_3_4 /*flags*/, cv::UMatUsageFlags /*usageFlags*/) const
{
	size_t total = CV_ELEM_SIZE(type);
	for (int i = dims-1; i >= 0; i--)
	{
		if (step)
		{
			if (data0 && step[i] != CV_AUTOSTEP)
			{
				CV_Assert(total <= step[i]);
				total = step[i];
			}
			else
			{
				step[i] = total;
			}
		}

		total *= sizes[i];
	}

	cv::UMatData* u = new cv::UMatData(this);
	u->size = total;

	if (data0)
	{
		u->data = u->origdata = static_cast<uchar*>(data0);
		u->flags |= cv::UMatData::USER_ALLOCATED;
	}
	else
	{
		void* ptr = 0;
		if(cudaMallocManaged(&ptr, total) != cudaSuccess)
		{
			abort();
		}


		u->data = u->origdata = static_cast<uchar*>(ptr);
	}

	return u;
}

bool UniformAllocator::allocate(cv::UMatData* u, AccessFlag_v_3_4 /*accessFlags*/, cv::UMatUsageFlags /*usageFlags*/) const
{
	return (u != NULL);
}

void UniformAllocator::deallocate(cv::UMatData* u) const
{
	if (!u)
		return;

	CV_Assert(u->urefcount >= 0);
	CV_Assert(u->refcount >= 0);

	if (u->refcount == 0)
	{
		if ( !(u->flags & cv::UMatData::USER_ALLOCATED) )
		{
			cudaFree(u->origdata);
			u->origdata = 0;
		}

		delete u;
	}
}

