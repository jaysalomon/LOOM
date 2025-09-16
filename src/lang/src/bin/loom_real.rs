// REAL WORKING LOOM - Actually does something!

use std::io::{self, Write};
use loom_lang::actual_implementation::WorkingLoomSystem;

fn main() {
    println!("=== REAL LOOM IMPLEMENTATION ===");
    println!("This ACTUALLY stores topology in memory-mapped files");
    println!("This ACTUALLY learns and remembers");
    println!();

    // Create or load actual topology
    let mut system = match WorkingLoomSystem::new("topology.loom") {
        Ok(s) => s,
        Err(e) => {
            eprintln!("Failed to create topology: {}", e);
            return;
        }
    };

    println!("Commands:");
    println!("  learn <text>  - Actually create nodes and connections");
    println!("  query <text>  - Actually search and activate topology");
    println!("  quit         - Exit");
    println!();

    loop {
        print!("> ");
        io::stdout().flush().unwrap();

        let mut input = String::new();
        io::stdin().read_line(&mut input).unwrap();
        let input = input.trim();

        if input == "quit" {
            break;
        }

        if input.starts_with("learn ") {
            let text = &input[6..];
            let result = system.actually_learn(text);
            println!("{}", result);
        } else if input.starts_with("query ") {
            let text = &input[6..];
            let response = system.actually_respond(text);
            println!("{}", response);
        } else if !input.is_empty() {
            println!("Unknown command. Use 'learn <text>' or 'query <text>'");
        }
    }

    println!("Topology saved to disk. Will persist for next run!");
}