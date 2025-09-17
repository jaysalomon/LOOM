/*
 * loom_to_kernel.h
 *
 * Connects LOOM parser output to kernel relational processors
 * Converts parsed LOOM syntax into executable topology
 */

#ifndef LOOM_TO_KERNEL_H
#define LOOM_TO_KERNEL_H

#include <Arduino.h>
#include <map>
#include <vector>

// Forward declarations
class LoomKernel;
class RelationalProcessor;

// ============================================================================
// LOOM Parser to Kernel Bridge
// ============================================================================

class LoomToKernel {
private:
    LoomKernel* kernel;

    // Track current context during parsing
    String currentPattern;
    RelationalProcessor* currentProcessor;

public:
    LoomToKernel(LoomKernel* k) : kernel(k), currentProcessor(nullptr) {}

    // Process a parsed LOOM weave statement
    void processWeave(String name, char symbol, int r, int g, int b, float* vector) {
        Serial.printf("[LOOM->KERNEL] Creating %s (%c) RGB(%d,%d,%d)\n",
                     name.c_str(), symbol, r, g, b);

        kernel->createNumber(name, symbol, r, g, b);

        // Set vector if provided
        if (vector && kernel->processors.count(name)) {
            memcpy(kernel->processors[name]->vector, vector, VECTOR_DIM * sizeof(float));
        }
    }

    // Process a relation definition
    void processRelation(String target, String pattern, float strength) {
        Serial.printf("[LOOM->KERNEL] Adding relation: %s from %s (%.2f)\n",
                     target.c_str(), pattern.c_str(), strength);

        if (kernel->processors.count(target)) {
            kernel->processors[target]->addRelation(pattern, strength);
        }
    }

    // Process an evolution rule
    void processEvolve(String name, String condition, String action) {
        Serial.printf("[LOOM->KERNEL] Evolution: %s when %s then %s\n",
                     name.c_str(), condition.c_str(), action.c_str());

        // Store evolution rules for continuous processing
        // These drive the autonomous discovery
    }

    // Process a pattern match query
    void processQuery(String pattern) {
        Serial.printf("[LOOM->KERNEL] Query: %s\n", pattern.c_str());
        kernel->processQuery(pattern);
    }

    // Process topology activation
    void activateTopology(String name) {
        Serial.printf("[LOOM->KERNEL] Activating topology: %s\n", name.c_str());

        if (name == "MathKernel") {
            // Main kernel already running
            kernel->printTopology();
        }
    }

    // Convert LOOM pattern syntax to kernel pattern
    String convertPattern(String loomPattern) {
        // Convert LOOM syntax like "{two, two}" to "two+two"
        loomPattern.replace("{", "");
        loomPattern.replace("}", "");
        loomPattern.replace(",", "+");
        loomPattern.replace(" × ", "*");
        loomPattern.replace(" / ", "/");
        loomPattern.trim();

        return loomPattern;
    }
};

// ============================================================================
// Extended LOOM Interpreter for Kernel Operations
// ============================================================================

class KernelInterpreter {
private:
    LoomToKernel* bridge;
    std::map<String, float> variables;

public:
    KernelInterpreter(LoomToKernel* b) : bridge(b) {}

    // Execute parsed LOOM statements
    void execute(String statement) {
        statement.trim();

        // Parse weave statements
        if (statement.startsWith("weave ")) {
            parseWeave(statement);
        }
        // Parse relation definitions
        else if (statement.indexOf(".relations") > 0) {
            parseRelations(statement);
        }
        // Parse evolution rules
        else if (statement.startsWith("evolve ")) {
            parseEvolve(statement);
        }
        // Parse queries
        else if (statement.startsWith("query ")) {
            String pattern = statement.substring(6);
            bridge->processQuery(pattern);
        }
        // Parse topology activation
        else if (statement.indexOf(".activate()") > 0) {
            int dot = statement.indexOf('.');
            String topology = statement.substring(0, dot);
            bridge->activateTopology(topology);
        }
    }

private:
    void parseWeave(String statement) {
        // Parse: weave two { symbol: '2', vector: [...], color: RGB(0,255,0) }

        int start = statement.indexOf("weave ") + 6;
        int brace = statement.indexOf('{');
        String name = statement.substring(start, brace);
        name.trim();

        // Extract properties
        char symbol = '0';
        int r = 0, g = 0, b = 0;
        float vector[VECTOR_DIM] = {0};

        // Find symbol
        int symbolPos = statement.indexOf("symbol:");
        if (symbolPos > 0) {
            int quote = statement.indexOf('\'', symbolPos);
            symbol = statement[quote + 1];
        }

        // Find color
        int colorPos = statement.indexOf("RGB(");
        if (colorPos > 0) {
            sscanf(statement.c_str() + colorPos, "RGB(%d,%d,%d)", &r, &g, &b);
        }

        // Find vector
        int vectorPos = statement.indexOf("vector:");
        if (vectorPos > 0) {
            int bracket = statement.indexOf('[', vectorPos);
            int endBracket = statement.indexOf(']', bracket);
            String vectorStr = statement.substring(bracket + 1, endBracket);

            // Parse vector values
            int idx = 0;
            int pos = 0;
            while (pos < vectorStr.length() && idx < VECTOR_DIM) {
                int comma = vectorStr.indexOf(',', pos);
                if (comma == -1) comma = vectorStr.length();

                vector[idx++] = vectorStr.substring(pos, comma).toFloat();
                pos = comma + 1;
            }
        }

        bridge->processWeave(name, symbol, r, g, b, vector);
    }

    void parseRelations(String statement) {
        // Parse: four.relations { {two, two} ~> four: 1.0 }

        int dot = statement.indexOf('.');
        String target = statement.substring(0, dot);
        target.trim();

        // Find relation patterns
        int pos = statement.indexOf('{', dot);
        while (pos > 0 && pos < statement.length()) {
            int arrow = statement.indexOf("~>", pos);
            if (arrow == -1) break;

            // Extract pattern
            int patternStart = statement.indexOf('{', pos + 1);
            int patternEnd = statement.indexOf('}', patternStart);
            String pattern = statement.substring(patternStart, patternEnd + 1);

            // Extract strength
            int colon = statement.indexOf(':', arrow);
            int nextComma = statement.indexOf(',', colon);
            if (nextComma == -1) nextComma = statement.indexOf('}', colon);

            float strength = 1.0;
            if (colon > 0) {
                strength = statement.substring(colon + 1, nextComma).toFloat();
            }

            // Convert and add relation
            pattern = bridge->convertPattern(pattern);
            bridge->processRelation(target, pattern, strength);

            pos = statement.indexOf('{', patternEnd + 1);
        }
    }

    void parseEvolve(String statement) {
        // Parse: evolve explore { ... }

        int start = statement.indexOf("evolve ") + 7;
        int brace = statement.indexOf('{');
        String name = statement.substring(start, brace);
        name.trim();

        // For now, just register the evolution
        bridge->processEvolve(name, "", "explore");
    }
};

#endif // LOOM_TO_KERNEL_H