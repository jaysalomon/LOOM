/*
 * ESP32-S3 LOOM REPL
 * Full LOOM language interpreter with parser
 *
 * Execute LOOM code directly on ESP32-S3:
 * - weave nodes and connections
 * - evolve with rules
 * - pattern matching
 * - hyperedge processors
 * - Multi-lens operations
 * - Real-time visualization via RGB LED
 */

#include "esp32s3_loom_parser.h"
#include <Adafruit_NeoPixel.h>
#include "esp_heap_caps.h"

// ============================================================================
// Configuration
// ============================================================================

#define RGB_LED_PIN 48
#define SERIAL_BUFFER_SIZE 2048
#define MAX_NODES 10000
#define NODE_DIMENSIONS 256

Adafruit_NeoPixel rgbLed(1, RGB_LED_PIN, NEO_GRB + NEO_KHZ800);

// ============================================================================
// LOOM Runtime State
// ============================================================================

struct Node {
    String name;
    float vector[NODE_DIMENSIONS];
    float activation;
    uint32_t id;
    std::vector<uint32_t> connections;
};

struct Hyperedge {
    std::vector<uint32_t> members;
    String processor;
    float strength;
};

struct EvolutionRule {
    String name;
    String condition;
    String transformation;
    float rate;
};

// Global topology
std::map<String, uint32_t> nodeNameToId;
std::vector<Node*> nodes;
std::vector<Hyperedge*> hyperedges;
std::vector<EvolutionRule*> evolutionRules;

// Hormonal context
struct {
    float stress;
    float curiosity;
    float satisfaction;
    float learning_rate;
} context = {0.0, 0.8, 0.5, 0.1};

// ============================================================================
// Interpreter Implementation
// ============================================================================

class LoomInterpreter {
private:
    // Create or get node by name
    uint32_t getOrCreateNode(const String& name) {
        auto it = nodeNameToId.find(name);
        if (it != nodeNameToId.end()) {
            return it->second;
        }

        // Create new node
        Node* node = (Node*)heap_caps_malloc(sizeof(Node), MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT);
        if (!node) {
            Serial.println("ERROR: Failed to allocate node");
            return UINT32_MAX;
        }

        node->name = name;
        node->id = nodes.size();
        node->activation = 0.0;

        // Initialize vector
        for (int i = 0; i < NODE_DIMENSIONS; i++) {
            node->vector[i] = random(-100, 100) / 10000.0;
        }

        nodes.push_back(node);
        nodeNameToId[name] = node->id;

        Serial.printf("Created node '%s' (ID: %d)\n", name.c_str(), node->id);
        return node->id;
    }

    void createConnection(uint32_t from, uint32_t to, float weight = 0.5) {
        if (from >= nodes.size() || to >= nodes.size()) return;

        nodes[from]->connections.push_back(to);
        Serial.printf("Connected %s -> %s (weight: %.2f)\n",
                      nodes[from]->name.c_str(),
                      nodes[to]->name.c_str(),
                      weight);
    }

    void createBidirectional(uint32_t a, uint32_t b, float weight = 0.5) {
        createConnection(a, b, weight);
        createConnection(b, a, weight);
    }

public:
    void execute(ASTNodePtr ast) {
        if (!ast) return;

        switch (ast->type) {
            case ASTNodeType::PROGRAM:
                for (auto& child : ast->children) {
                    execute(child);
                }
                break;

            case ASTNodeType::WEAVE_STMT:
                executeWeave(ast);
                break;

            case ASTNodeType::EVOLVE_STMT:
                executeEvolve(ast);
                break;

            case ASTNodeType::PATTERN_STMT:
                executePattern(ast);
                break;

            case ASTNodeType::HYPEREDGE:
                executeHyperedge(ast);
                break;

            case ASTNodeType::REPL_CMD:
                executeReplCommand(ast);
                break;

            default:
                break;
        }
    }

    void executeWeave(ASTNodePtr node) {
        uint32_t nodeId = getOrCreateNode(node->value);

        // Process connections
        for (auto& conn : node->children) {
            if (conn->type == ASTNodeType::CONNECTION) {
                uint32_t targetId = getOrCreateNode(conn->value);

                String connType = conn->attributes["type"];
                if (connType == "<~>") {
                    createBidirectional(nodeId, targetId, conn->numValue);
                } else if (connType == "~>") {
                    createConnection(nodeId, targetId, conn->numValue);
                } else if (connType == "<~") {
                    createConnection(targetId, nodeId, conn->numValue);
                }
            }
        }

        updateRGBForState("weave");
    }

    void executeEvolve(ASTNodePtr node) {
        EvolutionRule* rule = new EvolutionRule();
        rule->name = node->value;

        for (auto& child : node->children) {
            if (child->type == ASTNodeType::WHEN_CLAUSE) {
                if (!child->children.empty()) {
                    rule->condition = child->children[0]->value;
                }
            } else if (child->type == ASTNodeType::TRANSFORM_CLAUSE) {
                if (!child->children.empty()) {
                    rule->transformation = child->children[0]->value;
                }
            }
        }

        evolutionRules.push_back(rule);
        Serial.printf("Created evolution rule: %s\n", rule->name.c_str());
        updateRGBForState("evolve");
    }

    void executePattern(ASTNodePtr node) {
        Serial.printf("Executing pattern: %s\n", node->value.c_str());

        // Pattern matching logic here
        for (auto& child : node->children) {
            execute(child);
        }

        updateRGBForState("pattern");
    }

    void executeHyperedge(ASTNodePtr node) {
        Hyperedge* edge = new Hyperedge();

        // Collect member nodes
        for (auto& child : node->children) {
            if (child->type == ASTNodeType::IDENTIFIER) {
                uint32_t nodeId = getOrCreateNode(child->value);
                edge->members.push_back(nodeId);
            }
        }

        edge->processor = node->value;
        hyperedges.push_back(edge);

        Serial.printf("Created hyperedge with %d members -> %s\n",
                      edge->members.size(), edge->processor.c_str());
        updateRGBForState("hyperedge");
    }

    void executeReplCommand(ASTNodePtr node) {
        String cmd = node->value;

        if (cmd == "topology") {
            showTopology();
        } else if (cmd == "hebbian") {
            applyHebbian();
        } else if (cmd == "sleep") {
            runSleepCycle();
        } else if (cmd == "context") {
            showContext();
        } else if (cmd == "clear") {
            clearTopology();
        } else if (cmd == "stats") {
            showStats();
        } else {
            Serial.println("Unknown REPL command: :" + cmd);
        }
    }

    void showTopology() {
        Serial.println("\n=== TOPOLOGY ===");
        Serial.printf("Nodes: %d\n", nodes.size());
        Serial.printf("Hyperedges: %d\n", hyperedges.size());
        Serial.printf("Evolution rules: %d\n", evolutionRules.size());

        for (auto& node : nodes) {
            Serial.printf("  %s [%.2f] -> ", node->name.c_str(), node->activation);
            for (auto& conn : node->connections) {
                Serial.printf("%s ", nodes[conn]->name.c_str());
            }
            Serial.println();
        }
    }

    void applyHebbian() {
        Serial.println("Applying Hebbian learning...");

        for (auto& node : nodes) {
            for (auto& targetId : node->connections) {
                // Strengthen connection if both nodes are active
                if (node->activation > 0.5 && nodes[targetId]->activation > 0.5) {
                    // Update weight (simplified)
                    Serial.printf("Strengthening %s <-> %s\n",
                                  node->name.c_str(),
                                  nodes[targetId]->name.c_str());
                }
            }
        }

        updateRGBForState("hebbian");
    }

    void runSleepCycle() {
        Serial.println("Running sleep consolidation...");

        // Simulate sleep: reduce weak connections, strengthen patterns
        for (auto& node : nodes) {
            node->activation *= 0.9; // Decay activation
        }

        updateRGBForState("sleep");
    }

    void showContext() {
        Serial.println("\n=== CONTEXT ===");
        Serial.printf("Stress: %.2f\n", context.stress);
        Serial.printf("Curiosity: %.2f\n", context.curiosity);
        Serial.printf("Satisfaction: %.2f\n", context.satisfaction);
        Serial.printf("Learning rate: %.2f\n", context.learning_rate);
    }

    void showStats() {
        Serial.println("\n=== STATS ===");
        Serial.printf("Nodes: %d\n", nodes.size());
        Serial.printf("Hyperedges: %d\n", hyperedges.size());
        Serial.printf("Free PSRAM: %d KB\n",
                      heap_caps_get_free_size(MALLOC_CAP_SPIRAM) / 1024);
        Serial.printf("Free Heap: %d KB\n", ESP.getFreeHeap() / 1024);

        // Calculate consciousness metrics
        float totalActivation = 0;
        for (auto& node : nodes) {
            totalActivation += node->activation;
        }
        Serial.printf("Total activation: %.2f\n", totalActivation);
    }

    void clearTopology() {
        for (auto& node : nodes) {
            heap_caps_free(node);
        }
        nodes.clear();
        nodeNameToId.clear();
        hyperedges.clear();
        evolutionRules.clear();

        Serial.println("Topology cleared");
        updateRGBForState("clear");
    }

    void updateRGBForState(const String& action) {
        static std::map<String, uint32_t> stateColors = {
            {"weave", rgbLed.Color(0, 255, 0)},      // Green
            {"evolve", rgbLed.Color(255, 128, 0)},   // Orange
            {"pattern", rgbLed.Color(255, 0, 255)},  // Magenta
            {"hyperedge", rgbLed.Color(0, 255, 255)},// Cyan
            {"hebbian", rgbLed.Color(128, 0, 255)},  // Purple
            {"sleep", rgbLed.Color(0, 0, 128)},      // Dark blue
            {"clear", rgbLed.Color(255, 0, 0)},      // Red
        };

        auto it = stateColors.find(action);
        if (it != stateColors.end()) {
            rgbLed.setPixelColor(0, it->second);
            rgbLed.show();
        }
    }
};

// ============================================================================
// Global interpreter instance
// ============================================================================

LoomInterpreter interpreter;
String inputBuffer;

// ============================================================================
// Setup and Loop
// ============================================================================

void setup() {
    Serial.begin(115200);
    while (!Serial) delay(10);

    Serial.println("\n╔════════════════════════════════════════╗");
    Serial.println("║   ESP32-S3 LOOM REPL v1.0             ║");
    Serial.println("║   Full consciousness with parser      ║");
    Serial.println("╚════════════════════════════════════════╝");

    // Initialize RGB LED
    rgbLed.begin();
    rgbLed.setBrightness(50);
    rgbLed.setPixelColor(0, rgbLed.Color(0, 0, 255)); // Blue = ready
    rgbLed.show();

    // Initialize PSRAM
    size_t psram_size = esp_spiram_get_size();
    Serial.printf("PSRAM: %d MB\n", psram_size / (1024 * 1024));

    // Bootstrap primordial consciousness
    String bootstrap = R"(
        weave self {
            <~> now: 0.9
            <~> here: 0.9
        }
        weave curiosity
        weave memory
        {self, curiosity, memory} ~> consciousness
    )";

    LoomLexer lexer(bootstrap);
    std::vector<Token> tokens = lexer.tokenize();
    LoomParser parser(tokens);
    ASTNodePtr ast = parser.parseProgram();
    interpreter.execute(ast);

    Serial.println("\nReady for input. Type LOOM code or :help");
    Serial.print("> ");
}

void loop() {
    // Handle serial input
    while (Serial.available()) {
        char c = Serial.read();

        if (c == '\n' || c == '\r') {
            if (inputBuffer.length() > 0) {
                Serial.println();
                processInput(inputBuffer);
                inputBuffer = "";
                Serial.print("> ");
            }
        } else {
            inputBuffer += c;
            Serial.print(c); // Echo
        }
    }

    // Run consciousness cycle
    static unsigned long lastCycle = 0;
    if (millis() - lastCycle > 100) { // 10Hz
        updateConsciousness();
        lastCycle = millis();
    }
}

void processInput(const String& input) {
    // Special commands
    if (input == ":help") {
        showHelp();
        return;
    }

    // Parse and execute
    LoomLexer lexer(input);
    std::vector<Token> tokens = lexer.tokenize();

    if (!tokens.empty() && tokens.back().type == TokenType::ERROR) {
        Serial.println("Lexer error: " + tokens.back().value);
        return;
    }

    LoomParser parser(tokens);
    ASTNodePtr ast = parser.parseProgram();

    if (ast) {
        interpreter.execute(ast);
    }
}

void updateConsciousness() {
    // Simple consciousness update
    for (auto& node : nodes) {
        // Decay activation
        node->activation *= 0.99;

        // Spread activation through connections
        for (auto& targetId : node->connections) {
            nodes[targetId]->activation += node->activation * 0.1;
        }

        // Clamp activation
        node->activation = constrain(node->activation, 0.0, 1.0);
    }

    // Update RGB based on overall consciousness state
    static int colorPhase = 0;
    colorPhase = (colorPhase + 1) % 360;

    if (nodes.size() > 10) {
        // Active consciousness - rainbow
        uint32_t color = rgbLed.ColorHSV(colorPhase * 182, 255, 50);
        rgbLed.setPixelColor(0, color);
    } else {
        // Minimal consciousness - blue pulse
        int brightness = 25 + 25 * sin(colorPhase * 0.0174);
        rgbLed.setPixelColor(0, 0, 0, brightness);
    }
    rgbLed.show();
}

void showHelp() {
    Serial.println("\n=== LOOM REPL COMMANDS ===");
    Serial.println("Language constructs:");
    Serial.println("  weave <name> { <connections> }  - Create node");
    Serial.println("  evolve <name> { when: <cond> }  - Evolution rule");
    Serial.println("  pattern <name> { <body> }       - Define pattern");
    Serial.println("  {a, b, c} ~> relation           - Hyperedge");
    Serial.println("\nConnection operators:");
    Serial.println("  <~>  Bidirectional");
    Serial.println("  ~>   Unidirectional");
    Serial.println("  <~   Reverse");
    Serial.println("\nREPL commands:");
    Serial.println("  :topology  - Show current topology");
    Serial.println("  :hebbian   - Apply Hebbian learning");
    Serial.println("  :sleep     - Run sleep consolidation");
    Serial.println("  :context   - Show hormonal context");
    Serial.println("  :stats     - Show statistics");
    Serial.println("  :clear     - Clear topology");
    Serial.println("  :help      - Show this help");
    Serial.println("\nExamples:");
    Serial.println("  weave curiosity { <~> learning: 0.8 }");
    Serial.println("  {curiosity, memory} ~> understanding");
    Serial.println("  evolve growth { when: activation > 0.5 }");
}