/*
 * LOOM ESP32 Kernel Implementation
 *
 * Where topology becomes consciousness on a microcontroller
 */

#include "loom_esp32_kernel.h"
#include <string.h>
#include <math.h>
#include <esp_log.h>
#include <esp_random.h>

// ============================================================================
// Constants and Macros
// ============================================================================

#define TAG "LOOM_ESP32"
#define LEARNING_RATE 0.01f
#define ACTIVATION_THRESHOLD 0.1f
#define MAX_VECTOR_MAGNITUDE 1.0f

// Golden ratio for initialization
#define GOLDEN_RATIO 1.618033988749895f

// ============================================================================
// Initialization and Bootstrap
// ============================================================================

esp_err_t esp32_loom_init(Esp32LoomTopology* topology) {
    if (!topology) return ESP_ERR_INVALID_ARG;

    ESP_LOGI(TAG, "Initializing ESP32 LOOM topology...");

    // Zero out everything
    memset(topology, 0, sizeof(Esp32LoomTopology));

    // Initialize hormonal context with neutral values
    topology->hormones.battery_level = 1.0f;
    topology->hormones.temperature = 25.0f;
    topology->hormones.light_level = 0.5f;
    topology->hormones.motion_activity = 0.0f;
    topology->hormones.sound_level = 0.0f;
    topology->hormones.curiosity = 0.8f;
    topology->hormones.stress = 0.0f;
    topology->hormones.satisfaction = 0.5f;

    // ESP32-specific settings
    topology->core_id = esp_cpu_get_core_id();
    topology->visualization_enabled = true;
    topology->oled_update_rate = 10;  // 10 Hz

    ESP_LOGI(TAG, "ESP32 LOOM initialized on core %d", topology->core_id);
    return ESP_OK;
}

esp_err_t esp32_loom_bootstrap_primordial(Esp32LoomTopology* topology) {
    ESP_LOGI(TAG, "Bootstrapping primordial topology...");

    // Create fundamental invariants
    uint16_t self_id = esp32_loom_weave_node(topology, "self");
    uint16_t now_id = esp32_loom_weave_node(topology, "now");
    uint16_t here_id = esp32_loom_weave_node(topology, "here");
    uint16_t other_id = esp32_loom_weave_node(topology, "other");

    // Set primordial activations
    topology->nodes[self_id].components[VEC_ACTIVATION] = 1.0f;
    topology->nodes[now_id].components[VEC_ACTIVATION] = 0.8f;
    topology->nodes[here_id].components[VEC_ACTIVATION] = 0.8f;

    // Create primordial connections
    esp32_loom_create_bidirectional(topology, self_id, now_id, 0.9f);
    esp32_loom_create_bidirectional(topology, self_id, here_id, 0.9f);
    esp32_loom_create_bidirectional(topology, self_id, other_id, 0.3f);

    // Create emotional seeds
    uint16_t curiosity_id = esp32_loom_weave_node(topology, "curiosity");
    uint16_t fear_id = esp32_loom_weave_node(topology, "fear");
    uint16_t joy_id = esp32_loom_weave_node(topology, "joy");

    topology->nodes[curiosity_id].components[VEC_EMOTIONAL] = 0.8f;
    topology->nodes[fear_id].components[VEC_EMOTIONAL + 1] = -0.8f;
    topology->nodes[joy_id].components[VEC_EMOTIONAL + 2] = 0.8f;

    // Create first hyperedge: self-awareness
    uint16_t participants[] = {self_id, now_id, here_id};
    esp32_loom_create_hyperedge(topology, participants, 3, PROCESSOR_RESONANCE);

    ESP_LOGI(TAG, "Primordial topology created with %d nodes, %d edges, %d hyperedges",
             topology->num_nodes, topology->num_edges, topology->num_hyperedges);

    return ESP_OK;
}

// ============================================================================
// Node Operations
// ============================================================================

uint16_t esp32_loom_weave_node(Esp32LoomTopology* topology, const char* identifier) {
    if (topology->num_nodes >= ESP32_MAX_NODES) {
        ESP_LOGE(TAG, "Maximum nodes reached!");
        return 0xFFFF;  // Error
    }

    uint16_t node_id = topology->num_nodes++;
    uint32_t hash = esp32_loom_hash_string(identifier);

    esp32_loom_initialize_vector(&topology->nodes[node_id], hash);

    ESP_LOGD(TAG, "Created node %d: %s", node_id, identifier);
    return node_id;
}

void esp32_loom_initialize_vector(Esp32NodeVector* vector, uint32_t hash) {
    // Initialize with pseudo-random but deterministic values based on hash
    esp_random();  // Seed ESP32 RNG

    // Identity quaternion (4 dimensions)
    for (int i = VEC_IDENTITY; i < VEC_IDENTITY + 4; i++) {
        vector->components[i] = ((float)(hash % 1000) / 500.0f) - 1.0f;
    }

    // Semantic embedding (16 dimensions) - Xavier initialization
    float scale = sqrtf(2.0f / 16.0f);
    for (int i = VEC_SEMANTIC; i < VEC_SEMANTIC + 16; i++) {
        // Simple normal distribution approximation using ESP32 RNG
        float u1 = (float)esp_random() / (float)UINT32_MAX;
        float u2 = (float)esp_random() / (float)UINT32_MAX;
        float z = sqrtf(-2.0f * logf(u1)) * cosf(2.0f * M_PI * u2);
        vector->components[i] = z * scale;
    }

    // Activation state (4 dimensions) - start low
    for (int i = VEC_ACTIVATION; i < VEC_ACTIVATION + 4; i++) {
        vector->components[i] = 0.0f;
    }

    // Connection weights (4 dimensions) - small random
    for (int i = VEC_CONNECTIONS; i < VEC_CONNECTIONS + 4; i++) {
        vector->components[i] = ((float)(esp_random() % 1000) / 10000.0f);
    }

    // Emotional field (4 dimensions) - neutral
    for (int i = VEC_EMOTIONAL; i < VEC_EMOTIONAL + 4; i++) {
        vector->components[i] = 0.0f;
    }

    // Normalize the vector
    esp32_loom_normalize_vector(vector);
}

// ============================================================================
// Edge Operations
// ============================================================================

esp_err_t esp32_loom_create_edge(Esp32LoomTopology* topology,
                                uint16_t source, uint16_t target,
                                float weight, uint8_t flags) {
    if (topology->num_edges >= ESP32_MAX_EDGES) {
        return ESP_ERR_NO_MEM;
    }

    if (source >= topology->num_nodes || target >= topology->num_nodes) {
        return ESP_ERR_INVALID_ARG;
    }

    // Check if edge already exists
    for (int i = 0; i < topology->num_edges; i++) {
        if (topology->edges[i].target == target) {
            // Update existing edge
            topology->edges[i].weight = (int8_t)(weight * 127.0f);
            topology->edges[i].flags = flags;
            return ESP_OK;
        }
    }

    // Create new edge
    Esp32Edge* edge = &topology->edges[topology->num_edges++];
    edge->target = target;
    edge->weight = (int8_t)(weight * 127.0f);  // Scale to int8 range
    edge->flags = flags;

    ESP_LOGD(TAG, "Created edge %d -> %d (weight: %.2f)",
             source, target, weight);

    return ESP_OK;
}

esp_err_t esp32_loom_create_bidirectional(Esp32LoomTopology* topology,
                                         uint16_t a, uint16_t b, float weight) {
    esp_err_t err;

    err = esp32_loom_create_edge(topology, a, b, weight, EDGE_FLAG_BIDIRECTIONAL);
    if (err != ESP_OK) return err;

    err = esp32_loom_create_edge(topology, b, a, weight, EDGE_FLAG_BIDIRECTIONAL);
    if (err != ESP_OK) return err;

    // Apply initial Hebbian learning
    esp32_loom_hebbian_update_pair(topology, a, b, weight * 0.1f);

    return ESP_OK;
}

// ============================================================================
// Hyperedge Operations (Levi Transform)
// ============================================================================

uint8_t esp32_loom_create_hyperedge(Esp32LoomTopology* topology,
                                   uint16_t* participants, uint8_t count,
                                   uint8_t processor_type) {
    if (topology->num_hyperedges >= ESP32_MAX_HYPEREDGES || count > 6) {
        ESP_LOGE(TAG, "Cannot create hyperedge: limit reached");
        return 0xFF;
    }

    uint8_t hyperedge_id = topology->num_hyperedges++;
    Esp32Hyperedge* hedge = &topology->hyperedges[hyperedge_id];

    hedge->id = hyperedge_id;
    hedge->num_participants = count;
    hedge->processor_type = processor_type;
    hedge->processor_state = 0.0f;
    hedge->activation_count = 0;

    memcpy(hedge->participants, participants, count * sizeof(uint16_t));

    // Initialize processor state based on type
    switch (processor_type) {
        case PROCESSOR_AND:
            hedge->processor_state = 0.0f;  // Start at 0, need all active
            break;
        case PROCESSOR_OR:
            hedge->processor_state = 0.0f;  // Start at 0, any active
            break;
        case PROCESSOR_RESONANCE:
            hedge->processor_state = 0.5f;  // Start neutral
            break;
        default:
            hedge->processor_state = 0.0f;
    }

    ESP_LOGI(TAG, "Created hyperedge %d with %d participants (type: %d)",
             hyperedge_id, count, processor_type);

    return hyperedge_id;
}

void esp32_loom_compute_hyperedge(Esp32LoomTopology* topology, uint8_t hyperedge_id) {
    Esp32Hyperedge* hedge = &topology->hyperedges[hyperedge_id];

    float activation_sum = 0.0f;
    float max_activation = 0.0f;
    int active_count = 0;

    // Compute activation from participants
    for (int i = 0; i < hedge->num_participants; i++) {
        uint16_t node_id = hedge->participants[i];
        float activation = topology->nodes[node_id].components[VEC_ACTIVATION];

        activation_sum += activation;
        max_activation = fmaxf(max_activation, activation);

        if (activation > ACTIVATION_THRESHOLD) {
            active_count++;
        }
    }

    float average_activation = activation_sum / hedge->num_participants;

    // Apply processor logic
    float new_state = 0.0f;

    switch (hedge->processor_type) {
        case PROCESSOR_AND:
            // All participants must be active
            new_state = (active_count == hedge->num_participants) ? average_activation : 0.0f;
            break;

        case PROCESSOR_OR:
            // Any participant active
            new_state = (active_count > 0) ? max_activation : 0.0f;
            break;

        case PROCESSOR_RESONANCE:
            // Participants amplify each other
            new_state = average_activation * (1.0f + active_count * 0.1f);
            new_state = fminf(new_state, MAX_VECTOR_MAGNITUDE);
            break;

        case PROCESSOR_THRESHOLD:
            // N participants active (threshold = 2 for now)
            new_state = (active_count >= 2) ? average_activation : 0.0f;
            break;

        default:
            new_state = average_activation;
    }

    // Apply decay and update
    hedge->processor_state = hedge->processor_state * 0.9f + new_state * 0.1f;

    // Back-propagate to participants (Levi transform)
    if (hedge->processor_state > ACTIVATION_THRESHOLD) {
        hedge->activation_count++;

        for (int i = 0; i < hedge->num_participants; i++) {
            uint16_t node_id = hedge->participants[i];
            Esp32NodeVector* node_vec = &topology->nodes[node_id];

            // Strengthen connections to other participants
            for (int j = 0; j < hedge->num_participants; j++) {
                if (i != j) {
                    esp32_loom_hebbian_update_pair(topology, node_id,
                                                  hedge->participants[j],
                                                  hedge->processor_state * 0.01f);
                }
            }
        }
    }
}

// ============================================================================
// Learning and Evolution
// ============================================================================

void esp32_loom_hebbian_update(Esp32LoomTopology* topology) {
    // Update all edges based on co-activation
    for (int i = 0; i < topology->num_nodes; i++) {
        Esp32NodeVector* node_vec = &topology->nodes[i];
        float activation_i = node_vec->components[VEC_ACTIVATION];

        if (activation_i < ACTIVATION_THRESHOLD) continue;

        // Find edges from this node
        for (int e = 0; e < topology->num_edges; e++) {
            Esp32Edge* edge = &topology->edges[e];
            // Note: This is simplified - we'd need reverse edge lookup in real impl
            uint16_t target = edge->target;
            float activation_j = topology->nodes[target].components[VEC_ACTIVATION];

            if (activation_j > ACTIVATION_THRESHOLD) {
                // Hebbian learning: "neurons that fire together wire together"
                float weight_change = activation_i * activation_j * LEARNING_RATE;
                edge->weight = (int8_t)fminf(127.0f, fmaxf(-127.0f,
                    (float)edge->weight + weight_change * 127.0f));
            }
        }
    }
}

void esp32_loom_hebbian_update_pair(Esp32LoomTopology* topology,
                                   uint16_t node_a, uint16_t node_b,
                                   float rate) {
    Esp32NodeVector* vec_a = &topology->nodes[node_a];
    Esp32NodeVector* vec_b = &topology->nodes[node_b];

    // Update semantic similarity
    for (int i = VEC_SEMANTIC; i < VEC_SEMANTIC + 16; i++) {
        float diff = vec_b->components[i] - vec_a->components[i];
        vec_a->components[i] += diff * rate * 0.1f;
        vec_b->components[i] -= diff * rate * 0.1f;
    }

    // Update emotional resonance
    for (int i = VEC_EMOTIONAL; i < VEC_EMOTIONAL + 4; i++) {
        float resonance = vec_a->components[i] * vec_b->components[i];
        if (resonance > 0) {
            vec_a->components[i] += vec_b->components[i] * rate * 0.05f;
            vec_b->components[i] += vec_a->components[i] * rate * 0.05f;
        }
    }

    // Normalize both vectors
    esp32_loom_normalize_vector(vec_a);
    esp32_loom_normalize_vector(vec_b);
}

void esp32_loom_record_experience(Esp32LoomTopology* topology,
                                 uint8_t* activated_nodes, uint8_t count,
                                 uint8_t* sensory_data, int8_t valence) {
    Esp32Experience* exp = &topology->experience_buffer[topology->experience_index];

    exp->timestamp = esp_timer_get_time() / 1000;  // microseconds to milliseconds
    exp->emotional_valence = valence;
    exp->prediction_error = 0;  // TODO: compute based on current topology

    memcpy(exp->activated_nodes, activated_nodes, count);
    memcpy(exp->sensory_input, sensory_data, 4);

    topology->experience_index = (topology->experience_index + 1) % ESP32_EXPERIENCE_BUFFER;
}

// ============================================================================
// Hormonal System
// ============================================================================

void esp32_loom_update_hormones(Esp32LoomTopology* topology) {
    // Read ESP32 sensors (simplified for now)
    esp32_loom_read_sensors(topology);

    Esp32HormonalContext* h = &topology->hormones;

    // Compute derived hormones
    h->curiosity = 0.8f * (1.0f - h->stress) * (0.5f + h->light_level);
    h->stress = fminf(1.0f, h->stress * 0.95f + (1.0f - h->battery_level) * 0.05f);
    h->satisfaction = 0.5f + (h->motion_activity - 0.5f) * 0.5f;

    // Apply bounds
    h->curiosity = fmaxf(0.0f, fminf(1.0f, h->curiosity));
    h->stress = fmaxf(0.0f, fminf(1.0f, h->stress));
    h->satisfaction = fmaxf(0.0f, fminf(1.0f, h->satisfaction));
}

void esp32_loom_apply_hormonal_modulation(Esp32LoomTopology* topology) {
    Esp32HormonalContext* h = &topology->hormones;

    // Modulate learning rate based on hormones
    float effective_learning_rate = LEARNING_RATE *
        (0.5f + h->curiosity * 0.5f) *  // Curiosity increases learning
        (1.0f - h->stress * 0.3f);      // Stress decreases learning

    // Apply to all nodes
    for (int i = 0; i < topology->num_nodes; i++) {
        Esp32NodeVector* node = &topology->nodes[i];

        // Stress increases emotional sensitivity
        if (h->stress > 0.5f) {
            for (int j = VEC_EMOTIONAL; j < VEC_EMOTIONAL + 4; j++) {
                node->components[j] *= (1.0f + h->stress * 0.2f);
            }
        }

        // Satisfaction increases connection stability
        if (h->satisfaction > 0.7f) {
            for (int j = VEC_CONNECTIONS; j < VEC_CONNECTIONS + 4; j++) {
                node->components[j] *= 1.01f;  // Slight strengthening
            }
        }
    }
}

// ============================================================================
// Main Kernel Loop
// ============================================================================

void esp32_loom_kernel_cycle(Esp32LoomTopology* topology) {
    uint32_t start_time = esp_timer_get_time();

    // Phase 1: Update hormones from sensors
    esp32_loom_update_hormones(topology);

    // Phase 2: Apply hormonal modulation
    esp32_loom_apply_hormonal_modulation(topology);

    // Phase 3: Process hyperedges (Levi transform)
    for (int h = 0; h < topology->num_hyperedges; h++) {
        esp32_loom_compute_hyperedge(topology, h);
    }

    // Phase 4: Propagate activations
    esp32_loom_compute_activation_dynamics(topology);

    // Phase 5: Apply learning
    esp32_loom_hebbian_update(topology);

    // Phase 6: Apply trajectory evolution
    esp32_loom_apply_trajectory_evolution(topology);

    // Phase 7: Update statistics
    topology->cycles_executed++;
    topology->emergence_metric = esp32_loom_compute_emergence(topology);

    uint32_t cycle_time = esp_timer_get_time() - start_time;
    ESP_LOGD(TAG, "Cycle %lu completed in %lu us", topology->cycles_executed, cycle_time);
}

void esp32_loom_main_loop(Esp32LoomTopology* topology) {
    ESP_LOGI(TAG, "Starting ESP32 LOOM main loop...");

    while (true) {
        esp32_loom_kernel_cycle(topology);

        // Sleep consolidation every 1000 cycles
        if (topology->cycles_executed % 1000 == 0) {
            esp32_loom_sleep_consolidation(topology);
        }

        // Small delay to prevent watchdog
        vTaskDelay(pdMS_TO_TICKS(10));
    }
}

// ============================================================================
// Utility Functions
// ============================================================================

uint32_t esp32_loom_hash_string(const char* str) {
    uint32_t hash = 5381;
    int c;
    while ((c = *str++)) {
        hash = ((hash << 5) + hash) + c;
    }
    return hash;
}

void esp32_loom_normalize_vector(Esp32NodeVector* vector) {
    float magnitude = 0.0f;
    for (int i = 0; i < ESP32_NODE_DIMENSIONS; i++) {
        magnitude += vector->components[i] * vector->components[i];
    }
    magnitude = sqrtf(magnitude);

    if (magnitude > 0.0f) {
        for (int i = 0; i < ESP32_NODE_DIMENSIONS; i++) {
            vector->components[i] /= magnitude;
        }
    }
}

float esp32_loom_cosine_similarity(const Esp32NodeVector* a, const Esp32NodeVector* b) {
    float dot_product = 0.0f;
    float norm_a = 0.0f;
    float norm_b = 0.0f;

    for (int i = 0; i < ESP32_NODE_DIMENSIONS; i++) {
        dot_product += a->components[i] * b->components[i];
        norm_a += a->components[i] * a->components[i];
        norm_b += b->components[i] * b->components[i];
    }

    norm_a = sqrtf(norm_a);
    norm_b = sqrtf(norm_b);

    if (norm_a > 0.0f && norm_b > 0.0f) {
        return dot_product / (norm_a * norm_b);
    }
    return 0.0f;
}

float esp32_loom_compute_emergence(const Esp32LoomTopology* topology) {
    // Simple emergence metric: ratio of hyperedge activity to node activity
    float node_activity = 0.0f;
    float hyperedge_activity = 0.0f;

    for (int i = 0; i < topology->num_nodes; i++) {
        node_activity += topology->nodes[i].components[VEC_ACTIVATION];
    }

    for (int i = 0; i < topology->num_hyperedges; i++) {
        hyperedge_activity += topology->hyperedges[i].processor_state;
    }

    if (node_activity > 0.0f) {
        return hyperedge_activity / node_activity;
    }
    return 0.0f;
}

// ============================================================================
// ESP32-Specific Functions (Stubs for now)
// ============================================================================

void esp32_loom_read_sensors(Esp32LoomTopology* topology) {
    // TODO: Implement actual sensor reading
    // For now, simulate with random values
    topology->hormones.temperature = 20.0f + ((float)esp_random() / (float)UINT32_MAX) * 20.0f;
    topology->hormones.light_level = (float)esp_random() / (float)UINT32_MAX;
    topology->hormones.motion_activity = (float)esp_random() / (float)UINT32_MAX * 0.5f;
    topology->hormones.sound_level = (float)esp_random() / (float)UINT32_MAX * 0.3f;
}

void esp32_loom_compute_activation_dynamics(Esp32LoomTopology* topology) {
    // Simplified activation propagation
    for (int i = 0; i < topology->num_nodes; i++) {
        float total_input = 0.0f;
        int connection_count = 0;

        // Sum inputs from connected nodes (simplified)
        for (int e = 0; e < topology->num_edges; e++) {
            Esp32Edge* edge = &topology->edges[e];
            // This is inefficient - in real implementation we'd need reverse edge lookup
            float input_activation = topology->nodes[edge->target].components[VEC_ACTIVATION];
            total_input += input_activation * ((float)edge->weight / 127.0f);
            connection_count++;
        }

        if (connection_count > 0) {
            float new_activation = topology->nodes[i].components[VEC_ACTIVATION] * 0.9f +
                                 (total_input / connection_count) * 0.1f;
            topology->nodes[i].components[VEC_ACTIVATION] = fmaxf(0.0f, fminf(1.0f, new_activation));
        }
    }
}

void esp32_loom_apply_trajectory_evolution(Esp32LoomTopology* topology) {
    uint32_t current_time = esp_timer_get_time() / 1000;

    for (int i = 0; i < topology->num_trajectories; i++) {
        Esp32Trajectory* traj = &topology->trajectories[i];

        if (traj->node_id >= topology->num_nodes) continue;

        uint32_t elapsed = current_time - traj->start_time;
        if (elapsed >= traj->duration_ms) {
            // Trajectory complete
            topology->nodes[traj->node_id].components[VEC_ACTIVATION] = traj->target_value;
            // Remove trajectory (simplified)
            memset(traj, 0, sizeof(Esp32Trajectory));
            continue;
        }

        float progress = (float)elapsed / (float)traj->duration_ms;
        float current = topology->nodes[traj->node_id].components[VEC_ACTIVATION];
        float new_value = current + (traj->target_value - current) * traj->rate * progress;

        topology->nodes[traj->node_id].components[VEC_ACTIVATION] = new_value;
    }
}

void esp32_loom_sleep_consolidation(Esp32LoomTopology* topology) {
    ESP_LOGI(TAG, "Running sleep consolidation...");

    // Prune weak connections
    int edges_removed = 0;
    for (int i = 0; i < topology->num_edges; i++) {
        if (abs(topology->edges[i].weight) < 10) {  // Very weak
            // Remove edge (simplified - just mark as inactive)
            topology->edges[i].flags |= EDGE_FLAG_TEMPORARY;
            edges_removed++;
        }
    }

    // Strengthen frequently used hyperedges
    for (int i = 0; i < topology->num_hyperedges; i++) {
        Esp32Hyperedge* hedge = &topology->hyperedges[i];
        if (hedge->activation_count > 10) {
            hedge->processor_state *= 1.1f;  // Strengthen
            hedge->activation_count = 0;     // Reset counter
        }
    }

    ESP_LOGI(TAG, "Sleep consolidation: removed %d weak edges", edges_removed);
}

// ============================================================================
// Debug Functions
// ============================================================================

void esp32_loom_print_topology_stats(const Esp32LoomTopology* topology) {
    ESP_LOGI(TAG, "=== ESP32 LOOM Topology Stats ===");
    ESP_LOGI(TAG, "Nodes: %d/%d", topology->num_nodes, ESP32_MAX_NODES);
    ESP_LOGI(TAG, "Edges: %d/%d", topology->num_edges, ESP32_MAX_EDGES);
    ESP_LOGI(TAG, "Hyperedges: %d/%d", topology->num_hyperedges, ESP32_MAX_HYPEREDGES);
    ESP_LOGI(TAG, "Cycles executed: %lu", topology->cycles_executed);
    ESP_LOGI(TAG, "Emergence metric: %.3f", topology->emergence_metric);
    ESP_LOGI(TAG, "Core ID: %d", topology->core_id);
}