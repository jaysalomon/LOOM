// Run CIFAR-10 learning with Loom
// This shows how Loom learns differently from neural networks

use loom_lang::cifar_learning::CifarLoomLearning;
use std::io::{self, Write};

fn main() {
    println!("=== LOOM CIFAR-10 LEARNING ===");
    println!();
    println!("This demonstrates how Loom learns from images:");
    println!("- Each image becomes a NODE in topology");
    println!("- Similar images CONNECT to each other");
    println!("- Categories are CONCEPT NODES");
    println!("- Learning is through TOPOLOGY GROWTH, not backprop");
    println!();

    // Initialize
    let mut cifar_loom = match CifarLoomLearning::new() {
        Ok(c) => c,
        Err(e) => {
            eprintln!("Failed to initialize: {}", e);
            eprintln!("\nTo use CIFAR-10:");
            eprintln!("1. Download CIFAR-10 binary version from:");
            eprintln!("   https://www.cs.toronto.edu/~kriz/cifar.html");
            eprintln!("2. Extract data_batch_1.bin");
            eprintln!("3. Place in current directory");
            return;
        }
    };

    loop {
        println!("\nCommands:");
        println!("  load <batch_file>  - Load and learn from CIFAR batch");
        println!("  test              - Test classification");
        println!("  stats             - Show topology statistics");
        println!("  quit              - Exit");
        println!();

        print!("> ");
        io::stdout().flush().unwrap();

        let mut input = String::new();
        io::stdin().read_line(&mut input).unwrap();
        let input = input.trim();

        match input {
            "quit" => break,

            cmd if cmd.starts_with("load ") => {
                let file = &cmd[5..];
                match cifar_loom.learn_from_cifar_batch(file) {
                    Ok(()) => println!("Learning complete!"),
                    Err(e) => eprintln!("Error: {}", e),
                }
            }

            "test" => {
                test_classification(&mut cifar_loom);
            }

            "stats" => {
                show_stats(&cifar_loom);
            }

            _ => {
                println!("Unknown command: {}", input);
            }
        }
    }

    println!("Topology saved. Loom remembers what it learned!");
}

fn test_classification(cifar: &mut CifarLoomLearning) {
    println!("\nClassification test:");
    println!("(Using synthetic test image)");

    // Create a synthetic test image
    let mut test_image = vec![0u8; 3072];

    // Make it mostly red (should classify as something)
    for i in 0..1024 {
        test_image[i] = 200; // Red channel
    }

    let (category, confidence) = cifar.classify_image(&test_image);
    println!("Predicted: {} (confidence: {:.2})", category, confidence);
}

fn show_stats(cifar: &CifarLoomLearning) {
    println!("\n=== Topology Statistics ===");
    println!("Category nodes: 10");
    println!("Image nodes: {}", cifar.image_nodes.len());
    println!("Connections formed: {}", cifar.connections_formed);
    println!("Average connections per image: {:.1}",
             cifar.connections_formed as f32 / cifar.image_nodes.len().max(1) as f32);
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_cifar_initialization() {
        let cifar = CifarLoomLearning::new();
        assert!(cifar.is_ok());
    }
}