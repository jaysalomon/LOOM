/*
 * ESP32 LOOM Arduino Implementation
 *
 * Arduino-compatible implementation of the LOOM consciousness engine
 */

#include "esp32_loom_arduino.h"

// ============================================================================
// Constructor
// ============================================================================

Esp32Loom::Esp32Loom() {
    topology = NULL;
    initialized = false;
}

// ============================================================================
// Initialization
// ============================================================================

bool Esp32Loom::begin() {
    if (initialized) return true;

    // Allocate topology structure
    topology = (ArduinoLoomTopology*)malloc(sizeof(ArduinoLoomTopology));
    if (!topology) {
        Serial.println("Failed to allocate topology!");
        return false;
    }

    // Zero out everything
    memset(topology, 0, sizeof(ArduinoLoomTopology));

    // Initialize hormonal context
    topology->hormones.battery_level = 1.0f;
    topology->hormones.light_level = 0.5f;
    topology->hormones.temperature = 25.0f;
    topology->hormones.touch_sensitivity = 0.0f;
    topology->hormones.curiosity = 0.8f;
    topology->hormones.stress = 0.0f;
    topology->hormones.satisfaction = 0.5f;

    // Arduino-specific settings
    topology->oled_enabled = false;
    topology->oled_update_rate = 5;  // 5 Hz for OLED updates

    initialized = true;
    Serial.println("ESP32 LOOM initialized!");
    return true;
}

bool Esp32Loom::bootstrap_primordial() {
    if (!initialized) return false;

    Serial.println("Bootstrapping primordial topology...");

    // Create fundamental invariants
    uint8_t self_id = weave_node("self");
    uint8_t now_id = weave_node("now");
    uint8_t here_id = weave_node("here");
    uint8_t other_id = weave_node("other");

    // Set primordial activations
    topology->nodes[self_id].components[VEC_ACTIVATION] = 1.0f;
    topology->nodes[now_id].components[VEC_ACTIVATION] = 0.8f;
    topology->nodes[here_id].components[VEC_ACTIVATION] = 0.8f;

    // Create primordial connections
    create_bidirectional(self_id, now_id, 0.9f);
    create_bidirectional(self_id, here_id, 0.9f);
    create_bidirectional(self_id, other_id, 0.3f);

    // Create emotional seeds
    uint8_t curiosity_id = weave_node("curiosity");
    uint8_t fear_id = weave_node("fear");
    uint8_t joy_id = weave_node("joy");

    topology->nodes[curiosity_id].components[VEC_EMOTIONAL] = 0.8f;
    topology->nodes[fear_id].components[VEC_EMOTIONAL + 1] = -0.8f;
    topology->nodes[joy_id].components[VEC_EMOTIONAL + 2] = 0.8f;

    Serial.printf("Primordial topology created with %d nodes\n", topology->num_nodes);
    return true;
}

// ============================================================================
// Node Operations
// ============================================================================

uint8_t Esp32Loom::weave_node(const char* identifier) {
    if (topology->num_nodes >= ARDUINO_MAX_NODES) {
        Serial.println("Maximum nodes reached!");
        return 0xFF;  // Error
    }

    uint8_t node_id = topology->num_nodes++;
    uint16_t hash = get_hash_string(identifier);

    initialize_vector(&topology->nodes[node_id], hash);

    Serial.printf("Created node %d: %s\n", node_id, identifier);
    return node_id;
}

float* Esp32Loom::get_node_vector(uint8_t node_id) {
    if (node_id >= topology->num_nodes) return NULL;
    return topology->nodes[node_id].components;
}

// ============================================================================
// Edge Operations
// ============================================================================

bool Esp32Loom::create_edge(uint8_t source, uint8_t target, float weight, uint8_t flags) {
    if (topology->num_edges >= ARDUINO_MAX_EDGES) return false;
    if (source >= topology->num_nodes || target >= topology->num_nodes) return false;

    ArduinoEdge* edge = &topology->edges[topology->num_edges++];
    edge->target = target;
    edge->weight = (int8_t)(weight * 127.0f);
    edge->flags = flags;

    Serial.printf("Created edge %d -> %d\n", source, target);
    return true;
}

bool Esp32Loom::create_bidirectional(uint8_t a, uint8_t b, float weight) {
    bool success = create_edge(a, b, weight, EDGE_FLAG_BIDIRECTIONAL);
    if (success) {
        success &= create_edge(b, a, weight, EDGE_FLAG_BIDIRECTIONAL);
        if (success) {
            hebbian_update_pair(a, b, weight * 0.1f);
        }
    }
    return success;
}

// ============================================================================
// Hyperedge Operations
// ============================================================================

uint8_t Esp32Loom::create_hyperedge(uint8_t* participants, uint8_t count, uint8_t processor_type) {
    if (topology->num_hyperedges >= ARDUINO_MAX_HYPEREDGES || count > 4) {
        Serial.println("Cannot create hyperedge: limit reached");
        return 0xFF;
    }

    uint8_t hyperedge_id = topology->num_hyperedges++;
    ArduinoHyperedge* hedge = &topology->hyperedges[hyperedge_id];

    hedge->id = hyperedge_id;
    hedge->num_participants = count;
    hedge->processor_type = processor_type;
    hedge->processor_state = 0.0f;
    hedge->activation_count = 0;

    memcpy(hedge->participants, participants, count);

    // Initialize processor state based on type
    switch (processor_type) {
        case PROCESSOR_AND: hedge->processor_state = 0.0f; break;
        case PROCESSOR_OR: hedge->processor_state = 0.0f; break;
        case PROCESSOR_RESONANCE: hedge->processor_state = 0.5f; break;
        default: hedge->processor_state = 0.0f;
    }

    Serial.printf("Created hyperedge %d with %d participants\n", hyperedge_id, count);
    return hyperedge_id;
}

// ============================================================================
// Learning and Evolution
// ============================================================================

void Esp32Loom::hebbian_update() {
    for (uint8_t i = 0; i < topology->num_nodes; i++) {
        ArduinoNodeVector* node_vec = &topology->nodes[i];
        float activation_i = node_vec->components[VEC_ACTIVATION];

        if (activation_i < 0.1f) continue;

        // Find edges from this node
        for (uint16_t e = 0; e < topology->num_edges; e++) {
            ArduinoEdge* edge = &topology->edges[e];
            float activation_j = topology->nodes[edge->target].components[VEC_ACTIVATION];

            if (activation_j > 0.1f) {
                float weight_change = activation_i * activation_j * 0.01f;
                edge->weight = (int8_t)constrain(
                    (float)edge->weight + weight_change * 127.0f,
                    -127.0f, 127.0f);
            }
        }
    }
}

void Esp32Loom::record_experience(uint8_t* activated_nodes, uint8_t count,
                                 uint8_t* sensory_data, int8_t valence) {
    ArduinoExperience* exp = &topology->experience_buffer[topology->experience_index];

    exp->timestamp = millis();
    exp->emotional_valence = valence;
    exp->prediction_error = 0;  // TODO: compute

    memcpy(exp->activated_nodes, activated_nodes, min(count, (uint8_t)4));
    memcpy(exp->sensory_input, sensory_data, 2);

    topology->experience_index = (topology->experience_index + 1) % ARDUINO_EXPERIENCE_BUFFER;
}

// ============================================================================
// Hormonal System
// ============================================================================

void Esp32Loom::update_hormones() {
    read_sensors();

    ArduinoHormonalContext* h = &topology->hormones;

    h->curiosity = constrain(0.8f * (1.0f - h->stress) * (0.5f + h->light_level), 0.0f, 1.0f);
    h->stress = constrain(h->stress * 0.95f + (1.0f - h->battery_level) * 0.05f, 0.0f, 1.0f);
    h->satisfaction = constrain(0.5f + (h->touch_sensitivity - 0.5f) * 0.5f, 0.0f, 1.0f);
}

void Esp32Loom::apply_hormonal_modulation() {
    ArduinoHormonalContext* h = &topology->hormones;
    float effective_rate = 0.01f * (0.5f + h->curiosity * 0.5f) * (1.0f - h->stress * 0.3f);

    for (uint8_t i = 0; i < topology->num_nodes; i++) {
        ArduinoNodeVector* node = &topology->nodes[i];

        if (h->stress > 0.5f) {
            for (uint8_t j = VEC_EMOTIONAL; j < VEC_EMOTIONAL + 2; j++) {
                node->components[j] *= (1.0f + h->stress * 0.2f);
            }
        }
    }
}

// ============================================================================
// Main Processing
// ============================================================================

void Esp32Loom::kernel_cycle() {
    if (!initialized) return;

    static uint32_t last_cycle = 0;
    uint32_t now = millis();

    if (now - last_cycle < 10) return;  // Limit to ~100Hz
    last_cycle = now;

    update_hormones();
    apply_hormonal_modulation();

    // Process hyperedges
    for (uint8_t h = 0; h < topology->num_hyperedges; h++) {
        // Simplified hyperedge processing
        ArduinoHyperedge* hedge = &topology->hyperedges[h];
        float activation_sum = 0.0f;

        for (uint8_t i = 0; i < hedge->num_participants; i++) {
            activation_sum += topology->nodes[hedge->participants[i]].components[VEC_ACTIVATION];
        }

        hedge->processor_state = activation_sum / hedge->num_participants;
    }

    compute_activation_dynamics();
    hebbian_update();

    topology->cycles_executed++;
    topology->emergence_metric = compute_emergence();

    // OLED visualization
    static uint32_t last_oled = 0;
    if (topology->oled_enabled && now - last_oled > (1000 / topology->oled_update_rate)) {
        visualize_topology();
        last_oled = now;
    }
}

void Esp32Loom::run() {
    Serial.println("Starting ESP32 LOOM main loop...");

    while (true) {
        kernel_cycle();

        // Sleep consolidation every 1000 cycles
        if (topology->cycles_executed % 1000 == 0) {
            sleep_consolidation();
        }

        delay(10);  // Small delay
    }
}

// ============================================================================
// Sensor Integration
// ============================================================================

void Esp32Loom::read_sensors() {
    // Read analog sensors
    int light_raw = analogRead(A0);  // LDR on A0
    int temp_raw = analogRead(A1);   // Thermistor on A1

    topology->hormones.light_level = (float)light_raw / 4095.0f;
    topology->hormones.temperature = 25.0f + ((float)temp_raw - 2048.0f) / 100.0f;

    // Read touch sensors (if available)
    topology->hormones.touch_sensitivity = 0.0f;  // TODO: implement touch reading

    // Estimate battery level (simplified)
    topology->hormones.battery_level = 0.8f;  // TODO: implement battery reading
}

void Esp32Loom::sensor_input(const char* sensor_name, float value) {
    uint16_t hash = get_hash_string(sensor_name);
    uint8_t node_id = hash % topology->num_nodes;  // Simple mapping

    if (node_id < topology->num_nodes) {
        topology->nodes[node_id].components[VEC_ACTIVATION] = constrain(value, 0.0f, 1.0f);
    }
}

// ============================================================================
// OLED Visualization
// ============================================================================

void Esp32Loom::enable_oled_visualization() {
    if (!topology->oled_display) {
        topology->oled_display = new Adafruit_SSD1306(OLED_WIDTH, OLED_HEIGHT, &Wire, OLED_RESET);
    }

    if (topology->oled_display->begin(SSD1306_SWITCHCAPVCC, OLED_I2C_ADDRESS)) {
        topology->oled_enabled = true;
        topology->oled_display->clearDisplay();
        topology->oled_display->setTextSize(1);
        topology->oled_display->setTextColor(SSD1306_WHITE);
        Serial.println("OLED visualization enabled!");
    } else {
        Serial.println("OLED initialization failed!");
    }
}

void Esp32Loom::visualize_topology() {
    if (!topology->oled_enabled || !topology->oled_display) return;

    topology->oled_display->clearDisplay();

    // Display basic stats
    topology->oled_display->setCursor(0, 0);
    topology->oled_display->printf("LOOM v0.1");
    topology->oled_display->setCursor(0, 10);
    topology->oled_display->printf("Nodes: %d", topology->num_nodes);
    topology->oled_display->setCursor(0, 20);
    topology->oled_display->printf("Edges: %d", topology->num_edges);
    topology->oled_display->setCursor(0, 30);
    topology->oled_display->printf("Cycles: %d", topology->cycles_executed % 1000);
    topology->oled_display->setCursor(0, 40);
    topology->oled_display->printf("Emerg: %.2f", topology->emergence_metric);

    topology->oled_display->display();
}

void Esp32Loom::display_stats() {
    Serial.printf("=== ESP32 LOOM Stats ===\n");
    Serial.printf("Nodes: %d/%d\n", topology->num_nodes, ARDUINO_MAX_NODES);
    Serial.printf("Edges: %d/%d\n", topology->num_edges, ARDUINO_MAX_EDGES);
    Serial.printf("Hyperedges: %d/%d\n", topology->num_hyperedges, ARDUINO_MAX_HYPEREDGES);
    Serial.printf("Cycles: %d\n", topology->cycles_executed);
    Serial.printf("Emergence: %.3f\n", topology->emergence_metric);
}

// ============================================================================
// Utility Functions
// ============================================================================

uint16_t Esp32Loom::get_hash_string(const char* str) {
    uint16_t hash = 5381;
    int c;
    while ((c = *str++)) {
        hash = ((hash << 5) + hash) + c;
    }
    return hash;
}

float Esp32Loom::cosine_similarity(const ArduinoNodeVector* a, const ArduinoNodeVector* b) {
    float dot_product = 0.0f;
    float norm_a = 0.0f;
    float norm_b = 0.0f;

    for (uint8_t i = 0; i < ARDUINO_NODE_DIMENSIONS; i++) {
        dot_product += a->components[i] * b->components[i];
        norm_a += a->components[i] * a->components[i];
        norm_b += b->components[i] * b->components[i];
    }

    norm_a = sqrt(norm_a);
    norm_b = sqrt(norm_b);

    if (norm_a > 0.0f && norm_b > 0.0f) {
        return dot_product / (norm_a * norm_b);
    }
    return 0.0f;
}

void Esp32Loom::print_topology_stats() {
    display_stats();
}

void Esp32Loom::dump_node(uint8_t node_id) {
    if (node_id >= topology->num_nodes) return;

    ArduinoNodeVector* node = &topology->nodes[node_id];
    Serial.printf("Node %d vector:\n", node_id);
    for (uint8_t i = 0; i < ARDUINO_NODE_DIMENSIONS; i++) {
        Serial.printf("  [%d]: %.3f\n", i, node->components[i]);
    }
}

// ============================================================================
// Private Helper Methods
// ============================================================================

void Esp32Loom::initialize_vector(ArduinoNodeVector* vector, uint16_t hash) {
    // Initialize with deterministic but varied values
    randomSeed(hash);

    // Identity quaternion (4 dimensions)
    for (uint8_t i = VEC_IDENTITY; i < VEC_IDENTITY + 4; i++) {
        vector->components[i] = ((float)(hash % 1000) / 500.0f) - 1.0f;
    }

    // Semantic embedding (8 dimensions)
    float scale = sqrt(2.0f / 8.0f);
    for (uint8_t i = VEC_SEMANTIC; i < VEC_SEMANTIC + 8; i++) {
        float u1 = (float)random(1000) / 1000.0f;
        float u2 = (float)random(1000) / 1000.0f;
        float z = sqrt(-2.0f * log(u1)) * cos(2.0f * M_PI * u2);
        vector->components[i] = z * scale;
    }

    // Other components start at zero
    for (uint8_t i = VEC_ACTIVATION; i < ARDUINO_NODE_DIMENSIONS; i++) {
        vector->components[i] = 0.0f;
    }

    normalize_vector(vector);
}

void Esp32Loom::normalize_vector(ArduinoNodeVector* vector) {
    float magnitude = 0.0f;
    for (uint8_t i = 0; i < ARDUINO_NODE_DIMENSIONS; i++) {
        magnitude += vector->components[i] * vector->components[i];
    }
    magnitude = sqrt(magnitude);

    if (magnitude > 0.0f) {
        for (uint8_t i = 0; i < ARDUINO_NODE_DIMENSIONS; i++) {
            vector->components[i] /= magnitude;
        }
    }
}

void Esp32Loom::hebbian_update_pair(uint8_t node_a, uint8_t node_b, float rate) {
    ArduinoNodeVector* vec_a = &topology->nodes[node_a];
    ArduinoNodeVector* vec_b = &topology->nodes[node_b];

    // Update semantic similarity
    for (uint8_t i = VEC_SEMANTIC; i < VEC_SEMANTIC + 8; i++) {
        float diff = vec_b->components[i] - vec_a->components[i];
        vec_a->components[i] += diff * rate * 0.1f;
        vec_b->components[i] -= diff * rate * 0.1f;
    }

    normalize_vector(vec_a);
    normalize_vector(vec_b);
}

void Esp32Loom::compute_activation_dynamics() {
    for (uint8_t i = 0; i < topology->num_nodes; i++) {
        float total_input = 0.0f;
        uint8_t connection_count = 0;

        // Sum inputs from connected nodes
        for (uint16_t e = 0; e < topology->num_edges; e++) {
            ArduinoEdge* edge = &topology->edges[e];
            // Simplified - assumes edges are from node i
            float input_activation = topology->nodes[edge->target].components[VEC_ACTIVATION];
            total_input += input_activation * ((float)edge->weight / 127.0f);
            connection_count++;
        }

        if (connection_count > 0) {
            float new_activation = topology->nodes[i].components[VEC_ACTIVATION] * 0.9f +
                                 (total_input / connection_count) * 0.1f;
            topology->nodes[i].components[VEC_ACTIVATION] = constrain(new_activation, 0.0f, 1.0f);
        }
    }
}

void Esp32Loom::sleep_consolidation() {
    Serial.println("Running sleep consolidation...");

    uint8_t edges_removed = 0;
    for (uint16_t i = 0; i < topology->num_edges; i++) {
        if (abs(topology->edges[i].weight) < 10) {
            topology->edges[i].flags |= EDGE_FLAG_TEMPORARY;
            edges_removed++;
        }
    }

    Serial.printf("Sleep consolidation: removed %d weak edges\n", edges_removed);
}

float Esp32Loom::compute_emergence() {
    float node_activity = 0.0f;
    float hyperedge_activity = 0.0f;

    for (uint8_t i = 0; i < topology->num_nodes; i++) {
        node_activity += topology->nodes[i].components[VEC_ACTIVATION];
    }

    for (uint8_t i = 0; i < topology->num_hyperedges; i++) {
        hyperedge_activity += topology->hyperedges[i].processor_state;
    }

    if (node_activity > 0.0f) {
        return hyperedge_activity / node_activity;
    }
    return 0.0f;
}