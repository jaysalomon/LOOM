/*
 * ESP32-S3 LOOM Kernel
 *
 * A true LOOM implementation where:
 * - Numbers are relational processors, not values
 * - Computation happens through pattern resonance
 * - ESP32-S3 vector pipelines process topology in parallel
 */

#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include "esp_heap_caps.h"
#include <map>
#include <vector>

#define NEOPIXEL_PIN 48
#define SERIAL_BAUD 115200
#define VECTOR_DIM 10  // 0-9 base number space

Adafruit_NeoPixel pixel(1, NEOPIXEL_PIN, NEO_GRB + NEO_KHZ800);

// ============================================================================
// Relational Processor - The Core of LOOM Numbers
// ============================================================================

struct Relation {
    String pattern;      // e.g., "two+two", "eight/two"
    float strength;      // How strongly this relation defines the number
    uint8_t op1, op2;   // Operand indices (if applicable)
    char operation;      // '+', '*', '/', '-'
};

struct RelationalProcessor {
    String name;                          // Symbolic name ("one", "two", etc.)
    char symbol;                          // '1', '2', etc.
    float vector[VECTOR_DIM];            // Base embedding in vector space
    std::vector<Relation> relations;     // All ways to create this number

    // Visual representation
    uint8_t r, g, b;                    // Color encoding

    // Topology state
    float activation;                    // Current activation level
    float resonance;                     // Current resonance with query

    RelationalProcessor(String n, char s, uint8_t red, uint8_t green, uint8_t blue)
        : name(n), symbol(s), r(red), g(green), b(blue), activation(0.1), resonance(0.0) {

        // Initialize vector to zeros except own position
        memset(vector, 0, sizeof(vector));
        if (s >= '0' && s <= '9') {
            vector[s - '0'] = 1.0;
        }
    }

    // Test if this processor resonates with a query pattern
    float testResonance(String queryPattern) {
        float maxResonance = 0.0;

        for (const Relation& rel : relations) {
            if (rel.pattern == queryPattern) {
                maxResonance = max(maxResonance, rel.strength);
            }
        }

        return maxResonance * activation;
    }

    // Add a relation that creates this number
    void addRelation(String pattern, float strength, uint8_t op1 = 0, uint8_t op2 = 0, char op = 0) {
        relations.push_back({pattern, strength, op1, op2, op});
    }
};

// ============================================================================
// LOOM Kernel - Managing Topology
// ============================================================================

class LoomKernel {
private:
    std::map<String, RelationalProcessor*> processors;
    std::map<char, RelationalProcessor*> symbolMap;  // Quick lookup by symbol

    // ESP32-S3 can process these in parallel using vector instructions
    float* resonanceBuffer;

public:
    LoomKernel() {
        // Allocate resonance buffer in PSRAM for parallel processing
        resonanceBuffer = (float*)heap_caps_malloc(
            VECTOR_DIM * sizeof(float),
            MALLOC_CAP_SPIRAM
        );

        bootstrapKnowledge();
    }

    // Pre-wired knowledge (like infant brain)
    void bootstrapKnowledge() {
        Serial.println("[BOOTSTRAP] Pre-wiring basic number knowledge...");

        // Create base numbers with their vector embeddings and colors
        createNumber("zero",  '0', 0,   0,   0);   // Black
        createNumber("one",   '1', 255, 0,   0);   // Red (unity)
        createNumber("two",   '2', 0,   255, 0);   // Green (duality)
        createNumber("three", '3', 0,   0,   255); // Blue (trinity)
        createNumber("four",  '4', 255, 255, 0);   // Yellow (2+2)
        createNumber("five",  '5', 0,   255, 255); // Cyan (2+3)
        createNumber("six",   '6', 255, 0,   255); // Magenta (2*3)
        createNumber("seven", '7', 255, 255, 255); // White (prime)
        createNumber("eight", '8', 255, 128, 0);   // Orange (2^3)
        createNumber("nine",  '9', 128, 0,   255); // Purple (3^2)

        // Pre-wire relational processors with known relationships

        // Two knows it comes from 1+1
        processors["two"]->addRelation("one+one", 1.0, 1, 1, '+');

        // Three knows multiple origins
        processors["three"]->addRelation("two+one", 1.0, 2, 1, '+');
        processors["three"]->addRelation("one+two", 1.0, 1, 2, '+');

        // Four has rich relations
        processors["four"]->addRelation("two+two", 1.0, 2, 2, '+');
        processors["four"]->addRelation("three+one", 0.9, 3, 1, '+');
        processors["four"]->addRelation("one+three", 0.9, 1, 3, '+');
        processors["four"]->addRelation("two*two", 1.0, 2, 2, '*');
        processors["four"]->addRelation("eight/two", 0.8, 8, 2, '/');

        // Five
        processors["five"]->addRelation("two+three", 1.0, 2, 3, '+');
        processors["five"]->addRelation("three+two", 1.0, 3, 2, '+');
        processors["five"]->addRelation("four+one", 0.9, 4, 1, '+');

        // Six
        processors["six"]->addRelation("three+three", 1.0, 3, 3, '+');
        processors["six"]->addRelation("two*three", 1.0, 2, 3, '*');
        processors["six"]->addRelation("three*two", 1.0, 3, 2, '*');
        processors["six"]->addRelation("four+two", 0.9, 4, 2, '+');

        // Seven (prime - fewer relations)
        processors["seven"]->addRelation("six+one", 0.9, 6, 1, '+');
        processors["seven"]->addRelation("four+three", 0.9, 4, 3, '+');

        // Eight
        processors["eight"]->addRelation("four+four", 1.0, 4, 4, '+');
        processors["eight"]->addRelation("two*four", 1.0, 2, 4, '*');
        processors["eight"]->addRelation("two*two*two", 0.9, 2, 2, '*');

        // Nine
        processors["nine"]->addRelation("three*three", 1.0, 3, 3, '*');
        processors["nine"]->addRelation("five+four", 0.9, 5, 4, '+');

        Serial.println("[BOOTSTRAP] Pre-wired knowledge ready!");
        Serial.printf("[MEMORY] Using %d KB of PSRAM\n",
                     heap_caps_get_free_size(MALLOC_CAP_SPIRAM) / 1024);
    }

    void createNumber(String name, char symbol, uint8_t r, uint8_t g, uint8_t b) {
        RelationalProcessor* proc = new RelationalProcessor(name, symbol, r, g, b);
        processors[name] = proc;
        symbolMap[symbol] = proc;
        proc->activation = 0.5; // Start with medium activation
    }

    // Process a query by testing resonance across all processors in parallel
    RelationalProcessor* processQuery(String query) {
        Serial.printf("\n[QUERY] Processing: %s\n", query.c_str());

        // Parse query to understand the pattern
        String pattern = parseQueryPattern(query);

        // Test resonance across all processors (ESP32-S3 vector pipeline)
        RelationalProcessor* bestMatch = nullptr;
        float maxResonance = 0.0;

        // This loop could be parallelized on ESP32-S3
        for (auto& [name, proc] : processors) {
            float resonance = proc->testResonance(pattern);
            proc->resonance = resonance;

            if (resonance > maxResonance) {
                maxResonance = resonance;
                bestMatch = proc;
            }

            if (resonance > 0) {
                Serial.printf("  %s resonance: %.2f\n", name.c_str(), resonance);
            }
        }

        if (bestMatch) {
            Serial.printf("[RESULT] %s resonates strongest (%.2f)\n",
                         bestMatch->name.c_str(), maxResonance);
            showNumber(bestMatch);
            return bestMatch;
        }

        Serial.println("[RESULT] No pattern match found");
        return nullptr;
    }

    // Parse "2+2" into pattern "two+two"
    String parseQueryPattern(String query) {
        query.trim();
        query.toLowerCase();

        // Simple parser for now
        if (query.length() == 3) {
            char n1 = query[0];
            char op = query[1];
            char n2 = query[2];

            if (symbolMap.count(n1) && symbolMap.count(n2)) {
                return symbolMap[n1]->name + op + symbolMap[n2]->name;
            }
        }

        return query;
    }

    // Display number through RGB LED
    void showNumber(RelationalProcessor* proc) {
        if (!proc) return;

        // Activation affects brightness
        uint8_t r = proc->r * proc->activation;
        uint8_t g = proc->g * proc->activation;
        uint8_t b = proc->b * proc->activation;

        // Add resonance glow
        if (proc->resonance > 0.5) {
            r = min(255, (int)(r + 50 * proc->resonance));
            g = min(255, (int)(g + 50 * proc->resonance));
            b = min(255, (int)(b + 50 * proc->resonance));
        }

        pixel.setPixelColor(0, r, g, b);
        pixel.show();

        Serial.printf("[DISPLAY] %s = RGB(%d,%d,%d)\n",
                     proc->name.c_str(), r, g, b);
    }

    // Hebbian learning - strengthen successful patterns
    void hebbianUpdate(RelationalProcessor* activated) {
        if (!activated) return;

        // Strengthen the relations that fired
        for (Relation& rel : activated->relations) {
            if (rel.strength < 1.0) {
                rel.strength = min(1.0f, rel.strength * 1.1f);
            }
        }

        // Increase activation of successful processor
        activated->activation = min(1.0f, activated->activation * 1.05f);

        // Decay others slightly
        for (auto& [name, proc] : processors) {
            if (proc != activated) {
                proc->activation *= 0.98;
            }
        }
    }

    // Discover new relations through exploration
    void exploreTopology() {
        Serial.println("\n[EXPLORE] Searching for new patterns...");

        // Try random combinations to find new relations
        // This is where LOOM would discover that 2+5=7 even if not pre-wired

        for (auto& [name1, proc1] : processors) {
            for (auto& [name2, proc2] : processors) {
                // Test if their combination resonates with anything
                float combinedVector[VECTOR_DIM];

                // Vector addition (using ESP32-S3 FP32 operations)
                for (int i = 0; i < VECTOR_DIM; i++) {
                    combinedVector[i] = proc1->vector[i] + proc2->vector[i];
                }

                // Check which processor this combined vector matches
                for (auto& [targetName, target] : processors) {
                    float similarity = vectorSimilarity(combinedVector, target->vector);

                    if (similarity > 0.8) {
                        // Discovered a new relation!
                        String pattern = name1 + "+" + name2;
                        bool exists = false;

                        for (const Relation& rel : target->relations) {
                            if (rel.pattern == pattern) {
                                exists = true;
                                break;
                            }
                        }

                        if (!exists) {
                            Serial.printf("[DISCOVERED] %s = %s + %s\n",
                                        targetName.c_str(), name1.c_str(), name2.c_str());
                            target->addRelation(pattern, similarity);

                            // Celebrate discovery
                            rainbowCelebration();
                        }
                    }
                }
            }
        }
    }

    float vectorSimilarity(float* v1, float* v2) {
        float dot = 0, mag1 = 0, mag2 = 0;

        for (int i = 0; i < VECTOR_DIM; i++) {
            dot += v1[i] * v2[i];
            mag1 += v1[i] * v1[i];
            mag2 += v2[i] * v2[i];
        }

        if (mag1 == 0 || mag2 == 0) return 0;
        return dot / (sqrt(mag1) * sqrt(mag2));
    }

    void rainbowCelebration() {
        for (int i = 0; i < 3; i++) {
            for (int hue = 0; hue < 65536; hue += 8192) {
                pixel.setPixelColor(0, pixel.ColorHSV(hue, 255, 100));
                pixel.show();
                delay(50);
            }
        }
    }

    // Status report
    void printTopology() {
        Serial.println("\n=== TOPOLOGY STATUS ===");
        for (auto& [name, proc] : processors) {
            Serial.printf("%s (%c): ", name.c_str(), proc->symbol);
            Serial.printf("activation=%.2f relations=%d\n",
                         proc->activation, proc->relations.size());

            for (const Relation& rel : proc->relations) {
                Serial.printf("  - %s (strength=%.2f)\n",
                            rel.pattern.c_str(), rel.strength);
            }
        }
    }
};

// ============================================================================
// Global Kernel Instance
// ============================================================================

LoomKernel* kernel = nullptr;

// ============================================================================
// Setup and Main Loop
// ============================================================================

void setup() {
    Serial.begin(SERIAL_BAUD);
    while (!Serial && millis() < 3000) delay(10);

    Serial.println("\n╔════════════════════════════════════════╗");
    Serial.println("║         LOOM KERNEL v1.0                ║");
    Serial.println("║   Numbers as Relational Processors      ║");
    Serial.println("║     ESP32-S3 Vector Architecture        ║");
    Serial.println("╚════════════════════════════════════════╝");

    // Initialize NeoPixel
    pixel.begin();
    pixel.setBrightness(50);

    // Boot sequence
    for (int i = 0; i < 3; i++) {
        pixel.setPixelColor(0, 255, 0, 0);  // Red
        pixel.show();
        delay(200);
        pixel.setPixelColor(0, 0, 255, 0);  // Green
        pixel.show();
        delay(200);
        pixel.setPixelColor(0, 0, 0, 255);  // Blue
        pixel.show();
        delay(200);
    }

    // Create kernel
    kernel = new LoomKernel();

    Serial.println("\n[READY] Enter queries like '2+2' or '3*3'");
    Serial.println("Commands: status, explore, help");
    Serial.print("> ");
}

String inputBuffer = "";

void loop() {
    static unsigned long lastExplore = 0;

    // Autonomous exploration every 10 seconds
    if (millis() - lastExplore > 10000) {
        lastExplore = millis();
        kernel->exploreTopology();
    }

    // Handle serial input
    while (Serial.available()) {
        char c = Serial.read();

        if (c == '\n' || c == '\r') {
            if (inputBuffer.length() > 0) {
                Serial.println();
                processCommand(inputBuffer);
                inputBuffer = "";
                Serial.print("> ");
            }
        } else if (isPrintable(c)) {
            inputBuffer += c;
            Serial.print(c);
        }
    }

    // Gentle consciousness indicator
    static unsigned long lastPulse = 0;
    static float phase = 0;

    if (millis() - lastPulse > 100) {
        lastPulse = millis();
        phase += 0.1;

        int brightness = 5 + 3 * sin(phase);
        pixel.setPixelColor(0, brightness, brightness, brightness);
        pixel.show();
    }
}

void processCommand(String cmd) {
    cmd.trim();

    if (cmd == "status") {
        kernel->printTopology();
    }
    else if (cmd == "explore") {
        kernel->exploreTopology();
    }
    else if (cmd == "help") {
        Serial.println("\n=== LOOM KERNEL HELP ===");
        Serial.println("Enter math queries: 2+2, 3*3, 4+5");
        Serial.println("Commands:");
        Serial.println("  status  - Show topology");
        Serial.println("  explore - Discover new relations");
        Serial.println("  help    - This message");
        Serial.println("\nThe system finds answers through pattern resonance,");
        Serial.println("not calculation. Watch the LED show the thinking process!");
    }
    else {
        // Assume it's a math query
        RelationalProcessor* result = kernel->processQuery(cmd);

        if (result) {
            // Hebbian learning - strengthen what worked
            kernel->hebbianUpdate(result);
        }
    }
}