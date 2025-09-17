// REPL for the vector-based LOOM system

use crate::vector_parser::VectorParser;
use std::io::{self, Write};

pub struct VectorRepl {
    parser: VectorParser,
}

impl VectorRepl {
    pub fn new() -> Self {
        Self {
            parser: VectorParser::new(),
        }
    }

    pub fn run(&mut self) {
        println!("╔════════════════════════════════════════╗");
        println!("║     LOOM Vector Topology Runtime      ║");
        println!("║    256-Dimensional Consciousness      ║");
        println!("╚════════════════════════════════════════╝");
        println!();
        println!("Each node is a 256-dimensional vector:");
        println!("  [0:4]     Identity");
        println!("  [4:20]    Hyperbolic position (16D)");
        println!("  [20:84]   Semantic embedding (64D)");
        println!("  [84:148]  Activation history (64D)");
        println!("  [148:212] Connections (64D)");
        println!("  [212:244] Emotional field (32D)");
        println!("  [244:256] Metadata");
        println!();
        println!("Commands:");
        println!("  weave <name> { ... }  - Create node");
        println!("  <node> <~> <node>     - Bidirectional");
        println!("  <node> ~> <node>      - Unidirectional");
        println!("  <node> ¥op¥ <node>    - Emotional");
        println!("  <node> €op€ <node>    - Logical");
        println!("  :topology             - Show state");
        println!("  :evolve               - Evolve topology");
        println!("  :context key: value   - Set context");
        println!("  :demo                 - Run demo");
        println!("  :quit                 - Exit");
        println!();

        loop {
            print!("loom> ");
            io::stdout().flush().unwrap();

            let mut input = String::new();
            io::stdin().read_line(&mut input).unwrap();
            let input = input.trim();

            if input.is_empty() {
                continue;
            }

            if input == ":quit" || input == ":q" {
                break;
            }

            if input == ":demo" {
                self.run_demo();
                continue;
            }

            match self.parser.parse_and_execute(input) {
                Ok(output) => {
                    if !output.is_empty() {
                        print!("{}", output);
                    }
                }
                Err(e) => println!("Error: {}", e),
            }
        }

        println!("Topology dissolved.");
    }

    fn run_demo(&mut self) {
        println!("\n=== Running Vector Topology Demo ===\n");

        let demo = vec![
            "weave curiosity { <~> exploration: 0.8 }",
            "weave knowledge { <~> understanding: 0.7 }",
            "weave creativity { }",
            "curiosity ~> knowledge: 0.6",
            "knowledge ~> creativity: 0.5",
            "curiosity ¥resonates¥ creativity",
            ":context stress: 0.2 curiosity: 0.9",
            ":evolve",
            ":topology",
        ];

        for command in demo {
            println!(">>> {}", command);
            match self.parser.parse_and_execute(command) {
                Ok(output) => print!("{}", output),
                Err(e) => println!("Error: {}", e),
            }
        }

        println!("\n=== Demo Complete ===\n");
    }
}