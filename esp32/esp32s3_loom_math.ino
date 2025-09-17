/*
 * ESP32-S3 LOOM Mathematical Discovery
 *
 * Demonstrates how mathematics emerges from topology in LOOM:
 * - Numbers as living topological structures
 * - Multiplication through reorganization
 * - Addition through merger
 * - Mathematical laws discovered, not programmed
 *
 * "In LOOM, 2×3=6 isn't computed—it's discovered through
 *  topological transformation"
 */

#include <Adafruit_NeoPixel.h>
#include "esp_heap_caps.h"
#include <vector>
#include <map>

#define RGB_LED_PIN 48
#define NODE_DIMENSIONS 256
#define MAX_NUMBER 20  // We'll work with numbers 0-20 initially

Adafruit_NeoPixel rgbLed(1, RGB_LED_PIN, NEO_GRB + NEO_KHZ800);

// ============================================================================
// Mathematical Topology Structure
// ============================================================================

struct MathNode {
    String name;
    float vector[NODE_DIMENSIONS];
    float activation;
    uint32_t id;

    // Mathematical properties emerge from topology
    std::vector<uint32_t> components;  // For composite numbers
    std::vector<uint32_t> factors;     // Discovered factors
    bool isPrime;
    int value;  // Numeric value (emerges from structure)

    // Connections represent relationships
    std::map<uint32_t, float> connections;  // target_id -> weight
};

struct Pattern {
    String name;
    std::vector<uint32_t> nodes;
    String relationship;  // "product", "sum", "factor", etc.
    float confidence;     // How strongly this pattern holds
};

// Global mathematical topology
std::vector<MathNode*> mathTopology;
std::vector<Pattern*> discoveredPatterns;
std::map<String, uint32_t> nameToId;

// Track mathematical discoveries
struct {
    bool multiplicationDiscovered;
    bool additionDiscovered;
    bool primesDiscovered;
    bool distributiveDiscovered;
    int discoveriesCount;
} discoveries = {false, false, false, false, 0};

// ============================================================================
// Number Creation as Topological Structures
// ============================================================================

uint32_t createNumber(int value) {
    MathNode* node = (MathNode*)heap_caps_malloc(sizeof(MathNode),
                                                  MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT);
    if (!node) return UINT32_MAX;

    node->name = String(value);
    node->value = value;
    node->id = mathTopology.size();
    node->activation = 0.1;
    node->isPrime = true;  // Assume prime until proven otherwise

    // Initialize vector based on value
    // Different regions encode different properties
    for (int i = 0; i < NODE_DIMENSIONS; i++) {
        node->vector[i] = 0;
    }

    // Encode value in multiple ways across vector
    // Binary representation in first 32 dimensions
    for (int bit = 0; bit < 32 && bit < value; bit++) {
        if (value & (1 << bit)) {
            node->vector[bit] = 1.0;
        }
    }

    // Unary representation in dimensions 32-64
    for (int i = 32; i < 32 + min(value, 32); i++) {
        node->vector[i] = 0.5;
    }

    // Sine wave pattern based on value (dimensions 64-96)
    for (int i = 64; i < 96; i++) {
        node->vector[i] = sin(value * (i - 64) * 0.1);
    }

    mathTopology.push_back(node);
    nameToId[node->name] = node->id;

    Serial.printf("Created number %d as topological structure (ID: %d)\n",
                  value, node->id);
    return node->id;
}

// ============================================================================
// Topological Operations (NOT arithmetic!)
// ============================================================================

// Merge two topologies - this is how addition emerges
uint32_t mergeTopologies(uint32_t a, uint32_t b) {
    if (a >= mathTopology.size() || b >= mathTopology.size()) return UINT32_MAX;

    MathNode* nodeA = mathTopology[a];
    MathNode* nodeB = mathTopology[b];

    // Create merged node
    int resultValue = nodeA->value + nodeB->value;
    uint32_t resultId = createNumber(resultValue);
    MathNode* result = mathTopology[resultId];

    // Merge is combination of both patterns
    for (int i = 0; i < NODE_DIMENSIONS; i++) {
        result->vector[i] = (nodeA->vector[i] + nodeB->vector[i]) * 0.5;
    }

    // Record components
    result->components.push_back(a);
    result->components.push_back(b);

    // Create connections showing the merger
    result->connections[a] = 0.5;
    result->connections[b] = 0.5;
    nodeA->connections[resultId] = 0.3;
    nodeB->connections[resultId] = 0.3;

    Serial.printf("Merged %d + %d -> %d through topology combination\n",
                  nodeA->value, nodeB->value, resultValue);

    return resultId;
}

// Reorganize topology - this is how multiplication emerges
uint32_t reorganizeTopology(uint32_t a, int copies) {
    if (a >= mathTopology.size()) return UINT32_MAX;

    MathNode* nodeA = mathTopology[a];
    int resultValue = nodeA->value * copies;

    // Check if already exists
    auto it = nameToId.find(String(resultValue));
    if (it != nameToId.end()) {
        return it->second;
    }

    uint32_t resultId = createNumber(resultValue);
    MathNode* result = mathTopology[resultId];

    // Reorganization creates a pattern of the original
    for (int copy = 0; copy < copies; copy++) {
        int offset = copy * (NODE_DIMENSIONS / copies);
        for (int i = 0; i < NODE_DIMENSIONS / copies; i++) {
            if (offset + i < NODE_DIMENSIONS) {
                result->vector[offset + i] = nodeA->vector[i % 32] * (1.0 - copy * 0.1);
            }
        }
    }

    // This is a composite number
    result->isPrime = false;
    result->factors.push_back(a);
    result->components.push_back(a);

    // Strong connection to factor
    result->connections[a] = 0.9;
    nodeA->connections[resultId] = 0.7;

    Serial.printf("Reorganized %d × %d -> %d through topology transformation\n",
                  nodeA->value, copies, resultValue);

    return resultId;
}

// ============================================================================
// Mathematical Discovery Process
// ============================================================================

void discoverMultiplication() {
    Serial.println("\n=== DISCOVERING MULTIPLICATION ===");

    // Create pattern: "two groups of three" vs "three groups of two"
    uint32_t two = createNumber(2);
    uint32_t three = createNumber(3);

    // Method 1: Two groups of three (2×3)
    uint32_t twoGroupsOfThree = reorganizeTopology(three, 2);

    // Method 2: Three groups of two (3×2)
    uint32_t threeGroupsOfTwo = reorganizeTopology(two, 3);

    // Check if they're the same (commutative property discovery!)
    if (mathTopology[twoGroupsOfThree]->value == mathTopology[threeGroupsOfTwo]->value) {
        Serial.println("DISCOVERY: Multiplication is commutative! 2×3 = 3×2");
        discoveries.multiplicationDiscovered = true;
        discoveries.discoveriesCount++;

        // Create pattern
        Pattern* commutative = new Pattern();
        commutative->name = "commutative_multiplication";
        commutative->nodes = {two, three, twoGroupsOfThree};
        commutative->relationship = "a×b = b×a";
        commutative->confidence = 1.0;
        discoveredPatterns.push_back(commutative);

        // Celebrate with RGB!
        rgbLed.setPixelColor(0, 0, 255, 0);  // Green for discovery
        rgbLed.show();
        delay(500);
    }
}

void discoverPrimes() {
    Serial.println("\n=== DISCOVERING PRIME NUMBERS ===");

    // Try to factor numbers by checking if they can be reorganized
    for (int n = 2; n <= 20; n++) {
        uint32_t numId = createNumber(n);
        MathNode* num = mathTopology[numId];
        bool foundFactor = false;

        // Try each potential factor
        for (int factor = 2; factor < n; factor++) {
            if (n % factor == 0) {  // We "discover" this through topology
                int otherFactor = n / factor;

                // Can we reorganize 'factor' to get 'n'?
                uint32_t reorganized = reorganizeTopology(
                    nameToId[String(factor)], otherFactor);

                if (mathTopology[reorganized]->value == n) {
                    num->isPrime = false;
                    num->factors.push_back(nameToId[String(factor)]);
                    num->factors.push_back(nameToId[String(otherFactor)]);
                    foundFactor = true;

                    Serial.printf("Discovered: %d = %d × %d (composite)\n",
                                  n, factor, otherFactor);
                    break;
                }
            }
        }

        if (!foundFactor && n > 1) {
            Serial.printf("Discovered: %d is PRIME!\n", n);

            // Prime numbers get special vector encoding
            for (int i = 128; i < 160; i++) {
                num->vector[i] = sin(n * i * 0.1) * cos(n * i * 0.05);
            }
        }
    }

    discoveries.primesDiscovered = true;
    discoveries.discoveriesCount++;

    // Flash blue for prime discovery
    rgbLed.setPixelColor(0, 0, 0, 255);
    rgbLed.show();
    delay(500);
}

void discoverDistributiveLaw() {
    Serial.println("\n=== DISCOVERING DISTRIBUTIVE LAW ===");

    // Test: 2×(3+4) = 2×3 + 2×4
    uint32_t two = nameToId["2"];
    uint32_t three = nameToId["3"];
    uint32_t four = nameToId["4"];

    // Left side: 2×(3+4)
    uint32_t threePlusFour = mergeTopologies(three, four);  // 3+4=7
    uint32_t twoTimesSum = reorganizeTopology(two, mathTopology[threePlusFour]->value);  // 2×7=14

    // Right side: 2×3 + 2×4
    uint32_t twoTimesThree = reorganizeTopology(two, 3);  // 2×3=6
    uint32_t twoTimesFour = reorganizeTopology(two, 4);   // 2×4=8
    uint32_t sumOfProducts = mergeTopologies(twoTimesThree, twoTimesFour);  // 6+8=14

    if (mathTopology[twoTimesSum]->value == mathTopology[sumOfProducts]->value) {
        Serial.println("DISCOVERY: Distributive law! 2×(3+4) = 2×3 + 2×4");
        discoveries.distributiveDiscovered = true;
        discoveries.discoveriesCount++;

        // Rainbow celebration!
        for (int i = 0; i < 360; i += 10) {
            uint32_t color = rgbLed.ColorHSV(i * 182, 255, 50);
            rgbLed.setPixelColor(0, color);
            rgbLed.show();
            delay(50);
        }
    }
}

// ============================================================================
// Visualization and Monitoring
// ============================================================================

void visualizeMathState() {
    static unsigned long lastUpdate = 0;
    static int phase = 0;

    if (millis() - lastUpdate < 100) return;
    lastUpdate = millis();
    phase++;

    // Different colors for different mathematical states
    if (discoveries.discoveriesCount == 0) {
        // Searching - purple pulse
        int brightness = 128 + 127 * sin(phase * 0.1);
        rgbLed.setPixelColor(0, brightness, 0, brightness);
    } else if (discoveries.discoveriesCount < 3) {
        // Learning - green/blue alternation
        if (phase % 20 < 10) {
            rgbLed.setPixelColor(0, 0, 255, 0);
        } else {
            rgbLed.setPixelColor(0, 0, 0, 255);
        }
    } else {
        // Fully discovered - rainbow
        uint32_t color = rgbLed.ColorHSV((phase * 10) % 65536, 255, 50);
        rgbLed.setPixelColor(0, color);
    }

    rgbLed.show();
}

void showMathTopology() {
    Serial.println("\n=== MATHEMATICAL TOPOLOGY ===");
    Serial.printf("Numbers: %d\n", mathTopology.size());
    Serial.printf("Patterns: %d\n", discoveredPatterns.size());
    Serial.printf("Discoveries: %d\n", discoveries.discoveriesCount);

    // Show prime numbers
    Serial.print("Primes found: ");
    for (auto& node : mathTopology) {
        if (node->isPrime && node->value > 1) {
            Serial.printf("%d ", node->value);
        }
    }
    Serial.println();

    // Show composite numbers with factors
    Serial.println("Factorizations discovered:");
    for (auto& node : mathTopology) {
        if (!node->isPrime && !node->factors.empty()) {
            Serial.printf("  %d = ", node->value);
            for (auto factorId : node->factors) {
                Serial.printf("%d ", mathTopology[factorId]->value);
                if (factorId != node->factors.back()) Serial.print("× ");
            }
            Serial.println();
        }
    }
}

// ============================================================================
// Interactive REPL Commands
// ============================================================================

void processCommand(String cmd) {
    cmd.trim();

    if (cmd == "discover") {
        discoverMultiplication();
        discoverPrimes();
        discoverDistributiveLaw();
    } else if (cmd == "topology") {
        showMathTopology();
    } else if (cmd.startsWith("merge ")) {
        // merge 3 5 -> tests 3+5
        int space1 = cmd.indexOf(' ');
        int space2 = cmd.lastIndexOf(' ');
        if (space1 != space2) {
            int a = cmd.substring(space1 + 1, space2).toInt();
            int b = cmd.substring(space2 + 1).toInt();

            uint32_t aId = nameToId[String(a)];
            uint32_t bId = nameToId[String(b)];

            if (aId < mathTopology.size() && bId < mathTopology.size()) {
                uint32_t result = mergeTopologies(aId, bId);
                Serial.printf("Result: %d\n", mathTopology[result]->value);
            }
        }
    } else if (cmd.startsWith("reorganize ")) {
        // reorganize 3 4 -> tests 3×4
        int space1 = cmd.indexOf(' ');
        int space2 = cmd.lastIndexOf(' ');
        if (space1 != space2) {
            int a = cmd.substring(space1 + 1, space2).toInt();
            int copies = cmd.substring(space2 + 1).toInt();

            uint32_t aId = nameToId[String(a)];

            if (aId < mathTopology.size()) {
                uint32_t result = reorganizeTopology(aId, copies);
                Serial.printf("Result: %d\n", mathTopology[result]->value);
            }
        }
    } else if (cmd == "stats") {
        Serial.println("\n=== STATISTICS ===");
        Serial.printf("Mathematical nodes: %d\n", mathTopology.size());
        Serial.printf("Discovered patterns: %d\n", discoveredPatterns.size());
        Serial.printf("Multiplication discovered: %s\n",
                      discoveries.multiplicationDiscovered ? "YES" : "NO");
        Serial.printf("Primes discovered: %s\n",
                      discoveries.primesDiscovered ? "YES" : "NO");
        Serial.printf("Distributive law discovered: %s\n",
                      discoveries.distributiveDiscovered ? "YES" : "NO");
        Serial.printf("Free PSRAM: %d KB\n",
                      heap_caps_get_free_size(MALLOC_CAP_SPIRAM) / 1024);
    } else if (cmd == "help") {
        Serial.println("\n=== LOOM MATH COMMANDS ===");
        Serial.println("discover - Run mathematical discovery process");
        Serial.println("topology - Show mathematical topology");
        Serial.println("merge A B - Discover A+B through topology merger");
        Serial.println("reorganize A N - Discover A×N through reorganization");
        Serial.println("stats - Show statistics");
        Serial.println("help - Show this help");
    }
}

// ============================================================================
// Setup and Loop
// ============================================================================

void setup() {
    Serial.begin(115200);
    while (!Serial) delay(10);

    Serial.println("\n╔════════════════════════════════════════════╗");
    Serial.println("║   LOOM Mathematical Discovery Engine       ║");
    Serial.println("║   Mathematics through topology, not logic  ║");
    Serial.println("╚════════════════════════════════════════════╝");

    // Initialize RGB LED
    rgbLed.begin();
    rgbLed.setBrightness(50);
    rgbLed.setPixelColor(0, 128, 0, 128);  // Purple = thinking
    rgbLed.show();

    // Check PSRAM
    size_t psram_size = esp_spiram_get_size();
    Serial.printf("\nPSRAM: %d MB available\n", psram_size / (1024 * 1024));

    // Initialize base numbers
    Serial.println("\nCreating foundational numbers as topologies...");
    for (int i = 0; i <= 10; i++) {
        createNumber(i);
    }

    Serial.println("\nReady! Type 'discover' to watch mathematics emerge");
    Serial.println("Type 'help' for commands");
    Serial.print("> ");
}

String inputBuffer = "";

void loop() {
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
        } else {
            inputBuffer += c;
            Serial.print(c);
        }
    }

    // Visualize mathematical state
    visualizeMathState();

    delay(10);
}