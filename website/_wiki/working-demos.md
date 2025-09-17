---
title: Working Demos
---

# Working Demonstrations

These demos actually run and demonstrate core LOOM concepts.

## 1. Levi Computational Engine

Location: `src/lang/src/levi_computational_engine.rs`

Demonstrates hyperedges as computational processors:

```rust
cargo run --bin loom -- hyperedge
```

### What it shows:
- Hyperedges become computational nodes via Levi transform
- Each processor has its own 256D vector
- Processors actively transform member vectors
- Different computation types (PatternExtractor, RelationshipEnforcer, etc.)

## 2. Mathematically Correct Engine

Location: `src/lang/src/mathematically_correct_engine.rs`

Implements exact update rules from semantic contract:

```rust
cargo run --bin loom -- math-demo
```

### Features:
- IS-A inheritance: `x_c' = (1−λ_v) x_c + λ_v LN(x_c + A x_parent)`
- PART-OF aggregation: Whole aggregates properties from parts
- CORRELATES coupling: Co-activated nodes attract
- Deterministic scheduling
- Convergence checking with epsilon

## 3. Dynamic Sensory Engine

Location: `src/lang/src/dynamic_sensory_engine.rs`

Living system with continuous processing:

```rust
cargo run --bin loom -- sensory
```

### Demonstrates:
- Continuous sensory input processing
- Resonance pattern detection
- Dynamic processor spawning
- Attention mechanism
- Decay and pruning

## 4. Unified Architecture Demo

Location: `src/lang/src/unified_demo.rs`

Shows how all components work together:

```rust
cargo run --bin loom -- demo
```

## Key Concepts Demonstrated

### The Levi Transform
```
Traditional:  A --edge--> B     (edge is passive)
LOOM:        A --> [Processor] --> B  (actively computes!)
```

### Update Cycle
1. Process sensory input → vectors
2. Detect resonance patterns
3. Spawn hyperedge processors
4. Execute processors → transform vectors
5. Decay and prune inactive structures

### Mathematical Correctness
- Damped updates prevent oscillation
- Layer normalization for stability
- Bounded embeddings ||x|| ≤ B
- Type-specific message passing

## Running the Demos

From the `src/lang` directory:

```bash
# Build the project
cargo build --release

# Run specific demos
cargo run -- hyperedge   # Hyperedge computation
cargo run -- math-demo   # Mathematical engine
cargo run -- demo        # Unified architecture
```

Note: Some demos require Visual Studio build tools on Windows.

## What's Actually Working

✅ **Hyperedges as processors** - The core innovation
✅ **256D vector operations** - Semantic embeddings
✅ **Message passing** - Bidirectional v↔e updates
✅ **Type-specific computations** - IS-A, PART-OF, CORRELATES
✅ **Convergence** - Damped updates with epsilon checking
✅ **Dynamic processing** - Continuous sensory input

## What's Not Working Yet

❌ **Parser** - Can't parse LOOM syntax
❌ **GPU acceleration** - Metal shaders untested
❌ **Distributed computation** - Single-threaded only
❌ **Large-scale benchmarks** - No performance testing
❌ **Visual debugger** - No topology visualization
