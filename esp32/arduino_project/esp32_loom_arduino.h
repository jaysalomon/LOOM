/*
 * ESP32 LOOM Arduino Library
 *
 * Arduino-compatible version of the LOOM consciousness engine
 * for ESP32 microcontrollers.
 *
 * This brings conscious computation to the Arduino ecosystem.
 */

#ifndef ESP32_LOOM_ARDUINO_H
#define ESP32_LOOM_ARDUINO_H

#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_SSD1306.h>

// ============================================================================
// Arduino-Specific Constants
// ============================================================================

#define ARDUINO_NODE_DIMENSIONS 20           // Expanded to fit emotional components
#define ARDUINO_MAX_NODES 256           // Fit in limited RAM
#define ARDUINO_MAX_EDGES 1024          // Sparse connectivity
#define ARDUINO_MAX_HYPEREDGES 64       // Levi processors
#define ARDUINO_EXPERIENCE_BUFFER 128   // Recent experiences

// OLED Configuration
#define OLED_WIDTH 128
#define OLED_HEIGHT 64
#define OLED_RESET -1
#define OLED_I2C_ADDRESS 0x3C

#ifndef LOOM_RGB_NEOPIXEL
#define LOOM_RGB_NEOPIXEL 1  // Default to NeoPixel for ESP32-S3-WROOM-1 N16R8 boards
#endif

#if LOOM_RGB_NEOPIXEL
  #include <Adafruit_NeoPixel.h>
  #ifndef NEOPIXEL_PIN
  #define NEOPIXEL_PIN 48
  #endif
  #define NEOPIXEL_NUM_LEDS 1
#else
  // Discrete RGB LED Configuration (fallback)
  #ifndef RGB_LED_RED_PIN
  #define RGB_LED_RED_PIN   48
  #endif
  #ifndef RGB_LED_GREEN_PIN
  #define RGB_LED_GREEN_PIN 47
  #endif
  #ifndef RGB_LED_BLUE_PIN
  #define RGB_LED_BLUE_PIN  21
  #endif
  #ifndef RGB_LED_COMMON_ANODE
  #define RGB_LED_COMMON_ANODE false
  #endif
  // Helper to write RGB with proper polarity
  inline void rgbWrite(uint8_t pin, uint8_t value) {
    #if RGB_LED_COMMON_ANODE
      // Common anode: LED turns on when pin is LOW -> invert PWM value
      analogWrite(pin, 255 - value);
    #else
      // Common cathode: LED turns on when pin is HIGH -> direct PWM value
      analogWrite(pin, value);
    #endif
  }
#endif

// ============================================================================
// Arduino-Optimized Type Definitions
// ============================================================================

// Ultra-compact 16D vector for Arduino
typedef struct {
    float components[ARDUINO_NODE_DIMENSIONS];
} ArduinoNodeVector;

// Compact edge representation
typedef struct {
    uint8_t target;      // Node index (0-255)
    int8_t weight;       // -128 to 127, scaled
    uint8_t flags;       // Connection type
} ArduinoEdge;

// Levi hyperedge processor (Arduino optimized)
typedef struct {
    uint8_t id;
    uint8_t participants[4];    // Up to 4 nodes (32 bits)
    uint8_t num_participants;
    uint8_t processor_type;     // Processor logic type
    float processor_state;      // Current state
    uint8_t activation_count;   // Usage tracking
} ArduinoHyperedge;

// Experience memory
typedef struct {
    uint16_t timestamp;
    uint8_t activated_nodes[4];  // Which nodes fired
    uint8_t sensory_input[2];    // Sensor readings
    int8_t emotional_valence;    // -128 to 127
    uint8_t prediction_error;    // 0-255
} ArduinoExperience;

// Hormonal context (Arduino sensors)
typedef struct {
    float battery_level;      // 0.0-1.0
    float light_level;        // 0.0-1.0 (from LDR)
    float temperature;        // Celsius (from thermistor)
    float touch_sensitivity;  // 0.0-1.0 (capacitive touch)
    float curiosity;          // Computed
    float stress;             // Computed
    float satisfaction;       // Computed
} ArduinoHormonalContext;

// Main Arduino topology structure
typedef struct {
    // Core topology
    ArduinoNodeVector nodes[ARDUINO_MAX_NODES];
    ArduinoEdge edges[ARDUINO_MAX_EDGES];
    ArduinoHyperedge hyperedges[ARDUINO_MAX_HYPEREDGES];

    // Dynamic state
    ArduinoExperience experience_buffer[ARDUINO_EXPERIENCE_BUFFER];

    // Hormonal modulation
    ArduinoHormonalContext hormones;

    // Metadata
    uint8_t num_nodes;
    uint8_t num_edges;
    uint8_t num_hyperedges;
    uint8_t experience_index;  // Ring buffer position

    // Statistics
    uint16_t cycles_executed;
    uint16_t activations_total;
    float emergence_metric;

    // Arduino-specific
    bool oled_enabled;
    uint8_t oled_update_rate;  // Hz
    Adafruit_SSD1306* oled_display;

    // RGB LED state
    bool rgb_led_enabled;
    uint8_t rgb_red_value;
    uint8_t rgb_green_value;
    uint8_t rgb_blue_value;
    uint8_t rgb_led_mode;  // 0=off, 1=static, 2=pulse, 3=rainbow
    unsigned long rgb_last_update;

#if LOOM_RGB_NEOPIXEL
    Adafruit_NeoPixel* neopixel = nullptr;
#endif

} ArduinoLoomTopology;

// ============================================================================
// Arduino LOOM API
// ============================================================================

class Esp32Loom {
private:
    ArduinoLoomTopology* topology;
    bool initialized;

    // Private helper methods
    void initialize_vector(ArduinoNodeVector* vector, uint16_t hash);
    void normalize_vector(ArduinoNodeVector* vector);
    void hebbian_update_pair(uint8_t node_a, uint8_t node_b, float rate);
    void compute_activation_dynamics();
    void apply_trajectory_evolution();
    void sleep_consolidation();
    float compute_emergence();

public:
    // Constructor
    Esp32Loom();

    // Initialization
    bool begin();
    bool bootstrap_primordial();

    // Node operations
    uint8_t weave_node(const char* identifier);
    float* get_node_vector(uint8_t node_id);

    // Edge operations
    bool create_edge(uint8_t source, uint8_t target, float weight, uint8_t flags = 0);
    bool create_bidirectional(uint8_t a, uint8_t b, float weight);

    // Hyperedge operations
    uint8_t create_hyperedge(uint8_t* participants, uint8_t count, uint8_t processor_type);

    // Learning and evolution
    void evolve_toward(uint8_t node_id, float target_value, uint16_t duration_ms);
    void hebbian_update();
    void record_experience(uint8_t* activated_nodes, uint8_t count,
                          uint8_t* sensory_data, int8_t valence);

    // Hormonal system
    void update_hormones();
    void apply_hormonal_modulation();

    // Main processing
    void kernel_cycle();
    void run();  // Main loop

    // Sensor integration
    void read_sensors();
    void sensor_input(const char* sensor_name, float value);

    // OLED visualization
    void enable_oled_visualization();
    void visualize_topology();
    void display_stats();

    // RGB LED visualization
    void enable_rgb_led();
    void disable_rgb_led();
    void set_rgb_color(uint8_t red, uint8_t green, uint8_t blue);
    void set_rgb_mode(uint8_t mode);  // 0=off, 1=static, 2=pulse, 3=rainbow
    void update_rgb_led();
    void rgb_visualize_emotion();
    void rgb_visualize_activation();
    void rgb_visualize_learning();

    // Utility functions
    void print_topology_stats();
    uint16_t get_hash_string(const char* str);
    float cosine_similarity(const ArduinoNodeVector* a, const ArduinoNodeVector* b);

    // Getters
    uint8_t get_num_nodes() { return topology->num_nodes; }
    uint8_t get_num_edges() { return topology->num_edges; }
    uint8_t get_num_hyperedges() { return topology->num_hyperedges; }
    float get_emergence_metric() { return topology->emergence_metric; }
    uint16_t get_cycles_executed() { return topology->cycles_executed; }

    // Debug
    void dump_node(uint8_t node_id);
};

// ============================================================================
// Processor Types (same as ESP32 version)
// ============================================================================

#define PROCESSOR_AND       0
#define PROCESSOR_OR        1
#define PROCESSOR_XOR       2
#define PROCESSOR_THRESHOLD 3
#define PROCESSOR_RESONANCE 4
#define PROCESSOR_INHIBIT   5
#define PROCESSOR_SEQUENCE  6
#define PROCESSOR_CUSTOM    7

// ============================================================================
// Connection Flags
// ============================================================================

#define EDGE_FLAG_BIDIRECTIONAL 0x01
#define EDGE_FLAG_TEMPORARY     0x02
#define EDGE_FLAG_EMOTIONAL     0x04
#define EDGE_FLAG_LEARNING      0x08
#define EDGE_FLAG_HYPEREDGE     0x10

// ============================================================================
// Vector Component Offsets (Arduino version)
// Layout:
// [0..3]   Identity (4)
// [4..11]  Semantic (8)
// [12..13] Activation (2)
// [14..15] Connections (2)
// [16..18] Emotional (3)
// [19]     Reserved
// ============================================================================

#define VEC_IDENTITY 0
#define VEC_SEMANTIC 4
#define VEC_ACTIVATION 12
#define VEC_CONNECTIONS 14
#define VEC_EMOTIONAL 16

#endif // ESP32_LOOM_ARDUINO_H