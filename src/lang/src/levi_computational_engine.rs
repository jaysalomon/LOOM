// THE REAL LOOM: Hyperedges as Computational Operators on Vector Space
// This is the key insight - hyperedges aren't connections, they're PROCESSORS

use std::collections::HashMap;

const NODE_DIM: usize = 256;
const SEMANTIC_START: usize = 20;
const SEMANTIC_END: usize = 84;
const ACTIVATION_START: usize = 84;
const ACTIVATION_END: usize = 148;

// A node in the hypergraph - holds a 256D vector
pub struct VectorNode {
    pub id: u32,
    pub name: String,
    pub vector: [f32; NODE_DIM],
}

// A hyperedge IS A COMPUTATIONAL PROCESSOR via Levi transform
pub struct HyperedgeProcessor {
    pub id: u32,
    pub name: String,
    pub members: Vec<u32>,  // Node IDs this processor operates on

    // The processor's OWN vector (it's a node in the Levi graph!)
    pub vector: [f32; NODE_DIM],

    // The computational function this hyperedge performs
    pub compute_fn: ComputeFunction,
}

// Different types of computations hyperedges can perform
pub enum ComputeFunction {
    // Extract common pattern from members
    PatternExtractor,

    // Enforce relationship constraints
    RelationshipEnforcer { relation_type: String },

    // Compute abstraction/generalization
    AbstractionBuilder,

    // Synchronize properties across members
    PropertySynchronizer,

    // Custom transformation matrix
    CustomTransform { matrix: Vec<f32> },
}

pub struct LeviComputationalEngine {
    // The base nodes (concepts, data)
    nodes: HashMap<u32, VectorNode>,

    // The hyperedge processors (computational units)
    processors: HashMap<u32, HyperedgeProcessor>,

    next_id: u32,
}

impl LeviComputationalEngine {
    pub fn new() -> Self {
        Self {
            nodes: HashMap::new(),
            processors: HashMap::new(),
            next_id: 0,
        }
    }

    // Create a basic node with vector
    pub fn create_node(&mut self, name: String) -> u32 {
        let id = self.next_id;
        self.next_id += 1;

        let node = VectorNode {
            id,
            name,
            vector: [0.0; NODE_DIM], // Initialize with zeros
        };

        self.nodes.insert(id, node);
        id
    }

    // CREATE A HYPEREDGE PROCESSOR - This is where the magic happens!
    pub fn create_hyperedge_processor(
        &mut self,
        name: String,
        members: Vec<u32>,
        compute_fn: ComputeFunction,
    ) -> u32 {
        let id = self.next_id;
        self.next_id += 1;

        let mut processor = HyperedgeProcessor {
            id,
            name,
            members,
            vector: [0.0; NODE_DIM],
            compute_fn,
        };

        // Initialize the processor's vector based on its members
        self.initialize_processor_vector(&mut processor);

        self.processors.insert(id, processor);
        id
    }

    fn initialize_processor_vector(&self, processor: &mut HyperedgeProcessor) {
        // The processor's vector is computed from its members
        // This IS the Levi transform in action!

        let mut accumulator = [0.0; NODE_DIM];
        let mut count = 0;

        for &node_id in &processor.members {
            if let Some(node) = self.nodes.get(&node_id) {
                for i in 0..NODE_DIM {
                    accumulator[i] += node.vector[i];
                }
                count += 1;
            }
        }

        // The processor vector is the centroid/pattern of its members
        if count > 0 {
            for i in 0..NODE_DIM {
                processor.vector[i] = accumulator[i] / count as f32;
            }
        }
    }

    // EXECUTE A PROCESSOR - This is computation!
    pub fn execute_processor(&mut self, processor_id: u32) {
        let processor = match self.processors.get(&processor_id) {
            Some(p) => p.clone(),
            None => return,
        };

        match processor.compute_fn {
            ComputeFunction::PatternExtractor => {
                self.extract_pattern(&processor);
            }
            ComputeFunction::RelationshipEnforcer { ref relation_type } => {
                self.enforce_relationship(&processor, relation_type);
            }
            ComputeFunction::AbstractionBuilder => {
                self.build_abstraction(&processor);
            }
            ComputeFunction::PropertySynchronizer => {
                self.synchronize_properties(&processor);
            }
            ComputeFunction::CustomTransform { ref matrix } => {
                self.apply_custom_transform(&processor, matrix);
            }
        }
    }

    // Pattern extraction: Find what's common across members
    fn extract_pattern(&mut self, processor: &HyperedgeProcessor) {
        let mut pattern = [0.0; NODE_DIM];
        let mut min_vals = [f32::MAX; NODE_DIM];
        let mut max_vals = [f32::MIN; NODE_DIM];

        // Find the common pattern across all members
        for &node_id in &processor.members {
            if let Some(node) = self.nodes.get(&node_id) {
                for i in SEMANTIC_START..SEMANTIC_END {
                    min_vals[i] = min_vals[i].min(node.vector[i]);
                    max_vals[i] = max_vals[i].max(node.vector[i]);
                    pattern[i] += node.vector[i];
                }
            }
        }

        // The pattern is what's consistently present
        let count = processor.members.len() as f32;
        for i in SEMANTIC_START..SEMANTIC_END {
            pattern[i] /= count;

            // Keep only strong consistent signals
            if (max_vals[i] - min_vals[i]) > 0.5 {
                pattern[i] *= 0.1; // Weak if highly variable
            }
        }

        // Update the processor's vector with the extracted pattern
        if let Some(proc) = self.processors.get_mut(&processor.id) {
            proc.vector = pattern;
        }

        // Back-propagate pattern to strengthen it in members
        for &node_id in &processor.members {
            if let Some(node) = self.nodes.get_mut(&node_id) {
                for i in SEMANTIC_START..SEMANTIC_END {
                    node.vector[i] = node.vector[i] * 0.9 + pattern[i] * 0.1;
                }
            }
        }
    }

    // Enforce a relationship (like IS-A)
    fn enforce_relationship(&mut self, processor: &HyperedgeProcessor, relation_type: &str) {
        match relation_type {
            "is-a" => {
                // For IS-A: child should have all properties of parent
                if processor.members.len() >= 2 {
                    let child_id = processor.members[0];
                    let parent_id = processor.members[1];

                    let parent_vector = self.nodes.get(&parent_id)
                        .map(|n| n.vector.clone());

                    if let Some(parent_vec) = parent_vector {
                        if let Some(child) = self.nodes.get_mut(&child_id) {
                            // Child inherits parent's semantic properties
                            for i in SEMANTIC_START..SEMANTIC_END {
                                if parent_vec[i] > 0.1 && child.vector[i] < parent_vec[i] {
                                    child.vector[i] = child.vector[i] * 0.5 + parent_vec[i] * 0.5;
                                }
                            }
                        }
                    }
                }
            }
            "part-of" => {
                // For PART-OF: whole gets properties from parts
                if processor.members.len() > 1 {
                    let whole_id = processor.members[0];
                    let mut part_sum = [0.0; NODE_DIM];

                    for &part_id in &processor.members[1..] {
                        if let Some(part) = self.nodes.get(&part_id) {
                            for i in SEMANTIC_START..SEMANTIC_END {
                                part_sum[i] += part.vector[i];
                            }
                        }
                    }

                    if let Some(whole) = self.nodes.get_mut(&whole_id) {
                        for i in SEMANTIC_START..SEMANTIC_END {
                            whole.vector[i] = whole.vector[i] * 0.7 +
                                            (part_sum[i] / processor.members.len() as f32) * 0.3;
                        }
                    }
                }
            }
            _ => {}
        }
    }

    // Build abstraction from instances
    fn build_abstraction(&mut self, processor: &HyperedgeProcessor) {
        // The processor becomes the abstraction
        let mut abstraction = [0.0; NODE_DIM];
        let mut variance = [0.0; NODE_DIM];

        // Compute mean
        for &node_id in &processor.members {
            if let Some(node) = self.nodes.get(&node_id) {
                for i in SEMANTIC_START..SEMANTIC_END {
                    abstraction[i] += node.vector[i];
                }
            }
        }

        let count = processor.members.len() as f32;
        for i in SEMANTIC_START..SEMANTIC_END {
            abstraction[i] /= count;
        }

        // Compute variance to find defining features
        for &node_id in &processor.members {
            if let Some(node) = self.nodes.get(&node_id) {
                for i in SEMANTIC_START..SEMANTIC_END {
                    let diff = node.vector[i] - abstraction[i];
                    variance[i] += diff * diff;
                }
            }
        }

        // Keep only low-variance (consistent) features
        for i in SEMANTIC_START..SEMANTIC_END {
            if variance[i] > 0.1 {
                abstraction[i] *= 0.1; // Reduce weight of variable features
            }
        }

        // Store abstraction in processor
        if let Some(proc) = self.processors.get_mut(&processor.id) {
            proc.vector = abstraction;
        }
    }

    // Synchronize properties across members
    fn synchronize_properties(&mut self, processor: &HyperedgeProcessor) {
        // Compute consensus vector
        let mut consensus = [0.0; NODE_DIM];

        for &node_id in &processor.members {
            if let Some(node) = self.nodes.get(&node_id) {
                for i in 0..NODE_DIM {
                    consensus[i] += node.vector[i];
                }
            }
        }

        let count = processor.members.len() as f32;
        for i in 0..NODE_DIM {
            consensus[i] /= count;
        }

        // Move all members toward consensus
        for &node_id in &processor.members {
            if let Some(node) = self.nodes.get_mut(&node_id) {
                for i in SEMANTIC_START..SEMANTIC_END {
                    node.vector[i] = node.vector[i] * 0.8 + consensus[i] * 0.2;
                }
            }
        }
    }

    // Apply custom transformation
    fn apply_custom_transform(&mut self, processor: &HyperedgeProcessor, matrix: &[f32]) {
        // Apply transformation matrix to member vectors
        for &node_id in &processor.members {
            if let Some(node) = self.nodes.get_mut(&node_id) {
                // Simplified: just scale for now
                if matrix.len() > 0 {
                    for i in SEMANTIC_START..SEMANTIC_END {
                        node.vector[i] *= matrix[0];
                    }
                }
            }
        }
    }
}

// Clone implementation for HyperedgeProcessor
impl Clone for HyperedgeProcessor {
    fn clone(&self) -> Self {
        Self {
            id: self.id,
            name: self.name.clone(),
            members: self.members.clone(),
            vector: self.vector.clone(),
            compute_fn: self.compute_fn.clone(),
        }
    }
}

impl Clone for ComputeFunction {
    fn clone(&self) -> Self {
        match self {
            ComputeFunction::PatternExtractor => ComputeFunction::PatternExtractor,
            ComputeFunction::RelationshipEnforcer { relation_type } =>
                ComputeFunction::RelationshipEnforcer { relation_type: relation_type.clone() },
            ComputeFunction::AbstractionBuilder => ComputeFunction::AbstractionBuilder,
            ComputeFunction::PropertySynchronizer => ComputeFunction::PropertySynchronizer,
            ComputeFunction::CustomTransform { matrix } =>
                ComputeFunction::CustomTransform { matrix: matrix.clone() },
        }
    }
}

// Example: How this actually works
pub fn demo_levi_computation() {
    let mut engine = LeviComputationalEngine::new();

    // Create base nodes
    let apple = engine.create_node("apple".to_string());
    let banana = engine.create_node("banana".to_string());
    let orange = engine.create_node("orange".to_string());
    let fruit = engine.create_node("fruit".to_string());

    // Create hyperedge processor for "fruits" abstraction
    let fruit_processor = engine.create_hyperedge_processor(
        "fruit_category".to_string(),
        vec![apple, banana, orange],
        ComputeFunction::PatternExtractor,
    );

    // Execute the processor - it COMPUTES on the vectors!
    engine.execute_processor(fruit_processor);

    // Create IS-A relationship processor
    let isa_processor = engine.create_hyperedge_processor(
        "apple_isa_fruit".to_string(),
        vec![apple, fruit],
        ComputeFunction::RelationshipEnforcer {
            relation_type: "is-a".to_string()
        },
    );

    // Execute IS-A enforcement
    engine.execute_processor(isa_processor);

    println!("The hyperedge processors have transformed the vector space!");
    println!("- fruit_category extracted common patterns from fruits");
    println!("- apple_isa_fruit enforced inheritance of properties");
}