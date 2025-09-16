use crate::parser::Parser;
use crate::interpreter::Interpreter;
use std::io::{self, Write};

pub struct Repl {
    interpreter: Interpreter,
}

impl Repl {
    pub fn new() -> Self {
        Self {
            interpreter: Interpreter::new(),
        }
    }

    pub fn run(&mut self) {
        println!("╔═══════════════════════════════════════════════════════════════╗");
        println!("║                    LOOM TOPOLOGY RUNTIME                      ║");
        println!("║              Living Computation Through Topology              ║");
        println!("╠═══════════════════════════════════════════════════════════════╣");
        println!("║ Commands:                                                     ║");
        println!("║   weave <name> {{ ... }}    - Create a node with connections   ║");
        println!("║   evolve <name> {{ ... }}   - Apply evolution rules            ║");
        println!("║   flow <name> {{ ... }}     - Define activation flow           ║");
        println!("║   :topology              - Show current topology state        ║");
        println!("║   :context <h> <level>   - Set hormonal context              ║");
        println!("║   :hebbian               - Apply Hebbian learning step       ║");
        println!("║   :sleep                 - Run sleep consolidation           ║");
        println!("║   :load <file>           - Load Loom source file             ║");
        println!("║   :help                  - Show this help                    ║");
        println!("║   :quit                  - Exit REPL                         ║");
        println!("╚═══════════════════════════════════════════════════════════════╝");
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

            if input.starts_with(':') {
                if !self.handle_command(input) {
                    break;
                }
            } else {
                self.execute_loom(input);
            }
        }
    }

    fn handle_command(&mut self, command: &str) -> bool {
        let parts: Vec<&str> = command.split_whitespace().collect();

        match parts[0] {
            ":quit" | ":q" => {
                println!("Topology entering dormant state...");
                return false;
            }
            ":topology" | ":t" => {
                self.interpreter.show_topology();
            }
            ":context" => {
                if parts.len() == 3 {
                    let hormone = parts[1];
                    if let Ok(level) = parts[2].parse::<f32>() {
                        self.interpreter.set_context(hormone, level);
                        println!("Set {} to {}", hormone, level);
                    }
                } else {
                    println!("Usage: :context <hormone> <level>");
                }
            }
            ":hebbian" => {
                self.run_hebbian();
            }
            ":sleep" => {
                self.run_sleep_consolidation();
            }
            ":load" => {
                if parts.len() == 2 {
                    self.load_file(parts[1]);
                } else {
                    println!("Usage: :load <filename>");
                }
            }
            ":help" | ":h" => {
                self.show_help();
            }
            _ => {
                println!("Unknown command: {}", parts[0]);
            }
        }

        true
    }

    fn execute_loom(&mut self, input: &str) {
        // Handle multi-line input
        let input = if input.ends_with('{') && !input.contains('}') {
            self.read_multiline(input)
        } else {
            input.to_string()
        };

        let mut parser = Parser::new(&input);
        match parser.parse() {
            Ok(program) => {
                if let Err(e) = self.interpreter.execute(program) {
                    println!("Execution error: {}", e);
                }
            }
            Err(e) => {
                println!("Parse error: {}", e);
            }
        }
    }

    fn read_multiline(&self, first_line: &str) -> String {
        let mut result = first_line.to_string();
        let mut depth = first_line.chars().filter(|&c| c == '{').count() as i32
            - first_line.chars().filter(|&c| c == '}').count() as i32;

        while depth > 0 {
            print!("  ... ");
            io::stdout().flush().unwrap();

            let mut line = String::new();
            io::stdin().read_line(&mut line).unwrap();

            depth += line.chars().filter(|&c| c == '{').count() as i32;
            depth -= line.chars().filter(|&c| c == '}').count() as i32;

            result.push('\n');
            result.push_str(&line);
        }

        result
    }

    fn run_hebbian(&mut self) {
        println!("Running Hebbian learning step...");

        // Get all nodes as "active" for demo
        let runtime = self.interpreter.get_runtime();
        let nodes = runtime.nodes.read().unwrap();
        let active: Vec<u32> = nodes.keys().cloned().collect();

        if active.len() >= 2 {
            runtime.hebbian_step(&active, 0.1);
            println!("Hebbian step completed for {} nodes", active.len());
        } else {
            println!("Need at least 2 nodes for Hebbian learning");
        }
    }

    fn run_sleep_consolidation(&mut self) {
        println!("Entering sleep cycle...");
        println!("  Processing antibodies...");
        println!("  Extracting patterns...");
        println!("  Consolidating memories...");

        self.interpreter.get_runtime().sleep_consolidation();

        println!("Sleep consolidation complete");
    }

    fn load_file(&mut self, filename: &str) {
        match std::fs::read_to_string(filename) {
            Ok(content) => {
                println!("Loading {}...", filename);
                self.execute_loom(&content);
            }
            Err(e) => {
                println!("Error loading file: {}", e);
            }
        }
    }

    fn show_help(&self) {
        println!("\n=== Loom REPL Commands ===");
        println!("Loom syntax:");
        println!("  weave <name> {{ <~> target: strength, ... }}");
        println!("  evolve <name> {{ when: condition, transform: a ~> b, rate: value }}");
        println!("  {{node1, node2, ...}} ~> hyperedge_name");
        println!();
        println!("REPL commands:");
        println!("  :topology    - Show current topology state");
        println!("  :context <hormone> <level> - Set hormonal context");
        println!("  :hebbian     - Run Hebbian learning step");
        println!("  :sleep       - Run sleep consolidation");
        println!("  :load <file> - Load and execute Loom file");
        println!("  :quit        - Exit REPL");
        println!();
    }
}