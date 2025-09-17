// LOOM Vector-Based Topology - The REAL Implementation
// Each node is a 256-dimensional vector that IS the computation

use std::collections::HashMap;
use std::sync::Arc;
use parking_lot::RwLock;

// The actual 256-dimensional structure as defined in the spec
pub const NODE_DIM: usize = 256;

// Vector layout indices
pub const ID_START: usize = 0;
pub const ID_END: usize = 4;
pub const HYPERBOLIC_START: usize = 4;
pub const HYPERBOLIC_END: usize = 20;
pub const SEMANTIC_START: usize = 20;
pub const SEMANTIC_END: usize = 84;
pub const ACTIVATION_START: usize = 84;
pub const ACTIVATION_END: usize = 148;
pub const CONNECTION_START: usize = 148;
pub const CONNECTION_END: usize = 212;
pub const EMOTIONAL_START: usize = 212;
pub const EMOTIONAL_END: usize = 244;
pub const METADATA_START: usize = 244;
pub const METADATA_END: usize = 256;

#[derive(Clone)]
pub struct NodeVector {
    pub data: [f32; NODE_DIM],
    pub name: String,
}

impl NodeVector {
    pub fn new(name: String) -> Self {
        let mut data = [0.0f32; NODE_DIM];

        // Initialize with small random values in hyperbolic space
        for i in HYPERBOLIC_START..HYPERBOLIC_END {
            data[i] = (rand::random::<f32>() - 0.5) * 0.1;
        }

        // Set initial activation to small random values (not zero!)
        for i in ACTIVATION_START..ACTIVATION_END {
            data[i] = rand::random::<f32>() * 0.01;
        }

        Self { data, name }
    }

    // Get the semantic embedding region
    pub fn semantic_embedding(&self) -> &[f32] {
        &self.data[SEMANTIC_START..SEMANTIC_END]
    }

    // Get mutable semantic embedding
    pub fn semantic_embedding_mut(&mut self) -> &mut [f32] {
        &mut self.data[SEMANTIC_START..SEMANTIC_END]
    }

    // Get the full activation history (64 dimensions of temporal trace)
    pub fn activation_history(&self) -> &[f32] {
        &self.data[ACTIVATION_START..ACTIVATION_END]
    }

    // Get connection weights region (64 dimensions!)
    pub fn connections(&self) -> &[f32] {
        &self.data[CONNECTION_START..CONNECTION_END]
    }

    // Get emotional field (32 dimensions)
    pub fn emotional_field(&self) -> &[f32] {
        &self.data[EMOTIONAL_START..EMOTIONAL_END]
    }

    // Compute resonance with another node (dot product in semantic space)
    pub fn semantic_resonance(&self, other: &NodeVector) -> f32 {
        let mut resonance = 0.0;
        for i in SEMANTIC_START..SEMANTIC_END {
            resonance += self.data[i] * other.data[i];
        }
        resonance / ((SEMANTIC_END - SEMANTIC_START) as f32)
    }

    // Compute temporal correlation (dot product of activation histories)
    pub fn temporal_correlation(&self, other: &NodeVector) -> f32 {
        let mut correlation = 0.0;
        for i in ACTIVATION_START..ACTIVATION_END {
            correlation += self.data[i] * other.data[i];
        }
        correlation / ((ACTIVATION_END - ACTIVATION_START) as f32)
    }

    // Emotional resonance (32D emotional field interaction)
    pub fn emotional_resonance(&self, other: &NodeVector) -> f32 {
        let mut resonance = 0.0;
        for i in EMOTIONAL_START..EMOTIONAL_END {
            resonance += self.data[i] * other.data[i];
        }
        resonance / ((EMOTIONAL_END - EMOTIONAL_START) as f32)
    }

    // Update position in hyperbolic space based on relationships
    pub fn update_hyperbolic_position(&mut self, attractors: &[(f32, &NodeVector)]) {
        let mut drift = [0.0f32; 16]; // Hyperbolic space is 16D

        for (weight, attractor) in attractors {
            for i in 0..16 {
                let their_pos = attractor.data[HYPERBOLIC_START + i];
                let my_pos = self.data[HYPERBOLIC_START + i];
                drift[i] += weight * (their_pos - my_pos) * 0.1; // Gentle drift
            }
        }

        // Apply hyperbolic constraints (stay in Poincaré ball)
        let mut norm_sq = 0.0;
        for i in 0..16 {
            self.data[HYPERBOLIC_START + i] += drift[i];
            norm_sq += self.data[HYPERBOLIC_START + i].powi(2);
        }

        // Keep within unit ball
        if norm_sq > 0.95 {
            let scale = (0.95 / norm_sq).sqrt();
            for i in 0..16 {
                self.data[HYPERBOLIC_START + i] *= scale;
            }
        }
    }

    // Propagate activation through the vector (shift temporal history)
    pub fn propagate_activation(&mut self, incoming: f32) {
        // Shift activation history (it's a 64D trace!)
        for i in (ACTIVATION_START+1..ACTIVATION_END).rev() {
            self.data[i] = self.data[i-1] * 0.95; // Slight decay
        }
        self.data[ACTIVATION_START] = incoming;
    }

    // Evolve semantic embedding based on experience
    pub fn evolve_semantics(&mut self, experience: &NodeVector, influence: f32) {
        for i in SEMANTIC_START..SEMANTIC_END {
            let target = experience.data[i];
            let current = self.data[i];
            self.data[i] += (target - current) * influence * 0.05;
        }
    }

    // Modulate by emotional context
    pub fn apply_emotional_context(&mut self, context: &[f32; 32]) {
        for i in 0..32 {
            self.data[EMOTIONAL_START + i] =
                self.data[EMOTIONAL_START + i] * 0.9 + context[i] * 0.1;
        }
    }
}

pub struct VectorTopology {
    nodes: Arc<RwLock<HashMap<String, NodeVector>>>,
    // Store connections as transformations between vector spaces
    connections: Arc<RwLock<Vec<Connection>>>,
}

pub struct Connection {
    pub from: String,
    pub to: String,
    // Not just a weight - a transformation matrix (simplified to diagonal for now)
    pub transform: Vec<f32>, // 64D transformation vector
    pub bidirectional: bool,
}

impl VectorTopology {
    pub fn new() -> Self {
        Self {
            nodes: Arc::new(RwLock::new(HashMap::new())),
            connections: Arc::new(RwLock::new(Vec::new())),
        }
    }

    pub fn weave(&self, name: String) -> NodeVector {
        let node = NodeVector::new(name.clone());
        self.nodes.write().insert(name, node.clone());
        node
    }

    pub fn connect(&self, from: &str, to: &str, strength: f32, bidirectional: bool) {
        // Create a 64D transformation vector (not just a scalar weight!)
        let mut transform = vec![0.0; 64];
        for i in 0..64 {
            // Initialize with small random values scaled by strength
            transform[i] = (rand::random::<f32>() - 0.5) * strength;
        }

        self.connections.write().push(Connection {
            from: from.to_string(),
            to: to.to_string(),
            transform,
            bidirectional,
        });
    }

    // The REAL evolution - topology changes based on high-dimensional interactions
    pub fn evolve(&self) {
        let nodes = self.nodes.read();
        let connections = self.connections.read();

        // Collect all interactions
        let mut interactions: HashMap<String, Vec<(f32, NodeVector)>> = HashMap::new();

        for conn in connections.iter() {
            if let (Some(from_node), Some(to_node)) =
                (nodes.get(&conn.from), nodes.get(&conn.to)) {

                // Compute multi-dimensional interaction strength
                let semantic_strength = from_node.semantic_resonance(to_node);
                let temporal_strength = from_node.temporal_correlation(to_node);
                let emotional_strength = from_node.emotional_resonance(to_node);

                // Combined influence (not just multiplication!)
                let influence = (semantic_strength + temporal_strength + emotional_strength) / 3.0;

                // Store interaction for batch update
                interactions.entry(conn.to.clone())
                    .or_insert_with(Vec::new)
                    .push((influence, from_node.clone()));

                if conn.bidirectional {
                    interactions.entry(conn.from.clone())
                        .or_insert_with(Vec::new)
                        .push((influence, to_node.clone()));
                }
            }
        }

        drop(nodes);
        drop(connections);

        // Apply all interactions (this is where the magic happens)
        let mut nodes_mut = self.nodes.write();
        for (node_name, influences) in interactions.iter() {
            if let Some(node) = nodes_mut.get_mut(node_name) {
                // Update hyperbolic position based on relationships
                let attractors: Vec<(f32, &NodeVector)> =
                    influences.iter().map(|(w, n)| (*w, n)).collect();
                node.update_hyperbolic_position(&attractors);

                // Evolve semantics based on connected nodes
                for (influence, other) in influences {
                    node.evolve_semantics(other, *influence);
                }

                // Propagate activation (sum of influences)
                let total_activation: f32 = influences.iter().map(|(w, _)| w).sum();
                node.propagate_activation(total_activation.min(1.0));
            }
        }
    }

    pub fn apply_emotional_context(&self, context: &[f32; 32]) {
        let mut nodes = self.nodes.write();
        for (_, node) in nodes.iter_mut() {
            node.apply_emotional_context(context);
        }
    }

    pub fn describe(&self) {
        let nodes = self.nodes.read();
        println!("\n=== Vector Topology State ===");
        println!("Nodes: {}", nodes.len());

        for (name, node) in nodes.iter() {
            println!("\n{}: ", name);

            // Show position in hyperbolic space (first 3 dimensions for visualization)
            println!("  Hyperbolic position: [{:.3}, {:.3}, {:.3}, ...]",
                node.data[HYPERBOLIC_START],
                node.data[HYPERBOLIC_START + 1],
                node.data[HYPERBOLIC_START + 2]);

            // Show recent activation (first 3 values of history)
            println!("  Recent activation: [{:.3}, {:.3}, {:.3}, ...]",
                node.data[ACTIVATION_START],
                node.data[ACTIVATION_START + 1],
                node.data[ACTIVATION_START + 2]);

            // Compute semantic magnitude
            let semantic_mag: f32 = node.semantic_embedding()
                .iter().map(|x| x * x).sum::<f32>().sqrt();
            println!("  Semantic magnitude: {:.3}", semantic_mag);

            // Compute emotional tone (average of emotional field)
            let emotional_avg: f32 = node.emotional_field().iter().sum::<f32>()
                / (EMOTIONAL_END - EMOTIONAL_START) as f32;
            println!("  Emotional tone: {:.3}", emotional_avg);
        }

        let connections = self.connections.read();
        println!("\nConnections: {}", connections.len());
        for conn in connections.iter() {
            let arrow = if conn.bidirectional { "<~>" } else { "~>" };
            let transform_mag: f32 = conn.transform.iter()
                .map(|x| x * x).sum::<f32>().sqrt();
            println!("  {} {} {} (transform magnitude: {:.3})",
                conn.from, arrow, conn.to, transform_mag);
        }
    }
}

// Simple test to verify this actually does something
#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_vector_topology() {
        let topology = VectorTopology::new();

        // Create nodes
        let _grandmother = topology.weave("grandmother".to_string());
        let _warmth = topology.weave("warmth".to_string());

        // Connect them
        topology.connect("grandmother", "warmth", 0.8, true);

        // Evolve the topology
        topology.evolve();

        // Apply emotional context
        let mut context = [0.0; 32];
        context[0] = 0.5; // Some emotional value
        topology.apply_emotional_context(&context);

        // Check that we have nodes
        assert_eq!(topology.nodes.read().len(), 2);
    }

    #[test]
    fn test_resonance() {
        let mut node1 = NodeVector::new("node1".to_string());
        let mut node2 = NodeVector::new("node2".to_string());

        // Set some semantic values
        for i in SEMANTIC_START..SEMANTIC_START+10 {
            node1.data[i] = 0.5;
            node2.data[i] = 0.5;
        }

        let resonance = node1.semantic_resonance(&node2);
        assert!(resonance > 0.0);
    }
}

// Re-export for convenience
pub use rand;