use logos::Logos;

#[derive(Logos, Debug, PartialEq, Clone)]
pub enum Token {
    // Keywords
    #[token("weave")]
    Weave,
    #[token("evolve")]
    Evolve,
    #[token("pattern")]
    Pattern,
    #[token("flow")]
    Flow,
    #[token("topology")]
    Topology,
    #[token("compartment")]
    Compartment,
    #[token("lens")]
    Lens,
    #[token("when")]
    When,
    #[token("during")]
    During,
    #[token("bootstrap")]
    Bootstrap,
    #[token("invariants")]
    Invariants,
    #[token("context")]
    Context,
    #[token("hormonal_context")]
    HormonalContext,
    #[token("evolves_with")]
    EvolvesWith,
    #[token("modulates")]
    Modulates,
    #[token("from")]
    From,
    #[token("through")]
    Through,
    #[token("modulated_by")]
    ModulatedBy,
    #[token("transform")]
    Transform,
    #[token("rate")]
    Rate,
    #[token("over")]
    Over,
    #[token("every")]
    Every,
    #[token("where")]
    Where,
    #[token("resolution")]
    Resolution,
    #[token("priority")]
    Priority,
    #[token("preserve")]
    Preserve,
    #[token("process_during")]
    ProcessDuring,

    // Structural Operators
    #[token("<~>")]
    Bidirectional,
    #[token("~>")]
    Unidirectional,
    #[token("<=>")]
    Equivalence,
    #[token("^")]
    Abstraction,
    #[token("⊗")]
    ContextModulation,

    // Temporal Operators
    #[token("@>")]
    SuddenTransition,
    #[token("≈>")]
    StochasticEvolution,
    #[token("↝")]
    DecayToward,
    #[token("↟")]
    GrowthToward,
    #[token("⟳")]
    CyclicEvolution,

    // Domain-Specific Operators (with delimiters)
    #[regex(r"¥[^¥]+¥", |lex| lex.slice()[1..lex.slice().len()-1].to_string())]
    EmotionalOp(String),

    #[regex(r"€[^€]+€", |lex| lex.slice()[1..lex.slice().len()-1].to_string())]
    LogicalOp(String),

    #[regex(r"µ[^µ]+µ", |lex| lex.slice()[1..lex.slice().len()-1].to_string())]
    TemporalOp(String),

    #[regex(r"∫[^∫]+∫", |lex| lex.slice()[1..lex.slice().len()-1].to_string())]
    SocialOp(String),

    // Antibody syntax
    #[token("!{")]
    AntibodyStart,

    // Identifiers and literals
    #[regex(r"[a-zA-Z_][a-zA-Z0-9_]*", |lex| lex.slice().to_string())]
    Identifier(String),

    #[regex(r"@[a-zA-Z_][a-zA-Z0-9_]*", |lex| lex.slice()[1..].to_string())]
    ContextVariable(String),

    #[regex(r"[0-9]+(\.[0-9]+)?", |lex| lex.slice().parse::<f32>().unwrap())]
    Number(f32),

    #[regex(r#""([^"]*)""#, |lex| lex.slice()[1..lex.slice().len()-1].to_string())]
    String(String),

    // Collection access
    #[token("[*]")]
    AllInCollection,

    #[regex(r"\[[0-9]+\]", |lex| {
        let s = lex.slice();
        s[1..s.len()-1].parse::<usize>().unwrap()
    })]
    IndexAccess(usize),

    #[regex(r"\[[0-9]+\.\.[0-9]+\]", |lex| {
        let s = lex.slice();
        let inner = &s[1..s.len()-1];
        let parts: Vec<&str> = inner.split("..").collect();
        (parts[0].parse::<usize>().unwrap(), parts[1].parse::<usize>().unwrap())
    })]
    RangeAccess(usize, usize),

    // Structural tokens
    #[token("{")]
    LeftBrace,
    #[token("}")]
    RightBrace,
    #[token("(")]
    LeftParen,
    #[token(")")]
    RightParen,
    #[token("[")]
    LeftBracket,
    #[token("]")]
    RightBracket,
    #[token(",")]
    Comma,
    #[token(":")]
    Colon,
    #[token(";")]
    Semicolon,
    #[token("→")]
    Arrow,
    #[token("↔")]
    BiArrow,
    #[token(".")]
    Dot,
    #[token("*")]
    Star,
    #[token("+")]
    Plus,
    #[token("-")]
    Minus,
    #[token("/")]
    Divide,
    #[token("=")]
    Equals,
    #[token(">")]
    GreaterThan,
    #[token("<")]
    LessThan,
    #[token(">=")]
    GreaterEqual,
    #[token("<=")]
    LessEqual,
    #[token("and")]
    And,
    #[token("or")]
    Or,
    #[token("not")]
    Not,

    // Time units
    #[token("ms")]
    Milliseconds,
    #[token("seconds")]
    Seconds,
    #[token("minutes")]
    Minutes,
    #[token("hours")]
    Hours,
    #[token("lifetime")]
    Lifetime,

    // Special values
    #[token("now")]
    Now,
    #[token("positive")]
    Positive,
    #[token("negative")]
    Negative,
    #[token("neutral")]
    Neutral,
    #[token("high")]
    High,
    #[token("low")]
    Low,

    // Skip whitespace and comments
    #[regex(r"[ \t\n\f]+", logos::skip)]
    #[regex(r"//[^\n]*", logos::skip)]
    #[regex(r"/\*[^*]*\*+(?:[^/*][^*]*\*+)*/", logos::skip)]
    #[error]
    Error,
}

pub struct Lexer<'a> {
    inner: logos::Lexer<'a, Token>,
}

impl<'a> Lexer<'a> {
    pub fn new(input: &'a str) -> Self {
        Self {
            inner: Token::lexer(input),
        }
    }

    pub fn next_token(&mut self) -> Option<Token> {
        self.inner.next()
    }

    pub fn span(&self) -> std::ops::Range<usize> {
        self.inner.span()
    }

    pub fn slice(&self) -> &str {
        self.inner.slice()
    }

    pub fn tokens(input: &'a str) -> Vec<Token> {
        Token::lexer(input).collect()
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_basic_weaving() {
        let input = "weave grandmother {
            <~> warmth: 0.8
            ~> stories: 0.9
        }";

        let tokens = Lexer::tokens(input);
        assert_eq!(tokens[0], Token::Weave);
        assert_eq!(tokens[1], Token::Identifier("grandmother".to_string()));
        assert_eq!(tokens[2], Token::LeftBrace);
        assert_eq!(tokens[3], Token::Bidirectional);
        assert_eq!(tokens[4], Token::Identifier("warmth".to_string()));
        assert_eq!(tokens[5], Token::Colon);
        assert_eq!(tokens[6], Token::Number(0.8));
    }

    #[test]
    fn test_emotional_operators() {
        let input = "node1 ¥resonates¥ node2";
        let tokens = Lexer::tokens(input);
        assert_eq!(tokens[1], Token::EmotionalOp("resonates".to_string()));
    }

    #[test]
    fn test_logical_operators() {
        let input = "belief €contradicts€ observation";
        let tokens = Lexer::tokens(input);
        assert_eq!(tokens[1], Token::LogicalOp("contradicts".to_string()));
    }

    #[test]
    fn test_context_variables() {
        let input = "@stress_hormone > 0.7";
        let tokens = Lexer::tokens(input);
        assert_eq!(tokens[0], Token::ContextVariable("stress_hormone".to_string()));
        assert_eq!(tokens[1], Token::GreaterThan);
        assert_eq!(tokens[2], Token::Number(0.7));
    }

    #[test]
    fn test_antibody_syntax() {
        let input = "!{node1 €contradicts€ node2}";
        let tokens = Lexer::tokens(input);
        assert_eq!(tokens[0], Token::AntibodyStart);
    }

    #[test]
    fn test_evolution() {
        let input = "x ~> 5 over 10 seconds";
        let tokens = Lexer::tokens(input);
        assert_eq!(tokens[1], Token::Unidirectional);
        assert_eq!(tokens[2], Token::Number(5.0));
        assert_eq!(tokens[3], Token::Over);
        assert_eq!(tokens[4], Token::Number(10.0));
        assert_eq!(tokens[5], Token::Seconds);
    }
}