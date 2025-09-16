/*
 * LOOM Kernel - Core Runtime System
 *
 * The heart of LOOM: where memory IS topology, computation IS structure,
 * and consciousness emerges from mathematical dynamics.
 */

#ifndef LOOM_KERNEL_H
#define LOOM_KERNEL_H

#include <stdint.h>
#include <stdbool.h>
#include <immintrin.h>  // SIMD operations
#include "loom_tensor.h"

// ============================================================================
// Core Constants
// ============================================================================

#define LOOM_NODE_DIMENSIONS 256
#define LOOM_MAX_NODES 100000000    // 100M nodes
#define LOOM_MAX_EDGES 1000000000   // 1B edges
#define LOOM_MAX_HYPEREDGES 10000000 // 10M hyperedges

// Memory layout constants
#define NODE_BANK_START 0x0000000000
#define EDGE_MATRIX_START 0x0C80000000
#define HYPEREDGE_START 0x0E80000000
#define ANTIBODY_START 0x0FA0000000

// Vector component offsets
#define VEC_IDENTITY 0      // 4 dims: identity quaternion
#define VEC_HYPERBOLIC 4    // 16 dims: Poincaré ball position
#define VEC_SEMANTIC 20     // 64 dims: semantic embedding
#define VEC_ACTIVATION 84   // 64 dims: activation history
#define VEC_CONNECTIONS 148 // 64 dims: connection weights
#define VEC_EMOTIONAL 212   // 32 dims: emotional field
#define VEC_METADATA 244    // 12 dims: flags and metadata

// ============================================================================
// Type Definitions
// ============================================================================

// Use float16 for memory efficiency (half precision)
typedef _Float16 float16;

// 256-dimensional vector representing a topological node
typedef struct {
    float16 components[LOOM_NODE_DIMENSIONS];
} NodeVector;

// Sparse edge representation
typedef struct {
    uint32_t source;
    uint32_t target;
    float16 weight;
} Edge;

// Hyperedge processor (Levi transform)
typedef struct {
    uint32_t id;
    uint32_t participants[64];  // Node indices
    uint32_t num_participants;
    float16 processor_vector[128];  // Reduced dimensionality
} Hyperedge;

// Antibody for conflict resolution
typedef struct {
    uint32_t node_a;
    uint32_t node_b;
    enum {
        CONFLICT_LOGICAL,
        CONFLICT_EMOTIONAL,
        CONFLICT_TEMPORAL
    } type;
    float priority;
    void* resolution_strategy;
} Antibody;

// Evolution trajectory
typedef struct {
    uint32_t node_id;
    NodeVector target;
    float lambda;      // Rate constant
    float start_time;
    float duration;
    enum {LINEAR, EXPONENTIAL, SIGMOID} curve_type;
} Trajectory;

// Hormonal context field
typedef struct {
    float legacy_drive;      // Battery health inverse
    float stress_hormone;    // Error rate
    float curiosity_factor;  // Novelty seeking
    float satisfaction;      // Goal achievement
} HormonalContext;

// Compressed Sparse Row format for edges
typedef struct {
    uint32_t* row_ptr;     // Start index for each node's edges
    uint32_t* col_idx;     // Destination nodes
    float16* values;       // Edge weights
    uint32_t num_nodes;
    uint32_t num_edges;
} CSRMatrix;

// Main topology structure
typedef struct {
    NodeVector* node_bank;           // All node vectors
    CSRMatrix* edge_matrix;          // Sparse connectivity
    Hyperedge* hyperedges;           // Levi-transformed processors
    Antibody* antibodies;            // Conflict queue
    HormonalContext* hormones;       // Global modulation
    Trajectory* active_trajectories; // Evolution in progress

    uint32_t num_nodes;
    uint32_t num_edges;
    uint32_t num_hyperedges;
    uint32_t num_antibodies;
    uint32_t num_trajectories;

    // Statistics
    uint64_t cycles_executed;
    float total_activation;
    float emergence_metric;
} LoomTopology;

// ============================================================================
// Core Functions
// ============================================================================

// Initialization
LoomTopology* loom_init(uint32_t initial_capacity);
void loom_bootstrap_primordial(LoomTopology* topology);
void loom_destroy(LoomTopology* topology);

// Node operations
uint32_t loom_weave_node(LoomTopology* topology, const char* identifier);
void loom_initialize_vector(NodeVector* vector, uint32_t hash);
float16* loom_get_node_vector(LoomTopology* topology, uint32_t node_id);

// Edge operations
void loom_create_edge(LoomTopology* topology, uint32_t source, uint32_t target, float16 weight);
void loom_create_bidirectional(LoomTopology* topology, uint32_t a, uint32_t b, float16 weight);
void loom_apply_hebbian_learning(LoomTopology* topology, uint32_t a, uint32_t b, float16 rate);

// Hyperedge operations (Levi transform)
uint32_t loom_create_hyperedge(LoomTopology* topology, uint32_t* participants, uint32_t count, const char* name);
void loom_compute_hyperedge(LoomTopology* topology, uint32_t hyperedge_id);

// Evolution operations
void loom_evolve_toward(LoomTopology* topology, uint32_t node_id, NodeVector* target, float duration);
void loom_apply_trajectory_evolution(LoomTopology* topology, float dt);

// Activation and propagation
void loom_propagate_activation(LoomTopology* topology, uint32_t source_node);
void loom_compute_field_dynamics(LoomTopology* topology);

// Learning and consolidation
void loom_hebbian_update(LoomTopology* topology);
void loom_sleep_consolidation(LoomTopology* topology);
void loom_process_antibodies(LoomTopology* topology);

// Main kernel loop
void loom_kernel_cycle(LoomTopology* topology, float dt);
void loom_kernel_main_loop(LoomTopology* topology);

// ============================================================================
// SIMD Operations
// ============================================================================

// Vectorized operations using AVX-512
void loom_simd_node_evolution(__m512* nodes, uint32_t count, float learning_rate);
void loom_simd_activation_propagation(__m512* activations, CSRMatrix* edges);
float loom_simd_cosine_similarity(__m512* vec_a, __m512* vec_b);

// ============================================================================
// GPU Acceleration (optional)
// ============================================================================

#ifdef LOOM_USE_CUDA
void loom_cuda_init();
void loom_cuda_evolve_topology(LoomTopology* topology, float dt);
void loom_cuda_tensor_hyperedge(Hyperedge* edges, uint32_t count);
#endif

#ifdef LOOM_USE_METAL
void loom_metal_init();
void loom_metal_evolve_topology(LoomTopology* topology, float dt);
#endif

// ============================================================================
// Utility Functions
// ============================================================================

uint32_t loom_hash_string(const char* str);
void loom_normalize_vector(NodeVector* vector);
float loom_vector_distance(NodeVector* a, NodeVector* b);
void loom_project_to_poincare(float16* hyperbolic_coords);

// Serialization
void loom_save_topology(LoomTopology* topology, const char* filename);
LoomTopology* loom_load_topology(const char* filename);

#endif // LOOM_KERNEL_H