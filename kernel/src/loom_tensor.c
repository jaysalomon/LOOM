// CPU fallback LoomTensor implementation
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "loom_tensor.h"
#include "loom_kernel.h"

#ifdef _WIN32
#include <malloc.h>
#endif

static void* loom_aligned_alloc(size_t alignment, size_t size) {
#ifdef _WIN32
    return _aligned_malloc(size, alignment);
#else
    void* ptr = NULL;
    if (posix_memalign(&ptr, alignment, size) != 0) return NULL;
    return ptr;
#endif
}

static void loom_aligned_free(void* p) {
#ifdef _WIN32
    _aligned_free(p);
#else
    free(p);
#endif
}

typedef struct LoomTensor {
    LoomDevice device;
    LoomDType dtype;
    int ndim;
    size_t dims[8];
    size_t elem_count;
    void* data; // host-accessible data for CPU device
    void* stream;
} LoomTensor;

static size_t _count_elems(const size_t* dims, int ndim) {
    size_t n = 1;
    for (int i = 0; i < ndim; ++i) n *= dims[i];
    return n;
}

LoomTensor* loom_tensor_create(LoomDevice device, LoomDType dtype, const size_t* dims, int ndim) {
    LoomTensor* t = (LoomTensor*)malloc(sizeof(LoomTensor));
    if (!t) return NULL;
    t->device = device;
    t->dtype = dtype;
    t->ndim = ndim;
    t->elem_count = _count_elems(dims, ndim);
    for (int i = 0; i < ndim && i < 8; ++i) t->dims[i] = dims[i];
    t->stream = NULL;
    if (device == LOOM_DEVICE_CPU) {
    size_t bytes = t->elem_count * sizeof(float);
    t->data = loom_aligned_alloc(64, bytes);
    if (!t->data) { free(t); return NULL; }
    memset(t->data, 0, bytes);
    } else {
        // Device not implemented: leave data NULL for now
        t->data = NULL;
    }
    return t;
}

void loom_tensor_free(LoomTensor* t) {
    if (!t) return;
    if (t->data) loom_aligned_free(t->data);
    free(t);
}

void* loom_tensor_data(LoomTensor* t) {
    if (!t) return NULL;
    return t->data;
}

void loom_tensor_sync(LoomTensor* t) {
    // CPU: nothing to do. Device backends should implement synchronization.
    (void)t;
}

void loom_tensor_fill(LoomTensor* t, double value) {
    if (!t || !t->data) return;
    float v = (float)value;
    float* f = (float*)t->data;
    for (size_t i = 0; i < t->elem_count; ++i) f[i] = v;
}

void loom_tensor_copy(LoomTensor* dst, LoomTensor* src) {
    if (!dst || !src) return;
    if (dst->elem_count != src->elem_count) return;
    if (dst->data && src->data) {
        memcpy(dst->data, src->data, dst->elem_count * sizeof(float));
    }
}

void loom_tensor_gemm(LoomTensor* A, LoomTensor* B, LoomTensor* C, int transA, int transB, double alpha, double beta) {
    // Naive CPU GEMM for 2D tensors only. A: MxK, B: KxN, C: MxN
    if (!A || !B || !C) return;
    int M = (int)A->dims[0];
    int K = (int)A->dims[1];
    int N = (int)B->dims[1];
    float* a = (float*)A->data;
    float* b = (float*)B->data;
    float* c = (float*)C->data;
    float af = (float)alpha;
    float bf = (float)beta;
    for (int i = 0; i < M; ++i) {
        for (int j = 0; j < N; ++j) {
            float s = 0.0f;
            for (int k = 0; k < K; ++k) {
                float aa = a[i*K + k];
                float bb = b[k*N + j];
                s += aa * bb;
            }
            c[i*N + j] = af * s + bf * c[i*N + j];
        }
    }
}

void loom_tensor_reduce_sum(LoomTensor* A, LoomTensor* out, int axis) {
    if (!A || !out) return;
    // only implement reduce over axis 0 for 2D A: MxN -> out: 1xN
    int M = (int)A->dims[0];
    int N = (int)A->dims[1];
    float* a = (float*)A->data;
    float* o = (float*)out->data;
    for (int j = 0; j < N; ++j) o[j] = 0.0f;
    for (int i = 0; i < M; ++i) {
        for (int j = 0; j < N; ++j) o[j] += a[i*N + j];
    }
}

// Sparse helpers: placeholders
LoomCSR* loom_csr_create(uint32_t rows, uint32_t cols, uint32_t nnz, uint32_t* row_ptr, uint32_t* col_idx, void* values, LoomDType val_dtype) {
    (void)rows; (void)cols; (void)nnz; (void)row_ptr; (void)col_idx; (void)values; (void)val_dtype;
    return NULL;
}
void loom_csr_free(LoomCSR* csr) { (void)csr; }
void loom_csr_spmm(LoomCSR* csr, LoomTensor* dense, LoomTensor* out) { (void)csr; (void)dense; (void)out; }

// Device enumeration / stream
void loom_device_query(LoomDevice* devices, int* count) { (void)devices; *count = 1; devices[0] = LOOM_DEVICE_CPU; }
void loom_tensor_set_stream(LoomTensor* t, void* stream_handle) { if (t) t->stream = stream_handle; }

// Async ops: simple sync wrappers for CPU
LoomOpHandle* loom_enqueue_gemm(LoomTensor* A, LoomTensor* B, LoomTensor* C) { (void)A; (void)B; (void)C; return NULL; }
void loom_wait_op(LoomOpHandle* h) { (void)h; }

// For now, a simple mean aggregation across participants' semantic dims
void loom_compute_hyperedge(struct LoomTopology* topology, uint32_t hyperedge_id) {
    // Very small example: read the hyperedge participant list from topology
    // and average the semantic components into the processor_vector.
    // This is intentionally tiny — the real implementation will use vectorized
    // code and float16 conversions.
    (void)topology; (void)hyperedge_id;
    fprintf(stderr, "loom_compute_hyperedge: CPU baseline called (no-op)\n");
}
    // temp buffer for mean
    float mean[128];
    for (size_t i = 0; i < proc_len; i++) mean[i] = 0.0f;

    for (uint32_t i = 0; i < he->num_participants; i++) {
        uint32_t nid = he->participants[i];
        if (nid >= topology->num_nodes) continue;
        NodeVector* nv = &topology->node_bank[nid];

        // copy semantic region into first 64 entries of processor vector space
        for (size_t d = 0; d < semantic_len; d++) {
            float v = (float)nv->components[semantic_start + d];
            mean[d] += v;
        }
    }

    float inv = 1.0f / (float)he->num_participants;
    for (size_t d = 0; d < semantic_len; d++) {
        mean[d] *= inv;
    }

    // Fill processor_vector: first 64 dims with mean, remainder zeros
    for (size_t p = 0; p < proc_len; p++) {
        if (p < semantic_len) he->processor_vector[p] = (float16)mean[p];
        else he->processor_vector[p] = (float16)0.0f;
    }
}
