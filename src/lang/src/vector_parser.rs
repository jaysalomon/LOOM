// Parser for LOOM that understands vector topology

use crate::lexer::{Token, Lexer};
use crate::vector_topology::{VectorTopology, NodeVector};
use std::collections::HashMap;

pub struct VectorParser {
    topology: VectorTopology,
    context: [f32; 32], // Emotional context
}

impl VectorParser {
    pub fn new() -> Self {
        Self {
            topology: VectorTopology::new(),
            context: [0.0; 32],
        }
    }

    pub fn parse_and_execute(&mut self, input: &str) -> Result<String, String> {
        let tokens = Lexer::tokens(input);
        let mut pos = 0;
        let mut output = String::new();

        while pos < tokens.len() {
            match &tokens[pos] {
                Token::Weave => {
                    pos += 1;
                    self.parse_weave(&tokens, &mut pos)?;
                    output.push_str("Node woven into topology\n");
                }
                Token::Identifier(from) => {
                    let from = from.clone();
                    pos += 1;
                    if pos < tokens.len() {
                        match &tokens[pos] {
                            Token::Bidirectional => {
                                pos += 1;
                                let to = self.get_identifier(&tokens, &mut pos)?;
                                let strength = self.get_strength(&tokens, &mut pos)?;
                                self.topology.connect(&from, &to, strength, true);
                                output.push_str(&format!("Connected {} <~> {}\n", from, to));
                            }
                            Token::Unidirectional => {
                                pos += 1;
                                let to = self.get_identifier(&tokens, &mut pos)?;
                                let strength = self.get_strength(&tokens, &mut pos)?;
                                self.topology.connect(&from, &to, strength, false);
                                output.push_str(&format!("Connected {} ~> {}\n", from, to));
                            }
                            Token::EmotionalOp(op) => {
                                let op = op.clone();
                                pos += 1;
                                let to = self.get_identifier(&tokens, &mut pos)?;
                                self.apply_emotional_operation(&from, &op, &to);
                                output.push_str(&format!("{} ¥{}¥ {}\n", from, op, to));
                            }
                            Token::LogicalOp(op) => {
                                let op = op.clone();
                                pos += 1;
                                let to = self.get_identifier(&tokens, &mut pos)?;
                                self.apply_logical_operation(&from, &op, &to);
                                output.push_str(&format!("{} €{}€ {}\n", from, op, to));
                            }
                            _ => pos += 1,
                        }
                    }
                }
                Token::Colon => {
                    pos += 1;
                    if pos < tokens.len() {
                        if let Token::Identifier(cmd) = &tokens[pos] {
                            match cmd.as_str() {
                                "topology" => {
                                    self.topology.describe();
                                    output.push_str("Topology displayed\n");
                                }
                                "evolve" => {
                                    self.topology.evolve();
                                    output.push_str("Topology evolved\n");
                                }
                                "context" => {
                                    pos += 1;
                                    self.parse_context(&tokens, &mut pos)?;
                                    output.push_str("Context updated\n");
                                }
                                _ => {}
                            }
                            pos += 1;
                        }
                    }
                }
                _ => pos += 1,
            }
        }

        Ok(output)
    }

    fn parse_weave(&mut self, tokens: &[Token], pos: &mut usize) -> Result<(), String> {
        let name = self.get_identifier(tokens, pos)?;
        let _node = self.topology.weave(name.clone());

        // Check for opening brace
        if *pos < tokens.len() && tokens[*pos] == Token::LeftBrace {
            *pos += 1;

            // Parse connections inside the brace
            while *pos < tokens.len() && tokens[*pos] != Token::RightBrace {
                match &tokens[*pos] {
                    Token::Bidirectional => {
                        *pos += 1;
                        let target = self.get_identifier(tokens, pos)?;
                        let strength = self.get_strength(tokens, pos)?;
                        self.topology.connect(&name, &target, strength, true);
                    }
                    Token::Unidirectional => {
                        *pos += 1;
                        let target = self.get_identifier(tokens, pos)?;
                        let strength = self.get_strength(tokens, pos)?;
                        self.topology.connect(&name, &target, strength, false);
                    }
                    _ => *pos += 1,
                }
            }

            // Skip closing brace
            if *pos < tokens.len() && tokens[*pos] == Token::RightBrace {
                *pos += 1;
            }
        }

        Ok(())
    }

    fn parse_context(&mut self, tokens: &[Token], pos: &mut usize) -> Result<(), String> {
        // Parse emotional context values
        while *pos < tokens.len() {
            if let Token::Identifier(dimension) = &tokens[*pos] {
                *pos += 1;
                if *pos < tokens.len() && tokens[*pos] == Token::Colon {
                    *pos += 1;
                    if let Token::Number(value) = &tokens[*pos] {
                        // Map dimension name to context index (simplified)
                        let index = match dimension.as_str() {
                            "stress" => 0,
                            "curiosity" => 1,
                            "joy" => 2,
                            "fear" => 3,
                            _ => 0,
                        };
                        self.context[index] = *value;
                        *pos += 1;
                    }
                }
            } else {
                break;
            }
        }

        // Apply context to topology
        self.topology.apply_emotional_context(&self.context);
        Ok(())
    }

    fn apply_emotional_operation(&mut self, from: &str, op: &str, to: &str) {
        // Emotional operations modify the emotional fields
        match op {
            "resonates" => {
                // Create strong bidirectional emotional connection
                self.topology.connect(from, to, 0.9, true);
            }
            "harmonizes" => {
                // Synchronize emotional fields
                self.topology.connect(from, to, 0.7, true);
            }
            "spreads" => {
                // Unidirectional emotional influence
                self.topology.connect(from, to, 0.6, false);
            }
            _ => {}
        }
    }

    fn apply_logical_operation(&mut self, from: &str, op: &str, to: &str) {
        // Logical operations affect semantic space
        match op {
            "implies" => {
                self.topology.connect(from, to, 0.8, false);
            }
            "contradicts" => {
                // Create inhibitory connection (negative in some dimensions)
                self.topology.connect(from, to, -0.5, true);
            }
            "supports" => {
                self.topology.connect(from, to, 0.6, false);
            }
            _ => {}
        }
    }

    fn get_identifier(&self, tokens: &[Token], pos: &mut usize) -> Result<String, String> {
        if *pos < tokens.len() {
            if let Token::Identifier(name) = &tokens[*pos] {
                *pos += 1;
                return Ok(name.clone());
            }
        }
        Err("Expected identifier".to_string())
    }

    fn get_strength(&self, tokens: &[Token], pos: &mut usize) -> Result<f32, String> {
        // Check for : weight syntax
        if *pos < tokens.len() && tokens[*pos] == Token::Colon {
            *pos += 1;
            if *pos < tokens.len() {
                if let Token::Number(n) = &tokens[*pos] {
                    *pos += 1;
                    return Ok(*n);
                }
            }
        }
        Ok(0.5) // Default strength
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_parse_weave() {
        let mut parser = VectorParser::new();
        let input = "weave grandmother { <~> warmth: 0.8 }";
        let result = parser.parse_and_execute(input);
        assert!(result.is_ok());
    }

    #[test]
    fn test_emotional_operator() {
        let mut parser = VectorParser::new();
        let input = "joy ¥resonates¥ happiness";
        let result = parser.parse_and_execute(input);
        assert!(result.is_ok());
    }
}