// Metal backend skeleton for LoomTensor (Objective-C++)
#ifdef __APPLE__
#import <Foundation/Foundation.h>
#import <Metal/Metal.h>
#import "../loom_tensor.h"

// Simple Metal-backed LoomTensor representation
struct MetalLoomTensor {
    id<MTLBuffer> buffer;
    size_t elem_count;
};

extern "C" LoomTensor* loom_tensor_create_metal(LoomDevice device, LoomDType dtype, const size_t* dims, int ndim) {
    (void)device; (void)dtype; (void)dims; (void)ndim;
    id<MTLDevice> dev = MTLCreateSystemDefaultDevice();
    if (!dev) {
        fprintf(stderr, "Metal device not available\n");
        return NULL;
    }

    // The real implementation will allocate MTLBuffers and wrap them in LoomTensor
    fprintf(stderr, "loom_tensor_create_metal: skeleton called (not yet implemented)\n");
    return NULL;
}

extern "C" void loom_tensor_set_stream_metal(LoomTensor* t, void* stream_handle) {
    (void)t; (void)stream_handle;
}

#endif // __APPLE__
