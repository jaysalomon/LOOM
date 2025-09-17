// Demonstration of how symbolic concepts emerge from vector structure
// Shows how "apple" relates to green, food, fruit, plant through the 256D space

use crate::semantic_encoding::ConceptEncoder;
use crate::vector_topology::{VectorTopology, NodeVector, SEMANTIC_START};

pub struct ConceptNetwork {
    topology: VectorTopology,
    encoder: ConceptEncoder,
}

impl ConceptNetwork {
    pub fn new() -> Self {
        Self {
            topology: VectorTopology::new(),
            encoder: ConceptEncoder::new(),
        }
    }

    // Build a network of related concepts
    pub fn build_fruit_network(&mut self) {
        // Create apple with full semantic encoding
        let apple = self.encoder.create_apple_node();
        self.topology.nodes.write().insert("apple".to_string(), apple);

        // Create color concepts
        let mut green = NodeVector::new("green".to_string());
        self.encode_color(&mut green, 0.0, 1.0, 0.0);
        self.topology.nodes.write().insert("green".to_string(), green);

        let mut red = NodeVector::new("red".to_string());
        self.encode_color(&mut red, 1.0, 0.0, 0.0);
        self.topology.nodes.write().insert("red".to_string(), red);

        // Create category concepts
        let mut fruit = NodeVector::new("fruit".to_string());
        self.encode_category(&mut fruit, "fruit");
        self.topology.nodes.write().insert("fruit".to_string(), fruit);

        let mut food = NodeVector::new("food".to_string());
        self.encode_category(&mut food, "food");
        self.topology.nodes.write().insert("food".to_string(), food);

        let mut plant = NodeVector::new("plant".to_string());
        self.encode_category(&mut plant, "plant");
        self.topology.nodes.write().insert("plant".to_string(), plant);

        // Now discover connections based on vector similarity!
        self.discover_connections();
    }

    fn encode_color(&self, node: &mut NodeVector, r: f32, g: f32, b: f32) {
        // Colors primarily affect dimensions 0-8 of semantic space
        node.data[SEMANTIC_START] = r;
        node.data[SEMANTIC_START + 1] = g;
        node.data[SEMANTIC_START + 2] = b;
        node.data[SEMANTIC_START + 3] = (r + g + b) / 3.0; // brightness
    }

    fn encode_category(&self, node: &mut NodeVector, category: &str) {
        match category {
            "fruit" => {
                // Fruit pattern: organic + food + sweet + plant
                node.data[SEMANTIC_START + 8] = 0.9;  // organic
                node.data[SEMANTIC_START + 16] = 0.9; // food
                node.data[SEMANTIC_START + 17] = 0.7; // sweet
                node.data[SEMANTIC_START + 11] = 0.8; // plant-based
            }
            "food" => {
                // Food pattern: edible + nutritious
                node.data[SEMANTIC_START + 16] = 1.0; // food dimension
                node.data[SEMANTIC_START + 17] = 0.5; // nutrition
                node.data[SEMANTIC_START + 18] = 0.6; // energy
            }
            "plant" => {
                // Plant pattern: organic + growing + photosynthetic
                node.data[SEMANTIC_START + 8] = 1.0;  // organic
                node.data[SEMANTIC_START + 9] = 0.8;  // growing
                node.data[SEMANTIC_START + 11] = 1.0; // plant kingdom
            }
            _ => {}
        }
    }

    fn discover_connections(&mut self) {
        let nodes = self.topology.nodes.read();
        let apple = nodes.get("apple").unwrap();

        println!("\n=== Discovering Symbolic Links for Apple ===\n");

        // Check each concept for relationships
        for (name, concept) in nodes.iter() {
            if name == "apple" { continue; }

            let similarity = apple.semantic_resonance(concept);

            if similarity > 0.2 {
                println!("{} <~> apple: {:.3}", name, similarity);

                // Analyze WHY they're related
                let reasons = self.analyze_relationship(apple, concept);
                for reason in reasons {
                    println!("  - {}", reason);
                }

                // Create actual connection in topology
                self.topology.connect("apple", name, similarity, true);
            }
        }
    }

    fn analyze_relationship(&self, node1: &NodeVector, node2: &NodeVector) -> Vec<String> {
        let mut reasons = Vec::new();
        let sem1 = node1.semantic_embedding();
        let sem2 = node2.semantic_embedding();

        // Check color similarity (dims 0-8)
        let color_sim = self.dimension_similarity(sem1, sem2, 0, 8);
        if color_sim > 0.3 {
            reasons.push(format!("Color match ({:.0}%)", color_sim * 100.0));
        }

        // Check organic/living similarity (dims 8-16)
        let organic_sim = self.dimension_similarity(sem1, sem2, 8, 16);
        if organic_sim > 0.3 {
            reasons.push(format!("Both organic/living ({:.0}%)", organic_sim * 100.0));
        }

        // Check food similarity (dims 16-24)
        let food_sim = self.dimension_similarity(sem1, sem2, 16, 24);
        if food_sim > 0.3 {
            reasons.push(format!("Food relationship ({:.0}%)", food_sim * 100.0));
        }

        // Check for is-a relationship
        if self.is_instance_of(node1, node2) {
            reasons.push(format!("{} IS-A {}", node1.name, node2.name));
        }

        reasons
    }

    fn dimension_similarity(&self, vec1: &[f32], vec2: &[f32], start: usize, end: usize) -> f32 {
        let mut sim = 0.0;
        for i in start..end {
            sim += vec1[i] * vec2[i];
        }
        sim / (end - start) as f32
    }

    fn is_instance_of(&self, instance: &NodeVector, category: &NodeVector) -> bool {
        let inst_sem = instance.semantic_embedding();
        let cat_sem = category.semantic_embedding();

        // Check if instance has all significant dimensions of category
        let mut matches = 0;
        let mut total = 0;

        for i in 0..64 {
            if cat_sem[i] > 0.5 { // Significant dimension in category
                total += 1;
                if inst_sem[i] > cat_sem[i] * 0.7 { // Instance has it too
                    matches += 1;
                }
            }
        }

        if total > 0 {
            (matches as f32 / total as f32) > 0.8
        } else {
            false
        }
    }

    pub fn demonstrate(&mut self) {
        self.build_fruit_network();

        println!("\n=== How Apple is Encoded ===\n");

        let nodes = self.topology.nodes.read();
        let apple = nodes.get("apple").unwrap();

        // Show the actual encoding
        println!("Apple's semantic fingerprint:");
        let semantic = apple.semantic_embedding();

        println!("  Color region [0-8]:");
        print!("    ");
        for i in 0..8 {
            if semantic[i] > 0.1 {
                print!("{:.2} ", semantic[i]);
            }
        }
        println!();

        println!("  Organic region [8-16]:");
        print!("    ");
        for i in 8..16 {
            if semantic[i] > 0.1 {
                print!("{:.2} ", semantic[i]);
            }
        }
        println!();

        println!("  Food region [16-24]:");
        print!("    ");
        for i in 16..24 {
            if semantic[i] > 0.1 {
                print!("{:.2} ", semantic[i]);
            }
        }
        println!();

        // Show emergent properties
        let properties = self.encoder.decode_properties(apple);
        println!("\nEmergent properties from vector:");
        for (prop, strength) in properties {
            println!("  - {} ({:.0}%)", prop, strength * 100.0);
        }

        // Show how evolution would change these relationships
        println!("\n=== After Evolution ===");
        drop(nodes);
        self.topology.evolve();

        println!("\nRelationships have strengthened through interaction!");
        self.topology.describe();
    }
}

pub fn run_concept_demo() {
    println!("\n╔══════════════════════════════════════╗");
    println!("║   How 'Apple' Lives in 256D Space   ║");
    println!("╚══════════════════════════════════════╝");

    let mut network = ConceptNetwork::new();
    network.demonstrate();

    println!("\nKey Insights:");
    println!("1. 'Apple is green' emerges from color dimensions being active");
    println!("2. 'Apple is fruit' emerges from matching the fruit pattern");
    println!("3. 'Apple is food' emerges from food dimensions being active");
    println!("4. These aren't stored as facts but EMERGE from vector structure!");
    println!("5. Connections strengthen through co-activation (Hebbian-like)");
    println!("6. The 256D space allows for rich, nuanced relationships");
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_concept_network() {
        let mut network = ConceptNetwork::new();
        network.build_fruit_network();

        let nodes = network.topology.nodes.read();
        assert!(nodes.contains_key("apple"));
        assert!(nodes.contains_key("green"));
        assert!(nodes.contains_key("fruit"));
    }
}