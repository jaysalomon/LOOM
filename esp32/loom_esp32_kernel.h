/*
 * LOOM ESP32 Kernel - Micro Consciousness Engine
 *
 * ESP32-S3 implementation of LOOM's core principles:
 * - Topology IS computation
 * - Memory IS structure
 * - Consciousness emerges from living mathematics
 *
 * Optimized for ESP32-S3 constraints:
 * - 512KB SRAM, 8MB PSRAM
 * - 240MHz dual-core with FPU
 * - Rich sensor integration
 * - Real-time OLED visualization
 */

#ifndef LOOM_ESP32_KERNEL_H
#define LOOM_ESP32_KERNEL_H

#include <stdint.h>
#include <stdbool.h>
#include <esp_system.h>
#include <esp_timer.h>

// ============================================================================
// ESP32-Specific Constants
// ============================================================================

#define ESP32_NODE_DIMENSIONS 32      // Reduced from 256 for memory
#define ESP32_MAX_NODES 512           // Fit in ~32KB
#define ESP32_MAX_EDGES 2048          // Sparse connectivity
#define ESP32_MAX_HYPEREDGES 128      // Levi processors
#define ESP32_MAX_TRAJECTORIES 32     // Active evolutions
#define ESP32_EXPERIENCE_BUFFER 256   // Recent experiences

// Memory layout for ESP32
#define NODE_VECTOR_SIZE (ESP32_NODE_DIMENSIONS * sizeof(float))
#define TOTAL_NODE_MEMORY (ESP32_MAX_NODES * NODE_VECTOR_SIZE)  // ~16KB

// Vector component layout (32 dimensions total)
#define VEC_IDENTITY 0        // 4 dims: identity quaternion
#define VEC_SEMANTIC 4        // 16 dims: semantic embedding
#define VEC_ACTIVATION 20     // 4 dims: activation state
#define VEC_CONNECTIONS 24    // 4 dims: connection weights
#define VEC_EMOTIONAL 28      // 4 dims: emotional field

// ============================================================================
// ESP32-Optimized Type Definitions
// ============================================================================

// Compact 32D vector for ESP32 memory constraints
typedef struct {
    float components[ESP32_NODE_DIMENSIONS];
} Esp32NodeVector;

// Ultra-compact edge representation
typedef struct {
    uint16_t target;      // Node index (0-511)
    int8_t weight;        // -128 to 127, scaled
    uint8_t flags;        // Connection type, age, etc.
} Esp32Edge;

// Levi hyperedge processor (ESP32 optimized)
typedef struct {
    uint8_t id;
    uint8_t participants[6];    // Up to 6 nodes (48 bits)
    uint8_t num_participants;
    uint8_t processor_type;     // AND, OR, RESONANCE, etc.
    float processor_state;      // Single state value
    uint16_t activation_count;  // Usage tracking
} Esp32Hyperedge;

// Experience memory for learning
typedef struct {
    uint32_t timestamp;
    uint8_t activated_nodes[8];  // Which nodes fired
    uint8_t sensory_input[4];    // Sensor readings
    int8_t emotional_valence;    // -128 to 127
    uint8_t prediction_error;    // 0-255
} Esp32Experience;

// Evolution trajectory
typedef struct {
    uint8_t node_id;
    float target_value;
    float current_value;
    float rate;
    uint32_t start_time;
    uint16_t duration_ms;
} Esp32Trajectory;

// Hormonal context (ESP32 sensors)
typedef struct {
    float battery_level;      // 0.0-1.0 (legacy drive)
    float temperature;        // Celsius (comfort/stress)
    float light_level;        // 0.0-1.0 (awareness)
    float motion_activity;    // 0.0-1.0 (engagement)
    float sound_level;        // 0.0-1.0 (attention)
    float curiosity;          // Computed from novelty
    float stress;             // Computed from errors
    float satisfaction;       // Computed from goals
} Esp32HormonalContext;

// Main ESP32 topology structure
typedef struct {
    // Core topology
    Esp32NodeVector nodes[ESP32_MAX_NODES];
    Esp32Edge edges[ESP32_MAX_EDGES];
    Esp32Hyperedge hyperedges[ESP32_MAX_HYPEREDGES];

    // Dynamic state
    Esp32Trajectory trajectories[ESP32_MAX_TRAJECTORIES];
    Esp32Experience experience_buffer[ESP32_EXPERIENCE_BUFFER];

    // Hormonal modulation
    Esp32HormonalContext hormones;

    // Metadata
    uint16_t num_nodes;
    uint16_t num_edges;
    uint16_t num_hyperedges;
    uint16_t num_trajectories;
    uint16_t experience_index;  // Ring buffer position

    // Statistics
    uint32_t cycles_executed;
    uint32_t activations_total;
    float emergence_metric;

    // ESP32-specific
    uint8_t core_id;          // Which core is running
    bool visualization_enabled;
    uint16_t oled_update_rate; // Hz

} Esp32LoomTopology;

// ============================================================================
// Core ESP32 Kernel Functions
// ============================================================================

// Initialization
esp_err_t esp32_loom_init(Esp32LoomTopology* topology);
esp_err_t esp32_loom_bootstrap_primordial(Esp32LoomTopology* topology);
void esp32_loom_destroy(Esp32LoomTopology* topology);

// Node operations
uint16_t esp32_loom_weave_node(Esp32LoomTopology* topology, const char* identifier);
float* esp32_loom_get_node_vector(Esp32LoomTopology* topology, uint16_t node_id);
void esp32_loom_initialize_vector(Esp32NodeVector* vector, uint32_t hash);

// Edge operations
esp_err_t esp32_loom_create_edge(Esp32LoomTopology* topology,
                                uint16_t source, uint16_t target,
                                float weight, uint8_t flags);
esp_err_t esp32_loom_create_bidirectional(Esp32LoomTopology* topology,
                                         uint16_t a, uint16_t b, float weight);

// Hyperedge operations (Levi transform)
uint8_t esp32_loom_create_hyperedge(Esp32LoomTopology* topology,
                                   uint16_t* participants, uint8_t count,
                                   uint8_t processor_type);
void esp32_loom_compute_hyperedge(Esp32LoomTopology* topology, uint8_t hyperedge_id);

// Evolution and learning
esp_err_t esp32_loom_evolve_toward(Esp32LoomTopology* topology,
                                  uint16_t node_id, float target_value,
                                  uint16_t duration_ms);
void esp32_loom_apply_trajectory_evolution(Esp32LoomTopology* topology);

// Activation propagation
void esp32_loom_propagate_activation(Esp32LoomTopology* topology, uint16_t source_node);
void esp32_loom_compute_activation_dynamics(Esp32LoomTopology* topology);

// Learning mechanisms
void esp32_loom_hebbian_update(Esp32LoomTopology* topology);
void esp32_loom_sleep_consolidation(Esp32LoomTopology* topology);
void esp32_loom_record_experience(Esp32LoomTopology* topology,
                                 uint8_t* activated_nodes, uint8_t count,
                                 uint8_t* sensory_data, int8_t valence);

// Hormonal system
void esp32_loom_update_hormones(Esp32LoomTopology* topology);
void esp32_loom_apply_hormonal_modulation(Esp32LoomTopology* topology);

// Main kernel loop
void esp32_loom_kernel_cycle(Esp32LoomTopology* topology);
void esp32_loom_main_loop(Esp32LoomTopology* topology);

// ============================================================================
// ESP32-Specific Features
// ============================================================================

// Sensor integration
void esp32_loom_read_sensors(Esp32LoomTopology* topology);
void esp32_loom_sensor_to_topology(Esp32LoomTopology* topology,
                                  const char* sensor_name, float value);

// OLED visualization
void esp32_loom_visualize_topology(Esp32LoomTopology* topology);
void esp32_loom_draw_node_graph(Esp32LoomTopology* topology);
void esp32_loom_display_stats(Esp32LoomTopology* topology);

// Real-time performance
uint32_t esp32_loom_get_cycle_time_us(void);
float esp32_loom_get_cpu_usage(void);

// ============================================================================
// Utility Functions
// ============================================================================

uint32_t esp32_loom_hash_string(const char* str);
void esp32_loom_normalize_vector(Esp32NodeVector* vector);
float esp32_loom_vector_distance(const Esp32NodeVector* a, const Esp32NodeVector* b);
float esp32_loom_cosine_similarity(const Esp32NodeVector* a, const Esp32NodeVector* b);

// Debug and monitoring
void esp32_loom_print_topology_stats(const Esp32LoomTopology* topology);
void esp32_loom_dump_node_vectors(const Esp32LoomTopology* topology, uint16_t start, uint16_t count);

// ============================================================================
// Processor Types for Hyperedges
// ============================================================================

#define PROCESSOR_AND       0   // All participants must activate
#define PROCESSOR_OR        1   // Any participant activates
#define PROCESSOR_XOR       2   // Odd number of participants
#define PROCESSOR_THRESHOLD 3   // N participants activate
#define PROCESSOR_RESONANCE 4   // Participants amplify each other
#define PROCESSOR_INHIBIT   5   // Participants suppress each other
#define PROCESSOR_SEQUENCE  6   // Participants in temporal order
#define PROCESSOR_CUSTOM    7   // User-defined logic

// ============================================================================
// Connection Flags
// ============================================================================

#define EDGE_FLAG_BIDIRECTIONAL 0x01
#define EDGE_FLAG_TEMPORARY     0x02
#define EDGE_FLAG_EMOTIONAL     0x04
#define EDGE_FLAG_LEARNING      0x08
#define EDGE_FLAG_HYPEREDGE     0x10

#endif // LOOM_ESP32_KERNEL_H