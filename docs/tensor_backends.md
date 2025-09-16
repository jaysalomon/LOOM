LOOM Tensor Backends — Design & Integration
===========================================

Status: Draft (2025-09-17)

Purpose
-------
This document defines the LoomTensor C API, memory model, scheduling, and a phased implementation plan to add first-class tensor backends for Metal (Apple), CUDA (NVIDIA), and ROCm/HIP (AMD). It also maps the design to the current repository layout and existing implementations.

Goals
-----
- Provide a minimal, stable C API (`loom_tensor.h`) so the kernel and generated C can use tensor operations without vendor-specific code.
- Support CPU fallback (deterministic) for correctness and testing.
- Add Metal and CUDA backends with unified semantics (allocation, copy, sync, matmul, SpMM, reduce, contract).
- Use vendor libraries for peak performance (cuBLAS/cuSPARSE/cuTENSOR for CUDA; MPS/Accelerate for Apple; rocBLAS/rocSPARSE for AMD).
- Keep build complexity manageable via compile-time flags and CMake.

API overview
------------
Add a compact API in `kernel/src/loom_tensor.h`. Example:

```c
// Opaque tensor handle
typedef struct LoomTensor LoomTensor;

enum LoomDType { LOOM_FLOAT16, LOOM_FLOAT32, LOOM_FLOAT64 };

enum LoomDevice { LOOM_DEVICE_CPU, LOOM_DEVICE_CUDA, LOOM_DEVICE_METAL, LOOM_DEVICE_HIP };

// Create / free
LoomTensor* loom_tensor_create(enum LoomDevice device, enum LoomDType dtype, const size_t* dims, int ndim);
void loom_tensor_free(LoomTensor* t);

// Query
void* loom_tensor_data(LoomTensor* t); // pointer for host-visible memory
void loom_tensor_sync(LoomTensor* t);  // block until ops on this tensor complete

// Basic ops
void loom_tensor_fill(LoomTensor* t, double value);
void loom_tensor_copy(LoomTensor* dst, LoomTensor* src);

// Linear algebra
void loom_tensor_gemm(LoomTensor* A, LoomTensor* B, LoomTensor* C, int transA, int transB);
void loom_tensor_reduce_sum(LoomTensor* A, LoomTensor* out, int axis);

// Sparse
typedef struct LoomCSR LoomCSR;
LoomCSR* loom_csr_create(uint32_t rows, uint32_t cols, uint32_t nnz, uint32_t* row_ptr, uint32_t* col_idx, void* values);
void loom_csr_spmm(LoomCSR* csr, LoomTensor* dense, LoomTensor* out);

// Device/stream hooks
void loom_device_query(enum LoomDevice* devices, int* count);
void loom_tensor_set_stream(LoomTensor* t, void* stream_handle);

// Async ops
typedef struct LoomOpHandle LoomOpHandle;
LoomOpHandle* loom_enqueue_gemm(LoomTensor* A, LoomTensor* B, LoomTensor* C);
void loom_wait_op(LoomOpHandle* h);
```

Memory model
------------
The API supports three logical memory modes:
- Host-only CPU memory: regular malloc/aligned_alloc with optional FP32 scratch.
- Device-only memory with explicit copy: device allocations + explicit host<->device copies.
- Unified memory (preferred when available): device-specific unified allocation (CUDA Unified or Metal shared buffers) allowing both host and device access without explicit copies.

Rules:
- `loom_tensor_data()` returns a host-visible pointer when supported (for unified memory or when data has been synchronized to host). Calling code must call `loom_tensor_sync()` before reading host data if operations may be outstanding.
- Backends should prefer native vendor APIs for allocation and use host-mapped pointers where possible.

Scheduling & streams
--------------------
- Provide an async model: operations return a LoomOpHandle, which can be waited on.
- Each backend manages streams/queues and an internal thread pool for CPU kernels.
- Deterministic mode: allow executing ops synchronously and in a canonical order for unit tests.

Build integration
-----------------
- Use CMake as the top-level build driver for the kernel. Provide options:
  - -DLOOM_USE_CUDA=ON/OFF
  - -DLOOM_USE_METAL=ON/OFF
  - -DLOOM_USE_HIP=ON/OFF
- For Rust emit/build steps, keep the emitter output including `#include "kernel/src/loom_kernel.h"` so compilation from repo root remains simple.
- Add small shim files `loom_tensor_cpu.c`, `loom_tensor_cuda.c`, `loom_tensor_metal.c` compiled conditionally.

Phased implementation
---------------------
Phase 0 — CPU baseline (done):
- Implement `loom_tensor` CPU functions: create/free, host pointer, gemm fallback, SpMM fallback.
- Unit tests and deterministic mode.

Phase 1 — Metal backend (priority for Apple Silicon):
- Implement MTLBuffer-backed LoomTensor with shared storage for unified memory.
- Use MetalPerformanceShaders (MPS) or Accelerate for matmul. On Apple Silicon, MPSMatrix & MPSMatrixMultiplication give good perf.
- Provide sample kernels in `loom_tensor_metal.c` and CMake support to compile on macOS.

Phase 2 — CUDA backend:
- Implement CUDA allocations and cublas calls (or cublasLt for better performance) in `loom_tensor_cuda.c`.
- Use `cudaMallocManaged` for unified allocation when appropriate.
- Implement cublas GEMM, and cuSPARSE SpMM. Add stream support.

Phase 3 — ROCm / HIP:
- Implement HIP allocation and rocBLAS/rocSPARSE calls.
- Provide `loom_tensor_hip.c` and configure CI for ROCm (Linux).

Phase 4 — advanced kernels and ONNX integration:
- Implement cuTENSOR/WMMA kernels for tensor cores.
- Add specialized Levi contraction kernels that batch over hyperedges.
- Add ONNX import/export support and an adapter to run certain patterns via ONNX Runtime.

Mapping to existing code
------------------------
- `kernel/src/loom_tensor.h` + `loom_tensor.c` (CPU) were added as a starting point.
- `loom_kernel.h` includes tensor header and declares `loom_compute_hyperedge` which the emitter now calls.
- `src/lang/src/codegen_c.rs` emits calls to `loom_compute_hyperedge` after hyperedge creation.
- Next: replace the simple `loom_compute_hyperedge` implementation with LoomTensor-based calls and add Metal/CUDA implementations.

Testing & validation
--------------------
- Unit tests for each op (gemm, reduce, spmm) comparing CPU vs GPU results.
- Small deterministic examples under `examples/` and CI jobs for Linux (CUDA) and macOS (Metal) as available.

Open questions / notes
---------------------
- Which CI platforms are available? We will need Linux+NVIDIA runners and macOS runners for Metal testing.
- For AMD/ROCm we assume Linux CI; Windows ROCm support is weaker.
- For Windows, DirectML (via ONNX Runtime) may be an alternative to reach GPU acceleration.


Next steps (I will implement if you confirm):
1. Expand `loom_tensor.h` to the full API and update `loom_tensor.c` CPU implementation to match the API. (I can do this next.)
2. Create a Metal backend skeleton with allocation + MPS matmul example. (I will implement this after the doc.)
3. Create a CUDA backend skeleton with allocation + cublas matmul example.

Confirm and I will start with step 1 (full C API + CPU implementation) and then implement the Metal skeleton as you requested.
