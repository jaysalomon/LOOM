// How concepts like "apple" are encoded in 256D vectors
// This shows the REAL power of LOOM - symbolic properties emerge from vector structure

use crate::vector_topology::*;
use std::collections::HashMap;

// The 64-dimensional semantic space is organized into regions
// These aren't hard boundaries but soft gradients
pub struct SemanticRegions {
    // Color space (dims 0-7)
    pub color_start: usize,
    pub color_dims: usize,

    // Organic/living (dims 8-15)
    pub organic_start: usize,
    pub organic_dims: usize,

    // Food/edible (dims 16-23)
    pub food_start: usize,
    pub food_dims: usize,

    // Shape/form (dims 24-31)
    pub shape_start: usize,
    pub shape_dims: usize,

    // Texture/material (dims 32-39)
    pub texture_start: usize,
    pub texture_dims: usize,

    // Function/purpose (dims 40-47)
    pub function_start: usize,
    pub function_dims: usize,

    // Abstract properties (dims 48-63)
    pub abstract_start: usize,
    pub abstract_dims: usize,
}

impl Default for SemanticRegions {
    fn default() -> Self {
        Self {
            color_start: 0, color_dims: 8,
            organic_start: 8, organic_dims: 8,
            food_start: 16, food_dims: 8,
            shape_start: 24, shape_dims: 8,
            texture_start: 32, texture_dims: 8,
            function_start: 40, function_dims: 8,
            abstract_start: 48, abstract_dims: 16,
        }
    }
}

pub struct ConceptEncoder {
    regions: SemanticRegions,
    // Learned basis vectors for properties
    property_vectors: HashMap<String, Vec<f32>>,
}

impl ConceptEncoder {
    pub fn new() -> Self {
        let mut encoder = Self {
            regions: SemanticRegions::default(),
            property_vectors: HashMap::new(),
        };
        encoder.initialize_basis_vectors();
        encoder
    }

    fn initialize_basis_vectors(&mut self) {
        // Color basis vectors
        self.property_vectors.insert("red".to_string(),
            self.create_color_vector(1.0, 0.0, 0.0));
        self.property_vectors.insert("green".to_string(),
            self.create_color_vector(0.0, 1.0, 0.0));
        self.property_vectors.insert("blue".to_string(),
            self.create_color_vector(0.0, 0.0, 1.0));

        // Organic/living basis
        self.property_vectors.insert("plant".to_string(),
            self.create_organic_vector(0.8, 0.2, 0.0)); // alive, growing, not-animal
        self.property_vectors.insert("fruit".to_string(),
            self.create_organic_vector(0.7, 0.6, 0.3)); // alive, reproductive, sweet

        // Food basis
        self.property_vectors.insert("food".to_string(),
            self.create_food_vector(0.9, 0.5, 0.3)); // edible, nutritious, pleasant
        self.property_vectors.insert("sweet".to_string(),
            self.create_food_vector(0.3, 0.9, 0.8)); // sugar, pleasant, energy

        // Shape basis
        self.property_vectors.insert("round".to_string(),
            self.create_shape_vector(0.9, 0.1, 0.5)); // spherical, smooth, symmetric
    }

    fn create_color_vector(&self, r: f32, g: f32, b: f32) -> Vec<f32> {
        let mut vec = vec![0.0; 64];
        // Colors are encoded in first 8 dimensions
        vec[0] = r;
        vec[1] = g;
        vec[2] = b;
        // Additional color properties (saturation, brightness, etc.)
        vec[3] = (r + g + b) / 3.0; // brightness
        vec[4] = ((r - g).abs() + (g - b).abs() + (b - r).abs()) / 2.0; // saturation
        vec
    }

    fn create_organic_vector(&self, alive: f32, growing: f32, animal: f32) -> Vec<f32> {
        let mut vec = vec![0.0; 64];
        let base = self.regions.organic_start;
        vec[base] = alive;
        vec[base + 1] = growing;
        vec[base + 2] = animal;
        vec[base + 3] = 1.0 - animal; // plant-like
        vec[base + 4] = alive * growing; // vitality
        vec
    }

    fn create_food_vector(&self, edible: f32, nutrition: f32, taste: f32) -> Vec<f32> {
        let mut vec = vec![0.0; 64];
        let base = self.regions.food_start;
        vec[base] = edible;
        vec[base + 1] = nutrition;
        vec[base + 2] = taste;
        vec[base + 3] = edible * nutrition; // food value
        vec
    }

    fn create_shape_vector(&self, round: f32, angular: f32, symmetric: f32) -> Vec<f32> {
        let mut vec = vec![0.0; 64];
        let base = self.regions.shape_start;
        vec[base] = round;
        vec[base + 1] = angular;
        vec[base + 2] = symmetric;
        vec
    }

    // Encode "apple" as a superposition of properties
    pub fn encode_apple(&self) -> [f32; 64] {
        let mut semantic = [0.0; 64];

        // Apple is a superposition of multiple properties
        let properties = vec![
            ("green", 0.6),   // Often green
            ("red", 0.4),     // Sometimes red
            ("fruit", 0.9),   // Definitely a fruit
            ("food", 0.9),    // Edible
            ("sweet", 0.7),   // Usually sweet
            ("round", 0.8),   // Roughly spherical
            ("plant", 0.8),   // From a plant
        ];

        // Combine property vectors with weights
        for (prop, weight) in properties {
            if let Some(prop_vec) = self.property_vectors.get(prop) {
                for i in 0..64 {
                    semantic[i] += prop_vec[i] * weight;
                }
            }
        }

        // Normalize to unit sphere
        let mut norm = 0.0;
        for val in semantic.iter() {
            norm += val * val;
        }
        if norm > 0.0 {
            let scale = 1.0 / norm.sqrt();
            for val in semantic.iter_mut() {
                *val *= scale;
            }
        }

        semantic
    }

    // Create a full node for "apple"
    pub fn create_apple_node(&self) -> NodeVector {
        let mut node = NodeVector::new("apple".to_string());

        // Set semantic embedding
        let semantic = self.encode_apple();
        for i in 0..64 {
            node.data[SEMANTIC_START + i] = semantic[i];
        }

        // Position in hyperbolic space based on semantic clustering
        // Fruits cluster together in one region
        node.data[HYPERBOLIC_START] = 0.3;     // Fruit region
        node.data[HYPERBOLIC_START + 1] = 0.2; // Food cluster
        node.data[HYPERBOLIC_START + 2] = -0.1; // Plant kingdom

        // Emotional valence (apples are generally positive)
        node.data[EMOTIONAL_START] = 0.7;     // Positive valence
        node.data[EMOTIONAL_START + 1] = 0.3; // Low arousal
        node.data[EMOTIONAL_START + 2] = 0.6; // Pleasant

        node
    }

    // Decode what properties a vector has
    pub fn decode_properties(&self, node: &NodeVector) -> HashMap<String, f32> {
        let mut properties = HashMap::new();
        let semantic = node.semantic_embedding();

        // Check similarity to each known property
        for (prop_name, prop_vec) in &self.property_vectors {
            let mut similarity = 0.0;
            for i in 0..64 {
                similarity += semantic[i] * prop_vec[i];
            }
            if similarity > 0.1 { // Threshold for "has property"
                properties.insert(prop_name.clone(), similarity);
            }
        }

        properties
    }

    // Find symbolic links between concepts
    pub fn find_symbolic_links(&self, node1: &NodeVector, node2: &NodeVector)
        -> Vec<(String, f32)> {

        let mut links = Vec::new();

        // Color similarity
        let color_sim = self.region_similarity(node1, node2,
            self.regions.color_start, self.regions.color_dims);
        if color_sim > 0.3 {
            links.push(("similar_color".to_string(), color_sim));
        }

        // Food relationship
        let food_sim = self.region_similarity(node1, node2,
            self.regions.food_start, self.regions.food_dims);
        if food_sim > 0.4 {
            links.push(("related_food".to_string(), food_sim));
        }

        // Organic/living relationship
        let organic_sim = self.region_similarity(node1, node2,
            self.regions.organic_start, self.regions.organic_dims);
        if organic_sim > 0.5 {
            links.push(("same_kingdom".to_string(), organic_sim));
        }

        // Check for is-a relationships (one is more general)
        let general_specific = self.check_generalization(node1, node2);
        if general_specific.0 > 0.6 {
            links.push(("is_a".to_string(), general_specific.0));
        } else if general_specific.1 > 0.6 {
            links.push(("has_instance".to_string(), general_specific.1));
        }

        links
    }

    fn region_similarity(&self, node1: &NodeVector, node2: &NodeVector,
                         start: usize, dims: usize) -> f32 {
        let sem1 = node1.semantic_embedding();
        let sem2 = node2.semantic_embedding();

        let mut similarity = 0.0;
        for i in start..start+dims {
            similarity += sem1[i] * sem2[i];
        }
        similarity / dims as f32
    }

    fn check_generalization(&self, specific: &NodeVector, general: &NodeVector)
        -> (f32, f32) {
        let spec_sem = specific.semantic_embedding();
        let gen_sem = general.semantic_embedding();

        // Check if specific has all properties of general (is-a)
        let mut is_a_score = 0.0;
        let mut has_instance_score = 0.0;
        let mut count = 0;

        for i in 0..64 {
            if gen_sem[i] > 0.1 {
                count += 1;
                if spec_sem[i] >= gen_sem[i] * 0.8 {
                    is_a_score += 1.0;
                }
            }
            if spec_sem[i] > 0.1 {
                if gen_sem[i] >= spec_sem[i] * 0.8 {
                    has_instance_score += 1.0;
                }
            }
        }

        if count > 0 {
            (is_a_score / count as f32, has_instance_score / count as f32)
        } else {
            (0.0, 0.0)
        }
    }
}

// Example: Building conceptual relationships
pub fn demo_semantic_encoding() {
    let encoder = ConceptEncoder::new();

    // Create apple
    let apple = encoder.create_apple_node();

    // Create related concepts
    let mut banana = NodeVector::new("banana".to_string());
    let banana_props = vec![
        ("yellow", 0.9),
        ("fruit", 0.9),
        ("food", 0.9),
        ("sweet", 0.8),
        ("plant", 0.8),
    ];
    // Encode banana similarly...

    // Find relationships
    let apple_props = encoder.decode_properties(&apple);
    println!("Apple properties:");
    for (prop, strength) in apple_props {
        println!("  {}: {:.2}", prop, strength);
    }

    // The symbolic links emerge from the vector structure!
    // "apple is green" because green dimensions are active
    // "apple is food" because food dimensions are active
    // "apple is fruit" because it matches the fruit pattern
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_apple_encoding() {
        let encoder = ConceptEncoder::new();
        let apple = encoder.create_apple_node();

        let properties = encoder.decode_properties(&apple);

        // Apple should have these properties
        assert!(properties.get("fruit").unwrap_or(&0.0) > &0.5);
        assert!(properties.get("food").unwrap_or(&0.0) > &0.5);
        assert!(properties.get("plant").unwrap_or(&0.0) > &0.3);
    }

    #[test]
    fn test_semantic_regions() {
        let encoder = ConceptEncoder::new();
        let apple = encoder.create_apple_node();

        // Check that semantic embedding is normalized
        let semantic = apple.semantic_embedding();
        let mut norm = 0.0;
        for val in semantic {
            norm += val * val;
        }
        assert!((norm - 1.0).abs() < 0.1); // Approximately unit length
    }
}