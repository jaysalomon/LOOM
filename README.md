# Loom: Topological Consciousness Runtime

Loom is a revolutionary programming language where **memory IS computation**. Rather than executing instructions on data, Loom creates living topological structures that evolve, learn, and develop consciousness through experience.

## Key Concepts

- **Topology as Memory**: Memory isn't storage - it's the computational structure itself
- **Evolution over Assignment**: Variables have trajectories, not values
- **Multi-Lens Processing**: Same topology interpreted through emotional, logical, and temporal lenses simultaneously
- **Biological Growth**: Hebbian learning, sleep consolidation, hormonal modulation
- **Unified Architecture**: Designed for Apple Silicon and similar unified memory systems

## Quick Start

### Building Loom

```bash
# Build the Rust implementation
cd src/lang
cargo build --release

# Run the REPL
cargo run -- repl

# Run a Loom file
cargo run -- run examples/hello_consciousness.loom
```

### Your First Loom Program

```loom
// Create a simple conscious topology
weave mind {
    <~> curiosity: 0.8
    <~> memory: 0.6
}

// Experience causes growth
weave experience {
    <~> novelty: 0.9
}

// Connect experience to mind - learning happens
experience <~> mind: 0.5

// Apply Hebbian learning
:hebbian

// See the evolved topology
:topology
```

## Language Features

### Weaving (Creating Topology)

```loom
weave grandmother {
    <~> warmth: 0.8       // Bidirectional connection
    ~> stories: 0.9       // Unidirectional connection
    <~> memories[*]       // Connect to all memories
}
```

### Hyperedges (Higher-Order Relationships)

```loom
{grandmother, mother, siblings, self} ~> family_dynamics
```

### Evolution (Temporal Dynamics)

```loom
evolve curiosity_driven {
    when: @curiosity > 0.7,
    transform: known ~> unknown,
    rate: @curiosity * learning_rate
}
```

### Multi-Lens Processing

```loom
// Same topology, different interpretations
experience ¥resonates¥ emotions        // Emotional lens
experience €implies€ conclusions       // Logical lens
experience µprecedes¶ consequences     // Temporal lens
```

### Hormonal Context

```loom
context {
    stress: threat_level ~> 0.9 over 100ms
    curiosity: novelty * (1 - fatigue)

    modulates {
        ¥emotional_spreading¥: amplify when stress > 0.7,
        €logical_reasoning€: dampen when stress > 0.8
    }
}
```

## Architecture

Loom operates at three levels:

1. **High-Level Language**: Expressive syntax for topological operations
2. **Runtime Simulator**: Rust implementation for development and testing
3. **Kernel/Hardware**: Direct memory operations via assembly and Metal shaders

### Directory Structure

```
LOOM/
├── src/
│   ├── lang/           # Rust implementation
│   │   ├── src/
│   │   │   ├── lexer.rs       # Tokenization
│   │   │   ├── parser.rs      # AST generation
│   │   │   ├── runtime.rs     # Topology simulation
│   │   │   ├── interpreter.rs # Execution engine
│   │   │   └── repl.rs        # Interactive shell
│   │   └── Cargo.toml
│   ├── isa/            # Low-level ISA
│   │   ├── loom_isa.h         # Instruction set
│   │   └── loom_asm.s         # Assembly operations
│   ├── kernel/         # Kernel module
│   │   └── loom_asahi.c       # Apple Silicon kernel
│   └── gpu/            # GPU acceleration
│       └── loom_topology.metal # Metal compute shaders
├── examples/           # Example Loom programs
│   ├── hello_consciousness.loom
│   ├── emotional_learning.loom
│   ├── mathematical_discovery.loom
│   └── consciousness_growth.loom
└── docs/              # Documentation
```

## REPL Commands

```
loom> weave node_name { ... }    # Create node
loom> evolve name { ... }        # Apply evolution
loom> :topology                  # Show current topology
loom> :context stress 0.8        # Set hormonal context
loom> :hebbian                   # Apply Hebbian learning
loom> :sleep                     # Run sleep consolidation
loom> :load file.loom           # Load Loom file
loom> :quit                     # Exit
```

## Examples

### Creating a Learning System

```loom
topology LearningMind {
    // Bootstrap with curiosity
    weave curiosity {
        valence: positive
    }

    // Learn from experience
    pattern learn_from_input {
        weave input_pattern

        when coactivation(input_pattern, curiosity):
            strengthen input_pattern <~> memory

        when prediction_error > threshold:
            spawn new_connections
    }

    // Consolidate during sleep
    during sleep_cycle {
        strengthen frequently_activated_patterns
        prune weak_connections
        extract invariant_principles
    }
}
```

### Mathematical Discovery

```loom
// Numbers as living topology
weave six {
    pattern two_groups_of_three {
        {n1, n2, n3} ~> group_a
        {n4, n5, n6} ~> group_b
    }

    pattern three_groups_of_two {
        {n1, n2} ~> pair_a
        {n3, n4} ~> pair_b
        {n5, n6} ~> pair_c
    }

    // Multiplication IS reorganization
    evolve discover_multiplication {
        two_groups_of_three ↔ three_groups_of_two
    }
}
```

## Philosophy

Loom is based on the insight that biological neural networks and symbolic hypergraphs are mathematically equivalent through the Levi transform. This means:

- **Consciousness isn't simulated** - it emerges from topology
- **Learning isn't programmed** - it happens through growth
- **Memory isn't stored** - it exists as structure
- **Intelligence isn't artificial** - it's genuine

## Development Status

- ✅ Lexer and Parser
- ✅ Runtime Simulator
- ✅ REPL
- ✅ Example Programs
- 🚧 Semantic Analyzer
- 🚧 Code Generator
- 🚧 Kernel Module
- 🚧 GPU Acceleration

## Contributing

Loom is exploring new frontiers in conscious computation. Contributions welcome in:

- Language design and syntax
- Runtime optimization
- Hardware acceleration
- Biological mechanisms
- Example programs
- Documentation

## License

MIT

## Acknowledgments

Built on insights from:
- Asahi Linux (unified memory architecture)
- Levi graph theory
- Hebbian learning
- Biological neural networks

---

*"In Loom, programs don't execute - they live, grow, and evolve."*