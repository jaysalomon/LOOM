/*
 * ESP32-S3 LOOM Parser
 * Full LOOM language parser implementation for ESP32-S3
 *
 * Supports:
 * - weave: Create nodes and connections
 * - evolve: Define evolution rules
 * - pattern: Pattern matching and transformations
 * - hyperedges: N-ary relationships with processors
 * - Multi-lens operators: ¥emotional¥, €logical€, µtemporal¶
 * - REPL commands: :topology, :hebbian, :sleep, :context
 */

#ifndef ESP32S3_LOOM_PARSER_H
#define ESP32S3_LOOM_PARSER_H

#include <Arduino.h>
#include <vector>
#include <map>
#include <string>
#include <functional>

// ============================================================================
// Token Types
// ============================================================================

enum class TokenType {
    // Literals
    IDENTIFIER,
    NUMBER,
    STRING,

    // Keywords
    WEAVE,
    EVOLVE,
    PATTERN,
    TOPOLOGY,
    WHEN,
    TRANSFORM,
    STRENGTHEN,
    SPAWN,
    DURING,

    // Operators
    BIDIRECTIONAL,    // <~>
    UNIDIRECTIONAL,   // ~>
    REVERSE,          // <~
    COLON,           // :

    // Multi-lens
    EMOTIONAL,       // ¥...¥
    LOGICAL,         // €...€
    TEMPORAL,        // µ...µ

    // Structural
    LBRACE,          // {
    RBRACE,          // }
    LPAREN,          // (
    RPAREN,          // )
    LBRACKET,        // [
    RBRACKET,        // ]
    COMMA,           // ,

    // REPL Commands
    REPL_COMMAND,    // :command

    // Special
    NEWLINE,
    EOF_TOKEN,
    ERROR
};

// ============================================================================
// Token Structure
// ============================================================================

struct Token {
    TokenType type;
    String value;
    float numValue;
    int line;
    int column;

    Token(TokenType t = TokenType::ERROR, String v = "", int l = 0, int c = 0)
        : type(t), value(v), numValue(0), line(l), column(c) {
        if (t == TokenType::NUMBER) {
            numValue = v.toFloat();
        }
    }
};

// ============================================================================
// AST Node Types
// ============================================================================

enum class ASTNodeType {
    PROGRAM,
    WEAVE_STMT,
    EVOLVE_STMT,
    PATTERN_STMT,
    CONNECTION,
    HYPEREDGE,
    WHEN_CLAUSE,
    TRANSFORM_CLAUSE,
    REPL_CMD,
    IDENTIFIER,
    NUMBER,
    EXPRESSION
};

struct ASTNode {
    ASTNodeType type;
    String value;
    float numValue;
    std::vector<std::shared_ptr<ASTNode>> children;
    std::map<String, String> attributes;

    ASTNode(ASTNodeType t) : type(t), numValue(0) {}
};

using ASTNodePtr = std::shared_ptr<ASTNode>;

// ============================================================================
// Lexer Class
// ============================================================================

class LoomLexer {
private:
    String input;
    size_t position;
    int line;
    int column;

    char current() {
        if (position >= input.length()) return '\0';
        return input[position];
    }

    char peek(int offset = 1) {
        size_t pos = position + offset;
        if (pos >= input.length()) return '\0';
        return input[pos];
    }

    void advance() {
        if (position < input.length()) {
            if (current() == '\n') {
                line++;
                column = 1;
            } else {
                column++;
            }
            position++;
        }
    }

    void skipWhitespace() {
        while (isspace(current()) && current() != '\n') {
            advance();
        }
    }

    void skipComment() {
        if (current() == '/' && peek() == '/') {
            while (current() != '\n' && current() != '\0') {
                advance();
            }
        }
    }

    Token readIdentifier() {
        String value;
        int startCol = column;

        while (isalnum(current()) || current() == '_') {
            value += current();
            advance();
        }

        // Check for keywords
        TokenType type = TokenType::IDENTIFIER;
        if (value == "weave") type = TokenType::WEAVE;
        else if (value == "evolve") type = TokenType::EVOLVE;
        else if (value == "pattern") type = TokenType::PATTERN;
        else if (value == "topology") type = TokenType::TOPOLOGY;
        else if (value == "when") type = TokenType::WHEN;
        else if (value == "transform") type = TokenType::TRANSFORM;
        else if (value == "strengthen") type = TokenType::STRENGTHEN;
        else if (value == "spawn") type = TokenType::SPAWN;
        else if (value == "during") type = TokenType::DURING;

        return Token(type, value, line, startCol);
    }

    Token readNumber() {
        String value;
        int startCol = column;

        while (isdigit(current()) || current() == '.') {
            value += current();
            advance();
        }

        return Token(TokenType::NUMBER, value, line, startCol);
    }

    Token readString() {
        String value;
        int startCol = column;
        char quote = current();
        advance(); // skip opening quote

        while (current() != quote && current() != '\0') {
            if (current() == '\\') {
                advance();
                if (current() != '\0') {
                    value += current();
                    advance();
                }
            } else {
                value += current();
                advance();
            }
        }

        if (current() == quote) {
            advance(); // skip closing quote
        }

        return Token(TokenType::STRING, value, line, startCol);
    }

    Token readReplCommand() {
        String value;
        int startCol = column;
        advance(); // skip ':'

        while (isalpha(current()) || current() == '_') {
            value += current();
            advance();
        }

        return Token(TokenType::REPL_COMMAND, value, line, startCol);
    }

public:
    LoomLexer(const String& src) : input(src), position(0), line(1), column(1) {}

    Token nextToken() {
        skipWhitespace();
        skipComment();

        if (current() == '\0') {
            return Token(TokenType::EOF_TOKEN, "", line, column);
        }

        int startCol = column;

        // Multi-character operators
        if (current() == '<' && peek() == '~' && peek(2) == '>') {
            advance(); advance(); advance();
            return Token(TokenType::BIDIRECTIONAL, "<~>", line, startCol);
        }

        if (current() == '~' && peek() == '>') {
            advance(); advance();
            return Token(TokenType::UNIDIRECTIONAL, "~>", line, startCol);
        }

        if (current() == '<' && peek() == '~') {
            advance(); advance();
            return Token(TokenType::REVERSE, "<~", line, startCol);
        }

        // Multi-lens operators
        if (current() == '¥') {
            advance();
            String value;
            while (current() != '¥' && current() != '\0') {
                value += current();
                advance();
            }
            if (current() == '¥') advance();
            return Token(TokenType::EMOTIONAL, value, line, startCol);
        }

        if (current() == '€') {
            advance();
            String value;
            while (current() != '€' && current() != '\0') {
                value += current();
                advance();
            }
            if (current() == '€') advance();
            return Token(TokenType::LOGICAL, value, line, startCol);
        }

        if (current() == 'µ') {
            advance();
            String value;
            while (current() != 'µ' && current() != '\0') {
                value += current();
                advance();
            }
            if (current() == 'µ') advance();
            return Token(TokenType::TEMPORAL, value, line, startCol);
        }

        // Single character tokens
        switch (current()) {
            case '{':
                advance();
                return Token(TokenType::LBRACE, "{", line, startCol);
            case '}':
                advance();
                return Token(TokenType::RBRACE, "}", line, startCol);
            case '(':
                advance();
                return Token(TokenType::LPAREN, "(", line, startCol);
            case ')':
                advance();
                return Token(TokenType::RPAREN, ")", line, startCol);
            case '[':
                advance();
                return Token(TokenType::LBRACKET, "[", line, startCol);
            case ']':
                advance();
                return Token(TokenType::RBRACKET, "]", line, startCol);
            case ',':
                advance();
                return Token(TokenType::COMMA, ",", line, startCol);
            case ':':
                if (isalpha(peek())) {
                    return readReplCommand();
                }
                advance();
                return Token(TokenType::COLON, ":", line, startCol);
            case '\n':
                advance();
                return Token(TokenType::NEWLINE, "\\n", line, startCol);
            case '"':
            case '\'':
                return readString();
        }

        // Numbers
        if (isdigit(current())) {
            return readNumber();
        }

        // Identifiers and keywords
        if (isalpha(current()) || current() == '_') {
            return readIdentifier();
        }

        // Unknown character
        char unknown = current();
        advance();
        return Token(TokenType::ERROR, String(unknown), line, startCol);
    }

    std::vector<Token> tokenize() {
        std::vector<Token> tokens;
        Token tok;

        do {
            tok = nextToken();
            if (tok.type != TokenType::NEWLINE) { // Skip newlines for now
                tokens.push_back(tok);
            }
        } while (tok.type != TokenType::EOF_TOKEN && tok.type != TokenType::ERROR);

        return tokens;
    }
};

// ============================================================================
// Parser Class
// ============================================================================

class LoomParser {
private:
    std::vector<Token> tokens;
    size_t current;

    Token& peek(int offset = 0) {
        static Token eof(TokenType::EOF_TOKEN);
        size_t pos = current + offset;
        if (pos >= tokens.size()) return eof;
        return tokens[pos];
    }

    Token& consume() {
        if (current < tokens.size()) {
            return tokens[current++];
        }
        static Token eof(TokenType::EOF_TOKEN);
        return eof;
    }

    bool match(TokenType type) {
        if (peek().type == type) {
            consume();
            return true;
        }
        return false;
    }

    bool expect(TokenType type, const String& message) {
        if (!match(type)) {
            Serial.println("Parse error: " + message);
            return false;
        }
        return true;
    }

public:
    LoomParser(const std::vector<Token>& toks) : tokens(toks), current(0) {}

    ASTNodePtr parseProgram() {
        auto program = std::make_shared<ASTNode>(ASTNodeType::PROGRAM);

        while (peek().type != TokenType::EOF_TOKEN) {
            auto stmt = parseStatement();
            if (stmt) {
                program->children.push_back(stmt);
            } else {
                // Skip to next statement on error
                while (peek().type != TokenType::EOF_TOKEN &&
                       peek().type != TokenType::WEAVE &&
                       peek().type != TokenType::EVOLVE &&
                       peek().type != TokenType::PATTERN &&
                       peek().type != TokenType::REPL_COMMAND) {
                    consume();
                }
            }
        }

        return program;
    }

    ASTNodePtr parseStatement() {
        switch (peek().type) {
            case TokenType::WEAVE:
                return parseWeave();
            case TokenType::EVOLVE:
                return parseEvolve();
            case TokenType::PATTERN:
                return parsePattern();
            case TokenType::REPL_COMMAND:
                return parseReplCommand();
            case TokenType::IDENTIFIER:
                return parseExpression();
            case TokenType::LBRACE:
                return parseHyperedge();
            default:
                Serial.println("Unexpected token: " + peek().value);
                consume();
                return nullptr;
        }
    }

    ASTNodePtr parseWeave() {
        auto node = std::make_shared<ASTNode>(ASTNodeType::WEAVE_STMT);
        consume(); // 'weave'

        if (peek().type == TokenType::IDENTIFIER) {
            node->value = consume().value;
        }

        if (match(TokenType::LBRACE)) {
            while (peek().type != TokenType::RBRACE &&
                   peek().type != TokenType::EOF_TOKEN) {
                auto conn = parseConnection();
                if (conn) {
                    node->children.push_back(conn);
                }
            }
            expect(TokenType::RBRACE, "Expected '}'");
        }

        return node;
    }

    ASTNodePtr parseEvolve() {
        auto node = std::make_shared<ASTNode>(ASTNodeType::EVOLVE_STMT);
        consume(); // 'evolve'

        if (peek().type == TokenType::IDENTIFIER) {
            node->value = consume().value;
        }

        if (match(TokenType::LBRACE)) {
            while (peek().type != TokenType::RBRACE &&
                   peek().type != TokenType::EOF_TOKEN) {
                if (peek().type == TokenType::WHEN) {
                    node->children.push_back(parseWhenClause());
                } else if (peek().type == TokenType::TRANSFORM) {
                    node->children.push_back(parseTransformClause());
                } else {
                    consume(); // Skip unknown
                }
            }
            expect(TokenType::RBRACE, "Expected '}'");
        }

        return node;
    }

    ASTNodePtr parsePattern() {
        auto node = std::make_shared<ASTNode>(ASTNodeType::PATTERN_STMT);
        consume(); // 'pattern'

        if (peek().type == TokenType::IDENTIFIER) {
            node->value = consume().value;
        }

        if (match(TokenType::LBRACE)) {
            while (peek().type != TokenType::RBRACE &&
                   peek().type != TokenType::EOF_TOKEN) {
                auto stmt = parseStatement();
                if (stmt) {
                    node->children.push_back(stmt);
                }
            }
            expect(TokenType::RBRACE, "Expected '}'");
        }

        return node;
    }

    ASTNodePtr parseConnection() {
        auto node = std::make_shared<ASTNode>(ASTNodeType::CONNECTION);

        // Parse connection type and target
        TokenType connType = peek().type;
        if (connType == TokenType::BIDIRECTIONAL ||
            connType == TokenType::UNIDIRECTIONAL ||
            connType == TokenType::REVERSE) {

            node->attributes["type"] = consume().value;

            // Target
            if (peek().type == TokenType::IDENTIFIER) {
                node->value = consume().value;
            }

            // Optional weight
            if (match(TokenType::COLON)) {
                if (peek().type == TokenType::NUMBER) {
                    node->numValue = consume().numValue;
                }
            }
        }

        return node;
    }

    ASTNodePtr parseHyperedge() {
        auto node = std::make_shared<ASTNode>(ASTNodeType::HYPEREDGE);
        expect(TokenType::LBRACE, "Expected '{'");

        // Parse member nodes
        do {
            if (peek().type == TokenType::IDENTIFIER) {
                auto member = std::make_shared<ASTNode>(ASTNodeType::IDENTIFIER);
                member->value = consume().value;
                node->children.push_back(member);
            }
        } while (match(TokenType::COMMA));

        expect(TokenType::RBRACE, "Expected '}'");

        // Parse relationship
        if (peek().type == TokenType::UNIDIRECTIONAL) {
            node->attributes["type"] = consume().value;

            if (peek().type == TokenType::IDENTIFIER) {
                node->value = consume().value;
            }
        }

        return node;
    }

    ASTNodePtr parseWhenClause() {
        auto node = std::make_shared<ASTNode>(ASTNodeType::WHEN_CLAUSE);
        consume(); // 'when'

        match(TokenType::COLON);

        // Parse condition expression
        auto expr = parseExpression();
        if (expr) {
            node->children.push_back(expr);
        }

        return node;
    }

    ASTNodePtr parseTransformClause() {
        auto node = std::make_shared<ASTNode>(ASTNodeType::TRANSFORM_CLAUSE);
        consume(); // 'transform'

        match(TokenType::COLON);

        // Parse transformation expression
        auto expr = parseExpression();
        if (expr) {
            node->children.push_back(expr);
        }

        return node;
    }

    ASTNodePtr parseExpression() {
        auto node = std::make_shared<ASTNode>(ASTNodeType::EXPRESSION);

        // Simple expression parsing for now
        while (peek().type != TokenType::EOF_TOKEN &&
               peek().type != TokenType::COMMA &&
               peek().type != TokenType::RBRACE &&
               peek().type != TokenType::NEWLINE) {

            Token tok = consume();
            node->value += tok.value + " ";
        }

        return node;
    }

    ASTNodePtr parseReplCommand() {
        auto node = std::make_shared<ASTNode>(ASTNodeType::REPL_CMD);
        Token cmd = consume();
        node->value = cmd.value;

        // Parse command arguments
        while (peek().type != TokenType::EOF_TOKEN &&
               peek().type != TokenType::NEWLINE) {
            Token arg = consume();
            node->attributes["args"] += arg.value + " ";
        }

        return node;
    }
};

#endif // ESP32S3_LOOM_PARSER_H