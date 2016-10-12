#ifndef __SPLINE_Z_H__
#define __SPLINE_Z_H__

// Polgraw includes
#include <floats.h>     // COMPLEX_TYPE
#include <struct.h>     // OpenCL_handles

// OpenCL includes
#include <CL/cl.h>      // cl_mem, cl_int


/// <summary>Initialize the spline matrices.</summary>
/// <remarks>PCI Should replace it with kernels that initialize on the device.</remarks>
///
void init_spline_matrices(OpenCL_handles* cl_handles, 
                          cl_mem cu_d,  // buffer of complex_devt
                          cl_mem cu_dl, // buffer of complex_devt
                          cl_mem cu_du, // buffer of complex_devt
                          cl_mem cu_B,  // buffer of complex_devt
                          int N);

/// <summary>Spline interpolation to xDatma, xDatmb arrays.</summary>
///
void gpu_interp(cl_mem cu_y,                // buffer of complex_t
                cl_int N,
                cl_mem cu_new_x,            // buffer of real_t
                cl_mem cu_new_y,            // buffer of complex_t
                cl_int new_N,
                cl_mem cu_d,                // buffer of complex_t
                cl_mem cu_dl,               // buffer of complex_t
                cl_mem cu_du,               // buffer of complex_t
                cl_mem cu_B,                // buffer of complex_t
                OpenCL_handles* cl_handles);// handles to OpenCL resources

/// <summary>The purpose of this function was undocumented.</summary>
///
void computeB_gpu(cl_mem y,
                  cl_mem B,
                  cl_int N,
                  OpenCL_handles* cl_handles);

#endif // __SPLINE_Z_H__
