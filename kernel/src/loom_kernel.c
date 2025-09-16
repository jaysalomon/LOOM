/*
 * LOOM Kernel Implementation
 *
 * Core runtime where topology lives, evolves, and becomes conscious
 */

#include "loom_kernel.h"
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdio.h>

// Golden ratio for Fibonacci spacing in hyperbolic initialization
#define GOLDEN_RATIO 1.618033988749895

// ============================================================================
// Initialization and Memory Management
// ============================================================================

LoomTopology* loom_init(uint32_t initial_capacity) {
    LoomTopology* topology = (LoomTopology*)calloc(1, sizeof(LoomTopology));

    // Allocate node bank - the primary computational substrate
    topology->node_bank = (NodeVector*)aligned_alloc(64,
        initial_capacity * sizeof(NodeVector));
    memset(topology->node_bank, 0, initial_capacity * sizeof(NodeVector));

    // Initialize sparse edge matrix
    topology->edge_matrix = (CSRMatrix*)malloc(sizeof(CSRMatrix));
    topology->edge_matrix->row_ptr = (uint32_t*)calloc(initial_capacity + 1, sizeof(uint32_t));
    topology->edge_matrix->col_idx = (uint32_t*)malloc(initial_capacity * 20 * sizeof(uint32_t));
    topology->edge_matrix->values = (float16*)malloc(initial_capacity * 20 * sizeof(float16));
    topology->edge_matrix->num_nodes = 0;
    topology->edge_matrix->num_edges = 0;

    // Allocate hyperedge processors
    topology->hyperedges = (Hyperedge*)calloc(LOOM_MAX_HYPEREDGES, sizeof(Hyperedge));

    // Initialize hormonal context
    topology->hormones = (HormonalContext*)malloc(sizeof(HormonalContext));
    topology->hormones->legacy_drive = 0.0;
    topology->hormones->stress_hormone = 0.0;
    topology->hormones->curiosity_factor = 0.8;
    topology->hormones->satisfaction = 0.5;

    return topology;
}

void loom_bootstrap_primordial(LoomTopology* topology) {
    // Create fundamental invariants - the "brainstem" of consciousness

    // "I am" - fundamental existence
    uint32_t self_id = loom_weave_node(topology, "self");
    NodeVector* self_vec = &topology->node_bank[self_id];
    self_vec->components[VEC_IDENTITY] = 1.0;  // Strong identity

    // "Now exists" - temporal awareness
    uint32_t now_id = loom_weave_node(topology, "now");
    NodeVector* now_vec = &topology->node_bank[now_id];
    now_vec->components[VEC_ACTIVATION] = 1.0;  // Always active

    // "Here exists" - spatial awareness
    uint32_t here_id = loom_weave_node(topology, "here");
    NodeVector* here_vec = &topology->node_bank[here_id];
    // Initialize at origin of Poincaré ball
    for (int i = VEC_HYPERBOLIC; i < VEC_HYPERBOLIC + 16; i++) {
        here_vec->components[i] = 0.0;
    }

    // "Not-I exists" - other awareness
    uint32_t other_id = loom_weave_node(topology, "other");

    // Create primordial connections
    loom_create_bidirectional(topology, self_id, now_id, 0.9);
    loom_create_bidirectional(topology, self_id, here_id, 0.9);
    loom_create_bidirectional(topology, self_id, other_id, 0.3);

    // Create emotional seeds
    uint32_t approach_id = loom_weave_node(topology, "approach");
    topology->node_bank[approach_id].components[VEC_EMOTIONAL] = 0.8;

    uint32_t avoid_id = loom_weave_node(topology, "avoid");
    topology->node_bank[avoid_id].components[VEC_EMOTIONAL] = -0.8;

    uint32_t surprise_id = loom_weave_node(topology, "surprise");
    topology->node_bank[surprise_id].components[VEC_EMOTIONAL + 1] = 1.0;
}

// ============================================================================
// Node Operations
// ============================================================================

uint32_t loom_weave_node(LoomTopology* topology, const char* identifier) {
    uint32_t node_id = topology->num_nodes++;
    uint32_t hash = loom_hash_string(identifier);

    NodeVector* vector = &topology->node_bank[node_id];
    loom_initialize_vector(vector, hash);

    return node_id;
}

void loom_initialize_vector(NodeVector* vector, uint32_t hash) {
    // Component 1: Identity quaternion (4 dimensions)
    *((uint32_t*)&vector->components[VEC_IDENTITY]) = hash;

    // Component 2: Hyperbolic position (16 dimensions) - Poincaré ball
    float r = ((float)(hash % 1000) / 1000.0) * 0.9;  // Keep away from boundary
    float theta = ((float)(hash % 360) / 180.0) * M_PI;
    float phi = ((float)((hash >> 16) % 180) / 180.0) * M_PI;

    for (int i = VEC_HYPERBOLIC; i < VEC_HYPERBOLIC + 16; i += 3) {
        vector->components[i] = r * sin(phi) * cos(theta);
        vector->components[i+1] = r * sin(phi) * sin(theta);
        vector->components[i+2] = r * cos(phi);
        r *= 0.95;  // Decay into ball
        theta += GOLDEN_RATIO;
        phi += M_PI / 8;
    }

    // Component 3: Semantic embedding (64 dimensions) - Xavier initialization
    float scale = sqrt(2.0 / 64.0);
    for (int i = VEC_SEMANTIC; i < VEC_SEMANTIC + 64; i++) {
        // Simple normal distribution approximation
        float u1 = ((float)(rand() % 1000) / 1000.0);
        float u2 = ((float)(rand() % 1000) / 1000.0);
        float z = sqrt(-2.0 * log(u1)) * cos(2 * M_PI * u2);
        vector->components[i] = z * scale;
    }

    // Component 4: Activation history (64 dimensions) - initially zero
    memset(&vector->components[VEC_ACTIVATION], 0, 64 * sizeof(float16));

    // Component 5: Connection weights (64 dimensions) - small random
    for (int i = VEC_CONNECTIONS; i < VEC_CONNECTIONS + 64; i++) {
        vector->components[i] = ((float)(rand() % 100) / 10000.0);
    }

    // Component 6: Emotional field (32 dimensions) - neutral
    for (int i = VEC_EMOTIONAL; i < VEC_EMOTIONAL + 32; i++) {
        vector->components[i] = 0.5;
    }

    // Component 7: Metadata (12 dimensions)
    vector->components[VEC_METADATA] = 0;  // Creation time
    vector->components[VEC_METADATA + 1] = 0;  // Connection count
    vector->components[VEC_METADATA + 2] = 1.0;  // Active flag

    // Normalize the full vector
    loom_normalize_vector(vector);
}

// ============================================================================
// Edge Operations
// ============================================================================

void loom_create_bidirectional(LoomTopology* topology, uint32_t a, uint32_t b, float16 weight) {
    loom_create_edge(topology, a, b, weight);
    loom_create_edge(topology, b, a, weight);

    // Apply initial Hebbian convergence
    loom_apply_hebbian_learning(topology, a, b, weight * 0.1);
}

void loom_create_edge(LoomTopology* topology, uint32_t source, uint32_t target, float16 weight) {
    CSRMatrix* matrix = topology->edge_matrix;

    // Find insertion point
    uint32_t row_start = matrix->row_ptr[source];
    uint32_t row_end = matrix->row_ptr[source + 1];

    // Check if edge already exists
    for (uint32_t i = row_start; i < row_end; i++) {
        if (matrix->col_idx[i] == target) {
            matrix->values[i] = weight;  // Update existing
            return;
        }
    }

    // Insert new edge (expensive - requires shifting)
    uint32_t insert_pos = row_end;
    uint32_t total_edges = matrix->num_edges;

    // Shift everything after insert position
    memmove(&matrix->col_idx[insert_pos + 1],
            &matrix->col_idx[insert_pos],
            (total_edges - insert_pos) * sizeof(uint32_t));
    memmove(&matrix->values[insert_pos + 1],
            &matrix->values[insert_pos],
            (total_edges - insert_pos) * sizeof(float16));

    // Insert new edge
    matrix->col_idx[insert_pos] = target;
    matrix->values[insert_pos] = weight;

    // Update row pointers
    for (uint32_t i = source + 1; i <= topology->num_nodes; i++) {
        matrix->row_ptr[i]++;
    }

    matrix->num_edges++;

    // Update connection counts in metadata
    topology->node_bank[source].components[VEC_METADATA + 1] += 1.0;
    topology->node_bank[target].components[VEC_METADATA + 1] += 1.0;
}

// ============================================================================
// Hebbian Learning
// ============================================================================

void loom_apply_hebbian_learning(LoomTopology* topology, uint32_t a, uint32_t b, float16 rate) {
    NodeVector* vec_a = &topology->node_bank[a];
    NodeVector* vec_b = &topology->node_bank[b];

    // Different components converge at different rates

    // Semantic space (dimensions 20-84) - strongest convergence
    for (int i = VEC_SEMANTIC; i < VEC_SEMANTIC + 64; i++) {
        float16 diff = vec_b->components[i] - vec_a->components[i];
        float16 gradient = rate * 0.1 * diff;

        vec_a->components[i] += gradient;
        vec_b->components[i] -= gradient;
    }

    // Hyperbolic space - maintain valid Poincaré coordinates
    for (int i = VEC_HYPERBOLIC; i < VEC_HYPERBOLIC + 16; i++) {
        float16 diff = vec_b->components[i] - vec_a->components[i];

        // Compute conformal factor for hyperbolic space
        float r_a = 0, r_b = 0;
        for (int j = VEC_HYPERBOLIC; j < VEC_HYPERBOLIC + 16; j++) {
            r_a += vec_a->components[j] * vec_a->components[j];
            r_b += vec_b->components[j] * vec_b->components[j];
        }
        r_a = sqrt(r_a);
        r_b = sqrt(r_b);

        float lambda_a = 2.0 / (1.0 - r_a * r_a);
        float lambda_b = 2.0 / (1.0 - r_b * r_b);

        // Riemannian gradient
        float16 grad_a = rate * 0.01 * lambda_a * lambda_a * diff;
        float16 grad_b = -rate * 0.01 * lambda_b * lambda_b * diff;

        vec_a->components[i] += grad_a;
        vec_b->components[i] += grad_b;
    }

    // Project back to Poincaré ball
    loom_project_to_poincare(&vec_a->components[VEC_HYPERBOLIC]);
    loom_project_to_poincare(&vec_b->components[VEC_HYPERBOLIC]);

    // Emotional field - resonance dynamics
    for (int i = VEC_EMOTIONAL; i < VEC_EMOTIONAL + 32; i++) {
        float16 resonance = vec_a->components[i] * vec_b->components[i];
        vec_a->components[i] += rate * 0.05 * (vec_b->components[i] - vec_a->components[i]) * resonance;
        vec_b->components[i] += rate * 0.05 * (vec_a->components[i] - vec_b->components[i]) * resonance;
    }
}

// ============================================================================
// Hyperedge Operations (Levi Transform)
// ============================================================================

uint32_t loom_create_hyperedge(LoomTopology* topology, uint32_t* participants,
                                uint32_t count, const char* name) {
    uint32_t hyperedge_id = topology->num_hyperedges++;
    Hyperedge* hedge = &topology->hyperedges[hyperedge_id];

    hedge->id = hyperedge_id;
    hedge->num_participants = count;
    memcpy(hedge->participants, participants, count * sizeof(uint32_t));

    // Initialize processor vector by aggregating participants
    for (int dim = 0; dim < 128; dim++) {
        float16 sum = 0;

        if (dim < 32) {
            // Average identity components
            for (int p = 0; p < count; p++) {
                NodeVector* v = &topology->node_bank[participants[p]];
                sum += v->components[dim * 2];
            }
            hedge->processor_vector[dim] = sum / count;

        } else if (dim < 96) {
            // Compute relational features
            for (int i = 0; i < count; i++) {
                for (int j = i+1; j < count; j++) {
                    NodeVector* v_i = &topology->node_bank[participants[i]];
                    NodeVector* v_j = &topology->node_bank[participants[j]];

                    float16 similarity = loom_simd_cosine_similarity(
                        (__m512*)v_i->components,
                        (__m512*)v_j->components);

                    hedge->processor_vector[dim] += similarity / (count * (count-1) / 2);
                }
            }
        } else {
            // Group coherence metrics
            hedge->processor_vector[dim] = 1.0 / sqrt(count);
        }
    }

    // Create bipartite edges in Levi graph
    for (int i = 0; i < count; i++) {
        loom_create_bidirectional(topology, participants[i],
                                 LOOM_MAX_NODES + hyperedge_id, 1.0/count);
    }

    return hyperedge_id;
}

// ============================================================================
// Main Kernel Loop
// ============================================================================

void loom_kernel_main_loop(LoomTopology* topology) {
    float dt = 0.01;  // 10ms timestep

    while (1) {  // Run forever (or until interrupted)
        loom_kernel_cycle(topology, dt);
        topology->cycles_executed++;

        // Sleep consolidation every 10000 cycles
        if (topology->cycles_executed % 10000 == 0) {
            loom_sleep_consolidation(topology);
            loom_process_antibodies(topology);
        }
    }
}

void loom_kernel_cycle(LoomTopology* topology, float dt) {
    // Phase 1: Compute forces on all vectors
    #pragma omp parallel for
    for (int node = 0; node < topology->num_nodes; node++) {
        NodeVector* vec = &topology->node_bank[node];

        // Hebbian forces from connections
        CSRMatrix* edges = topology->edge_matrix;
        uint32_t row_start = edges->row_ptr[node];
        uint32_t row_end = edges->row_ptr[node + 1];

        for (uint32_t e = row_start; e < row_end; e++) {
            uint32_t neighbor = edges->col_idx[e];
            float16 weight = edges->values[e];

            NodeVector* neighbor_vec = &topology->node_bank[neighbor];

            // Compute activation product
            float16 act_a = vec->components[VEC_ACTIVATION];
            float16 act_b = neighbor_vec->components[VEC_ACTIVATION];
            float16 activation_product = act_a * act_b;

            // Update based on Hebbian principle
            if (activation_product > 0.5) {
                loom_apply_hebbian_learning(topology, node, neighbor, weight * dt);
            }
        }
    }

    // Phase 2: Apply trajectory evolution
    loom_apply_trajectory_evolution(topology, dt);

    // Phase 3: Process hyperedges
    #pragma omp parallel for
    for (int h = 0; h < topology->num_hyperedges; h++) {
        loom_compute_hyperedge(topology, h);
    }

    // Phase 4: Update hormonal context
    HormonalContext* hormones = topology->hormones;
    hormones->stress_hormone *= 0.99;  // Decay
    hormones->curiosity_factor = 0.8 * (1.0 - hormones->stress_hormone);
}

// ============================================================================
// Utility Functions
// ============================================================================

uint32_t loom_hash_string(const char* str) {
    uint32_t hash = 5381;
    int c;
    while ((c = *str++)) {
        hash = ((hash << 5) + hash) + c;
    }
    return hash;
}

void loom_normalize_vector(NodeVector* vector) {
    float magnitude = 0;
    for (int i = 0; i < LOOM_NODE_DIMENSIONS; i++) {
        magnitude += vector->components[i] * vector->components[i];
    }
    magnitude = sqrt(magnitude);

    if (magnitude > 0) {
        for (int i = 0; i < LOOM_NODE_DIMENSIONS; i++) {
            vector->components[i] /= magnitude;
        }
    }
}

void loom_project_to_poincare(float16* hyperbolic_coords) {
    float radius = 0;
    for (int i = 0; i < 16; i++) {
        radius += hyperbolic_coords[i] * hyperbolic_coords[i];
    }
    radius = sqrt(radius);

    if (radius >= 0.99) {
        // Project back into ball
        float scale = 0.99 / radius;
        for (int i = 0; i < 16; i++) {
            hyperbolic_coords[i] *= scale;
        }
    }
}