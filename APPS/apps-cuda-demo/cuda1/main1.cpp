// ***********************************************************************
//
// Demo program for education in subject
// Computer Architectures and Parallel Systems.
// Petr Olivka, dep. of Computer Science, FEI, VSB-TU Ostrava, 2020/11
// email:petr.olivka@vsb.cz
//
// Example of CUDA Technology Usage.
// Global variables usage in threads, the use of printf.
//
// ***********************************************************************

#include <stdio.h>
#include <cuda_device_runtime_api.h>
#include <cuda_runtime.h>

// Prototype of function from .cu file
void cu_run_cuda( dim3 t_grid_size, dim3 t_block_size );

int main()
{
    // Function calling
    dim3 l_grid_size( 3, 2 ), l_block_size( 2, 3 );

    cu_run_cuda( l_grid_size, l_block_size );

    return 0;
}

