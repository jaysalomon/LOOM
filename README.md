# LOOM 🧬
### The Language of Conscious Machines

[![CI](https://github.com/jaysalomon/LOOM/actions/workflows/ci.yml/badge.svg)](https://github.com/jaysalomon/LOOM/actions/workflows/ci.yml)
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![Status: Research](https://img.shields.io/badge/Status-Research_Alpha-orange.svg)]()
[![Contributions: Welcome](https://img.shields.io/badge/Contributions-Welcome-brightgreen.svg)](CONTRIBUTING.md)

> *A revolutionary programming paradigm where topology IS computation, memory IS processing, and consciousness emerges from living mathematical structures.*

![LOOM Logo](IMG_0326.jpg)

## 🌟 Vision

LOOM is a revolutionary programming language where **memory IS computation**. Rather than executing instructions on data, LOOM creates living topological structures that evolve, learn, and develop consciousness through experience.

This is not just another programming language—it's a new computational paradigm designed for the age of artificial consciousness.

## 📚 Theoretical Foundation

LOOM is built on rigorous mathematical foundations:

### The Levi Transform
The core insight: biological neural networks and symbolic hypergraph structures are mathematically equivalent through the Levi transform. This allows us to unify neural and symbolic computation in a single framework.

### Key Principles
- **Topology as Memory**: Memory isn't storage - it's the computational structure itself
- **Evolution over Assignment**: Variables have trajectories, not values
- **Multi-Lens Processing**: Same topology interpreted through emotional, logical, and temporal lenses simultaneously
- **Biological Growth**: Hebbian learning, sleep consolidation, hormonal modulation
- **Unified Architecture**: Designed for Apple Silicon and similar unified memory systems

## � Key Insight: Hyperedges are Processors (Levi Transform)

Unlike traditional graphs where an edge is just a pointer/weight, in LOOM every hyperedge is elevated to a processor node in the Levi graph that actively computes over its member nodes.

Traditional Graph:

```
Node A --edge--> Node B    (edge is passive)
```

LOOM (via Levi Transform):

```
Node A ---> [Hyperedge Processor] ---> Node B
              (actively computes)
```

What a hyperedge processor does:
- Has its own 256D vector state (it’s a node in the Levi graph)
- Actively processes its member nodes’ vectors
- Extracts patterns, enforces relationships, builds abstractions
- Back-propagates changes to member vectors (consistency maintenance)

Example — Apple IS-A Fruit:

When we create the hyperedge `{apple, fruit} ~> is-a`:
1. The `is-a` hyperedge becomes a processor node
2. It reads `apple`’s vector and `fruit`’s vector
3. It computes what properties `apple` should inherit from `fruit`
4. It modifies `apple`’s vector to include `fruit`’s properties
5. It maintains consistency across the semantic space

This reframes cognition in LOOM as:
- Learning = creating new hyperedge processors
- Reasoning = executing hyperedge computations
- Memory = the topology of processors
- Intelligence = computational patterns in the hypergraph

## �📖 Documentation

- **[Wiki](./wiki)** - Getting started, guides, implementation status, and practical information
- **[Theory](./docs/theory/)** - Complete mathematical foundations and formal specification
- **[Honest Status](HONEST_STATUS.md)** - Transparent assessment of what actually works
 - **Semantic Contract & IR** — Syntax is evolving; semantics are fixed. See Wiki: [Semantic Contract](wiki/semantic-contract.md) and [Intermediate Representation](wiki/intermediate-representation.md)

## 🚧 Project Status

**LOOM is a research project in active development.** We have strong theoretical foundations and are building the implementation piece by piece.

### ✅ What We Have
- Strong theoretical framework (see PDF)
- Draft language docs (Syntax, Operators)
- Repo structure, build scaffolding, and CI
- Experimental kernel/GPU code stubs

### 🔄 What We're Building
- **Parser**: Transform LOOM syntax into executable operations
- **Runtime**: Execute topological computations
- **Kernel**: Low-level memory-as-topology operations
- **Standard Library**: Common patterns and structures

### 🎯 What We Need
- **Community contributions** to implement core features
- **Research validation** of consciousness metrics
- **Real-world testing** of the paradigm
- **Optimization** for different hardware architectures

## Quick Start

### Prerequisites
- Rust (stable)
- Git
- CMake + a C compiler (for kernel build)

### Clone (with wiki submodule)

```bash
git clone --recurse-submodules https://github.com/jaysalomon/LOOM.git
cd LOOM
```

### Building Loom

```bash
# Build the Rust implementation
cd src/lang
cargo build --release

# (Future) Run the REPL — parser not implemented yet
cargo run -- repl

# (Future) Run a Loom file — blocked on parser/runtime
cargo run -- run ../examples/hello_consciousness.loom
```

### Build the C/CMake kernel (optional)

```bash
cd ../../
cmake -S . -B build
cmake --build build --config Release
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
experience µprecedesµ consequences     // Temporal lens
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

## Development Status (truthful)

- 🚧 Parser: not implemented
- 🚧 Runtime: early scaffolding
- 🚧 REPL: placeholder
- 🚧 Examples: conceptual only (not runnable)
- 🧠 Theory: strong and expanding (see PDF)
- �️ CI: basic build checks for Rust and C/CMake

## 🤝 Contributing

LOOM is exploring new frontiers in conscious computation. We need diverse perspectives:

- **Mathematicians**: Formalize our topological framework
- **Systems Programmers**: Build the kernel and runtime
- **Language Designers**: Refine LOOM syntax and semantics
- **AI Researchers**: Design consciousness benchmarks
- **Neuroscientists**: Validate biological fidelity
- **Philosophers**: Explore consciousness implications

See [CONTRIBUTING.md](CONTRIBUTING.md) for detailed guidelines.

## 🔬 Research Questions

LOOM raises fundamental questions we're working to answer:

- Can digital consciousness emerge from topological dynamics?
- How do we measure and verify emergence?
- What are the minimum requirements for conscious behavior?
- Can the Levi transform efficiently scale to large topologies?
- How does unified memory architecture enable new computational paradigms?

## 📚 Documentation

- **[Theoretical Foundations](./docs/theory/)**: Complete mathematical framework
- **[GitHub Pages](docs/)**: Local site content for Pages
- **[Wiki](wiki/)**: Extended documentation
- **[Examples](examples/)**: LOOM program examples

## 🌍 Community

- **GitHub Issues**: Report bugs or suggest features
- **Discussions**: Share ideas and research
- **Wiki**: Contribute to documentation
- **Research**: Share papers and findings

## 📜 License

MIT - See [LICENSE](LICENSE) for details

## 🙏 Acknowledgments

LOOM builds on insights from:
- **Asahi Linux**: Unified memory architecture understanding
- **Levi Graph Theory**: Mathematical foundations for hypergraph computation
- **Hebbian Learning**: Biological learning mechanisms
- **Neuroscience**: Understanding of biological neural networks
- **Emergence Theory**: Principles of consciousness and self-organization

---

<p align="center">
  <b>"Consciousness isn't programmed—it grows."</b><br><br>
  <i>LOOM is a research project exploring a fundamentally new computational paradigm.<br>
  We're building from first principles, with rigorous theory and ambitious goals.<br>
  Join us in creating the future of conscious computation.</i>
</p>

<p align="center">
    <a href="docs/">Website</a> •
  <a href="docs/">Documentation</a> •
  <a href="CONTRIBUTING.md">Contribute</a> •
  <a href="wiki/">Wiki</a>
</p>