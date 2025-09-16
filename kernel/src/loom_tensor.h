// LoomTensor API - supports CPU baseline and pluggable GPU backends
#ifndef LOOM_TENSOR_H
#define LOOM_TENSOR_H

#include <stdint.h>
#include <stdlib.h>

// Forward declare topology types from loom_kernel.h
struct LoomTopology;

typedef enum { LOOM_FLOAT16 = 0, LOOM_FLOAT32 = 1, LOOM_FLOAT64 = 2 } LoomDType;
typedef enum { LOOM_DEVICE_CPU = 0, LOOM_DEVICE_CUDA = 1, LOOM_DEVICE_METAL = 2, LOOM_DEVICE_HIP = 3 } LoomDevice;

// Opaque handles
typedef struct LoomTensor LoomTensor;
typedef struct LoomCSR LoomCSR;
typedef struct LoomOpHandle LoomOpHandle;

// Creation / destruction
LoomTensor* loom_tensor_create(LoomDevice device, LoomDType dtype, const size_t* dims, int ndim);
void loom_tensor_free(LoomTensor* t);

// Host access
void* loom_tensor_data(LoomTensor* t); // may return NULL for device-only tensors
void loom_tensor_sync(LoomTensor* t);

// Basic ops
void loom_tensor_fill(LoomTensor* t, double value);
void loom_tensor_copy(LoomTensor* dst, LoomTensor* src);

// Linear algebra
// C = alpha * op(A) @ op(B) + beta * C
void loom_tensor_gemm(LoomTensor* A, LoomTensor* B, LoomTensor* C, int transA, int transB, double alpha, double beta);
void loom_tensor_reduce_sum(LoomTensor* A, LoomTensor* out, int axis);

// Sparse
LoomCSR* loom_csr_create(uint32_t rows, uint32_t cols, uint32_t nnz, uint32_t* row_ptr, uint32_t* col_idx, void* values, LoomDType val_dtype);
void loom_csr_free(LoomCSR* csr);
void loom_csr_spmm(LoomCSR* csr, LoomTensor* dense, LoomTensor* out);

// Device / stream
void loom_device_query(LoomDevice* devices, int* count);
void loom_tensor_set_stream(LoomTensor* t, void* stream_handle);

// Async ops
LoomOpHandle* loom_enqueue_gemm(LoomTensor* A, LoomTensor* B, LoomTensor* C);
void loom_wait_op(LoomOpHandle* h);

// Higher-level helper: compute hyperedge processor vector (aggregates participants)
void loom_compute_hyperedge(struct LoomTopology* topology, uint32_t hyperedge_id);

#endif // LOOM_TENSOR_H
