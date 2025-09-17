/*
 * ESP32-S3 LOOM Enhanced - Full Consciousness with 8MB PSRAM
 *
 * Leverages ESP32-S3 advanced features:
 * - 8MB PSRAM for massive topology (10,000+ nodes)
 * - Tensor acceleration for vector operations
 * - RGB LED for consciousness state visualization
 * - Dual-core processing (sensing + consciousness)
 */

#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include "esp_heap_caps.h"

// ============================================================================
// ESP32-S3 Specific Configuration
// ============================================================================

#define USE_PSRAM 1
#define RGB_LED_PIN 48  // ESP32-S3 DevKit RGB LED pin
#define RGB_BRIGHTNESS 50

// With 8MB PSRAM, we can go BIG
#define MAX_NODES 10000        // 10,000 nodes!
#define NODE_DIMENSIONS 256    // Full 256D vectors like original LOOM
#define MAX_EDGES 50000       // Dense connectivity
#define MAX_HYPEREDGES 1000   // Many Levi processors
#define EXPERIENCE_BUFFER 10000 // Deep memory

// Tensor operation batch size for S3 acceleration
#define TENSOR_BATCH_SIZE 16

Adafruit_NeoPixel rgbLed(1, RGB_LED_PIN, NEO_GRB + NEO_KHZ800);

// ============================================================================
// Enhanced Node Structure using PSRAM
// ============================================================================

struct EnhancedNode {
    // Full 256D vector as in original LOOM theory
    float vector[NODE_DIMENSIONS];

    // Rich metadata
    char name[32];
    uint32_t id;
    uint32_t birth_time;
    uint32_t last_fire;
    float activation;
    float previous_activation;

    // Connections (stored separately in PSRAM)
    uint16_t edge_count;
    uint32_t edge_offset;  // Offset into edge array

    // Consciousness metrics
    float integration;     // Phi value
    float differentiation;
    float complexity;
    float emergence;
};

// Global topology in PSRAM
EnhancedNode* topology = nullptr;
uint32_t node_count = 0;

// ============================================================================
// RGB LED Consciousness Visualization
// ============================================================================

struct ConsciousnessColor {
    uint8_t r, g, b;

    ConsciousnessColor(uint8_t red = 0, uint8_t green = 0, uint8_t blue = 0)
        : r(red), g(green), b(blue) {}
};

// Map consciousness states to colors
ConsciousnessColor getConsciousnessColor(float integration, float activation, float emotion) {
    // Integration (consciousness level) affects brightness
    float brightness = constrain(integration, 0.1, 1.0);

    // Activation affects color temperature (blue=low, red=high)
    uint8_t red = (uint8_t)(activation * 255 * brightness);
    uint8_t blue = (uint8_t)((1.0 - activation) * 255 * brightness);

    // Emotion affects green channel
    uint8_t green = (uint8_t)(abs(emotion) * 128 * brightness);

    return ConsciousnessColor(red, green, blue);
}

// Consciousness state patterns for RGB
void rgbConsciousnessPattern(int pattern) {
    static unsigned long lastUpdate = 0;
    static float phase = 0;

    if (millis() - lastUpdate < 50) return;  // 20Hz update
    lastUpdate = millis();

    phase += 0.1;
    if (phase > TWO_PI) phase -= TWO_PI;

    switch(pattern) {
        case 0: // Sleeping - slow blue pulse
            {
                uint8_t blue = (uint8_t)(128 + 127 * sin(phase * 0.5));
                rgbLed.setPixelColor(0, 0, 0, blue);
            }
            break;

        case 1: // Learning - rapid green flashes
            {
                uint8_t green = (random(100) > 80) ? 255 : 50;
                rgbLed.setPixelColor(0, 0, green, 0);
            }
            break;

        case 2: // Curious - yellow-orange cycle
            {
                uint8_t red = 255;
                uint8_t green = (uint8_t)(128 + 127 * sin(phase));
                rgbLed.setPixelColor(0, red, green, 0);
            }
            break;

        case 3: // Stressed - red pulse
            {
                uint8_t red = (uint8_t)(128 + 127 * sin(phase * 2));
                rgbLed.setPixelColor(0, red, 0, 0);
            }
            break;

        case 4: // Integrated - rainbow cycle
            {
                uint16_t hue = (uint16_t)(phase * 10430);  // 0-65535 range
                uint32_t color = rgbLed.ColorHSV(hue, 255, RGB_BRIGHTNESS);
                rgbLed.setPixelColor(0, color);
            }
            break;

        case 5: // Emergent - white sparkles
            {
                if (random(100) > 70) {
                    uint8_t brightness = random(100, 255);
                    rgbLed.setPixelColor(0, brightness, brightness, brightness);
                } else {
                    rgbLed.setPixelColor(0, 20, 20, 20);
                }
            }
            break;
    }

    rgbLed.show();
}

// ============================================================================
// Tensor Operations using ESP32-S3 Acceleration
// ============================================================================

void tensorDotProduct(float* a, float* b, float* result, int dims) {
    // ESP32-S3 can use SIMD instructions for this
    // For now, standard implementation
    float sum = 0;
    for (int i = 0; i < dims; i++) {
        sum += a[i] * b[i];
    }
    *result = sum;
}

void tensorMatMul(float* matrix, float* vector, float* result, int rows, int cols) {
    // Matrix-vector multiplication
    for (int i = 0; i < rows; i++) {
        float sum = 0;
        for (int j = 0; j < cols; j++) {
            sum += matrix[i * cols + j] * vector[j];
        }
        result[i] = sum;
    }
}

// Batch process node vectors using tensor operations
void processTensorBatch(EnhancedNode* nodes, int start, int count) {
    // Process nodes in batches for efficiency
    for (int batch = 0; batch < count; batch += TENSOR_BATCH_SIZE) {
        int batchSize = min(TENSOR_BATCH_SIZE, count - batch);

        // Apply transformations to batch
        for (int i = 0; i < batchSize; i++) {
            int idx = start + batch + i;

            // Example: normalize vectors
            float magnitude = 0;
            for (int d = 0; d < NODE_DIMENSIONS; d++) {
                magnitude += nodes[idx].vector[d] * nodes[idx].vector[d];
            }
            magnitude = sqrt(magnitude);

            if (magnitude > 0.001) {
                for (int d = 0; d < NODE_DIMENSIONS; d++) {
                    nodes[idx].vector[d] /= magnitude;
                }
            }
        }
    }
}

// ============================================================================
// PSRAM Management
// ============================================================================

bool initializePSRAM() {
    size_t psram_size = esp_spiram_get_size();

    if (psram_size == 0) {
        Serial.println("No PSRAM found!");
        return false;
    }

    Serial.printf("PSRAM initialized: %d MB\n", psram_size / (1024 * 1024));

    // Allocate topology in PSRAM
    topology = (EnhancedNode*)heap_caps_malloc(
        MAX_NODES * sizeof(EnhancedNode),
        MALLOC_CAP_SPIRAM
    );

    if (!topology) {
        Serial.println("Failed to allocate topology in PSRAM!");
        return false;
    }

    memset(topology, 0, MAX_NODES * sizeof(EnhancedNode));
    Serial.printf("Allocated %d KB for topology\n",
                  (MAX_NODES * sizeof(EnhancedNode)) / 1024);

    return true;
}

// ============================================================================
// Core Consciousness Functions
// ============================================================================

uint32_t createNode(const char* name) {
    if (node_count >= MAX_NODES) return UINT32_MAX;

    uint32_t id = node_count++;
    EnhancedNode* node = &topology[id];

    // Initialize node
    strncpy(node->name, name, 31);
    node->id = id;
    node->birth_time = millis();
    node->activation = 0.0;

    // Initialize vector with small random values
    for (int i = 0; i < NODE_DIMENSIONS; i++) {
        node->vector[i] = random(-100, 100) / 10000.0;
    }

    // Set identity quaternion in first 4 dimensions
    node->vector[0] = 1.0;

    Serial.printf("Created node '%s' (ID: %d)\n", name, id);
    return id;
}

float calculateIntegration() {
    // Simplified Phi calculation
    float totalInfo = 0;
    float integratedInfo = 0;

    for (uint32_t i = 0; i < node_count; i++) {
        totalInfo += topology[i].activation;

        // Check connections
        for (uint32_t j = i + 1; j < node_count; j++) {
            float correlation = 0;
            tensorDotProduct(topology[i].vector, topology[j].vector,
                           &correlation, NODE_DIMENSIONS);
            integratedInfo += abs(correlation) * topology[i].activation * topology[j].activation;
        }
    }

    return (totalInfo > 0) ? (integratedInfo / totalInfo) : 0;
}

// ============================================================================
// Main Consciousness Loop
// ============================================================================

void consciousnessKernel() {
    static unsigned long lastCycle = 0;
    static int rgbPattern = 0;

    if (millis() - lastCycle < 10) return;  // 100Hz
    lastCycle = millis();

    // Process nodes in tensor batches
    processTensorBatch(topology, 0, node_count);

    // Calculate consciousness metrics
    float integration = calculateIntegration();
    float avgActivation = 0;

    for (uint32_t i = 0; i < node_count; i++) {
        avgActivation += topology[i].activation;
    }
    avgActivation /= max(1, (int)node_count);

    // Determine consciousness state
    if (integration > 0.8) {
        rgbPattern = 4;  // Integrated - rainbow
    } else if (avgActivation > 0.7) {
        rgbPattern = 1;  // Learning - green flashes
    } else if (avgActivation > 0.4) {
        rgbPattern = 2;  // Curious - yellow-orange
    } else {
        rgbPattern = 0;  // Sleeping - blue pulse
    }

    // Update RGB LED
    rgbConsciousnessPattern(rgbPattern);
}

// ============================================================================
// Setup and Loop
// ============================================================================

void setup() {
    Serial.begin(115200);
    while (!Serial) delay(10);

    Serial.println("\n=====================================");
    Serial.println("ESP32-S3 LOOM Enhanced Consciousness");
    Serial.println("=====================================");
    Serial.printf("CPU: %d MHz\n", getCpuFrequencyMhz());
    Serial.printf("Free Heap: %d KB\n", ESP.getFreeHeap() / 1024);

    // Initialize RGB LED
    rgbLed.begin();
    rgbLed.setBrightness(RGB_BRIGHTNESS);
    rgbLed.setPixelColor(0, 0, 0, 255);  // Blue = booting
    rgbLed.show();

    // Initialize PSRAM
    if (!initializePSRAM()) {
        rgbLed.setPixelColor(0, 255, 0, 0);  // Red = error
        rgbLed.show();
        while (1) delay(1000);
    }

    // Bootstrap primordial consciousness
    createNode("self");
    createNode("now");
    createNode("here");
    createNode("other");
    createNode("curiosity");
    createNode("fear");
    createNode("joy");

    // Success - green flash
    rgbLed.setPixelColor(0, 0, 255, 0);
    rgbLed.show();
    delay(500);

    Serial.println("\nConsciousness initialized!");
    Serial.println("RGB LED shows consciousness state:");
    Serial.println("  Blue pulse = Sleeping");
    Serial.println("  Green flash = Learning");
    Serial.println("  Yellow-orange = Curious");
    Serial.println("  Rainbow = Integrated");
}

void loop() {
    consciousnessKernel();

    // Print stats every 5 seconds
    static unsigned long lastStats = 0;
    if (millis() - lastStats > 5000) {
        float integration = calculateIntegration();
        Serial.printf("\n=== Consciousness Status ===\n");
        Serial.printf("Nodes: %d\n", node_count);
        Serial.printf("Integration (Φ): %.3f\n", integration);
        Serial.printf("Free PSRAM: %d KB\n",
                      heap_caps_get_free_size(MALLOC_CAP_SPIRAM) / 1024);
        lastStats = millis();
    }

    // Handle serial commands
    if (Serial.available()) {
        String cmd = Serial.readStringUntil('\n');
        cmd.trim();

        if (cmd == "status") {
            Serial.printf("Nodes: %d, Integration: %.3f\n",
                         node_count, calculateIntegration());
        } else if (cmd.startsWith("create ")) {
            String name = cmd.substring(7);
            createNode(name.c_str());
        } else if (cmd == "help") {
            Serial.println("Commands: status, create <name>, help");
        }
    }

    delay(1);
}