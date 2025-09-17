// SIMPLE WORKING PARSER - Let's actually make this work!

use crate::lexer::{Token, Lexer};
use std::collections::HashMap;

// Simple AST structures that we can actually work with
#[derive(Debug, Clone)]
pub struct Program {
    pub statements: Vec<Statement>,
}

#[derive(Debug, Clone)]
pub enum Statement {
    Weave(WeaveStatement),
    Connect(ConnectStatement),
    Command(CommandStatement),
}

#[derive(Debug, Clone)]
pub struct WeaveStatement {
    pub name: String,
    pub connections: Vec<Connection>,
    pub properties: HashMap<String, f32>,
}

#[derive(Debug, Clone)]
pub struct Connection {
    pub connection_type: ConnectionType,
    pub target: String,
    pub weight: f32,
}

#[derive(Debug, Clone)]
pub enum ConnectionType {
    Bidirectional,   // <~>
    Unidirectional,  // ~>
    Equivalence,     // <=>
}

#[derive(Debug, Clone)]
pub struct ConnectStatement {
    pub from: String,
    pub to: String,
    pub connection_type: ConnectionType,
    pub weight: f32,
}

#[derive(Debug, Clone)]
pub enum CommandStatement {
    ShowTopology,
    ApplyHebbian,
    Sleep,
}

// The actual parser that works!
pub struct SimpleParser {
    tokens: Vec<Token>,
    current: usize,
}

impl SimpleParser {
    pub fn new(input: &str) -> Self {
        let tokens = Lexer::tokens(input);
        println!("Tokens: {:?}", tokens); // Debug output
        Self { tokens, current: 0 }
    }

    pub fn parse(&mut self) -> Result<Program, String> {
        let mut statements = Vec::new();

        while !self.is_at_end() {
            match self.parse_statement() {
                Ok(stmt) => statements.push(stmt),
                Err(e) => {
                    // Skip to next statement on error
                    println!("Parse error: {}", e);
                    self.advance();
                }
            }
        }

        Ok(Program { statements })
    }

    fn parse_statement(&mut self) -> Result<Statement, String> {
        match self.peek() {
            Some(Token::Weave) => self.parse_weave(),
            Some(Token::Colon) if self.peek_ahead(1) == Some(&Token::Identifier("topology".to_string())) => {
                self.parse_command()
            }
            Some(Token::Colon) if self.peek_ahead(1) == Some(&Token::Identifier("hebbian".to_string())) => {
                self.parse_command()
            }
            Some(Token::Colon) if self.peek_ahead(1) == Some(&Token::Identifier("sleep".to_string())) => {
                self.parse_command()
            }
            Some(Token::Identifier(name)) => {
                let name = name.clone();
                self.advance();

                // Check for connection operator
                match self.peek() {
                    Some(Token::Bidirectional) | Some(Token::Unidirectional) | Some(Token::Equivalence) => {
                        self.parse_connection(name)
                    }
                    _ => Err(format!("Unexpected token after identifier: {:?}", self.peek()))
                }
            }
            _ => Err(format!("Unexpected token: {:?}", self.peek()))
        }
    }

    fn parse_weave(&mut self) -> Result<Statement, String> {
        self.consume(Token::Weave)?;

        let name = match self.peek() {
            Some(Token::Identifier(n)) => {
                let name = n.clone();
                self.advance();
                name
            }
            _ => return Err("Expected node name after 'weave'".to_string())
        };

        let mut connections = Vec::new();
        let mut properties = HashMap::new();

        // Check for opening brace
        if self.peek() == Some(&Token::LeftBrace) {
            self.advance();

            while self.peek() != Some(&Token::RightBrace) && !self.is_at_end() {
                // Parse connection or property
                match self.peek() {
                    Some(Token::Bidirectional) => {
                        self.advance();
                        let target = self.consume_identifier()?;
                        let weight = if self.peek() == Some(&Token::Colon) {
                            self.advance();
                            self.consume_number()?
                        } else {
                            0.5 // Default weight
                        };
                        connections.push(Connection {
                            connection_type: ConnectionType::Bidirectional,
                            target,
                            weight,
                        });
                    }
                    Some(Token::Unidirectional) => {
                        self.advance();
                        let target = self.consume_identifier()?;
                        let weight = if self.peek() == Some(&Token::Colon) {
                            self.advance();
                            self.consume_number()?
                        } else {
                            0.5
                        };
                        connections.push(Connection {
                            connection_type: ConnectionType::Unidirectional,
                            target,
                            weight,
                        });
                    }
                    Some(Token::Equivalence) => {
                        self.advance();
                        let target = self.consume_identifier()?;
                        connections.push(Connection {
                            connection_type: ConnectionType::Equivalence,
                            target,
                            weight: 1.0,
                        });
                    }
                    Some(Token::Identifier(prop_name)) => {
                        let prop = prop_name.clone();
                        self.advance();
                        if self.peek() == Some(&Token::Colon) {
                            self.advance();
                            let value = self.consume_number()?;
                            properties.insert(prop, value);
                        }
                    }
                    _ => {
                        // Skip unknown tokens
                        self.advance();
                    }
                }
            }

            // Consume closing brace
            if self.peek() == Some(&Token::RightBrace) {
                self.advance();
            }
        }

        Ok(Statement::Weave(WeaveStatement {
            name,
            connections,
            properties,
        }))
    }

    fn parse_connection(&mut self, from: String) -> Result<Statement, String> {
        let conn_type = match self.peek() {
            Some(Token::Bidirectional) => {
                self.advance();
                ConnectionType::Bidirectional
            }
            Some(Token::Unidirectional) => {
                self.advance();
                ConnectionType::Unidirectional
            }
            Some(Token::Equivalence) => {
                self.advance();
                ConnectionType::Equivalence
            }
            _ => return Err("Expected connection operator".to_string())
        };

        let to = self.consume_identifier()?;

        let weight = if self.peek() == Some(&Token::Colon) {
            self.advance();
            self.consume_number()?
        } else {
            0.5
        };

        Ok(Statement::Connect(ConnectStatement {
            from,
            to,
            connection_type: conn_type,
            weight,
        }))
    }

    fn parse_command(&mut self) -> Result<Statement, String> {
        self.advance(); // consume ':'

        match self.peek() {
            Some(Token::Identifier(cmd)) if cmd == "topology" => {
                self.advance();
                Ok(Statement::Command(CommandStatement::ShowTopology))
            }
            Some(Token::Identifier(cmd)) if cmd == "hebbian" => {
                self.advance();
                Ok(Statement::Command(CommandStatement::ApplyHebbian))
            }
            Some(Token::Identifier(cmd)) if cmd == "sleep" => {
                self.advance();
                Ok(Statement::Command(CommandStatement::Sleep))
            }
            _ => Err("Unknown command".to_string())
        }
    }

    // Helper functions
    fn peek(&self) -> Option<&Token> {
        if self.current < self.tokens.len() {
            Some(&self.tokens[self.current])
        } else {
            None
        }
    }

    fn peek_ahead(&self, n: usize) -> Option<&Token> {
        if self.current + n < self.tokens.len() {
            Some(&self.tokens[self.current + n])
        } else {
            None
        }
    }

    fn advance(&mut self) -> Option<Token> {
        if !self.is_at_end() {
            self.current += 1;
            Some(self.tokens[self.current - 1].clone())
        } else {
            None
        }
    }

    fn is_at_end(&self) -> bool {
        self.current >= self.tokens.len()
    }

    fn consume(&mut self, expected: Token) -> Result<(), String> {
        if self.peek() == Some(&expected) {
            self.advance();
            Ok(())
        } else {
            Err(format!("Expected {:?}, got {:?}", expected, self.peek()))
        }
    }

    fn consume_identifier(&mut self) -> Result<String, String> {
        match self.peek() {
            Some(Token::Identifier(name)) => {
                let result = name.clone();
                self.advance();
                Ok(result)
            }
            _ => Err(format!("Expected identifier, got {:?}", self.peek()))
        }
    }

    fn consume_number(&mut self) -> Result<f32, String> {
        match self.peek() {
            Some(Token::Number(n)) => {
                let result = *n;
                self.advance();
                Ok(result)
            }
            _ => Err(format!("Expected number, got {:?}", self.peek()))
        }
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_parse_simple_weave() {
        let input = "weave grandmother {
            <~> warmth: 0.8
            ~> stories: 0.9
        }";

        let mut parser = SimpleParser::new(input);
        let program = parser.parse().unwrap();

        assert_eq!(program.statements.len(), 1);
        match &program.statements[0] {
            Statement::Weave(w) => {
                assert_eq!(w.name, "grandmother");
                assert_eq!(w.connections.len(), 2);
                assert_eq!(w.connections[0].target, "warmth");
                assert_eq!(w.connections[0].weight, 0.8);
            }
            _ => panic!("Expected weave statement")
        }
    }

    #[test]
    fn test_parse_connection() {
        let input = "node1 <~> node2: 0.7";

        let mut parser = SimpleParser::new(input);
        let program = parser.parse().unwrap();

        match &program.statements[0] {
            Statement::Connect(c) => {
                assert_eq!(c.from, "node1");
                assert_eq!(c.to, "node2");
                assert_eq!(c.weight, 0.7);
            }
            _ => panic!("Expected connect statement")
        }
    }
}