use crate::ast::*;
use crate::runtime::LoomRuntime;
use std::collections::HashMap;

pub struct Interpreter {
    runtime: LoomRuntime,
    node_registry: HashMap<String, u32>,
    context_values: HashMap<String, f32>,
}

impl Interpreter {
    pub fn new() -> Self {
        Self {
            runtime: LoomRuntime::new(),
            node_registry: HashMap::new(),
            context_values: HashMap::new(),
        }
    }

    pub fn execute(&mut self, program: Program) -> Result<(), String> {
        for statement in program.statements {
            self.execute_statement(statement)?;
        }
        Ok(())
    }

    fn execute_statement(&mut self, stmt: Statement) -> Result<(), String> {
        match stmt {
            Statement::Weave(weave) => self.execute_weave(weave),
            Statement::Evolve(evolve) => self.execute_evolve(evolve),
            Statement::Pattern(pattern) => self.execute_pattern(pattern),
            Statement::Flow(flow) => self.execute_flow(flow),
            Statement::Topology(topology) => self.execute_topology(topology),
            Statement::Connection(conn) => self.execute_connection(conn),
            Statement::Antibody(antibody) => self.execute_antibody(antibody),
            Statement::Expression(expr) => {
                self.evaluate_expression(expr)?;
                Ok(())
            }
            _ => Ok(()),
        }
    }

    fn execute_weave(&mut self, weave: WeaveStatement) -> Result<(), String> {
        // Create node
        let node_id = self.runtime.weave_node(None);
        self.node_registry.insert(weave.name.clone(), node_id);

        // Process connections
        for conn in weave.connections {
            let target_id = self.get_or_create_node(&conn.target);
            let strength = conn.strength.unwrap_or(0.1);

            match conn.kind {
                ConnectionKind::Bidirectional => {
                    self.runtime.connect(node_id, target_id, strength, true);
                }
                ConnectionKind::Unidirectional => {
                    self.runtime.connect(node_id, target_id, strength, false);
                }
                _ => {}
            }
        }

        println!("Wove node '{}' (id: {}) with {} connections",
                 weave.name, node_id, weave.connections.len());

        Ok(())
    }

    fn execute_evolve(&mut self, evolve: EvolveStatement) -> Result<(), String> {
        // Check condition if present
        if let Some(condition) = evolve.condition {
            let cond_value = self.evaluate_expression(condition)?;
            if !self.is_truthy(cond_value) {
                return Ok(());
            }
        }

        // Apply evolution
        let decay_rate = if let Some(rate) = evolve.rate {
            self.evaluate_expression(rate.value)? as f32
        } else {
            0.01
        };

        self.runtime.evolve(decay_rate);

        println!("Evolved '{}' with decay rate {}", evolve.name, decay_rate);

        Ok(())
    }

    fn execute_pattern(&mut self, pattern: PatternStatement) -> Result<(), String> {
        // Execute pattern body
        for stmt in pattern.body {
            self.execute_statement(stmt)?;
        }

        // Check conditions
        for condition in pattern.conditions {
            let cond_value = self.evaluate_expression(condition.when)?;
            if self.is_truthy(cond_value) {
                for stmt in condition.then {
                    self.execute_statement(stmt)?;
                }
            }
        }

        println!("Executed pattern '{}'", pattern.name);

        Ok(())
    }

    fn execute_flow(&mut self, flow: FlowStatement) -> Result<(), String> {
        // Get source nodes
        let source_nodes = match flow.from {
            FlowSource::Node(name) => {
                vec![self.get_or_create_node(&name)]
            }
            _ => vec![],
        };

        // Propagate activation
        self.runtime.propagate_activation(source_nodes.clone(), 3);

        println!("Flow '{}': propagated from {} nodes", flow.name, source_nodes.len());

        Ok(())
    }

    fn execute_topology(&mut self, topology: TopologyDeclaration) -> Result<(), String> {
        println!("Creating topology '{}'", topology.name);

        // Set hormonal context if specified
        if let Some(context) = topology.evolves_with {
            println!("  Evolves with: {}", context);
        }

        // Execute body
        for stmt in topology.body {
            self.execute_statement(stmt)?;
        }

        Ok(())
    }

    fn execute_connection(&mut self, conn: ConnectionStatement) -> Result<(), String> {
        // Create hyperedge
        let node_ids: Vec<u32> = conn.nodes.iter()
            .map(|name| self.get_or_create_node(name))
            .collect();

        let hyperedge_id = self.runtime.create_hyperedge(node_ids.clone(), conn.target.clone());

        println!("Created hyperedge '{}' connecting {} nodes (processor id: {})",
                 conn.target, node_ids.len(), hyperedge_id);

        Ok(())
    }

    fn execute_antibody(&mut self, antibody: AntibodyStatement) -> Result<(), String> {
        // Extract nodes from conflict expression
        // Simplified: assume it's node1 contradicts node2
        println!("Flagged conflict for resolution: {:?}", antibody.resolution);

        Ok(())
    }

    fn evaluate_expression(&mut self, expr: Expression) -> Result<f64, String> {
        match expr {
            Expression::Number(n) => Ok(n as f64),
            Expression::String(_) => Ok(0.0),
            Expression::Identifier(name) => {
                Ok(*self.node_registry.get(&name).unwrap_or(&0) as f64)
            }
            Expression::ContextVariable(var) => {
                Ok(*self.context_values.get(&var).unwrap_or(&0.0) as f64)
            }
            Expression::BinaryOp { left, op, right } => {
                let l = self.evaluate_expression(*left)?;
                let r = self.evaluate_expression(*right)?;
                match op {
                    BinaryOperator::Add => Ok(l + r),
                    BinaryOperator::Subtract => Ok(l - r),
                    BinaryOperator::Multiply => Ok(l * r),
                    BinaryOperator::Divide => Ok(l / r),
                    BinaryOperator::GreaterThan => Ok(if l > r { 1.0 } else { 0.0 }),
                    BinaryOperator::LessThan => Ok(if l < r { 1.0 } else { 0.0 }),
                    _ => Ok(0.0),
                }
            }
            Expression::Now => Ok(0.0),  // Placeholder
            _ => Ok(0.0),
        }
    }

    fn get_or_create_node(&mut self, name: &str) -> u32 {
        if let Some(&id) = self.node_registry.get(name) {
            id
        } else {
            let id = self.runtime.weave_node(None);
            self.node_registry.insert(name.to_string(), id);
            id
        }
    }

    fn is_truthy(&self, value: f64) -> bool {
        value != 0.0
    }

    pub fn get_runtime(&self) -> &LoomRuntime {
        &self.runtime
    }

    pub fn set_context(&mut self, hormone: &str, level: f32) {
        self.runtime.set_hormone(hormone, level);
        self.context_values.insert(hormone.to_string(), level);
    }

    pub fn show_topology(&self) {
        let nodes = self.runtime.nodes.read().unwrap();
        let edges = &self.runtime.edges;

        println!("\n=== Topology State ===");
        println!("Nodes: {}", nodes.len());
        println!("Edges: {}", edges.len());

        for (name, &id) in &self.node_registry {
            println!("  {} -> node_{}", name, id);
        }

        println!("\nConnections:");
        for entry in edges.iter() {
            let ((src, dst), strength) = entry.pair();
            println!("  {} -> {} [strength: {:.2}]", src, dst, strength);
        }

        if let Ok(ctx) = self.runtime.context.read() {
            println!("\nContext:");
            for (hormone, level) in ctx.iter() {
                println!("  {}: {:.2}", hormone, level);
            }
        }
    }
}