pub mod lexer;
pub mod ast;
pub mod runtime;
pub mod parser;
pub mod interpreter;
pub mod repl;
pub mod learning;
pub mod io_interface;
pub mod actual_implementation;
pub mod cifar_learning;
pub mod codegen_c;

// New working modules
pub mod parser_simple;
pub mod simple_runtime;
pub mod simple_repl;

// Vector-based implementation (the real approach)
pub mod vector_topology;
pub mod vector_parser;
pub mod vector_repl;
pub mod semantic_encoding;
pub mod concept_demo;

pub use runtime::LoomRuntime;
pub use interpreter::Interpreter;
pub use repl::Repl;
pub use actual_implementation::{RealLoomTopology, WorkingLoomSystem};
pub use cifar_learning::CifarLoomLearning;
pub use codegen_c::emit_c;

// Export the simple working versions
pub use parser_simple::SimpleParser;
pub use simple_runtime::SimpleRuntime;
pub use simple_repl::SimpleRepl;

// Export the vector-based versions
pub use vector_topology::VectorTopology;
pub use vector_parser::VectorParser;
pub use vector_repl::VectorRepl;

// The unified computational architecture
pub mod levi_computational_engine;
pub mod dynamic_sensory_engine;
pub mod unified_demo;
pub mod mathematically_correct_engine;

pub use levi_computational_engine::LeviComputationalEngine;
pub use dynamic_sensory_engine::DynamicSensoryEngine;
pub use unified_demo::{demonstrate_unified_architecture, demo_hyperedge_computation};
pub use mathematically_correct_engine::{MathematicallyCorrectEngine, demo_mathematical_loom};