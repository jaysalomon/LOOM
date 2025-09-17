// UNIFIED TOPOLOGY - The connections ARE the vectors, not separate!
// This is the RIGHT way to think about LOOM

use std::collections::HashMap;
use memmap2::{MmapMut, MmapOptions};
use std::fs::OpenOptions;
use std::sync::Arc;
use parking_lot::RwLock;

// The 256D vector IS everything - no separate edge list!
pub const NODE_DIM: usize = 256;

// Vector regions (same as before)
pub const ID_START: usize = 0;
pub const ID_END: usize = 4;
pub const HYPERBOLIC_START: usize = 4;
pub const HYPERBOLIC_END: usize = 20;
pub const SEMANTIC_START: usize = 20;
pub const SEMANTIC_END: usize = 84;
pub const ACTIVATION_START: usize = 84;
pub const ACTIVATION_END: usize = 148;

// THIS IS KEY: Connections are IN the vector!
pub const CONNECTION_START: usize = 148;
pub const CONNECTION_END: usize = 212;

pub const EMOTIONAL_START: usize = 212;
pub const EMOTIONAL_END: usize = 244;
pub const METADATA_START: usize = 244;
pub const METADATA_END: usize = 256;

pub struct UnifiedTopology {
    // Memory-mapped file containing ALL nodes
    mmap: Arc<RwLock<MmapMut>>,

    // Index to find nodes by name (only metadata, not topology!)
    name_index: Arc<RwLock<HashMap<String, u32>>>,

    // Total nodes
    node_count: usize,
    capacity: usize,
}

impl UnifiedTopology {
    pub fn create(path: &str, capacity: usize) -> Result<Self, Box<dyn std::error::Error>> {
        let file_size = capacity * NODE_DIM * 4; // 4 bytes per f32

        let file = OpenOptions::new()
            .read(true)
            .write(true)
            .create(true)
            .open(path)?;

        file.set_len(file_size as u64)?;
        let mmap = unsafe { MmapOptions::new().map_mut(&file)? };

        Ok(Self {
            mmap: Arc::new(RwLock::new(mmap)),
            name_index: Arc::new(RwLock::new(HashMap::new())),
            node_count: 0,
            capacity,
        })
    }

    pub fn weave(&mut self, name: String) -> u32 {
        let node_id = self.node_count as u32;
        let offset = self.node_count * NODE_DIM * 4;

        // Get mutable access to memory
        let mut mmap = self.mmap.write();
        let node_memory = &mut mmap[offset..offset + NODE_DIM * 4];

        // View as f32 array
        let node = unsafe {
            std::slice::from_raw_parts_mut(
                node_memory.as_mut_ptr() as *mut f32,
                NODE_DIM
            )
        };

        // Initialize node ID
        node[ID_START] = node_id as f32;

        // Initialize in hyperbolic space (small random position)
        for i in HYPERBOLIC_START..HYPERBOLIC_END {
            node[i] = (rand::random::<f32>() - 0.5) * 0.1;
        }

        // Store name mapping
        self.name_index.write().insert(name, node_id);
        self.node_count += 1;

        node_id
    }

    // HERE'S THE KEY: Connections are encoded IN the vector!
    pub fn connect(&mut self, from_id: u32, to_id: u32, strength: f32) {
        let from_offset = (from_id as usize) * NODE_DIM * 4;

        let mut mmap = self.mmap.write();
        let from_node = unsafe {
            std::slice::from_raw_parts_mut(
                mmap[from_offset..from_offset + NODE_DIM * 4].as_mut_ptr() as *mut f32,
                NODE_DIM
            )
        };

        // THE MAGIC: Encode connection directly in the CONNECTION region
        // We use a hash function to map target ID to a position in the 64D connection space
        let connection_dims = CONNECTION_END - CONNECTION_START; // 64 dimensions

        // Method 1: Direct encoding (simple but limited)
        // Each dimension can encode connection to specific node ranges
        let slot = (to_id as usize) % connection_dims;
        from_node[CONNECTION_START + slot] += strength;

        // Method 2: Distributed encoding (more robust)
        // Spread the connection across multiple dimensions
        let hash1 = (to_id * 0x9E3779B1) as usize;
        let hash2 = (to_id * 0x517CC1B7) as usize;
        let hash3 = (to_id * 0x11111111) as usize;

        from_node[CONNECTION_START + (hash1 % connection_dims)] += strength * 0.5;
        from_node[CONNECTION_START + (hash2 % connection_dims)] += strength * 0.3;
        from_node[CONNECTION_START + (hash3 % connection_dims)] += strength * 0.2;
    }

    // Check connection strength by looking at the vector itself
    pub fn connection_strength(&self, from_id: u32, to_id: u32) -> f32 {
        let from_offset = (from_id as usize) * NODE_DIM * 4;

        let mmap = self.mmap.read();
        let from_node = unsafe {
            std::slice::from_raw_parts(
                mmap[from_offset..from_offset + NODE_DIM * 4].as_ptr() as *const f32,
                NODE_DIM
            )
        };

        // Reconstruct connection strength from distributed encoding
        let connection_dims = CONNECTION_END - CONNECTION_START;
        let hash1 = (to_id * 0x9E3779B1) as usize;
        let hash2 = (to_id * 0x517CC1B7) as usize;
        let hash3 = (to_id * 0x11111111) as usize;

        let s1 = from_node[CONNECTION_START + (hash1 % connection_dims)];
        let s2 = from_node[CONNECTION_START + (hash2 % connection_dims)];
        let s3 = from_node[CONNECTION_START + (hash3 % connection_dims)];

        // Approximate reconstruction (not perfect due to collisions)
        (s1 * 0.5 + s2 * 0.3 + s3 * 0.2) / (0.5 + 0.3 + 0.2)
    }

    // Find what a node connects to by analyzing its connection vector
    pub fn find_connections(&self, node_id: u32) -> Vec<(u32, f32)> {
        let offset = (node_id as usize) * NODE_DIM * 4;

        let mmap = self.mmap.read();
        let node = unsafe {
            std::slice::from_raw_parts(
                mmap[offset..offset + NODE_DIM * 4].as_ptr() as *const f32,
                NODE_DIM
            )
        };

        let mut connections = Vec::new();

        // This is the limitation: we need to check all possible targets
        // In practice, we'd use a more sophisticated encoding or maintain
        // a sparse index for efficiency
        for target_id in 0..self.node_count as u32 {
            if target_id != node_id {
                let strength = self.connection_strength(node_id, target_id);
                if strength > 0.01 {
                    connections.push((target_id, strength));
                }
            }
        }

        connections
    }

    // THE LEVI TRANSFORM: Create hyperedge as a node!
    pub fn create_hyperedge(&mut self, members: Vec<u32>, name: String) -> u32 {
        // In LOOM, a hyperedge IS a node (Levi transform)
        let hyperedge_id = self.weave(name);

        // The hyperedge connects to all its members
        for member_id in members {
            // Bidirectional connections
            self.connect(hyperedge_id, member_id, 0.5);
            self.connect(member_id, hyperedge_id, 0.5);
        }

        // Mark this as a hyperedge in metadata
        let offset = (hyperedge_id as usize) * NODE_DIM * 4;
        let mut mmap = self.mmap.write();
        let node = unsafe {
            std::slice::from_raw_parts_mut(
                mmap[offset..offset + NODE_DIM * 4].as_mut_ptr() as *mut f32,
                NODE_DIM
            )
        };
        node[METADATA_START] = 1.0; // Flag: this is a hyperedge

        hyperedge_id
    }

    // Propagate activation through the unified topology
    pub fn propagate(&mut self, source_id: u32, activation: f32) {
        // Get source node's connections from its vector
        let connections = self.find_connections(source_id);

        // Update activation history for each connected node
        for (target_id, strength) in connections {
            let offset = (target_id as usize) * NODE_DIM * 4;
            let mut mmap = self.mmap.write();
            let target = unsafe {
                std::slice::from_raw_parts_mut(
                    mmap[offset..offset + NODE_DIM * 4].as_mut_ptr() as *mut f32,
                    NODE_DIM
                )
            };

            // Shift activation history
            for i in (ACTIVATION_START+1..ACTIVATION_END).rev() {
                target[i] = target[i-1] * 0.95;
            }
            target[ACTIVATION_START] = activation * strength;
        }
    }

    // Evolution happens IN PLACE by modifying vectors
    pub fn evolve(&mut self) {
        // For each node, evolve based on its connections
        for node_id in 0..self.node_count as u32 {
            let connections = self.find_connections(node_id);

            // Accumulate influence from connected nodes
            let mut semantic_drift = vec![0.0f32; SEMANTIC_END - SEMANTIC_START];
            let mut hyperbolic_drift = vec![0.0f32; HYPERBOLIC_END - HYPERBOLIC_START];

            for (connected_id, strength) in connections {
                let offset = (connected_id as usize) * NODE_DIM * 4;
                let mmap = self.mmap.read();
                let connected = unsafe {
                    std::slice::from_raw_parts(
                        mmap[offset..offset + NODE_DIM * 4].as_ptr() as *const f32,
                        NODE_DIM
                    )
                };

                // Semantic influence
                for i in 0..semantic_drift.len() {
                    semantic_drift[i] += connected[SEMANTIC_START + i] * strength * 0.01;
                }

                // Hyperbolic influence
                for i in 0..hyperbolic_drift.len() {
                    hyperbolic_drift[i] += connected[HYPERBOLIC_START + i] * strength * 0.01;
                }
            }

            // Apply the accumulated drift
            let offset = (node_id as usize) * NODE_DIM * 4;
            let mut mmap = self.mmap.write();
            let node = unsafe {
                std::slice::from_raw_parts_mut(
                    mmap[offset..offset + NODE_DIM * 4].as_mut_ptr() as *mut f32,
                    NODE_DIM
                )
            };

            for i in 0..semantic_drift.len() {
                node[SEMANTIC_START + i] += semantic_drift[i];
            }

            for i in 0..hyperbolic_drift.len() {
                node[HYPERBOLIC_START + i] += hyperbolic_drift[i];
            }

            // Keep in Poincaré ball
            let mut norm_sq = 0.0;
            for i in HYPERBOLIC_START..HYPERBOLIC_END {
                norm_sq += node[i] * node[i];
            }
            if norm_sq > 0.95 {
                let scale = (0.95 / norm_sq).sqrt();
                for i in HYPERBOLIC_START..HYPERBOLIC_END {
                    node[i] *= scale;
                }
            }
        }
    }
}

// Example: How apple-fruit-food forms a hyperedge
pub fn demo_unified_topology() {
    let mut topology = UnifiedTopology::create("loom_unified.dat", 1000).unwrap();

    // Create concept nodes
    let apple_id = topology.weave("apple".to_string());
    let fruit_id = topology.weave("fruit".to_string());
    let food_id = topology.weave("food".to_string());

    // Create connections (stored IN the vectors)
    topology.connect(apple_id, fruit_id, 0.9);  // Apple strongly connected to fruit
    topology.connect(fruit_id, food_id, 0.8);   // Fruit connected to food

    // Create a hyperedge for "edible_plants"
    let hyperedge = topology.create_hyperedge(
        vec![apple_id, fruit_id, food_id],
        "edible_plants".to_string()
    );

    println!("Created hyperedge {} connecting apple, fruit, food", hyperedge);

    // The connections are IN the vectors, not separate!
    let apple_connections = topology.find_connections(apple_id);
    println!("Apple connects to {} nodes through its vector", apple_connections.len());

    // Evolve the whole topology
    topology.evolve();

    println!("After evolution, the vectors have changed based on their connections!");
}