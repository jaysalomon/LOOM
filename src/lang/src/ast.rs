use std::collections::HashMap;

#[derive(Debug, Clone, PartialEq)]
pub struct Program {
    pub statements: Vec<Statement>,
}

#[derive(Debug, Clone, PartialEq)]
pub enum Statement {
    Weave(WeaveStatement),
    Evolve(EvolveStatement),
    Pattern(PatternStatement),
    Flow(FlowStatement),
    Topology(TopologyDeclaration),
    Compartment(CompartmentDeclaration),
    Experience(ExperienceBlock),
    Antibody(AntibodyStatement),
    Connection(ConnectionStatement),
    Expression(Expression),
}

#[derive(Debug, Clone, PartialEq)]
pub struct WeaveStatement {
    pub name: String,
    pub connections: Vec<Connection>,
    pub properties: HashMap<String, Expression>,
}

#[derive(Debug, Clone, PartialEq)]
pub struct Connection {
    pub kind: ConnectionKind,
    pub target: String,
    pub strength: Option<f32>,
    pub is_collection: bool,
}

#[derive(Debug, Clone, PartialEq)]
pub enum ConnectionKind {
    Bidirectional,    // <~>
    Unidirectional,   // ~>
    Equivalence,      // <=>
    Abstraction,      // ^
    Contextual,       // ⊗
}

#[derive(Debug, Clone, PartialEq)]
pub struct EvolveStatement {
    pub name: String,
    pub condition: Option<Expression>,
    pub transformation: Transformation,
    pub rate: Option<EvolutionRate>,
}

#[derive(Debug, Clone, PartialEq)]
pub struct Transformation {
    pub from: String,
    pub to: String,
    pub kind: TransformKind,
}

#[derive(Debug, Clone, PartialEq)]
pub enum TransformKind {
    Evolution,        // ~>
    Sudden,          // @>
    Stochastic,      // ≈>
    Decay,           // ↝
    Growth,          // ↟
    Cyclic,          // ⟳
}

#[derive(Debug, Clone, PartialEq)]
pub struct EvolutionRate {
    pub value: Expression,
    pub duration: Duration,
}

#[derive(Debug, Clone, PartialEq)]
pub enum Duration {
    Milliseconds(f32),
    Seconds(f32),
    Minutes(f32),
    Hours(f32),
    Lifetime,
    Every(Box<Duration>),
}

#[derive(Debug, Clone, PartialEq)]
pub struct PatternStatement {
    pub name: String,
    pub body: Vec<Statement>,
    pub conditions: Vec<Condition>,
}

#[derive(Debug, Clone, PartialEq)]
pub struct FlowStatement {
    pub name: String,
    pub from: FlowSource,
    pub through: String,
    pub modulated_by: Option<String>,
    pub generates: Option<String>,
}

#[derive(Debug, Clone, PartialEq)]
pub enum FlowSource {
    Node(String),
    Context(String),
    Expression(Expression),
}

#[derive(Debug, Clone, PartialEq)]
pub struct TopologyDeclaration {
    pub name: String,
    pub evolves_with: Option<String>,
    pub body: Vec<Statement>,
}

#[derive(Debug, Clone, PartialEq)]
pub struct CompartmentDeclaration {
    pub name: String,
    pub lens: String,
    pub operators: Vec<DomainOperator>,
    pub body: Vec<Statement>,
}

#[derive(Debug, Clone, PartialEq)]
pub enum DomainOperator {
    Emotional(String),
    Logical(String),
    Temporal(String),
    Social(String),
}

#[derive(Debug, Clone, PartialEq)]
pub struct ExperienceBlock {
    pub name: String,
    pub body: Vec<Statement>,
}

#[derive(Debug, Clone, PartialEq)]
pub struct AntibodyStatement {
    pub conflict: Expression,
    pub resolution: ResolutionStrategy,
    pub priority: Option<Priority>,
    pub preserve: Option<String>,
    pub process_during: Option<String>,
}

#[derive(Debug, Clone, PartialEq)]
pub enum ResolutionStrategy {
    AbstractToInvariant,
    Merge,
    Prune,
    Strengthen,
    Custom(String),
}

#[derive(Debug, Clone, PartialEq)]
pub enum Priority {
    High,
    Low,
    Conditional(Expression),
}

#[derive(Debug, Clone, PartialEq)]
pub struct ConnectionStatement {
    pub nodes: Vec<String>,
    pub operator: ConnectionKind,
    pub target: String,
}

#[derive(Debug, Clone, PartialEq)]
pub enum Expression {
    Number(f32),
    String(String),
    Identifier(String),
    ContextVariable(String),
    BinaryOp {
        left: Box<Expression>,
        op: BinaryOperator,
        right: Box<Expression>,
    },
    UnaryOp {
        op: UnaryOperator,
        operand: Box<Expression>,
    },
    DomainOp {
        left: Box<Expression>,
        domain: DomainKind,
        op: String,
        right: Box<Expression>,
    },
    IndexAccess {
        object: Box<Expression>,
        index: Index,
    },
    FieldAccess {
        object: Box<Expression>,
        field: String,
    },
    FunctionCall {
        name: String,
        args: Vec<Expression>,
    },
    List(Vec<Expression>),
    Now,
}

#[derive(Debug, Clone, PartialEq)]
pub enum Index {
    Single(usize),
    Range(usize, usize),
    All,
}

#[derive(Debug, Clone, PartialEq)]
pub enum BinaryOperator {
    Add,
    Subtract,
    Multiply,
    Divide,
    GreaterThan,
    LessThan,
    GreaterEqual,
    LessEqual,
    Equal,
    NotEqual,
    And,
    Or,
}

#[derive(Debug, Clone, PartialEq)]
pub enum UnaryOperator {
    Not,
    Negate,
}

#[derive(Debug, Clone, PartialEq)]
pub enum DomainKind {
    Emotional,
    Logical,
    Temporal,
    Social,
}

#[derive(Debug, Clone, PartialEq)]
pub struct Condition {
    pub when: Expression,
    pub then: Vec<Statement>,
}

// Helper functions for AST construction
impl WeaveStatement {
    pub fn new(name: String) -> Self {
        Self {
            name,
            connections: Vec::new(),
            properties: HashMap::new(),
        }
    }

    pub fn add_connection(&mut self, kind: ConnectionKind, target: String, strength: Option<f32>) {
        self.connections.push(Connection {
            kind,
            target,
            strength,
            is_collection: false,
        });
    }
}

impl TopologyDeclaration {
    pub fn new(name: String) -> Self {
        Self {
            name,
            evolves_with: None,
            body: Vec::new(),
        }
    }
}

impl Expression {
    pub fn binary(left: Expression, op: BinaryOperator, right: Expression) -> Self {
        Expression::BinaryOp {
            left: Box::new(left),
            op,
            right: Box::new(right),
        }
    }

    pub fn domain(left: Expression, domain: DomainKind, op: String, right: Expression) -> Self {
        Expression::DomainOp {
            left: Box::new(left),
            domain,
            op,
            right: Box::new(right),
        }
    }
}