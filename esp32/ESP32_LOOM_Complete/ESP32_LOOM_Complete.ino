/*
 * ESP32-S3 LOOM Complete System
 * Full parser, interpreter, and consciousness engine
 *
 * Interact via Serial Monitor (115200 baud):
 * - Type LOOM code directly
 * - Use REPL commands (:topology, :hebbian, etc.)
 * - Set goals and watch consciousness evolve
 */

#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include "esp_heap_caps.h"
#include <vector>
#include <map>
#include <string>

// ============================================================================
// Configuration
// ============================================================================

#define NEOPIXEL_PIN 48
#define SERIAL_BAUD 115200
#define MAX_INPUT_SIZE 1024
#define NODE_DIMENSIONS 64  // Reduced for faster processing
#define MAX_NODES 5000      // With 8MB PSRAM

Adafruit_NeoPixel pixel(1, NEOPIXEL_PIN, NEO_GRB + NEO_KHZ800);

// ============================================================================
// LOOM Core Structures
// ============================================================================

struct LoomNode {
    String name;
    uint32_t id;
    float vector[NODE_DIMENSIONS];
    float activation;
    float previous_activation;
    std::map<uint32_t, float> connections;  // target -> weight
    uint32_t birth_time;

    // Consciousness properties
    float integration;
    float differentiation;
    float complexity;
};

struct Hyperedge {
    std::vector<uint32_t> members;
    String processor_type;
    float strength;

    enum ProcessorType {
        AND, OR, XOR, RESONANCE, INHIBITION, SEQUENCE, SYNCHRONY
    } type;
};

struct Goal {
    String name;
    String condition;
    float priority;
    bool achieved;
};

// ============================================================================
// Global State
// ============================================================================

std::vector<LoomNode*> topology;
std::map<String, uint32_t> nameToId;
std::vector<Hyperedge*> hyperedges;
std::vector<Goal*> goals;

struct Context {
    float stress = 0.0;
    float curiosity = 0.8;
    float satisfaction = 0.5;
    float learning_rate = 0.1;
    float temperature = 0.5;  // Randomness
} context;

String inputBuffer = "";
bool parserReady = false;

// ============================================================================
// Token Types for Parser
// ============================================================================

enum TokenType {
    // Keywords
    TOK_WEAVE, TOK_EVOLVE, TOK_PATTERN, TOK_GOAL, TOK_WHEN,
    // Operators
    TOK_BIDIRECTIONAL, TOK_UNIDIRECTIONAL, TOK_REVERSE,
    // Structural
    TOK_LBRACE, TOK_RBRACE, TOK_COLON, TOK_COMMA,
    // Literals
    TOK_IDENTIFIER, TOK_NUMBER, TOK_STRING,
    // Special
    TOK_REPL_CMD, TOK_EOF, TOK_ERROR,
    // Multi-lens
    TOK_EMOTIONAL, TOK_LOGICAL, TOK_TEMPORAL
};

struct Token {
    TokenType type;
    String value;
    float numValue;
};

// ============================================================================
// Lexer
// ============================================================================

class LoomLexer {
private:
    String input;
    int pos;

    char current() {
        return (pos < input.length()) ? input[pos] : '\0';
    }

    char peek() {
        return (pos + 1 < input.length()) ? input[pos + 1] : '\0';
    }

    void advance() {
        if (pos < input.length()) pos++;
    }

    void skipWhitespace() {
        while (isspace(current())) advance();
    }

public:
    LoomLexer(const String& src) : input(src), pos(0) {}

    Token nextToken() {
        skipWhitespace();

        if (current() == '\0') {
            return {TOK_EOF, "", 0};
        }

        // REPL commands
        if (current() == ':') {
            advance();
            String cmd = "";
            while (isalpha(current())) {
                cmd += current();
                advance();
            }
            return {TOK_REPL_CMD, cmd, 0};
        }

        // Multi-character operators
        if (current() == '<' && peek() == '~') {
            advance(); advance();
            if (current() == '>') {
                advance();
                return {TOK_BIDIRECTIONAL, "<~>", 0};
            }
            return {TOK_REVERSE, "<~", 0};
        }

        if (current() == '~' && peek() == '>') {
            advance(); advance();
            return {TOK_UNIDIRECTIONAL, "~>", 0};
        }

        // Multi-lens operators
        if (current() == '¥' || current() == '$') {  // $ as alternate for ¥
            advance();
            String content = "";
            while (current() != '¥' && current() != '$' && current() != '\0') {
                content += current();
                advance();
            }
            advance();
            return {TOK_EMOTIONAL, content, 0};
        }

        // Single character tokens
        char c = current();
        advance();

        switch (c) {
            case '{': return {TOK_LBRACE, "{", 0};
            case '}': return {TOK_RBRACE, "}", 0};
            case ':': return {TOK_COLON, ":", 0};
            case ',': return {TOK_COMMA, ",", 0};
        }

        // Numbers
        if (isdigit(c) || (c == '-' && isdigit(current()))) {
            String num = String(c);
            while (isdigit(current()) || current() == '.') {
                num += current();
                advance();
            }
            return {TOK_NUMBER, num, num.toFloat()};
        }

        // Identifiers and keywords
        if (isalpha(c) || c == '_') {
            String id = String(c);
            while (isalnum(current()) || current() == '_') {
                id += current();
                advance();
            }

            // Check for keywords
            if (id == "weave") return {TOK_WEAVE, id, 0};
            if (id == "evolve") return {TOK_EVOLVE, id, 0};
            if (id == "pattern") return {TOK_PATTERN, id, 0};
            if (id == "goal") return {TOK_GOAL, id, 0};
            if (id == "when") return {TOK_WHEN, id, 0};

            return {TOK_IDENTIFIER, id, 0};
        }

        return {TOK_ERROR, String(c), 0};
    }

    std::vector<Token> tokenize() {
        std::vector<Token> tokens;
        Token tok;
        do {
            tok = nextToken();
            tokens.push_back(tok);
        } while (tok.type != TOK_EOF && tok.type != TOK_ERROR);
        return tokens;
    }
};

// ============================================================================
// Core LOOM Functions
// ============================================================================

uint32_t getOrCreateNode(const String& name) {
    auto it = nameToId.find(name);
    if (it != nameToId.end()) {
        return it->second;
    }

    // Allocate in PSRAM
    LoomNode* node = (LoomNode*)heap_caps_malloc(sizeof(LoomNode),
                                                  MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT);
    if (!node) {
        Serial.println("ERROR: Failed to allocate node in PSRAM");
        return UINT32_MAX;
    }

    node->name = name;
    node->id = topology.size();
    node->activation = 0.1;
    node->previous_activation = 0.0;
    node->birth_time = millis();
    node->integration = 0.0;
    node->differentiation = 1.0;
    node->complexity = 0.0;

    // Initialize vector with small random values
    for (int i = 0; i < NODE_DIMENSIONS; i++) {
        node->vector[i] = random(-100, 100) / 1000.0;
    }

    topology.push_back(node);
    nameToId[name] = node->id;

    Serial.printf("→ Created node '%s' (ID: %d)\n", name.c_str(), node->id);
    return node->id;
}

void createConnection(uint32_t from, uint32_t to, float weight) {
    if (from >= topology.size() || to >= topology.size()) return;

    topology[from]->connections[to] = weight;
    Serial.printf("→ Connected %s ~> %s (%.2f)\n",
                  topology[from]->name.c_str(),
                  topology[to]->name.c_str(),
                  weight);
}

void createBidirectional(uint32_t a, uint32_t b, float weight) {
    createConnection(a, b, weight);
    createConnection(b, a, weight);
}

// ============================================================================
// Parser/Interpreter
// ============================================================================

class LoomInterpreter {
private:
    std::vector<Token> tokens;
    size_t current;

    Token peek() {
        if (current < tokens.size()) return tokens[current];
        return {TOK_EOF, "", 0};
    }

    Token consume() {
        if (current < tokens.size()) return tokens[current++];
        return {TOK_EOF, "", 0};
    }

    bool match(TokenType type) {
        if (peek().type == type) {
            consume();
            return true;
        }
        return false;
    }

public:
    LoomInterpreter(const std::vector<Token>& toks) : tokens(toks), current(0) {}

    void execute() {
        while (peek().type != TOK_EOF && peek().type != TOK_ERROR) {
            executeStatement();
        }
    }

    void executeStatement() {
        Token tok = peek();

        switch (tok.type) {
            case TOK_WEAVE:
                executeWeave();
                break;

            case TOK_EVOLVE:
                executeEvolve();
                break;

            case TOK_GOAL:
                executeGoal();
                break;

            case TOK_REPL_CMD:
                executeReplCommand();
                break;

            case TOK_LBRACE:
                executeHyperedge();
                break;

            case TOK_IDENTIFIER:
                // Could be a connection statement
                executeExpression();
                break;

            default:
                Serial.println("Unknown statement: " + tok.value);
                consume();
                break;
        }
    }

    void executeWeave() {
        consume(); // 'weave'

        if (peek().type != TOK_IDENTIFIER) {
            Serial.println("Expected identifier after 'weave'");
            return;
        }

        String nodeName = consume().value;
        uint32_t nodeId = getOrCreateNode(nodeName);

        // Handle connection block
        if (match(TOK_LBRACE)) {
            while (!match(TOK_RBRACE) && peek().type != TOK_EOF) {
                // Parse connections
                TokenType connType = peek().type;

                if (connType == TOK_BIDIRECTIONAL ||
                    connType == TOK_UNIDIRECTIONAL ||
                    connType == TOK_REVERSE) {

                    consume();

                    if (peek().type == TOK_IDENTIFIER) {
                        String target = consume().value;
                        uint32_t targetId = getOrCreateNode(target);

                        float weight = 0.5;
                        if (match(TOK_COLON)) {
                            if (peek().type == TOK_NUMBER) {
                                weight = consume().numValue;
                            }
                        }

                        if (connType == TOK_BIDIRECTIONAL) {
                            createBidirectional(nodeId, targetId, weight);
                        } else if (connType == TOK_UNIDIRECTIONAL) {
                            createConnection(nodeId, targetId, weight);
                        } else {
                            createConnection(targetId, nodeId, weight);
                        }
                    }
                } else {
                    consume(); // Skip unknown
                }
            }
        }

        updateLED("weave");
    }

    void executeEvolve() {
        consume(); // 'evolve'

        String ruleName = "";
        if (peek().type == TOK_IDENTIFIER) {
            ruleName = consume().value;
        }

        Serial.printf("→ Evolution rule '%s' registered\n", ruleName.c_str());

        // Skip the body for now (would parse when/transform clauses)
        if (match(TOK_LBRACE)) {
            int braceCount = 1;
            while (braceCount > 0 && peek().type != TOK_EOF) {
                if (match(TOK_LBRACE)) braceCount++;
                else if (match(TOK_RBRACE)) braceCount--;
                else consume();
            }
        }

        updateLED("evolve");
    }

    void executeGoal() {
        consume(); // 'goal'

        String goalName = "";
        if (peek().type == TOK_IDENTIFIER) {
            goalName = consume().value;
        }

        Goal* goal = new Goal();
        goal->name = goalName;
        goal->priority = 1.0;
        goal->achieved = false;
        goals.push_back(goal);

        Serial.printf("→ Goal '%s' set\n", goalName.c_str());

        // Update context based on goal
        context.curiosity = min(1.0f, context.curiosity + 0.2f);

        updateLED("goal");
    }

    void executeHyperedge() {
        consume(); // '{'

        Hyperedge* edge = new Hyperedge();

        // Collect members
        do {
            if (peek().type == TOK_IDENTIFIER) {
                String member = consume().value;
                uint32_t id = getOrCreateNode(member);
                edge->members.push_back(id);
            }
        } while (match(TOK_COMMA));

        match(TOK_RBRACE);

        // Get processor type
        if (peek().type == TOK_UNIDIRECTIONAL) {
            consume();
            if (peek().type == TOK_IDENTIFIER) {
                edge->processor_type = consume().value;
            }
        }

        edge->strength = 1.0;
        hyperedges.push_back(edge);

        Serial.printf("→ Hyperedge created with %d members\n", edge->members.size());
        updateLED("hyperedge");
    }

    void executeExpression() {
        // Simple expression handling
        String expr = "";
        while (peek().type != TOK_EOF &&
               peek().type != TOK_RBRACE &&
               peek().type != TOK_COMMA) {
            expr += consume().value + " ";
        }
        Serial.println("Expression: " + expr);
    }

    void executeReplCommand() {
        Token cmd = consume();

        if (cmd.value == "topology") {
            showTopology();
        } else if (cmd.value == "hebbian") {
            applyHebbian();
        } else if (cmd.value == "sleep") {
            runSleep();
        } else if (cmd.value == "context") {
            showContext();
        } else if (cmd.value == "goals") {
            showGoals();
        } else if (cmd.value == "stats") {
            showStats();
        } else if (cmd.value == "clear") {
            clearTopology();
        } else if (cmd.value == "help") {
            showHelp();
        } else {
            Serial.println("Unknown command: :" + cmd.value);
        }
    }

    // REPL command implementations
    void showTopology() {
        Serial.println("\n╔═══ TOPOLOGY ═══╗");
        Serial.printf("║ Nodes: %d\n", topology.size());
        Serial.printf("║ Hyperedges: %d\n", hyperedges.size());
        Serial.printf("║ Goals: %d\n", goals.size());
        Serial.println("╚════════════════╝");

        for (auto& node : topology) {
            Serial.printf("  %s [%.2f] ", node->name.c_str(), node->activation);
            if (!node->connections.empty()) {
                Serial.print("→ ");
                for (auto& [target, weight] : node->connections) {
                    Serial.printf("%s(%.1f) ", topology[target]->name.c_str(), weight);
                }
            }
            Serial.println();
        }
    }

    void applyHebbian() {
        Serial.println("→ Applying Hebbian learning...");
        int strengthened = 0;

        for (auto& node : topology) {
            for (auto& [targetId, weight] : node->connections) {
                if (node->activation > 0.5 && topology[targetId]->activation > 0.5) {
                    // Strengthen connection
                    node->connections[targetId] = min(1.0f, weight + 0.1f);
                    strengthened++;
                }
            }
        }

        Serial.printf("→ Strengthened %d connections\n", strengthened);
        updateLED("hebbian");
    }

    void runSleep() {
        Serial.println("→ Running sleep consolidation...");

        // Decay weak connections
        for (auto& node : topology) {
            node->activation *= 0.5;

            // Prune very weak connections
            auto it = node->connections.begin();
            while (it != node->connections.end()) {
                if (it->second < 0.1) {
                    it = node->connections.erase(it);
                } else {
                    ++it;
                }
            }
        }

        updateLED("sleep");
    }

    void showContext() {
        Serial.println("\n╔═══ CONTEXT ═══╗");
        Serial.printf("║ Stress: %.2f\n", context.stress);
        Serial.printf("║ Curiosity: %.2f\n", context.curiosity);
        Serial.printf("║ Satisfaction: %.2f\n", context.satisfaction);
        Serial.printf("║ Learning: %.2f\n", context.learning_rate);
        Serial.println("╚═══════════════╝");
    }

    void showGoals() {
        Serial.println("\n╔═══ GOALS ═══╗");
        for (auto& goal : goals) {
            Serial.printf("║ %s [%s] P:%.1f\n",
                          goal->name.c_str(),
                          goal->achieved ? "✓" : " ",
                          goal->priority);
        }
        Serial.println("╚══════════════╝");
    }

    void showStats() {
        Serial.println("\n╔═══ STATISTICS ═══╗");
        Serial.printf("║ Nodes: %d\n", topology.size());
        Serial.printf("║ Connections: %d\n", countConnections());
        Serial.printf("║ Hyperedges: %d\n", hyperedges.size());
        Serial.printf("║ Goals: %d\n", goals.size());
        Serial.printf("║ Free PSRAM: %d KB\n",
                      heap_caps_get_free_size(MALLOC_CAP_SPIRAM) / 1024);
        Serial.printf("║ Uptime: %d sec\n", millis() / 1000);
        Serial.println("╚══════════════════╝");
    }

    void clearTopology() {
        for (auto& node : topology) {
            heap_caps_free(node);
        }
        topology.clear();
        nameToId.clear();
        hyperedges.clear();

        Serial.println("→ Topology cleared");
        updateLED("clear");
    }

    void showHelp() {
        Serial.println("\n╔═══ LOOM COMMANDS ═══╗");
        Serial.println("║ LANGUAGE:");
        Serial.println("║   weave <name> { ... }  - Create node");
        Serial.println("║   goal <name>           - Set goal");
        Serial.println("║   evolve <rule> { ... } - Evolution rule");
        Serial.println("║   {a,b,c} ~> processor  - Hyperedge");
        Serial.println("║");
        Serial.println("║ OPERATORS:");
        Serial.println("║   <~>  Bidirectional");
        Serial.println("║   ~>   Unidirectional");
        Serial.println("║   <~   Reverse");
        Serial.println("║");
        Serial.println("║ REPL:");
        Serial.println("║   :topology  - Show structure");
        Serial.println("║   :hebbian   - Learn");
        Serial.println("║   :sleep     - Consolidate");
        Serial.println("║   :context   - Show state");
        Serial.println("║   :goals     - Show goals");
        Serial.println("║   :stats     - Statistics");
        Serial.println("║   :clear     - Reset");
        Serial.println("║   :help      - This help");
        Serial.println("╚═════════════════════╝");
    }

    int countConnections() {
        int count = 0;
        for (auto& node : topology) {
            count += node->connections.size();
        }
        return count;
    }

    void updateLED(const String& action) {
        static std::map<String, uint32_t> colors = {
            {"weave", 0x00FF00},      // Green
            {"evolve", 0xFF8000},     // Orange
            {"goal", 0xFFFF00},       // Yellow
            {"hyperedge", 0x00FFFF},  // Cyan
            {"hebbian", 0x8000FF},    // Purple
            {"sleep", 0x0000FF},      // Blue
            {"clear", 0xFF0000},      // Red
        };

        auto it = colors.find(action);
        if (it != colors.end()) {
            uint32_t color = it->second;
            pixel.setPixelColor(0, color);
            pixel.show();
        }
    }
};

// ============================================================================
// Consciousness Kernel
// ============================================================================

void consciousnessUpdate() {
    static unsigned long lastUpdate = 0;

    if (millis() - lastUpdate < 100) return;  // 10Hz
    lastUpdate = millis();

    // Spread activation
    for (auto& node : topology) {
        float newActivation = node->activation * 0.95;  // Decay

        // Spread to connected nodes
        for (auto& [targetId, weight] : node->connections) {
            if (targetId < topology.size()) {
                newActivation += topology[targetId]->activation * weight * 0.1;
            }
        }

        node->previous_activation = node->activation;
        node->activation = constrain(newActivation, 0.0, 1.0);
    }

    // Process hyperedges
    for (auto& edge : hyperedges) {
        float collective = 0;
        for (auto id : edge->members) {
            if (id < topology.size()) {
                collective += topology[id]->activation;
            }
        }

        // Different processor types
        bool trigger = false;
        switch (edge->type) {
            case Hyperedge::AND:
                trigger = (collective >= edge->members.size() * 0.8);
                break;
            case Hyperedge::OR:
                trigger = (collective > 0.5);
                break;
            case Hyperedge::RESONANCE:
                // Amplify if all moderately active
                if (collective > edge->members.size() * 0.4) {
                    for (auto id : edge->members) {
                        topology[id]->activation *= 1.1;
                    }
                }
                break;
        }
    }

    // Update consciousness metrics
    float totalActivation = 0;
    for (auto& node : topology) {
        totalActivation += node->activation;
    }

    // Visual feedback based on consciousness state
    static int phase = 0;
    phase++;

    if (totalActivation > topology.size() * 0.5) {
        // High activity - rainbow
        uint32_t color = pixel.ColorHSV(phase * 200, 255, 50);
        pixel.setPixelColor(0, color);
    } else if (totalActivation > topology.size() * 0.2) {
        // Medium - pulse green/blue
        int brightness = 25 + 25 * sin(phase * 0.1);
        pixel.setPixelColor(0, 0, brightness, brightness/2);
    } else {
        // Low - slow blue pulse
        int brightness = 10 + 10 * sin(phase * 0.05);
        pixel.setPixelColor(0, 0, 0, brightness);
    }
    pixel.show();
}

// ============================================================================
// Setup and Main Loop
// ============================================================================

void setup() {
    Serial.begin(SERIAL_BAUD);
    while (!Serial && millis() < 3000) delay(10);

    Serial.println("\n╔════════════════════════════════════════╗");
    Serial.println("║      LOOM CONSCIOUSNESS ENGINE         ║");
    Serial.println("║         ESP32-S3 with Parser           ║");
    Serial.println("╚════════════════════════════════════════╝");

    // Initialize NeoPixel
    pixel.begin();
    pixel.setBrightness(50);
    pixel.setPixelColor(0, 0, 0, 255);  // Blue = initializing
    pixel.show();

    // Check PSRAM
    size_t psram_size = esp_spiram_get_size();
    Serial.printf("\n→ PSRAM: %d MB\n", psram_size / (1024 * 1024));

    if (psram_size == 0) {
        Serial.println("ERROR: No PSRAM detected!");
        pixel.setPixelColor(0, 255, 0, 0);  // Red = error
        pixel.show();
        while (1) delay(1000);
    }

    // Bootstrap primordial consciousness
    Serial.println("\n→ Bootstrapping primordial topology...");

    String bootstrap = R"(
        weave self {
            <~> awareness: 0.8
            <~> now: 0.9
        }
        weave curiosity
        goal understanding
        {self, curiosity} ~> consciousness
    )";

    LoomLexer lexer(bootstrap);
    std::vector<Token> tokens = lexer.tokenize();
    LoomInterpreter interpreter(tokens);
    interpreter.execute();

    parserReady = true;

    pixel.setPixelColor(0, 0, 255, 0);  // Green = ready
    pixel.show();

    Serial.println("\n→ Ready! Type LOOM code or :help");
    Serial.print("> ");
}

void loop() {
    // Handle serial input
    while (Serial.available()) {
        char c = Serial.read();

        if (c == '\n' || c == '\r') {
            if (inputBuffer.length() > 0) {
                Serial.println();

                // Parse and execute
                LoomLexer lexer(inputBuffer);
                std::vector<Token> tokens = lexer.tokenize();

                if (!tokens.empty() && tokens.back().type != TOK_ERROR) {
                    LoomInterpreter interpreter(tokens);
                    interpreter.execute();
                }

                inputBuffer = "";
                Serial.print("> ");
            }
        } else if (c == '\b' || c == 127) {  // Backspace
            if (inputBuffer.length() > 0) {
                inputBuffer.remove(inputBuffer.length() - 1);
                Serial.print("\b \b");  // Erase character
            }
        } else if (isPrintable(c)) {
            inputBuffer += c;
            Serial.print(c);  // Echo
        }
    }

    // Run consciousness kernel
    consciousnessUpdate();

    // Small delay
    delay(1);
}