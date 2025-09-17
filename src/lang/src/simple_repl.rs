// SIMPLE REPL - A REPL that actually works!

use crate::parser_simple::SimpleParser;
use crate::simple_runtime::SimpleRuntime;
use std::io::{self, Write};

pub struct SimpleRepl {
    runtime: SimpleRuntime,
}

impl SimpleRepl {
    pub fn new() -> Self {
        Self {
            runtime: SimpleRuntime::new(),
        }
    }

    pub fn run(&mut self) {
        println!("=================================");
        println!("     LOOM REPL v0.1-working");
        println!("=================================");
        println!("Finally, a REPL that does something!");
        println!();
        println!("Commands:");
        println!("  weave <name> {{ ... }}  - Create a node");
        println!("  <node> <~> <node>: n   - Connect nodes");
        println!("  :topology              - Show current topology");
        println!("  :hebbian               - Apply Hebbian learning");
        println!("  :sleep                 - Consolidate topology");
        println!("  :activate <node> n     - Activate a node");
        println!("  :help                  - Show this help");
        println!("  :quit                  - Exit");
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

            // Handle special commands
            if input.starts_with(':') {
                if !self.handle_command(input) {
                    break;
                }
                continue;
            }

            // Parse and execute LOOM code
            match self.execute_loom(input) {
                Ok(_) => {}
                Err(e) => println!("Error: {}", e),
            }
        }

        println!("Goodbye!");
    }

    fn execute_loom(&mut self, input: &str) -> Result<(), String> {
        let mut parser = SimpleParser::new(input);
        let program = parser.parse()?;
        self.runtime.execute(program)?;
        Ok(())
    }

    fn handle_command(&mut self, command: &str) -> bool {
        let parts: Vec<&str> = command.split_whitespace().collect();
        match parts.get(0).map(|s| *s) {
            Some(":quit") | Some(":q") => {
                return false;
            }
            Some(":help") | Some(":h") => {
                self.show_help();
            }
            Some(":topology") | Some(":t") => {
                self.runtime.show_topology();
            }
            Some(":hebbian") => {
                self.runtime.apply_hebbian();
            }
            Some(":sleep") => {
                self.runtime.apply_sleep();
            }
            Some(":activate") => {
                if parts.len() >= 3 {
                    let node = parts[1];
                    if let Ok(activation) = parts[2].parse::<f32>() {
                        self.runtime.propagate_activation(node, activation);
                    } else {
                        println!("Invalid activation value");
                    }
                } else {
                    println!("Usage: :activate <node> <value>");
                }
            }
            Some(":clear") => {
                // Clear the topology
                self.runtime = SimpleRuntime::new();
                println!("Topology cleared");
            }
            Some(":example") => {
                self.run_example();
            }
            _ => {
                println!("Unknown command: {}", command);
            }
        }
        true
    }

    fn show_help(&self) {
        println!("\n=== LOOM REPL Help ===");
        println!("Weaving:");
        println!("  weave <name> {{");
        println!("    <~> <target>: <weight>   # Bidirectional connection");
        println!("    ~> <target>: <weight>    # Unidirectional connection");
        println!("    <property>: <value>      # Node property");
        println!("  }}");
        println!();
        println!("Connections:");
        println!("  <node1> <~> <node2>: <weight>");
        println!("  <node1> ~> <node2>: <weight>");
        println!();
        println!("Commands:");
        println!("  :topology, :t     Show current topology");
        println!("  :hebbian          Apply Hebbian learning");
        println!("  :sleep            Consolidate (prune weak, strengthen strong)");
        println!("  :activate N val   Activate node N with value");
        println!("  :clear            Clear all topology");
        println!("  :example          Run example topology");
        println!("  :help, :h         Show this help");
        println!("  :quit, :q         Exit REPL");
        println!("===================\n");
    }

    fn run_example(&mut self) {
        println!("Running example: Creating a simple mind...\n");

        let example = r#"
            weave curiosity {
                <~> exploration: 0.8
                <~> learning: 0.7
            }

            weave memory {
                <~> experience: 0.6
            }

            curiosity <~> memory: 0.5
        "#;

        match self.execute_loom(example) {
            Ok(_) => {
                println!("\nExample topology created!");
                self.runtime.show_topology();

                println!("Activating curiosity...");
                self.runtime.propagate_activation("curiosity", 1.0);

                println!("\nApplying Hebbian learning...");
                self.runtime.apply_hebbian();

                println!("\nFinal topology:");
                self.runtime.show_topology();
            }
            Err(e) => println!("Example failed: {}", e),
        }
    }
}