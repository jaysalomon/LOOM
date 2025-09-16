use crate::lexer::{Token, Lexer};
use crate::ast::*;
use std::collections::HashMap;

pub struct Parser {
    tokens: Vec<Token>,
    current: usize,
}

impl Parser {
    pub fn new(input: &str) -> Self {
        let tokens = Lexer::tokens(input);
        Self { tokens, current: 0 }
    }

    pub fn parse(&mut self) -> Result<Program, String> {
        let mut statements = Vec::new();

        while !self.is_at_end() {
            statements.push(self.parse_statement()?);
        }

        Ok(Program { statements })
    }

    fn parse_statement(&mut self) -> Result<Statement, String> {
        match self.peek() {
            Some(Token::Weave) => self.parse_weave(),
            Some(Token::Evolve) => self.parse_evolve(),
            Some(Token::Pattern) => self.parse_pattern(),
            Some(Token::Flow) => self.parse_flow(),
            Some(Token::Topology) => self.parse_topology(),
            Some(Token::Compartment) => self.parse_compartment(),
            Some(Token::AntibodyStart) => self.parse_antibody(),
            Some(Token::LeftBrace) => self.parse_hyperedge(),
            _ => self.parse_expression_statement(),
        }
    }

    fn parse_weave(&mut self) -> Result<Statement, String> {
        self.consume(Token::Weave)?;

        let name = self.consume_identifier()?;
        let mut weave = WeaveStatement::new(name);

        self.consume(Token::LeftBrace)?;

        while !self.check(&Token::RightBrace) && !self.is_at_end() {
            // Parse connection or property
            if self.check(&Token::Bidirectional) ||
               self.check(&Token::Unidirectional) ||
               self.check(&Token::Equivalence) {
                let conn_type = self.parse_connection_type()?;
                let target = self.consume_identifier()?;

                let strength = if self.check(&Token::Colon) {
                    self.advance();
                    Some(self.consume_number()?)
                } else {
                    None
                };

                weave.add_connection(conn_type, target, strength);
            } else {
                // Property
                let prop_name = self.consume_identifier()?;
                self.consume(Token::Colon)?;
                let value = self.parse_expression()?;
                weave.properties.insert(prop_name, value);
            }
        }

        self.consume(Token::RightBrace)?;
        Ok(Statement::Weave(weave))
    }

    fn parse_evolve(&mut self) -> Result<Statement, String> {
        self.consume(Token::Evolve)?;

        let name = self.consume_identifier()?;
        self.consume(Token::LeftBrace)?;

        let mut condition = None;
        let mut transformation = None;
        let mut rate = None;

        while !self.check(&Token::RightBrace) && !self.is_at_end() {
            match self.peek() {
                Some(Token::When) => {
                    self.advance();
                    self.consume(Token::Colon)?;
                    condition = Some(self.parse_expression()?);
                    self.consume_optional(Token::Comma);
                }
                Some(Token::Transform) => {
                    self.advance();
                    self.consume(Token::Colon)?;
                    let from = self.consume_identifier()?;
                    let trans_kind = self.parse_transform_kind()?;
                    let to = self.consume_identifier()?;
                    transformation = Some(Transformation {
                        from,
                        to,
                        kind: trans_kind,
                    });
                    self.consume_optional(Token::Comma);
                }
                Some(Token::Rate) => {
                    self.advance();
                    self.consume(Token::Colon)?;
                    rate = Some(self.parse_evolution_rate()?);
                }
                _ => return Err("Unexpected token in evolve block".to_string()),
            }
        }

        self.consume(Token::RightBrace)?;

        Ok(Statement::Evolve(EvolveStatement {
            name,
            condition,
            transformation: transformation.ok_or("Evolution needs transformation")?,
            rate,
        }))
    }

    fn parse_pattern(&mut self) -> Result<Statement, String> {
        self.consume(Token::Pattern)?;

        let name = self.consume_identifier()?;
        self.consume(Token::LeftBrace)?;

        let mut body = Vec::new();
        let mut conditions = Vec::new();

        while !self.check(&Token::RightBrace) && !self.is_at_end() {
            if self.check(&Token::When) {
                self.advance();
                let expr = self.parse_expression()?;
                self.consume(Token::Colon)?;
                let then_body = self.parse_block()?;
                conditions.push(Condition {
                    when: expr,
                    then: then_body,
                });
            } else {
                body.push(self.parse_statement()?);
            }
        }

        self.consume(Token::RightBrace)?;

        Ok(Statement::Pattern(PatternStatement {
            name,
            body,
            conditions,
        }))
    }

    fn parse_flow(&mut self) -> Result<Statement, String> {
        self.consume(Token::Flow)?;

        let name = self.consume_identifier()?;
        self.consume(Token::LeftBrace)?;

        let mut from = None;
        let mut through = None;
        let mut modulated_by = None;
        let mut generates = None;

        while !self.check(&Token::RightBrace) && !self.is_at_end() {
            match self.peek() {
                Some(Token::From) => {
                    self.advance();
                    self.consume(Token::Colon)?;
                    from = Some(FlowSource::Node(self.consume_identifier()?));
                    self.consume_optional(Token::Comma);
                }
                Some(Token::Through) => {
                    self.advance();
                    self.consume(Token::Colon)?;
                    through = Some(self.consume_identifier()?);
                    self.consume_optional(Token::Comma);
                }
                Some(Token::ModulatedBy) => {
                    self.advance();
                    self.consume(Token::Colon)?;
                    modulated_by = Some(self.consume_context_var()?);
                }
                _ => break,
            }
        }

        self.consume(Token::RightBrace)?;

        Ok(Statement::Flow(FlowStatement {
            name,
            from: from.ok_or("Flow needs 'from'")?,
            through: through.ok_or("Flow needs 'through'")?,
            modulated_by,
            generates,
        }))
    }

    fn parse_topology(&mut self) -> Result<Statement, String> {
        self.consume(Token::Topology)?;

        let name = self.consume_identifier()?;

        let evolves_with = if self.check(&Token::EvolvesWith) {
            self.advance();
            Some(self.consume_identifier()?)
        } else {
            None
        };

        self.consume(Token::LeftBrace)?;

        let mut body = Vec::new();
        while !self.check(&Token::RightBrace) && !self.is_at_end() {
            body.push(self.parse_statement()?);
        }

        self.consume(Token::RightBrace)?;

        Ok(Statement::Topology(TopologyDeclaration {
            name,
            evolves_with,
            body,
        }))
    }

    fn parse_compartment(&mut self) -> Result<Statement, String> {
        self.consume(Token::Compartment)?;

        let name = self.consume_identifier()?;
        self.consume(Token::LeftBrace)?;

        let mut lens = String::new();
        let mut operators = Vec::new();
        let mut body = Vec::new();

        while !self.check(&Token::RightBrace) && !self.is_at_end() {
            if self.check(&Token::Lens) {
                self.advance();
                self.consume(Token::Colon)?;
                lens = self.consume_identifier()?;
            } else {
                body.push(self.parse_statement()?);
            }
        }

        self.consume(Token::RightBrace)?;

        Ok(Statement::Compartment(CompartmentDeclaration {
            name,
            lens,
            operators,
            body,
        }))
    }

    fn parse_antibody(&mut self) -> Result<Statement, String> {
        self.consume(Token::AntibodyStart)?;

        let conflict = self.parse_expression()?;
        self.consume(Token::RightBrace)?;
        self.consume(Token::LeftBrace)?;

        let mut resolution = ResolutionStrategy::AbstractToInvariant;
        let mut priority = None;
        let mut preserve = None;
        let mut process_during = None;

        while !self.check(&Token::RightBrace) && !self.is_at_end() {
            match self.peek() {
                Some(Token::Resolution) => {
                    self.advance();
                    self.consume(Token::Colon)?;
                    let res_name = self.consume_identifier()?;
                    resolution = match res_name.as_str() {
                        "AbstractToInvariant" => ResolutionStrategy::AbstractToInvariant,
                        "Merge" => ResolutionStrategy::Merge,
                        "Prune" => ResolutionStrategy::Prune,
                        _ => ResolutionStrategy::Custom(res_name),
                    };
                    self.consume_optional(Token::Comma);
                }
                Some(Token::Priority) => {
                    self.advance();
                    self.consume(Token::Colon)?;
                    priority = Some(self.parse_priority()?);
                    self.consume_optional(Token::Comma);
                }
                _ => break,
            }
        }

        self.consume(Token::RightBrace)?;

        Ok(Statement::Antibody(AntibodyStatement {
            conflict,
            resolution,
            priority,
            preserve,
            process_during,
        }))
    }

    fn parse_hyperedge(&mut self) -> Result<Statement, String> {
        self.consume(Token::LeftBrace)?;

        let mut nodes = Vec::new();
        nodes.push(self.consume_identifier()?);

        while self.check(&Token::Comma) {
            self.advance();
            nodes.push(self.consume_identifier()?);
        }

        self.consume(Token::RightBrace)?;

        let operator = self.parse_connection_type()?;
        let target = self.consume_identifier()?;

        Ok(Statement::Connection(ConnectionStatement {
            nodes,
            operator,
            target,
        }))
    }

    fn parse_expression_statement(&mut self) -> Result<Statement, String> {
        let expr = self.parse_expression()?;
        Ok(Statement::Expression(expr))
    }

    fn parse_expression(&mut self) -> Result<Expression, String> {
        self.parse_binary()
    }

    fn parse_binary(&mut self) -> Result<Expression, String> {
        let mut left = self.parse_primary()?;

        while let Some(op) = self.match_binary_op() {
            let right = self.parse_primary()?;
            left = Expression::binary(left, op, right);
        }

        Ok(left)
    }

    fn parse_primary(&mut self) -> Result<Expression, String> {
        match self.peek() {
            Some(Token::Number(n)) => {
                let val = *n;
                self.advance();
                Ok(Expression::Number(val))
            }
            Some(Token::String(s)) => {
                let val = s.clone();
                self.advance();
                Ok(Expression::String(val))
            }
            Some(Token::Identifier(id)) => {
                let val = id.clone();
                self.advance();

                // Check for domain operators
                if let Some(Token::EmotionalOp(op)) = self.peek() {
                    let op_name = op.clone();
                    self.advance();
                    let right = self.parse_primary()?;
                    return Ok(Expression::domain(
                        Expression::Identifier(val),
                        DomainKind::Emotional,
                        op_name,
                        right
                    ));
                }

                Ok(Expression::Identifier(val))
            }
            Some(Token::ContextVariable(var)) => {
                let val = var.clone();
                self.advance();
                Ok(Expression::ContextVariable(val))
            }
            Some(Token::Now) => {
                self.advance();
                Ok(Expression::Now)
            }
            _ => Err(format!("Unexpected token: {:?}", self.peek()))
        }
    }

    // Helper methods
    fn parse_connection_type(&mut self) -> Result<ConnectionKind, String> {
        match self.advance() {
            Some(Token::Bidirectional) => Ok(ConnectionKind::Bidirectional),
            Some(Token::Unidirectional) => Ok(ConnectionKind::Unidirectional),
            Some(Token::Equivalence) => Ok(ConnectionKind::Equivalence),
            Some(Token::Abstraction) => Ok(ConnectionKind::Abstraction),
            _ => Err("Expected connection operator".to_string()),
        }
    }

    fn parse_transform_kind(&mut self) -> Result<TransformKind, String> {
        match self.advance() {
            Some(Token::Unidirectional) => Ok(TransformKind::Evolution),
            Some(Token::SuddenTransition) => Ok(TransformKind::Sudden),
            Some(Token::StochasticEvolution) => Ok(TransformKind::Stochastic),
            _ => Ok(TransformKind::Evolution),
        }
    }

    fn parse_evolution_rate(&mut self) -> Result<EvolutionRate, String> {
        let value = self.parse_expression()?;

        let duration = if self.check(&Token::Over) {
            self.advance();
            let num = self.consume_number()?;
            match self.advance() {
                Some(Token::Seconds) => Duration::Seconds(num),
                Some(Token::Minutes) => Duration::Minutes(num),
                Some(Token::Hours) => Duration::Hours(num),
                _ => Duration::Seconds(num),
            }
        } else {
            Duration::Seconds(1.0)
        };

        Ok(EvolutionRate { value, duration })
    }

    fn parse_priority(&mut self) -> Result<Priority, String> {
        match self.peek() {
            Some(Token::High) => {
                self.advance();
                Ok(Priority::High)
            }
            Some(Token::Low) => {
                self.advance();
                Ok(Priority::Low)
            }
            _ => Ok(Priority::High),
        }
    }

    fn parse_block(&mut self) -> Result<Vec<Statement>, String> {
        let mut statements = Vec::new();

        self.consume(Token::LeftBrace)?;
        while !self.check(&Token::RightBrace) && !self.is_at_end() {
            statements.push(self.parse_statement()?);
        }
        self.consume(Token::RightBrace)?;

        Ok(statements)
    }

    fn match_binary_op(&mut self) -> Option<BinaryOperator> {
        match self.peek()? {
            Token::Plus => {
                self.advance();
                Some(BinaryOperator::Add)
            }
            Token::Minus => {
                self.advance();
                Some(BinaryOperator::Subtract)
            }
            Token::Star => {
                self.advance();
                Some(BinaryOperator::Multiply)
            }
            Token::Divide => {
                self.advance();
                Some(BinaryOperator::Divide)
            }
            Token::GreaterThan => {
                self.advance();
                Some(BinaryOperator::GreaterThan)
            }
            Token::LessThan => {
                self.advance();
                Some(BinaryOperator::LessThan)
            }
            _ => None,
        }
    }

    fn consume_identifier(&mut self) -> Result<String, String> {
        match self.advance() {
            Some(Token::Identifier(id)) => Ok(id.clone()),
            _ => Err("Expected identifier".to_string()),
        }
    }

    fn consume_context_var(&mut self) -> Result<String, String> {
        match self.advance() {
            Some(Token::ContextVariable(var)) => Ok(var.clone()),
            _ => Err("Expected context variable".to_string()),
        }
    }

    fn consume_number(&mut self) -> Result<f32, String> {
        match self.advance() {
            Some(Token::Number(n)) => Ok(*n),
            _ => Err("Expected number".to_string()),
        }
    }

    fn consume(&mut self, expected: Token) -> Result<(), String> {
        if self.check(&expected) {
            self.advance();
            Ok(())
        } else {
            Err(format!("Expected {:?}, got {:?}", expected, self.peek()))
        }
    }

    fn consume_optional(&mut self, token: Token) {
        if self.check(&token) {
            self.advance();
        }
    }

    fn check(&self, token: &Token) -> bool {
        if let Some(t) = self.peek() {
            std::mem::discriminant(t) == std::mem::discriminant(token)
        } else {
            false
        }
    }

    fn peek(&self) -> Option<&Token> {
        self.tokens.get(self.current)
    }

    fn advance(&mut self) -> Option<&Token> {
        if !self.is_at_end() {
            self.current += 1;
        }
        self.tokens.get(self.current - 1)
    }

    fn is_at_end(&self) -> bool {
        self.current >= self.tokens.len()
    }
}