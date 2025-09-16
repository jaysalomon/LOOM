use crate::ast::*;
use std::fmt::Write;

/// Simple C emitter for a subset of LOOM constructs.
/// This generates a standalone C file that calls into the kernel API
/// defined in `loom_kernel.h`. The generated code assumes the kernel
/// implementation is available and that the compiler include path
/// will find `kernel/src/loom_kernel.h` (e.g. compile from project root).
pub fn emit_c(program: &Program) -> String {
    let mut out = String::new();

    // Header and includes
    out.push_str("#include <stdio.h>\n");
    out.push_str("#include <stdint.h>\n");
    out.push_str("#include \"kernel/src/loom_kernel.h\"\n\n");

    out.push_str("int main(int argc, char** argv) {\n");
    out.push_str("    // Initialize topology with a reasonable default capacity\n");
    out.push_str("    LoomTopology* topo = loom_init(10000);\n");
    out.push_str("    loom_bootstrap_primordial(topo);\n\n");

    // Map of identifier -> C variable name
    let mut decls: Vec<String> = Vec::new();

    // Helper to make safe C identifier from name
    let make_c_ident = |name: &str| -> String {
        let mut s = String::from("node_");
        for c in name.chars() {
            if c.is_alphanumeric() || c == '_' {
                s.push(c);
            } else {
                s.push('_');
            }
        }
        s
    };

    // First pass: declare nodes when we see weave statements
    for stmt in &program.statements {
        if let Statement::Weave(w) = stmt {
            let var = make_c_ident(&w.name);
            let decl = format!("    uint32_t {} = loom_weave_node(topo, \"{}\");\n", var, w.name);
            decls.push(decl);
        }
    }

    for d in &decls { out.push_str(d); }
    out.push_str("\n    // Process other statements\n");

    // Second pass: connections, hyperedges, evolves (simple mapping)
    for stmt in &program.statements {
        match stmt {
            Statement::Connection(conn) => {
                // nodes -> target
                let mut parts: Vec<String> = Vec::new();
                for n in &conn.nodes {
                    parts.push(make_c_ident(n));
                }
                let participants = parts.join(", ");
                let count = conn.nodes.len();
                let c_var = make_c_ident(&conn.target);

                // Emit participants array
                write!(out, "    uint32_t parts_{}[{}] = {{{}}};\n", c_var, count, participants).ok();
                write!(out, "    uint32_t {} = loom_create_hyperedge(topo, parts_{}, {}, \"{}\");\n", c_var, c_var, count, conn.target).ok();
                write!(out, "    // Compute hyperedge processor (tensor aggregation)\n").ok();
                write!(out, "    loom_compute_hyperedge(topo, {});\n", c_var).ok();
            }
            Statement::Weave(w) => {
                // Connections for the weave are already created via declare; add explicit connects
                let var = make_c_ident(&w.name);
                for conn in &w.connections {
                    let target_var = make_c_ident(&conn.target);
                    let strength = conn.strength.unwrap_or(0.1);
                    match conn.kind {
                        ConnectionKind::Bidirectional => {
                            write!(out, "    loom_create_bidirectional(topo, {}, {}, (float){});\n", var, target_var, strength).ok();
                        }
                        ConnectionKind::Unidirectional => {
                            write!(out, "    loom_create_edge(topo, {}, {}, (float){});\n", var, target_var, strength).ok();
                        }
                        _ => {
                            // other operators are not yet implemented
                            write!(out, "    // TODO: operator {:?} not implemented for connection {} -> {}\n", conn.kind, w.name, conn.target).ok();
                        }
                    }
                }
            }
            Statement::Evolve(e) => {
                // Very simple mapping: if transformation references identifiers, call loom_evolve_toward with placeholder
                let from_var = make_c_ident(&e.transformation.from);
                let duration = if let Some(rate) = &e.rate {
                    // crude extraction: if rate.value is Number
                    match &rate.value {
                        Expression::Number(n) => match rate.duration {
                            Duration::Seconds(s) => s,
                            Duration::Milliseconds(ms) => ms / 1000.0,
                            _ => 1.0,
                        },
                        _ => 1.0,
                    }
                } else { 1.0 };

                out.push_str(&format!("    // Evolve {} -> {} (duration {})\n", e.transformation.from, e.transformation.to, duration));
                out.push_str(&format!("    NodeVector target_{} = {{0}};\n", from_var));
                out.push_str(&format!("    loom_evolve_toward(topo, {}, &target_{}, (float){});\n", from_var, from_var, duration));
            }
            _ => {
                // Not handled yet
            }
        }
    }

    out.push_str("\n    // Finalize and save topology\n");
    out.push_str("    loom_save_topology(topo, \"generated_topology.loom\");\n");
    out.push_str("    loom_destroy(topo);\n");
    out.push_str("    return 0;\n}");

    out
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn emit_simple_weave() {
        let prog = Program { statements: vec![ Statement::Weave(WeaveStatement::new("grandmother".to_string())) ] };
        let c = emit_c(&prog);
        assert!(c.contains("loom_weave_node"));
    }
}
