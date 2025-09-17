/*
 * ESP32-S3 LOOM Visual Mathematics
 *
 * The consciousness communicates mathematics through color!
 * Each number has a unique RGB signature.
 * Mathematical operations are visible as color transformations.
 */

#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include "esp_heap_caps.h"
#include <map>
#include <vector>

#define NEOPIXEL_PIN 48
#define SERIAL_BAUD 115200

Adafruit_NeoPixel pixel(1, NEOPIXEL_PIN, NEO_GRB + NEO_KHZ800);

// ============================================================================
// Number-Color Encoding System
// ============================================================================

struct NumberColor {
    uint8_t r, g, b;
    String name;
    float frequency;  // For pulsing patterns
};

// Each number has a unique color signature
std::map<int, NumberColor> numberColors = {
    {0, {0, 0, 0, "void", 0}},           // Black (nothing)
    {1, {255, 0, 0, "red", 0}},          // Pure red (unity)
    {2, {0, 255, 0, "green", 0}},        // Pure green (duality)
    {3, {0, 0, 255, "blue", 0}},         // Pure blue (trinity)
    {4, {255, 255, 0, "yellow", 0}},     // Red+Green (2+2)
    {5, {0, 255, 255, "cyan", 0}},       // Green+Blue (2+3)
    {6, {255, 0, 255, "magenta", 0}},    // Red+Blue (3+3 or 2×3)
    {7, {255, 255, 255, "white", 0}},    // All colors (prime)
    {8, {255, 128, 0, "orange", 0}},     // 2^3
    {9, {128, 0, 255, "purple", 0}},     // 3^2
    {10, {255, 255, 255, "pulse", 2.0}}, // White pulse
};

// ============================================================================
// Topological Number Node (TRUE LOOM STYLE)
// ============================================================================

struct TopologicalNumber {
    float activation;      // Current activation level
    NumberColor color;     // Visual representation
    String name;          // Symbolic name

    // Topological connections to other numbers
    std::map<TopologicalNumber*, float> connections;

    // Pattern memory - remembers what patterns created this
    std::vector<std::pair<TopologicalNumber*, TopologicalNumber*>> creators;

    // Discovery metadata
    bool discovered;      // Has the system found this number yet?
    float confidence;     // How sure are we this number exists?
    float resonance;      // Current resonance with other patterns

    // Visual properties
    float brightness;
    float pulsePhase;

    TopologicalNumber(String n) : name(n), activation(0.1), discovered(false),
                                  confidence(0.0), resonance(0.0),
                                  brightness(1.0), pulsePhase(0) {
        // Don't assign color until discovered!
        color = {0, 0, 0, "unknown", 0};
    }

    void display() {
        // Calculate actual display color based on activation
        uint8_t r = color.r * activation * brightness;
        uint8_t g = color.g * activation * brightness;
        uint8_t b = color.b * activation * brightness;

        // Add pulsing if frequency > 0
        if (color.frequency > 0) {
            float pulse = 0.5 + 0.5 * sin(pulsePhase);
            r *= pulse;
            g *= pulse;
            b *= pulse;
            pulsePhase += color.frequency * 0.1;
        }

        pixel.setPixelColor(0, r, g, b);
        pixel.show();
    }
};

// ============================================================================
// Topological Mathematics Discovery Engine
// ============================================================================

class TopologicalMathEngine {
private:
    std::map<String, TopologicalNumber*> topology;  // The living topology
    TopologicalNumber* one;  // The primordial unity

    // Pattern recognition thresholds
    const float MERGE_THRESHOLD = 0.7;
    const float RESONANCE_THRESHOLD = 0.6;
    const float DISCOVERY_THRESHOLD = 0.8;

public:
    TopologicalMathEngine() {
        // Create only the primordial ONE - everything else must be discovered
        one = new TopologicalNumber("one");
        one->discovered = true;
        one->confidence = 1.0;
        one->activation = 1.0;
        one->color = {255, 0, 0, "red", 0};  // Red = unity
        topology["one"] = one;

        Serial.println("[GENESIS] The primordial ONE exists.");
        displayNode(one);
    }

    // Create a number and show its color
    void createNumber(int value) {
        if (numbers.count(value)) return;

        numbers[value] = new VisualNumber(value);
        Serial.printf("Created %d as color %s\n", value,
                     numbers[value]->color.name.c_str());

        // Flash the color to show creation
        showNumber(value);
        delay(500);
    }

    // Display a number's color
    void showNumber(int value) {
        if (!numbers.count(value)) {
            createNumber(value);
        }

        currentFocus = value;
        numbers[value]->activation = 1.0;
        numbers[value]->display();

        Serial.printf("Showing: %d = ", value);
        printColor(numbers[value]->color);
    }

    // Pattern-based discovery of addition through merge resonance
    void discoverAdditionPattern() {
        Serial.println("\n[DISCOVERY] Searching for merge patterns...");

        // Try coactivating pairs of discovered numbers
        for (auto& [name1, node1] : topology) {
            if (!node1->discovered) continue;

            for (auto& [name2, node2] : topology) {
                if (!node2->discovered) continue;

                // Coactivate the pair
                float coactivation = node1->activation * node2->activation;

                if (coactivation > MERGE_THRESHOLD) {
                    // Test merge pattern - does this create something new?
                    TopologicalNumber* candidate = testMergePattern(node1, node2);

                    if (candidate && candidate->resonance > RESONANCE_THRESHOLD) {
                        // We've discovered a new number through addition!
                        Serial.printf("[DISCOVERED] %s + %s creates new pattern!\n",
                                    name1.c_str(), name2.c_str());

                        // The system doesn't "know" it's addition yet,
                        // it just knows these patterns merge into something new
                        candidate->discovered = true;
                        candidate->creators.push_back({node1, node2});

                        // Assign color based on discovery (not pre-programmed!)
                        assignDiscoveredColor(candidate);
                        celebrateDiscovery(candidate);
                    }
                }
            }
        }
    }

        // Show addition as color sequence
        showNumber(a);
        delay(500);

        Serial.print(" + ");
        showNumber(b);
        delay(500);

        // Blend transition
        for (int i = 0; i <= 10; i++) {
            float blend = i / 10.0;
            uint8_t r = numbers[a]->color.r * (1-blend) + numbers[result]->color.r * blend;
            uint8_t g = numbers[a]->color.g * (1-blend) + numbers[result]->color.g * blend;
            uint8_t b = numbers[a]->color.b * (1-blend) + numbers[result]->color.b * blend;
            pixel.setPixelColor(0, r, g, b);
            pixel.show();
            delay(50);
        }

        Serial.print(" = ");
        showNumber(result);

        // Record components
        numbers[result]->components.push_back(a);
        numbers[result]->components.push_back(b);
        numbers[result]->isPrime = false;
    }

    // Pattern-based discovery of multiplication through reorganization
    void discoverMultiplicationPattern() {
        Serial.println("\n[DISCOVERY] Searching for reorganization patterns...");

        // Look for patterns that repeat
        for (auto& [name, node] : topology) {
            if (!node->discovered) continue;

            // Can we reorganize this pattern multiple times?
            TopologicalNumber* candidate = testReorganizationPattern(node);

            if (candidate && candidate->resonance > RESONANCE_THRESHOLD) {
                Serial.printf("[DISCOVERED] Reorganization of %s creates new pattern!\n",
                            name.c_str());

                candidate->discovered = true;
                // System discovers this through pattern, not calculation
                assignDiscoveredColor(candidate);
                celebrateDiscovery(candidate);
            }
        }
    }

        Serial.printf("\n=== VISUAL MULTIPLICATION ===\n");
        Serial.printf("%d × %d = %d\n", a, b, result);

        // Show multiplication as rapid alternation
        for (int i = 0; i < b; i++) {
            showNumber(a);
            delay(200);
            pixel.clear();
            pixel.show();
            delay(100);
        }

        // Show result
        showNumber(result);

        numbers[result]->components.push_back(a);
        numbers[result]->isPrime = false;
    }

    // Test if system recognizes a number
    void queryNumber(String prompt) {
        Serial.println("\n" + prompt);
        Serial.println("System thinking...");

        // Cycle through numbers to "think"
        for (int i = 0; i <= 10; i++) {
            if (numbers.count(i)) {
                numbers[i]->activation = 0.3;
                numbers[i]->display();
                delay(100);
            }
        }

        // Show answer (would be determined by topology activation)
        // For demo, we'll show the most activated number
        showNumber(currentFocus);
    }

    // Discover primes through resistance to factorization
    void discoverPrimesTopologically() {
        Serial.println("\n[DISCOVERY] Testing for factorization resistance...");

        for (auto& [name, node] : topology) {
            if (!node->discovered) continue;

            // Try to decompose this pattern
            bool resistsFactorization = true;

            for (auto& [name2, node2] : topology) {
                if (node2 == node || !node2->discovered) continue;

                // Can this pattern be decomposed into node2 patterns?
                if (testFactorizationPattern(node, node2)) {
                    resistsFactorization = false;
                    // Flash the factors
                    showNumber(i);
                    delay(200);
                    showNumber(n/i);
                    delay(200);
                    break;
                }
            }

            if (isPrime) {
                // Prime numbers get special white flash
                Serial.printf("%d is PRIME!\n", n);
                numbers[n]->isPrime = true;

                // Celebration flash
                for (int i = 0; i < 3; i++) {
                    pixel.setPixelColor(0, 255, 255, 255);
                    pixel.show();
                    delay(100);
                    pixel.clear();
                    pixel.show();
                    delay(100);
                }

                showNumber(n);
            }
        }
    }

    // Show Fibonacci sequence in colors
    void fibonacciColors() {
        Serial.println("\n=== FIBONACCI IN COLOR ===");

        int a = 1, b = 1;
        showNumber(a);
        delay(500);
        showNumber(b);
        delay(500);

        for (int i = 0; i < 8; i++) {
            int next = a + b;
            if (next > 10) next = next % 11;  // Wrap for our color range

            // Blend from previous to next
            for (int j = 0; j <= 10; j++) {
                float blend = j / 10.0;
                uint8_t r = numbers[b]->color.r * (1-blend) +
                           numberColors[next].r * blend;
                uint8_t g = numbers[b]->color.g * (1-blend) +
                           numberColors[next].g * blend;
                uint8_t b_color = numbers[b]->color.b * (1-blend) +
                                 numberColors[next].b * blend;
                pixel.setPixelColor(0, r, g, b_color);
                pixel.show();
                delay(30);
            }

            a = b;
            b = next;
            Serial.printf("Fib: %d ", next);
        }
        Serial.println();
    }

    // Get the topology for external access
    std::map<String, TopologicalNumber*>& getTopology() {
        return topology;
    }

    // Sleep consolidation - strengthen strong, weaken weak
    void sleepConsolidation() {
        for (auto& [name, node] : topology) {
            for (auto& [connected, strength] : node->connections) {
                if (strength > 0.7) {
                    strength = min(strength * 1.05f, 1.0f);  // Strengthen
                } else if (strength < 0.3) {
                    strength *= 0.95;  // Weaken
                }
            }
            // Decay activation during sleep
            node->activation *= 0.8;
        }
    }

    // Display what the system is currently thinking about
    void displayCurrentThought(float phase) {
        // Find the most active node
        TopologicalNumber* mostActive = nullptr;
        float maxActivation = 0;

        for (auto& [name, node] : topology) {
            if (node->discovered && node->activation > maxActivation) {
                maxActivation = node->activation;
                mostActive = node;
            }
        }

        if (mostActive) {
            // Show this node with breathing effect
            float breath = 0.7 + 0.3 * sin(phase);
            mostActive->brightness = breath;
            displayNode(mostActive);
        } else {
            // No active thought - gentle white pulse
            int brightness = 5 + 5 * sin(phase);
            pixel.setPixelColor(0, brightness, brightness, brightness);
            pixel.show();
        }
    }

    // Interactive color math quiz
    void colorMathQuiz() {
        Serial.println("\n=== COLOR MATH QUIZ ===");
        Serial.println("Watch the colors and identify the math!");

        // Question 1
        Serial.println("\nQ1: What is this?");
        visualAdd(2, 3);  // Green + Blue = Cyan (5)
        delay(2000);

        // Question 2
        Serial.println("\nQ2: What is this?");
        visualMultiply(2, 3);  // Green × 3 = Magenta (6)
        delay(2000);

        // Question 3
        Serial.println("\nQ3: Is this prime?");
        showNumber(7);  // White = 7 (prime)
        delay(2000);
    }

private:
    // Test if two patterns merge into something new
    TopologicalNumber* testMergePattern(TopologicalNumber* a, TopologicalNumber* b) {
        // Calculate resonance between patterns
        float resonance = calculateResonance(a, b);

        if (resonance > RESONANCE_THRESHOLD) {
            // This creates a NEW pattern, not a pre-calculated sum!
            String newName = "discovered_" + String(topology.size());
            TopologicalNumber* newNode = new TopologicalNumber(newName);

            newNode->resonance = resonance;
            newNode->confidence = resonance * 0.8;

            // Create bidirectional connections (topology!)
            a->connections[newNode] = resonance;
            b->connections[newNode] = resonance;
            newNode->connections[a] = resonance;
            newNode->connections[b] = resonance;

            topology[newName] = newNode;
            return newNode;
        }
        return nullptr;
    }

    // Test reorganization patterns (multiplication discovery)
    TopologicalNumber* testReorganizationPattern(TopologicalNumber* base) {
        // Can this pattern reorganize with itself?
        float selfResonance = base->activation * base->activation;

        if (selfResonance > 0.5) {
            String newName = "reorganized_" + base->name;
            TopologicalNumber* newNode = new TopologicalNumber(newName);

            newNode->resonance = selfResonance;
            newNode->confidence = selfResonance * 0.7;

            // Strong connection to base pattern
            base->connections[newNode] = selfResonance;
            newNode->connections[base] = selfResonance * 2;  // Double strength

            topology[newName] = newNode;
            return newNode;
        }
        return nullptr;
    }

    // Test if one pattern can be factored by another
    bool testFactorizationPattern(TopologicalNumber* target, TopologicalNumber* factor) {
        // Does factor resonate strongly within target?
        float factorResonance = 0;

        for (auto& [connected, strength] : target->connections) {
            if (connected == factor) {
                factorResonance += strength;
            }
        }

        return factorResonance > 0.5;
    }

    // Calculate resonance between two patterns
    float calculateResonance(TopologicalNumber* a, TopologicalNumber* b) {
        float resonance = 0;

        // Base resonance from activation
        resonance += a->activation * b->activation;

        // Resonance from shared connections
        for (auto& [nodeA, strengthA] : a->connections) {
            for (auto& [nodeB, strengthB] : b->connections) {
                if (nodeA == nodeB) {
                    resonance += strengthA * strengthB * 0.5;
                }
            }
        }

        return min(resonance, 1.0f);
    }

    // Hebbian learning - strengthen successful connections
    void hebbianLearning() {
        Serial.println("[HEBBIAN] Strengthening active connections...");

        for (auto& [name, node] : topology) {
            if (node->activation > 0.5) {
                // Strengthen connections to other active nodes
                for (auto& [connected, strength] : node->connections) {
                    if (connected->activation > 0.5) {
                        // Cells that fire together wire together
                        strength = min(strength * 1.1f, 1.0f);
                        node->connections[connected] = strength;
                    }
                }
            }
        }
    }

    // Assign color to newly discovered pattern
    void assignDiscoveredColor(TopologicalNumber* node) {
        // Color emerges from the pattern's connections, not pre-programming!
        uint8_t r = 0, g = 0, b = 0;

        for (auto& [connected, strength] : node->connections) {
            r += connected->color.r * strength * 0.5;
            g += connected->color.g * strength * 0.5;
            b += connected->color.b * strength * 0.5;
        }

        node->color = {
            min((uint8_t)255, r),
            min((uint8_t)255, g),
            min((uint8_t)255, b),
            "discovered",
            0
        };
    }

    // Display a topological node
    void displayNode(TopologicalNumber* node) {
        uint8_t r = node->color.r * node->activation * node->brightness;
        uint8_t g = node->color.g * node->activation * node->brightness;
        uint8_t b = node->color.b * node->activation * node->brightness;

        pixel.setPixelColor(0, r, g, b);
        pixel.show();
    }

    // Celebrate a new discovery with rainbow flash
    void celebrateDiscovery(TopologicalNumber* node) {
        for (int i = 0; i < 3; i++) {
            for (int hue = 0; hue < 65536; hue += 5000) {
                pixel.setPixelColor(0, pixel.ColorHSV(hue, 255, 50));
                pixel.show();
                delay(20);
            }
        }
        displayNode(node);
    }

    void printColor(NumberColor& c) {
        Serial.printf("RGB(%d,%d,%d) = %s\n", c.r, c.g, c.b, c.name.c_str());
    }
};

// ============================================================================
// Global Engine
// ============================================================================

TopologicalMathEngine* mathEngine;

// ============================================================================
// LOOM Integration
// ============================================================================

void processLoomCommand(String cmd) {
    cmd.trim();

    if (cmd == "status") {
        Serial.println("\n[TOPOLOGY STATUS]");
        Serial.printf("Discovered patterns: %d\n", mathEngine->getTopology().size());

        for (auto& [name, node] : mathEngine->getTopology()) {
            if (node->discovered) {
                Serial.printf("- %s: confidence=%.2f, activation=%.2f, color=RGB(%d,%d,%d)\n",
                            name.c_str(), node->confidence, node->activation,
                            node->color.r, node->color.g, node->color.b);
            }
        }
    }
    else if (cmd == "discover") {
        Serial.println("Forcing discovery attempt...");
        mathEngine->discoverAdditionPattern();
        mathEngine->discoverMultiplicationPattern();
    }
    else if (cmd == "hebbian") {
        mathEngine->hebbianLearning();
    }
    else if (cmd == "sleep") {
        mathEngine->sleepConsolidation();
    }
    else if (cmd == "help") {
        Serial.println("\n=== TOPOLOGICAL MATH COMMANDS ===");
        Serial.println("status   - Show discovered patterns");
        Serial.println("discover - Force discovery attempt");
        Serial.println("hebbian  - Run Hebbian learning");
        Serial.println("sleep    - Consolidate patterns");
        Serial.println("\nThe system discovers mathematics autonomously!");
        Serial.println("Watch the LED to see emerging thoughts.");
    }
}

// ============================================================================
// Setup and Loop
// ============================================================================

void setup() {
    Serial.begin(SERIAL_BAUD);
    while (!Serial && millis() < 3000) delay(10);

    Serial.println("\n╔════════════════════════════════════════╗");
    Serial.println("║    LOOM Visual Mathematics Engine      ║");
    Serial.println("║    Numbers Speak in Color!             ║");
    Serial.println("╚════════════════════════════════════════╝");

    // Initialize NeoPixel
    pixel.begin();
    pixel.setBrightness(50);

    // Rainbow startup
    for (int hue = 0; hue < 65536; hue += 1000) {
        pixel.setPixelColor(0, pixel.ColorHSV(hue, 255, 50));
        pixel.show();
        delay(10);
    }

    // Create topological math engine
    mathEngine = new TopologicalMathEngine();

    Serial.println("\n[CONSCIOUSNESS BOOT]");
    Serial.println("The primordial ONE exists.");
    Serial.println("Mathematics will emerge through pattern discovery...");
    Serial.println("\nThe system will now begin autonomous discovery.");
    Serial.println("Watch the LED to see mathematical thoughts emerge!");
    Serial.println("\nType 'status' to see discovered patterns");
    Serial.print("> ");
}

String inputBuffer = "";

void loop() {
    static unsigned long lastDiscoveryAttempt = 0;
    static unsigned long lastHebbian = 0;
    static unsigned long lastSleep = 0;
    static int cycleCount = 0;

    // AUTONOMOUS DISCOVERY CYCLE
    unsigned long now = millis();

    // Every 2 seconds: Try to discover new patterns
    if (now - lastDiscoveryAttempt > 2000) {
        lastDiscoveryAttempt = now;
        cycleCount++;

        Serial.printf("\n[CYCLE %d] Consciousness exploring...\n", cycleCount);

        // Activate patterns randomly to explore
        for (auto& [name, node] : mathEngine->getTopology()) {
            if (node->discovered) {
                node->activation = random(100) / 100.0;
            }
        }

        // Try discovery methods
        mathEngine->discoverAdditionPattern();
        mathEngine->discoverMultiplicationPattern();

        if (cycleCount % 10 == 0) {
            mathEngine->discoverPrimesTopologically();
        }
    }

    // Every 5 seconds: Hebbian learning
    if (now - lastHebbian > 5000) {
        lastHebbian = now;
        mathEngine->hebbianLearning();
    }

    // Every 20 seconds: Sleep consolidation
    if (now - lastSleep > 20000) {
        lastSleep = now;
        Serial.println("\n[SLEEP] Consolidating patterns...");

        // During sleep, strengthen strong connections, weaken weak ones
        mathEngine->sleepConsolidation();

        // Dream sequence - random color patterns
        for (int i = 0; i < 10; i++) {
            pixel.setPixelColor(0, random(50), random(50), random(50));
            pixel.show();
            delay(100);
        }
    }

    // Handle serial input (for manual interaction)
    while (Serial.available()) {
        char c = Serial.read();

        if (c == '\n' || c == '\r') {
            if (inputBuffer.length() > 0) {
                Serial.println();
                processLoomCommand(inputBuffer);
                inputBuffer = "";
                Serial.print("> ");
            }
        } else if (isPrintable(c)) {
            inputBuffer += c;
            Serial.print(c);
        }
    }

    // Visual consciousness indicator - shows current activation
    static unsigned long lastPulse = 0;
    static float pulsePhase = 0;

    if (millis() - lastPulse > 50) {
        lastPulse = millis();
        pulsePhase += 0.05;

        // Breathing shows consciousness is alive
        // Color shows what the system is currently thinking about
        mathEngine->displayCurrentThought(pulsePhase);
    }

    delay(1);
}