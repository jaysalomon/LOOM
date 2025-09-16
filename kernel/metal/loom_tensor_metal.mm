// Metal backend skeleton for LoomTensor (Objective-C++)
#ifdef __APPLE__
#import <Foundation/Foundation.h>
#import <Metal/Metal.h>
#import "../loom_tensor.h"

// Metal-backed LoomTensor representation (private)
struct MetalLoomTensor {
    id<MTLBuffer> buffer;
    size_t elem_count;
    id<MTLDevice> device;
};

extern "C" LoomTensor* loom_tensor_create_metal(LoomDevice device, LoomDType dtype, const size_t* dims, int ndim) {
    (void)device; (void)dtype;
    if (ndim <= 0 || dims == NULL) return NULL;

    size_t elems = 1;
    for (int i = 0; i < ndim; ++i) elems *= dims[i];

    id<MTLDevice> dev = MTLCreateSystemDefaultDevice();
    if (!dev) {
        fprintf(stderr, "Metal device not available\n");
        return NULL;
    }

    MetalLoomTensor* mlt = (MetalLoomTensor*)malloc(sizeof(MetalLoomTensor));
    if (!mlt) return NULL;
    mlt->device = dev;
    mlt->elem_count = elems;

    // Allocate a buffer with CPU cache mode shared for host mapping
    size_t bytes = elems * sizeof(float);
    mlt->buffer = [dev newBufferWithLength:bytes options:MTLResourceStorageModeShared];
    if (!mlt->buffer) {
        free(mlt);
        return NULL;
    }

    // Wrap MetalLoomTensor into the opaque LoomTensor pointer.
    return (LoomTensor*)mlt;
}

extern "C" void loom_tensor_free_metal(LoomTensor* t) {
    MetalLoomTensor* mlt = (MetalLoomTensor*)t;
    if (!mlt) return;
    // Release Objective-C objects
    mlt->buffer = nil;
    free(mlt);
}

extern "C" void* loom_tensor_data_metal(LoomTensor* t) {
    MetalLoomTensor* mlt = (MetalLoomTensor*)t;
    if (!mlt || !mlt->buffer) return NULL;
    return mlt->buffer.contents;
}

extern "C" void loom_tensor_sync_metal(LoomTensor* t) {
    (void)t; // Shared buffers are coherent on macOS for CPU/GPU — no-op for now
}

extern "C" void loom_tensor_set_stream_metal(LoomTensor* t, void* stream_handle) {
    (void)t; (void)stream_handle;
}

#endif // __APPLE__
