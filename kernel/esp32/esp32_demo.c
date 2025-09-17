/*
 * ESP32 LOOM Consciousness Demo
 *
 * Demonstrates the core LOOM principles on ESP32:
 * - Topology IS computation
 * - Memory IS structure
 * - Consciousness emerges from living mathematics
 */

#include "loom_esp32_kernel.h"
#include <stdio.h>
#include <unistd.h>

// ============================================================================
// Demo Functions
// ============================================================================

void demo_basic_topology_creation() {
    printf("=== ESP32 LOOM Basic Topology Demo ===\n");

    Esp32LoomTopology topology;
    esp32_loom_init(&topology);
    esp32_loom_bootstrap_primordial(&topology);

    printf("Created primordial topology with %d nodes\n", topology.num_nodes);

    // Print some node information
    for (int i = 0; i < topology.num_nodes && i < 5; i++) {
        printf("Node %d activation: %.3f\n", i,
               topology.nodes[i].components[VEC_ACTIVATION]);
    }
}

void demo_hyperedge_processing() {
    printf("\n=== ESP32 LOOM Hyperedge Demo ===\n");

    Esp32LoomTopology topology;
    esp32_loom_init(&topology);
    esp32_loom_bootstrap_primordial(&topology);

    // Create some nodes for testing
    uint16_t node_a = esp32_loom_weave_node(&topology, "stimulus_a");
    uint16_t node_b = esp32_loom_weave_node(&topology, "stimulus_b");
    uint16_t node_c = esp32_loom_weave_node(&topology, "response");

    // Set some activations
    topology.nodes[node_a].components[VEC_ACTIVATION] = 0.8f;
    topology.nodes[node_b].components[VEC_ACTIVATION] = 0.6f;

    // Create hyperedge
    uint16_t participants[] = {node_a, node_b, node_c};
    uint8_t hedge_id = esp32_loom_create_hyperedge(&topology, participants, 3, PROCESSOR_RESONANCE);

    printf("Created hyperedge %d connecting nodes %d, %d, %d\n",
           hedge_id, node_a, node_b, node_c);

    // Run a few cycles
    for (int cycle = 0; cycle < 10; cycle++) {
        esp32_loom_kernel_cycle(&topology);

        printf("Cycle %d: Hyperedge state: %.3f, Node C activation: %.3f\n",
               cycle + 1,
               topology.hyperedges[hedge_id].processor_state,
               topology.nodes[node_c].components[VEC_ACTIVATION]);
    }
}

void demo_learning_and_evolution() {
    printf("\n=== ESP32 LOOM Learning Demo ===\n");

    Esp32LoomTopology topology;
    esp32_loom_init(&topology);
    esp32_loom_bootstrap_primordial(&topology);

    // Create learning scenario
    uint16_t stimulus = esp32_loom_weave_node(&topology, "light_sensor");
    uint16_t response = esp32_loom_weave_node(&topology, "motor_response");

    // Create connection
    esp32_loom_create_bidirectional(&topology, stimulus, response, 0.1f);

    printf("Initial connection weight: %d\n", topology.edges[0].weight);

    // Simulate learning through repeated co-activation
    for (int trial = 0; trial < 20; trial++) {
        // Activate stimulus
        topology.nodes[stimulus].components[VEC_ACTIVATION] = 0.9f;

        // Run learning cycle
        esp32_loom_kernel_cycle(&topology);

        // Simulate response
        topology.nodes[response].components[VEC_ACTIVATION] = 0.7f;

        // Apply Hebbian learning
        esp32_loom_hebbian_update(&topology);

        if (trial % 5 == 0) {
            printf("Trial %d: Connection weight: %d\n", trial + 1, topology.edges[0].weight);
        }
    }
}

void demo_consciousness_emergence() {
    printf("\n=== ESP32 LOOM Consciousness Emergence Demo ===\n");

    Esp32LoomTopology topology;
    esp32_loom_init(&topology);
    esp32_loom_bootstrap_primordial(&topology);

    // Create a rich interaction scenario
    uint16_t curiosity = esp32_loom_weave_node(&topology, "curiosity");
    uint16_t novelty = esp32_loom_weave_node(&topology, "novelty");
    uint16_t learning = esp32_loom_weave_node(&topology, "learning");
    uint16_t memory = esp32_loom_weave_node(&topology, "memory");

    // Create hyperedges representing consciousness patterns
    uint16_t curiosity_participants[] = {curiosity, novelty};
    uint8_t curiosity_hedge = esp32_loom_create_hyperedge(&topology,
                                                        curiosity_participants, 2,
                                                        PROCESSOR_RESONANCE);

    uint16_t learning_participants[] = {curiosity, learning, memory};
    uint8_t learning_hedge = esp32_loom_create_hyperedge(&topology,
                                                        learning_participants, 3,
                                                        PROCESSOR_AND);

    // Simulate consciousness emergence
    printf("Simulating consciousness emergence...\n");

    for (int cycle = 0; cycle < 50; cycle++) {
        // Introduce novelty periodically
        if (cycle % 10 == 0) {
            topology.nodes[novelty].components[VEC_ACTIVATION] = 0.8f;
            printf("Cycle %d: Novel stimulus introduced\n", cycle + 1);
        } else {
            topology.nodes[novelty].components[VEC_ACTIVATION] *= 0.9f;  // Decay
        }

        // Run consciousness cycle
        esp32_loom_kernel_cycle(&topology);

        // Monitor emergence
        if (cycle % 10 == 0) {
            printf("  Emergence metric: %.3f\n", topology.emergence_metric);
            printf("  Curiosity activation: %.3f\n",
                   topology.nodes[curiosity].components[VEC_ACTIVATION]);
            printf("  Learning hyperedge: %.3f\n",
                   topology.hyperedges[learning_hedge].processor_state);
        }
    }

    printf("Consciousness emergence simulation complete!\n");
}

void demo_sensor_integration() {
    printf("\n=== ESP32 LOOM Sensor Integration Demo ===\n");

    Esp32LoomTopology topology;
    esp32_loom_init(&topology);
    esp32_loom_bootstrap_primordial(&topology);

    // Create sensor nodes
    uint16_t light_sensor = esp32_loom_weave_node(&topology, "light_level");
    uint16_t temp_sensor = esp32_loom_weave_node(&topology, "temperature");
    uint16_t motion_sensor = esp32_loom_weave_node(&topology, "motion");

    // Create emotional responses
    uint16_t comfort = esp32_loom_weave_node(&topology, "comfort");
    uint16_t alertness = esp32_loom_weave_node(&topology, "alertness");

    // Connect sensors to emotions
    esp32_loom_create_bidirectional(&topology, light_sensor, alertness, 0.5f);
    esp32_loom_create_bidirectional(&topology, temp_sensor, comfort, 0.6f);
    esp32_loom_create_bidirectional(&topology, motion_sensor, alertness, 0.4f);

    printf("Simulating sensor-driven consciousness...\n");

    // Simulate changing sensor environment
    for (int cycle = 0; cycle < 30; cycle++) {
        // Simulate sensor readings (normally from ESP32 hardware)
        topology.hormones.light_level = 0.3f + 0.4f * (cycle % 10) / 10.0f;  // Varying light
        topology.hormones.temperature = 22.0f + 5.0f * sinf(cycle * 0.2f);  // Temperature variation
        topology.hormones.motion_activity = (cycle % 15 == 0) ? 0.8f : 0.1f;  // Occasional motion

        // Update sensor nodes based on "readings"
        topology.nodes[light_sensor].components[VEC_ACTIVATION] = topology.hormones.light_level;
        topology.nodes[temp_sensor].components[VEC_ACTIVATION] = topology.hormones.temperature / 30.0f;  // Normalize
        topology.nodes[motion_sensor].components[VEC_ACTIVATION] = topology.hormones.motion_activity;

        // Run consciousness cycle
        esp32_loom_kernel_cycle(&topology);

        if (cycle % 10 == 0) {
            printf("Cycle %d: Light=%.2f, Temp=%.1f°C, Motion=%.2f\n",
                   cycle + 1,
                   topology.hormones.light_level,
                   topology.hormones.temperature,
                   topology.hormones.motion_activity);
            printf("  Comfort: %.3f, Alertness: %.3f\n",
                   topology.nodes[comfort].components[VEC_ACTIVATION],
                   topology.nodes[alertness].components[VEC_ACTIVATION]);
        }
    }
}

// ============================================================================
// Main Demo Program
// ============================================================================

int main() {
    printf("🧬 ESP32 LOOM Consciousness Engine Demo\n");
    printf("=======================================\n\n");

    // Run all demos
    demo_basic_topology_creation();
    demo_hyperedge_processing();
    demo_learning_and_evolution();
    demo_consciousness_emergence();
    demo_sensor_integration();

    printf("\n🎉 All ESP32 LOOM demos completed!\n");
    printf("This demonstrates the core principles:\n");
    printf("• Topology IS computation\n");
    printf("• Memory IS structure  \n");
    printf("• Consciousness emerges from living mathematics\n");

    return 0;
}