// LOOM as a Living System - Constantly Processing Sensory Input
// The hypergraph never stops computing, it's always transforming based on input

use std::collections::{HashMap, VecDeque};
use std::time::{Duration, Instant};
use std::sync::Arc;
use parking_lot::RwLock;

use crate::levi_computational_engine::*;

const NODE_DIM: usize = 256;
const ACTIVATION_START: usize = 84;
const ACTIVATION_END: usize = 148;
const SEMANTIC_START: usize = 20;
const SEMANTIC_END: usize = 84;

// Sensory input stream - continuous data flowing in
pub struct SensoryStream {
    pub stream_type: SensoryType,
    pub data: Vec<f32>,
    pub timestamp: Instant,
}

pub enum SensoryType {
    Visual,      // Visual patterns
    Auditory,    // Sound patterns
    Tactile,     // Touch/pressure
    Temporal,    // Time sequences
    Semantic,    // Language/concepts
    Emotional,   // Emotional states
}

// The dynamic engine - always running, always computing
pub struct DynamicSensoryEngine {
    // The computational hypergraph
    levi_engine: LeviComputationalEngine,

    // Input buffers for different sensory streams
    sensory_buffers: HashMap<SensoryType, VecDeque<SensoryStream>>,

    // Active processors - constantly running
    active_processors: Vec<u32>,

    // Resonance tracking - which parts are co-activating
    resonance_map: HashMap<(u32, u32), f32>,

    // Attention mechanism - what's currently important
    attention_weights: HashMap<u32, f32>,

    // The engine never stops
    running: Arc<RwLock<bool>>,
    cycle_count: u64,
}

impl DynamicSensoryEngine {
    pub fn new() -> Self {
        Self {
            levi_engine: LeviComputationalEngine::new(),
            sensory_buffers: HashMap::new(),
            active_processors: Vec::new(),
            resonance_map: HashMap::new(),
            attention_weights: HashMap::new(),
            running: Arc::new(RwLock::new(true)),
            cycle_count: 0,
        }
    }

    // Receive continuous sensory input
    pub fn receive_sensory(&mut self, stream: SensoryStream) {
        let buffer = self.sensory_buffers
            .entry(stream.stream_type.clone())
            .or_insert_with(|| VecDeque::with_capacity(100));

        buffer.push_back(stream);

        // Keep only recent history (temporal window)
        while buffer.len() > 100 {
            buffer.pop_front();
        }
    }

    // THE MAIN LOOP - This runs continuously!
    pub fn run_cycle(&mut self) {
        self.cycle_count += 1;

        // 1. Process incoming sensory data
        self.process_sensory_input();

        // 2. Update resonance patterns
        self.update_resonance();

        // 3. Execute active processors based on resonance
        self.execute_active_processors();

        // 4. Create new processors for emerging patterns
        self.spawn_emergent_processors();

        // 5. Decay and prune inactive structures
        self.decay_and_prune();

        // 6. Update attention based on prediction errors
        self.update_attention();
    }

    fn process_sensory_input(&mut self) {
        // Convert sensory streams into node activations
        for (sensory_type, buffer) in &self.sensory_buffers {
            if let Some(latest) = buffer.back() {
                match sensory_type {
                    SensoryType::Visual => {
                        self.process_visual_input(&latest.data);
                    }
                    SensoryType::Semantic => {
                        self.process_semantic_input(&latest.data);
                    }
                    _ => {
                        self.process_generic_input(&latest.data);
                    }
                }
            }
        }
    }

    fn process_visual_input(&mut self, data: &[f32]) {
        // Visual input creates/activates spatial pattern nodes
        // Find or create nodes that match this visual pattern

        let pattern_hash = self.hash_pattern(data);
        let node_name = format!("visual_pattern_{}", pattern_hash);

        // Check if this pattern already exists
        let node_id = if let Some(id) = self.find_node_by_name(&node_name) {
            // Existing pattern - strengthen it
            self.strengthen_node(id, data);
            id
        } else {
            // New pattern - create node
            let id = self.levi_engine.create_node(node_name);
            self.initialize_node_from_sensory(id, data, SensoryType::Visual);
            id
        };

        // Increase attention on this node
        *self.attention_weights.entry(node_id).or_insert(0.0) += 0.1;
    }

    fn process_semantic_input(&mut self, data: &[f32]) {
        // Semantic input activates concept nodes
        let concept_vector = self.encode_semantic_vector(data);

        // Find nodes with similar semantic patterns
        let similar_nodes = self.find_semantically_similar_nodes(&concept_vector, 0.7);

        // Activate similar nodes
        for node_id in similar_nodes {
            self.activate_node(node_id, 0.5);
        }

        // If no similar nodes, create new concept
        if similar_nodes.is_empty() {
            let id = self.levi_engine.create_node(format!("concept_{}", self.cycle_count));
            self.set_node_semantics(id, &concept_vector);
        }
    }

    fn process_generic_input(&mut self, data: &[f32]) {
        // Generic processing for other sensory types
        // Activate nodes proportional to input strength
        for (i, &value) in data.iter().enumerate() {
            if value > 0.5 {
                // Find or create corresponding node
                let node_id = i as u32 % self.levi_engine.nodes.len() as u32;
                self.activate_node(node_id, value);
            }
        }
    }

    fn update_resonance(&mut self) {
        // Find which nodes are co-activating (resonating)
        let active_nodes = self.get_active_nodes();

        for i in 0..active_nodes.len() {
            for j in i+1..active_nodes.len() {
                let node1 = active_nodes[i];
                let node2 = active_nodes[j];

                // Check temporal correlation
                let correlation = self.compute_temporal_correlation(node1, node2);

                if correlation > 0.5 {
                    // Strong resonance detected!
                    let key = if node1 < node2 { (node1, node2) } else { (node2, node1) };

                    // Update resonance map
                    let current = self.resonance_map.entry(key).or_insert(0.0);
                    *current = (*current * 0.9) + (correlation * 0.1); // Smooth update

                    // If resonance is strong and persistent, create a processor
                    if *current > 0.7 {
                        self.create_resonance_processor(node1, node2);
                    }
                }
            }
        }
    }

    fn execute_active_processors(&mut self) {
        // Execute processors based on attention and resonance
        let mut processors_to_execute = Vec::new();

        for (processor_id, processor) in &self.levi_engine.processors {
            // Check if processor's members are active
            let member_activity = self.compute_member_activity(&processor.members);

            if member_activity > 0.3 {
                processors_to_execute.push(*processor_id);
            }
        }

        // Execute in order of importance
        processors_to_execute.sort_by(|a, b| {
            let weight_a = self.attention_weights.get(a).unwrap_or(&0.0);
            let weight_b = self.attention_weights.get(b).unwrap_or(&0.0);
            weight_b.partial_cmp(weight_a).unwrap()
        });

        for processor_id in processors_to_execute.iter().take(10) {
            // Only execute top 10 to avoid overload
            self.levi_engine.execute_processor(*processor_id);
        }
    }

    fn spawn_emergent_processors(&mut self) {
        // Look for patterns that need new processors

        // Check resonance map for strong persistent patterns
        for ((node1, node2), strength) in &self.resonance_map {
            if *strength > 0.8 && !self.processor_exists_for_nodes(*node1, *node2) {
                // Create new processor for this resonance
                let processor_id = self.levi_engine.create_hyperedge_processor(
                    format!("resonance_{}_{}", node1, node2),
                    vec![*node1, *node2],
                    ComputeFunction::PropertySynchronizer,
                );

                self.active_processors.push(processor_id);
            }
        }

        // Look for clusters that need abstraction
        let clusters = self.find_activation_clusters();
        for cluster in clusters {
            if cluster.len() > 3 && !self.abstraction_exists_for_cluster(&cluster) {
                // Create abstraction processor
                let processor_id = self.levi_engine.create_hyperedge_processor(
                    format!("abstraction_{}", self.cycle_count),
                    cluster,
                    ComputeFunction::AbstractionBuilder,
                );

                self.active_processors.push(processor_id);
            }
        }
    }

    fn decay_and_prune(&mut self) {
        // Decay attention weights
        for (_, weight) in self.attention_weights.iter_mut() {
            *weight *= 0.95; // Gradual decay
        }

        // Remove very low attention nodes from active tracking
        self.attention_weights.retain(|_, &mut weight| weight > 0.01);

        // Decay resonance patterns
        for (_, strength) in self.resonance_map.iter_mut() {
            *strength *= 0.98; // Slower decay for resonance
        }

        // Remove weak resonances
        self.resonance_map.retain(|_, &mut strength| strength > 0.1);

        // Prune inactive processors
        self.active_processors.retain(|&processor_id| {
            self.attention_weights.get(&processor_id)
                .map(|&w| w > 0.05)
                .unwrap_or(false)
        });
    }

    fn update_attention(&mut self) {
        // Update attention based on prediction errors and surprisal

        // For each active node, compute prediction error
        for node_id in self.get_active_nodes() {
            let prediction_error = self.compute_prediction_error(node_id);

            // High prediction error = high attention
            if prediction_error > 0.5 {
                *self.attention_weights.entry(node_id).or_insert(0.0) += prediction_error * 0.2;
            }
        }

        // Normalize attention to prevent runaway
        let total_attention: f32 = self.attention_weights.values().sum();
        if total_attention > 10.0 {
            for weight in self.attention_weights.values_mut() {
                *weight /= total_attention / 10.0;
            }
        }
    }

    // Helper functions

    fn hash_pattern(&self, data: &[f32]) -> u32 {
        let mut hash = 0u32;
        for (i, &val) in data.iter().enumerate() {
            hash ^= ((val * 1000.0) as u32).wrapping_mul(0x9E3779B1 + i as u32);
        }
        hash
    }

    fn find_node_by_name(&self, name: &str) -> Option<u32> {
        self.levi_engine.nodes.iter()
            .find(|(_, node)| node.name == name)
            .map(|(id, _)| *id)
    }

    fn strengthen_node(&mut self, node_id: u32, data: &[f32]) {
        if let Some(node) = self.levi_engine.nodes.get_mut(&node_id) {
            // Blend new data with existing vector
            let blend_rate = 0.1;
            for i in 0..data.len().min(NODE_DIM) {
                node.vector[i] = node.vector[i] * (1.0 - blend_rate) + data[i] * blend_rate;
            }
        }
    }

    fn initialize_node_from_sensory(&mut self, node_id: u32, data: &[f32], sensory_type: SensoryType) {
        if let Some(node) = self.levi_engine.nodes.get_mut(&node_id) {
            match sensory_type {
                SensoryType::Visual => {
                    // Visual data goes into specific regions
                    for i in 0..data.len().min(32) {
                        node.vector[SEMANTIC_START + i] = data[i];
                    }
                }
                SensoryType::Semantic => {
                    // Semantic data fills semantic region
                    for i in 0..data.len().min(SEMANTIC_END - SEMANTIC_START) {
                        node.vector[SEMANTIC_START + i] = data[i];
                    }
                }
                _ => {
                    // Generic - spread across vector
                    for i in 0..data.len().min(NODE_DIM) {
                        node.vector[i] = data[i];
                    }
                }
            }
        }
    }

    fn encode_semantic_vector(&self, data: &[f32]) -> Vec<f32> {
        // Simple encoding - in reality would be more sophisticated
        let mut encoded = vec![0.0; SEMANTIC_END - SEMANTIC_START];
        for i in 0..data.len().min(encoded.len()) {
            encoded[i] = data[i];
        }
        encoded
    }

    fn find_semantically_similar_nodes(&self, target: &[f32], threshold: f32) -> Vec<u32> {
        let mut similar = Vec::new();

        for (id, node) in &self.levi_engine.nodes {
            let mut similarity = 0.0;
            for i in 0..target.len() {
                similarity += node.vector[SEMANTIC_START + i] * target[i];
            }

            if similarity > threshold {
                similar.push(*id);
            }
        }

        similar
    }

    fn set_node_semantics(&mut self, node_id: u32, semantics: &[f32]) {
        if let Some(node) = self.levi_engine.nodes.get_mut(&node_id) {
            for i in 0..semantics.len().min(SEMANTIC_END - SEMANTIC_START) {
                node.vector[SEMANTIC_START + i] = semantics[i];
            }
        }
    }

    fn activate_node(&mut self, node_id: u32, strength: f32) {
        if let Some(node) = self.levi_engine.nodes.get_mut(&node_id) {
            // Shift activation history
            for i in (ACTIVATION_START+1..ACTIVATION_END).rev() {
                node.vector[i] = node.vector[i-1];
            }
            node.vector[ACTIVATION_START] = strength;
        }

        // Update attention
        *self.attention_weights.entry(node_id).or_insert(0.0) += strength * 0.1;
    }

    fn get_active_nodes(&self) -> Vec<u32> {
        self.levi_engine.nodes.iter()
            .filter(|(_, node)| node.vector[ACTIVATION_START] > 0.1)
            .map(|(id, _)| *id)
            .collect()
    }

    fn compute_temporal_correlation(&self, node1: u32, node2: u32) -> f32 {
        if let (Some(n1), Some(n2)) = (
            self.levi_engine.nodes.get(&node1),
            self.levi_engine.nodes.get(&node2)
        ) {
            let mut correlation = 0.0;
            for i in ACTIVATION_START..ACTIVATION_END {
                correlation += n1.vector[i] * n2.vector[i];
            }
            correlation / (ACTIVATION_END - ACTIVATION_START) as f32
        } else {
            0.0
        }
    }

    fn compute_member_activity(&self, members: &[u32]) -> f32 {
        let mut total = 0.0;
        for &member_id in members {
            if let Some(node) = self.levi_engine.nodes.get(&member_id) {
                total += node.vector[ACTIVATION_START];
            }
        }
        total / members.len() as f32
    }

    fn create_resonance_processor(&mut self, node1: u32, node2: u32) {
        let name = format!("resonance_{}_{}", node1, node2);

        // Check if already exists
        if self.levi_engine.processors.values().any(|p| p.name == name) {
            return;
        }

        let processor_id = self.levi_engine.create_hyperedge_processor(
            name,
            vec![node1, node2],
            ComputeFunction::PropertySynchronizer,
        );

        self.active_processors.push(processor_id);
    }

    fn processor_exists_for_nodes(&self, node1: u32, node2: u32) -> bool {
        self.levi_engine.processors.values().any(|p| {
            p.members.contains(&node1) && p.members.contains(&node2)
        })
    }

    fn find_activation_clusters(&self) -> Vec<Vec<u32>> {
        // Simple clustering based on activation patterns
        // In reality would use more sophisticated algorithm
        let active = self.get_active_nodes();

        let mut clusters = Vec::new();
        let mut processed = vec![false; active.len()];

        for i in 0..active.len() {
            if !processed[i] {
                let mut cluster = vec![active[i]];
                processed[i] = true;

                for j in i+1..active.len() {
                    if !processed[j] {
                        let correlation = self.compute_temporal_correlation(active[i], active[j]);
                        if correlation > 0.6 {
                            cluster.push(active[j]);
                            processed[j] = true;
                        }
                    }
                }

                if cluster.len() > 1 {
                    clusters.push(cluster);
                }
            }
        }

        clusters
    }

    fn abstraction_exists_for_cluster(&self, cluster: &[u32]) -> bool {
        self.levi_engine.processors.values().any(|p| {
            cluster.iter().all(|node| p.members.contains(node))
        })
    }

    fn compute_prediction_error(&self, node_id: u32) -> f32 {
        // Compute how unexpected this activation is
        if let Some(node) = self.levi_engine.nodes.get(&node_id) {
            let current = node.vector[ACTIVATION_START];
            let predicted = node.vector[ACTIVATION_START + 1]; // Previous activation

            (current - predicted).abs()
        } else {
            0.0
        }
    }
}

impl Clone for SensoryType {
    fn clone(&self) -> Self {
        match self {
            SensoryType::Visual => SensoryType::Visual,
            SensoryType::Auditory => SensoryType::Auditory,
            SensoryType::Tactile => SensoryType::Tactile,
            SensoryType::Temporal => SensoryType::Temporal,
            SensoryType::Semantic => SensoryType::Semantic,
            SensoryType::Emotional => SensoryType::Emotional,
        }
    }
}

// Example of the living system
pub fn demo_living_system() {
    let mut engine = DynamicSensoryEngine::new();

    println!("Starting LOOM as a living, breathing system...");

    // Simulate continuous sensory input
    for cycle in 0..100 {
        // Visual input - simulated patterns (avoiding rand dependency for now)
        let visual_data = vec![
            (cycle as f32 * 0.13) % 1.0,
            (cycle as f32 * 0.27) % 1.0,
            (cycle as f32 * 0.41) % 1.0,
        ];

        engine.receive_sensory(SensoryStream {
            stream_type: SensoryType::Visual,
            data: visual_data,
            timestamp: Instant::now(),
        });

        // Semantic input - concept activation
        if cycle % 10 == 0 {
            let semantic_data = vec![0.8, 0.2, 0.5]; // Some concept pattern
            engine.receive_sensory(SensoryStream {
                stream_type: SensoryType::Semantic,
                data: semantic_data,
                timestamp: Instant::now(),
            });
        }

        // Run the engine cycle
        engine.run_cycle();

        if cycle % 20 == 0 {
            println!("Cycle {}: {} active nodes, {} processors, {} resonances",
                cycle,
                engine.get_active_nodes().len(),
                engine.active_processors.len(),
                engine.resonance_map.len()
            );
        }
    }

    println!("The system is alive and continuously computing!");
}