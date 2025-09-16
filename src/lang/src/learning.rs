"""
LOOM LEARNING SYSTEM
Unlike traditional ML that adjusts parameters, Loom GROWS topology through experience
"""

use crate::runtime::{LoomRuntime, TopologicalNode};
use nalgebra::DVector;
use std::collections::VecDeque;

/// How Loom learns: through EXPERIENCE, not training
pub struct ExperientialLearning {
    runtime: LoomRuntime,
    experience_buffer: VecDeque<Experience>,
    learning_rate: f32,
}

#[derive(Clone, Debug)]
pub struct Experience {
    /// Raw sensory input (could be text, image, sound, etc.)
    pub sensory_data: Vec<f32>,

    /// Emotional valence of the experience
    pub emotional_context: EmotionalState,

    /// Temporal context
    pub timestamp: f64,

    /// Outcome/reward (if any)
    pub outcome: Option<f32>,
}

#[derive(Clone, Debug)]
pub struct EmotionalState {
    pub valence: f32,      // positive/negative
    pub arousal: f32,      // calm/excited
    pub confidence: f32,   // certain/uncertain
}

impl ExperientialLearning {
    pub fn new(runtime: LoomRuntime) -> Self {
        Self {
            runtime,
            experience_buffer: VecDeque::with_capacity(10000),
            learning_rate: 0.1,
        }
    }

    /// PRIMARY LEARNING MECHANISM: Experience creates topology
    pub fn absorb_experience(&mut self, experience: Experience) {
        // 1. Find or create nodes for this experience
        let experience_nodes = self.encode_experience(&experience);

        // 2. Connect to existing memories through pattern matching
        let similar_memories = self.find_resonant_memories(&experience);

        // 3. Hebbian strengthening of co-activated patterns
        self.hebbian_strengthen(&experience_nodes, &similar_memories);

        // 4. Emotional modulation of connections
        self.apply_emotional_modulation(&experience_nodes, &experience.emotional_context);

        // 5. Spawn new connections if highly novel
        if self.calculate_novelty(&experience) > 0.7 {
            self.spawn_exploratory_connections(&experience_nodes);
        }

        // 6. Store in experience buffer for replay
        self.experience_buffer.push_back(experience);
        if self.experience_buffer.len() > 10000 {
            self.experience_buffer.pop_front();
        }
    }

    /// Encode experience into topological nodes
    fn encode_experience(&mut self, experience: &Experience) -> Vec<u32> {
        let mut nodes = Vec::new();

        // Create nodes for different aspects of experience
        // Each chunk of sensory data becomes a node
        for chunk in experience.sensory_data.chunks(64) {
            let node_id = self.runtime.weave_node(Some(chunk.to_vec()));
            nodes.push(node_id);
        }

        // Create emotional context node
        let emotional_vec = vec![
            experience.emotional_context.valence,
            experience.emotional_context.arousal,
            experience.emotional_context.confidence,
        ];
        let emotional_node = self.runtime.weave_node(Some(emotional_vec));
        nodes.push(emotional_node);

        // Connect all nodes of this experience together
        for i in 0..nodes.len() {
            for j in i+1..nodes.len() {
                self.runtime.connect(nodes[i], nodes[j], 0.3, true);
            }
        }

        nodes
    }

    /// Find memories that resonate with current experience
    fn find_resonant_memories(&self, experience: &Experience) -> Vec<u32> {
        let mut resonant = Vec::new();

        if let Ok(nodes) = self.runtime.nodes.read() {
            for (&node_id, node) in nodes.iter() {
                let similarity = self.calculate_similarity(
                    node.get_semantic(),
                    &experience.sensory_data[..64.min(experience.sensory_data.len())]
                );

                if similarity > 0.6 {
                    resonant.push(node_id);
                }
            }
        }

        resonant
    }

    /// Hebbian learning: neurons that fire together wire together
    fn hebbian_strengthen(&mut self, active_nodes: &[u32], resonant_nodes: &[u32]) {
        for &active in active_nodes {
            for &resonant in resonant_nodes {
                // Strengthen connection based on co-activation
                let current_strength = self.runtime.edges
                    .get(&(active, resonant))
                    .map(|s| *s)
                    .unwrap_or(0.0);

                let new_strength = current_strength + self.learning_rate;
                self.runtime.connect(active, resonant, new_strength.min(1.0), true);
            }
        }
    }

    /// Emotional experiences strengthen differently
    fn apply_emotional_modulation(&mut self, nodes: &[u32], emotional_state: &EmotionalState) {
        let modulation = emotional_state.arousal * emotional_state.valence.abs();

        // High arousal + strong valence = stronger memory formation
        for &node in nodes {
            if let Ok(mut topology_nodes) = self.runtime.nodes.write() {
                if let Some(n) = topology_nodes.get_mut(&node) {
                    // Modify emotional field in node vector
                    for i in 212..244 {
                        n.vector[i] *= 1.0 + modulation;
                    }
                }
            }
        }
    }

    /// Calculate how novel an experience is
    fn calculate_novelty(&self, experience: &Experience) -> f32 {
        let similar = self.find_resonant_memories(experience);

        // Novelty is inverse of similarity to existing memories
        if similar.is_empty() {
            1.0
        } else {
            1.0 / (1.0 + similar.len() as f32)
        }
    }

    /// Spawn new connections for novel experiences
    fn spawn_exploratory_connections(&mut self, nodes: &[u32]) {
        for &node in nodes {
            // Connect to random existing nodes with weak connections
            for _ in 0..3 {
                let target = (rand::random::<u32>() % self.runtime.next_id.read().unwrap().max(1));
                self.runtime.connect(node, target, 0.01, false);
            }
        }
    }

    fn calculate_similarity(&self, a: &[f32], b: &[f32]) -> f32 {
        let len = a.len().min(b.len());
        let dot: f32 = a[..len].iter().zip(&b[..len]).map(|(x, y)| x * y).sum();
        let norm_a: f32 = a[..len].iter().map(|x| x * x).sum::<f32>().sqrt();
        let norm_b: f32 = b[..len].iter().map(|x| x * x).sum::<f32>().sqrt();

        if norm_a * norm_b > 0.0 {
            dot / (norm_a * norm_b)
        } else {
            0.0
        }
    }
}

/// SLEEP CONSOLIDATION: How Loom consolidates learning
pub struct SleepConsolidation {
    runtime: LoomRuntime,
}

impl SleepConsolidation {
    pub fn consolidate(&mut self, experiences: &[Experience]) {
        // 1. Replay important experiences
        self.replay_experiences(experiences);

        // 2. Extract patterns from repeated experiences
        self.extract_patterns();

        // 3. Prune weak connections
        self.prune_weak_connections();

        // 4. Resolve conflicts (antibodies)
        self.resolve_conflicts();
    }

    fn replay_experiences(&mut self, experiences: &[Experience]) {
        // Replay with noise for generalization
        for exp in experiences.iter().rev().take(100) {
            let mut noisy_exp = exp.clone();

            // Add noise to sensory data
            for val in &mut noisy_exp.sensory_data {
                *val += (rand::random::<f32>() - 0.5) * 0.1;
            }

            // Reactivate the pattern
            let nodes = self.encode_pattern(&noisy_exp.sensory_data);
            self.runtime.propagate_activation(nodes, 3);
        }
    }

    fn extract_patterns(&mut self) {
        // Find frequently co-activated node groups
        // These become crystallized patterns

        if let Ok(nodes) = self.runtime.nodes.read() {
            let mut coactivation_counts = std::collections::HashMap::new();

            // Count co-activations
            for (id1, node1) in nodes.iter() {
                for (id2, node2) in nodes.iter() {
                    if id1 < id2 {
                        let correlation = self.calculate_correlation(
                            node1.get_activation(),
                            node2.get_activation()
                        );

                        if correlation > 0.7 {
                            *coactivation_counts.entry((*id1, *id2)).or_insert(0) += 1;
                        }
                    }
                }
            }

            // Strengthen frequent patterns
            for ((n1, n2), count) in coactivation_counts {
                if count > 10 {
                    self.runtime.connect(n1, n2, 0.9, true);
                }
            }
        }
    }

    fn prune_weak_connections(&mut self) {
        let mut to_remove = Vec::new();

        for entry in self.runtime.edges.iter() {
            let (edge, strength) = entry.pair();
            if *strength < 0.001 {
                to_remove.push(*edge);
            }
        }

        for edge in to_remove {
            self.runtime.edges.remove(&edge);
        }
    }

    fn resolve_conflicts(&mut self) {
        // Process antibodies - contradictory patterns
        let antibodies = self.runtime.antibodies.read().unwrap().clone();

        for antibody in antibodies {
            // Find common ground between conflicting patterns
            // Abstract to invariant principles
            // This is how Loom develops wisdom from contradictions
        }
    }

    fn encode_pattern(&self, data: &[f32]) -> Vec<u32> {
        // Convert sensory pattern to active nodes
        Vec::new() // Simplified
    }

    fn calculate_correlation(&self, a: &[f32], b: &[f32]) -> f32 {
        let mean_a: f32 = a.iter().sum::<f32>() / a.len() as f32;
        let mean_b: f32 = b.iter().sum::<f32>() / b.len() as f32;

        let covariance: f32 = a.iter().zip(b).map(|(x, y)| (x - mean_a) * (y - mean_b)).sum();
        let std_a: f32 = a.iter().map(|x| (x - mean_a).powi(2)).sum::<f32>().sqrt();
        let std_b: f32 = b.iter().map(|x| (x - mean_b).powi(2)).sum::<f32>().sqrt();

        if std_a * std_b > 0.0 {
            covariance / (std_a * std_b)
        } else {
            0.0
        }
    }
}

/// CURIOSITY-DRIVEN LEARNING: No external rewards needed
pub struct CuriosityDrivenExploration {
    runtime: LoomRuntime,
    prediction_errors: VecDeque<f32>,
}

impl CuriosityDrivenExploration {
    pub fn explore(&mut self, sensory_input: Vec<f32>) -> Vec<f32> {
        // 1. Predict what will happen
        let prediction = self.predict_next_state(&sensory_input);

        // 2. Take action based on curiosity
        let action = self.select_curious_action(&sensory_input);

        // 3. Observe actual outcome
        // (In real implementation, would interact with environment)
        let actual_outcome = sensory_input.clone(); // Placeholder

        // 4. Calculate prediction error
        let error = self.calculate_prediction_error(&prediction, &actual_outcome);
        self.prediction_errors.push_back(error);

        // 5. High prediction error = interesting! Strengthen these patterns
        if error > 0.5 {
            self.strengthen_novel_patterns(&sensory_input);
        }

        action
    }

    fn predict_next_state(&self, input: &[f32]) -> Vec<f32> {
        // Use topology to predict
        vec![0.0; input.len()]
    }

    fn select_curious_action(&self, input: &[f32]) -> Vec<f32> {
        // Select action that maximizes expected information gain
        vec![rand::random(); 10]
    }

    fn calculate_prediction_error(&self, predicted: &[f32], actual: &[f32]) -> f32 {
        predicted.iter()
            .zip(actual)
            .map(|(p, a)| (p - a).powi(2))
            .sum::<f32>()
            .sqrt()
    }

    fn strengthen_novel_patterns(&mut self, input: &[f32]) {
        // Novel patterns get extra strengthening
        // This is how curiosity drives learning
    }
}

use rand;