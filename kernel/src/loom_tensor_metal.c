// Skeleton Metal backend for LoomTensor
#include <stdio.h>
#include "loom_tensor.h"

// Note: this file is a skeleton. It provides stubs so the build can link
// while Metal backend is implemented on macOS using Objective-C / Metal API.

LoomTensor* loom_tensor_create_metal(LoomDevice device, LoomDType dtype, const size_t* dims, int ndim) {
    (void)device; (void)dtype; (void)dims; (void)ndim;
    fprintf(stderr, "loom_tensor_create_metal: Not implemented on this platform.\n");
    return NULL;
}

void loom_tensor_set_stream_metal(LoomTensor* t, void* stream_handle) { (void)t; (void)stream_handle; }

// The actual Metal-backed implementations will be provided in a separate
// Objective-C translation unit that links against Metal.framework.
