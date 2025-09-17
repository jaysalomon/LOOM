// SIMPLE RUNTIME - Actually executes parsed LOOM code!

use crate::parser_simple::{Program, Statement, WeaveStatement, ConnectStatement, CommandStatement, ConnectionType};
use std::collections::HashMap;
use std::fmt;

// Simple node structure
#[derive(Debug, Clone)]
pub struct Node {
    pub name: String,
    pub activation: f32,
    pub properties: HashMap<String, f32>,
}

// Simple edge structure
#[derive(Debug, Clone)]
pub struct Edge {
    pub from: String,
    pub to: String,
    pub weight: f32,
    pub bidirectional: bool,
}

// The actual runtime that executes LOOM programs
pub struct SimpleRuntime {
    pub nodes: HashMap<String, Node>,
    pub edges: Vec<Edge>,
    pub context: HashMap<String, f32>,
}

impl SimpleRuntime {
    pub fn new() -> Self {
        Self {
            nodes: HashMap::new(),
            edges: Vec::new(),
            context: HashMap::new(),
        }
    }

    pub fn execute(&mut self, program: Program) -> Result<(), String> {
        for statement in program.statements {
            self.execute_statement(statement)?;
        }
        Ok(())
    }

    pub fn execute_statement(&mut self, statement: Statement) -> Result<(), String> {
        match statement {
            Statement::Weave(weave) => self.execute_weave(weave),
            Statement::Connect(connect) => self.execute_connect(connect),
            Statement::Command(command) => self.execute_command(command),
        }
    }

    fn execute_weave(&mut self, weave: WeaveStatement) -> Result<(), String> {
        println!("Creating node: {}", weave.name);

        // Create the node
        let mut node = Node {
            name: weave.name.clone(),
            activation: 0.0,
            properties: weave.properties.clone(),
        };

        // Store the node
        self.nodes.insert(weave.name.clone(), node);

        // Create connections
        for conn in weave.connections {
            // Ensure target node exists
            if !self.nodes.contains_key(&conn.target) {
                self.nodes.insert(conn.target.clone(), Node {
                    name: conn.target.clone(),
                    activation: 0.0,
                    properties: HashMap::new(),
                });
            }

            // Create edge based on connection type
            match conn.connection_type {
                ConnectionType::Bidirectional => {
                    self.edges.push(Edge {
                        from: weave.name.clone(),
                        to: conn.target.clone(),
                        weight: conn.weight,
                        bidirectional: true,
                    });
                    println!("  <~> {} (weight: {})", conn.target, conn.weight);
                }
                ConnectionType::Unidirectional => {
                    self.edges.push(Edge {
                        from: weave.name.clone(),
                        to: conn.target.clone(),
                        weight: conn.weight,
                        bidirectional: false,
                    });
                    println!("  ~> {} (weight: {})", conn.target, conn.weight);
                }
                ConnectionType::Equivalence => {
                    // Equivalence is like bidirectional with weight 1.0
                    self.edges.push(Edge {
                        from: weave.name.clone(),
                        to: conn.target.clone(),
                        weight: 1.0,
                        bidirectional: true,
                    });
                    println!("  <=> {}", conn.target);
                }
            }
        }

        Ok(())
    }

    fn execute_connect(&mut self, connect: ConnectStatement) -> Result<(), String> {
        // Ensure both nodes exist
        if !self.nodes.contains_key(&connect.from) {
            self.nodes.insert(connect.from.clone(), Node {
                name: connect.from.clone(),
                activation: 0.0,
                properties: HashMap::new(),
            });
        }
        if !self.nodes.contains_key(&connect.to) {
            self.nodes.insert(connect.to.clone(), Node {
                name: connect.to.clone(),
                activation: 0.0,
                properties: HashMap::new(),
            });
        }

        // Create edge
        let bidirectional = matches!(connect.connection_type,
            ConnectionType::Bidirectional | ConnectionType::Equivalence);

        self.edges.push(Edge {
            from: connect.from.clone(),
            to: connect.to.clone(),
            weight: connect.weight,
            bidirectional,
        });

        println!("Connected {} to {} (weight: {})", connect.from, connect.to, connect.weight);
        Ok(())
    }

    fn execute_command(&mut self, command: CommandStatement) -> Result<(), String> {
        match command {
            CommandStatement::ShowTopology => {
                self.show_topology();
                Ok(())
            }
            CommandStatement::ApplyHebbian => {
                self.apply_hebbian();
                Ok(())
            }
            CommandStatement::Sleep => {
                self.apply_sleep();
                Ok(())
            }
        }
    }

    pub fn show_topology(&self) {
        println!("\n=== Current Topology ===");
        println!("Nodes: {}", self.nodes.len());
        for (name, node) in &self.nodes {
            println!("  {} (activation: {:.2})", name, node.activation);
            if !node.properties.is_empty() {
                for (prop, val) in &node.properties {
                    println!("    {}: {:.2}", prop, val);
                }
            }
        }

        println!("\nEdges: {}", self.edges.len());
        for edge in &self.edges {
            let arrow = if edge.bidirectional { "<~>" } else { "~>" };
            println!("  {} {} {} (weight: {:.2})",
                edge.from, arrow, edge.to, edge.weight);
        }
        println!("======================\n");
    }

    pub fn apply_hebbian(&mut self) {
        println!("Applying Hebbian learning...");

        // Simple Hebbian: strengthen connections between co-activated nodes
        let mut updates = Vec::new();

        for edge in &self.edges {
            let from_activation = self.nodes.get(&edge.from)
                .map(|n| n.activation).unwrap_or(0.0);
            let to_activation = self.nodes.get(&edge.to)
                .map(|n| n.activation).unwrap_or(0.0);

            // If both nodes are active, strengthen connection
            if from_activation > 0.1 && to_activation > 0.1 {
                let correlation = from_activation * to_activation;
                let delta = 0.1 * correlation; // Learning rate of 0.1
                updates.push((edge.from.clone(), edge.to.clone(), delta));
            }
        }

        // Apply updates
        for (from, to, delta) in updates {
            for edge in &mut self.edges {
                if edge.from == from && edge.to == to {
                    edge.weight += delta;
                    println!("  Strengthened {} -> {} by {:.3}", from, to, delta);
                }
            }
        }
    }

    pub fn apply_sleep(&mut self) {
        println!("Applying sleep consolidation...");

        // Simple sleep: prune weak connections, strengthen strong ones
        self.edges.retain(|edge| {
            if edge.weight < 0.01 {
                println!("  Pruned weak connection: {} -> {}", edge.from, edge.to);
                false
            } else {
                true
            }
        });

        // Strengthen remaining connections slightly
        for edge in &mut self.edges {
            if edge.weight > 0.5 {
                edge.weight *= 1.05;
                println!("  Consolidated {} -> {} to {:.3}", edge.from, edge.to, edge.weight);
            }
        }
    }

    pub fn propagate_activation(&mut self, source: &str, activation: f32) {
        println!("Propagating activation from {} (strength: {})", source, activation);

        if let Some(node) = self.nodes.get_mut(source) {
            node.activation = activation;
        }

        // Simple spreading activation
        let mut activations = HashMap::new();
        activations.insert(source.to_string(), activation);

        // One step of propagation
        for edge in &self.edges {
            if let Some(&source_activation) = activations.get(&edge.from) {
                let spread = source_activation * edge.weight;
                let entry = activations.entry(edge.to.clone()).or_insert(0.0);
                *entry += spread;
                println!("  {} -> {} (spread: {:.3})", edge.from, edge.to, spread);
            }

            // Handle bidirectional edges
            if edge.bidirectional {
                if let Some(&target_activation) = activations.get(&edge.to) {
                    let spread = target_activation * edge.weight;
                    let entry = activations.entry(edge.from.clone()).or_insert(0.0);
                    *entry += spread;
                    println!("  {} <- {} (spread: {:.3})", edge.from, edge.to, spread);
                }
            }
        }

        // Update node activations
        for (name, activation) in activations {
            if let Some(node) = self.nodes.get_mut(&name) {
                node.activation = activation.min(1.0); // Cap at 1.0
            }
        }
    }
}

impl fmt::Display for SimpleRuntime {
    fn fmt(&self, f: &mut fmt::Formatter) -> fmt::Result {
        write!(f, "Runtime: {} nodes, {} edges", self.nodes.len(), self.edges.len())
    }
}