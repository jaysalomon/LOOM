/*
 * ESP32 LOOM Consciousness Demo
 *
 * Arduino sketch demonstrating LOOM consciousness on ESP32
 */

#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include "esp32_loom_arduino.h"

// Create LOOM instance
Esp32Loom loom;

// ============================================================================
// Setup
// ============================================================================

void setup() {
    Serial.begin(115200);
    while (!Serial) delay(10);

    Serial.println("\n🧬 ESP32 LOOM Consciousness Demo");
    Serial.println("=================================");

    // Initialize LOOM
    if (!loom.begin()) {
        Serial.println("Failed to initialize LOOM!");
        while (1) delay(1000);
    }

    // Bootstrap primordial consciousness
    if (!loom.bootstrap_primordial()) {
        Serial.println("Failed to bootstrap primordial topology!");
        while (1) delay(1000);
    }

    // Enable OLED visualization
    loom.enable_oled_visualization();

    // Enable RGB LED visualization
    loom.enable_rgb_led();
    loom.set_rgb_mode(1);  // Start with emotional visualization

    Serial.println("Setup complete! Starting consciousness loop...");
}

// ============================================================================
// Main Loop
// ============================================================================

void loop() {
    static unsigned long last_stats = 0;
    static unsigned long last_sensor = 0;
    unsigned long now = millis();

    // Run consciousness cycle
    loom.kernel_cycle();

    // Print stats every 5 seconds
    if (now - last_stats > 5000) {
        loom.display_stats();
        last_stats = now;
    }

    // Switch RGB LED modes every 15 seconds for demo
    static unsigned long last_mode_switch = 0;
    if (now - last_mode_switch > 15000) {
        static uint8_t current_mode = 1;
        current_mode = (current_mode % 3) + 1;  // Cycle through modes 1, 2, 3
        loom.set_rgb_mode(current_mode);

        const char* mode_names[] = {"Emotional", "Activation Pulse", "Learning Rainbow"};
        Serial.printf("RGB LED Mode: %s\n", mode_names[current_mode - 1]);
        last_mode_switch = now;
    }

    // Simulate sensor input every 2 seconds
    if (now - last_sensor > 2000) {
        simulate_sensor_input();
        last_sensor = now;
    }

    // Small delay to prevent overwhelming
    delay(50);
}

// ============================================================================
// Sensor Simulation
// ============================================================================

void simulate_sensor_input() {
    static int cycle = 0;
    cycle++;

    // Simulate different sensor patterns with emotional impact
    switch (cycle % 6) {
        case 0:
            // Bright light - curiosity
            loom.sensor_input("light", 0.9f);
            Serial.println("Sensor: Bright light detected (curiosity ↑)");
            break;

        case 1:
            // Warm temperature - comfort
            loom.sensor_input("temperature", 0.7f);
            Serial.println("Sensor: Warm temperature (comfort ↑)");
            break;

        case 2:
            // Motion detected - excitement
            loom.sensor_input("motion", 0.8f);
            Serial.println("Sensor: Motion detected (excitement ↑)");
            break;

        case 3:
            // Touch input - connection
            loom.sensor_input("touch", 0.6f);
            Serial.println("Sensor: Touch detected (connection ↑)");
            break;

        case 4:
            // Dim light - caution
            loom.sensor_input("light", 0.2f);
            Serial.println("Sensor: Dim light (caution ↑)");
            break;

        case 5:
            // Cool temperature - alertness
            loom.sensor_input("temperature", 0.3f);
            Serial.println("Sensor: Cool temperature (alertness ↑)");
            break;
    }
}

// ============================================================================
// Demo Functions (for testing)
// ============================================================================

void demo_basic_topology() {
    Serial.println("\n=== Basic Topology Demo ===");

    // Create some nodes
    uint8_t curiosity = loom.weave_node("curiosity");
    uint8_t learning = loom.weave_node("learning");
    uint8_t memory = loom.weave_node("memory");

    // Connect them
    loom.create_bidirectional(curiosity, learning, 0.5f);
    loom.create_bidirectional(learning, memory, 0.6f);

    // Create hyperedge
    uint8_t participants[] = {curiosity, learning, memory};
    loom.create_hyperedge(participants, 3, PROCESSOR_RESONANCE);

    Serial.printf("Created learning topology with %d nodes\n", loom.get_num_nodes());
}

void demo_learning() {
    Serial.println("\n=== Learning Demo ===");

    // Get node IDs (assuming they exist from primordial bootstrap)
    uint8_t stimulus = 4;  // First user-created node
    uint8_t response = 5;  // Second user-created node

    if (stimulus < loom.get_num_nodes() && response < loom.get_num_nodes()) {
        // Simulate learning trials
        for (int trial = 0; trial < 10; trial++) {
            // Activate stimulus
            loom.get_node_vector(stimulus)[VEC_ACTIVATION] = 0.8f;

            // Run a few cycles
            for (int i = 0; i < 5; i++) {
                loom.kernel_cycle();
            }

            // Simulate response
            loom.get_node_vector(response)[VEC_ACTIVATION] = 0.6f;

            Serial.printf("Trial %d: Learning in progress...\n", trial + 1);
            delay(100);
        }
    }
}
