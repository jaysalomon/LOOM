// Demonstration of LOOM's unified architecture
// Shows how hyperedges are computational processors acting on vector space

use crate::levi_computational_engine::*;
use crate::dynamic_sensory_engine::*;
use std::time::Instant;

pub fn demonstrate_unified_architecture() {
    println!("\n═══════════════════════════════════════════════════════════");
    println!("    LOOM UNIFIED ARCHITECTURE DEMONSTRATION");
    println!("    Hyperedges as Computational Processors via Levi Transform");
    println!("═══════════════════════════════════════════════════════════\n");

    // Part 1: Static demonstration of hyperedge processors
    println!("PART 1: Hyperedge Processors Transform Vector Space");
    println!("─────────────────────────────────────────────────────");

    let mut engine = LeviComputationalEngine::new();

    // Create concept nodes
    let apple = engine.create_node("apple".to_string());
    let banana = engine.create_node("banana".to_string());
    let orange = engine.create_node("orange".to_string());
    let fruit = engine.create_node("fruit".to_string());

    println!("Created nodes: apple, banana, orange, fruit");

    // Create a hyperedge processor that extracts patterns
    let fruit_pattern = engine.create_hyperedge_processor(
        "fruit_pattern".to_string(),
        vec![apple, banana, orange],
        ComputeFunction::PatternExtractor,
    );

    println!("\nCreated hyperedge processor 'fruit_pattern'");
    println!("  - Members: [apple, banana, orange]");
    println!("  - Function: PatternExtractor");

    // Execute the processor - it transforms the vector space!
    engine.execute_processor(fruit_pattern);
    println!("\n✓ Executed processor - extracted common patterns from fruits");

    // Create IS-A relationship processor
    let apple_isa = engine.create_hyperedge_processor(
        "apple_isa_fruit".to_string(),
        vec![apple, fruit],
        ComputeFunction::RelationshipEnforcer {
            relation_type: "is-a".to_string()
        },
    );

    println!("\nCreated hyperedge processor 'apple_isa_fruit'");
    println!("  - Members: [apple, fruit]");
    println!("  - Function: RelationshipEnforcer(is-a)");

    engine.execute_processor(apple_isa);
    println!("\n✓ Executed processor - apple now inherits fruit's properties");

    // Part 2: Dynamic living system
    println!("\n\nPART 2: Living System with Continuous Sensory Processing");
    println!("─────────────────────────────────────────────────────");

    let mut living_system = DynamicSensoryEngine::new();

    println!("\nSimulating sensory input over 50 cycles...\n");

    for cycle in 0..50 {
        // Inject visual patterns
        let visual_data = vec![
            (cycle as f32 * 0.13) % 1.0,
            (cycle as f32 * 0.27) % 1.0,
            (cycle as f32 * 0.41) % 1.0,
        ];

        living_system.receive_sensory(SensoryStream {
            stream_type: SensoryType::Visual,
            data: visual_data,
            timestamp: Instant::now(),
        });

        // Inject semantic concepts periodically
        if cycle % 10 == 0 {
            let semantic_data = vec![
                0.8,  // Strong semantic signal
                0.2,  // Weak semantic signal
                0.5,  // Medium semantic signal
            ];

            living_system.receive_sensory(SensoryStream {
                stream_type: SensoryType::Semantic,
                data: semantic_data,
                timestamp: Instant::now(),
            });
        }

        // Run the computational cycle
        living_system.run_cycle();

        // Report status periodically
        if cycle % 10 == 0 && cycle > 0 {
            let active_nodes = living_system.get_active_nodes();
            let processor_count = living_system.active_processors.len();
            let resonance_count = living_system.resonance_map.len();

            println!("Cycle {:3}: {} active nodes | {} processors | {} resonances",
                cycle, active_nodes.len(), processor_count, resonance_count);

            // Show some resonance patterns
            if resonance_count > 0 {
                print!("  Resonances: ");
                for ((n1, n2), strength) in living_system.resonance_map.iter().take(3) {
                    print!("({},{})={:.2} ", n1, n2, strength);
                }
                println!();
            }
        }
    }

    println!("\n═══════════════════════════════════════════════════════════");
    println!("KEY INSIGHTS DEMONSTRATED:");
    println!("─────────────────────────────────────────────────────");
    println!("1. Hyperedges are NOT passive connections");
    println!("2. Via Levi transform, hyperedges become COMPUTATIONAL NODES");
    println!("3. Each hyperedge processor actively transforms its member vectors");
    println!("4. The system continuously processes sensory input");
    println!("5. Resonance patterns spawn new processors dynamically");
    println!("6. The topology IS the computation, memory IS the processing");
    println!("═══════════════════════════════════════════════════════════\n");
}

// Simpler demo focused on the core concept
pub fn demo_hyperedge_computation() {
    println!("\n╔══════════════════════════════════════════════╗");
    println!("║  HYPEREDGE AS COMPUTATIONAL PROCESSOR DEMO  ║");
    println!("╚══════════════════════════════════════════════╝\n");

    let mut engine = LeviComputationalEngine::new();

    // Create two nodes
    let node_a = engine.create_node("A".to_string());
    let node_b = engine.create_node("B".to_string());

    println!("Step 1: Created nodes A and B with 256D vectors\n");

    // Create a hyperedge processor between them
    let processor = engine.create_hyperedge_processor(
        "A_synchronizes_B".to_string(),
        vec![node_a, node_b],
        ComputeFunction::PropertySynchronizer,
    );

    println!("Step 2: Created hyperedge processor 'A_synchronizes_B'");
    println!("        This is now a COMPUTATIONAL NODE in the Levi graph!\n");

    // Execute the processor
    println!("Step 3: Executing the processor...");
    engine.execute_processor(processor);

    println!("        ✓ Processor computed over A and B's vectors");
    println!("        ✓ Synchronized their semantic properties");
    println!("        ✓ Updated both vectors based on computation\n");

    println!("This is the KEY INSIGHT:");
    println!("────────────────────────");
    println!("Traditional Graph:  A ──edge──> B  (edge is passive data)");
    println!("LOOM via Levi:     A ──> [Processor] ──> B  (actively computes!)");
    println!("\nThe hyperedge DOES WORK on the vector space!");
}