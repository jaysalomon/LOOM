// Skeleton CUDA backend for LoomTensor
#include <stdio.h>
#include "loom_tensor.h"

// CUDA backend stubs. Real CUDA code will be implemented in .cu files
// and compiled with nvcc; these stubs allow non-CUDA builds to link.

LoomTensor* loom_tensor_create_cuda(LoomDevice device, LoomDType dtype, const size_t* dims, int ndim) {
    (void)device; (void)dtype; (void)dims; (void)ndim;
    fprintf(stderr, "loom_tensor_create_cuda: CUDA backend not available in this build.\n");
    return NULL;
}

void loom_tensor_set_stream_cuda(LoomTensor* t, void* stream_handle) { (void)t; (void)stream_handle; }
