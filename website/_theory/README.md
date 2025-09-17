---
title: Theory Documentation
order: 5
---

# LOOM Theory Documentation

## Mathematical Foundations

### Core Concepts

1. **[The Levi Transform](./levi-transform.md)** - How hypergraphs become computational engines
2. **[Vector Space Semantics](./vector-semantics.md)** - 256-dimensional embeddings and their regions
3. **[Update Rules](./update-rules.md)** - Mathematical formulations for processor computations
4. **[Convergence Properties](./convergence.md)** - Stability and boundedness guarantees

### Key Insight

The fundamental innovation of LOOM is that **hyperedges are processors**, not connections. Via the Levi transform, every hyperedge becomes a computational node that actively transforms its member vectors.

```
Traditional Graph:  A ──edge──> B  (edge is passive data)
LOOM via Levi:     A ──> [Processor] ──> B  (actively computes!)
```

### Mathematical Framework

The system evolves according to deterministic update rules:

- **Nodes**: `x ∈ R^256` - conceptual entities with embeddings
- **Processors**: `w ∈ R^256` - computational units (hyperedges in Levi form)
- **Updates**: Type-specific message passing with damping

#### Example: IS-A Inheritance

```
Child update: x_c' = (1−λ_v) x_c + λ_v LN(x_c + A x_parent)
Processor update: w' = (1−λ_e) w + λ_e LN(w + U[x_child, x_parent])
```

Where:
- `λ_v, λ_e ∈ (0,1]` - damping factors
- `LN` - layer normalization
- `A, U` - transformation matrices

### Semantic Contract

The [Semantic Contract](../../wiki/semantic-contract.md) defines the core semantics independent of syntax:

1. **Data Model**: Nodes, Processors, Incidence structure
2. **Update Schedule**: Deterministic execution order
3. **Invariants**: Boundedness, monotonicity, idempotence
4. **Processor Types**: IS-A, PART-OF, CORRELATES

### Intermediate Representation

The [IR Specification](../../wiki/intermediate-representation.md) provides a syntax-agnostic format:

```json
{
  "dim": 256,
  "nodes": [
    { "id": "apple", "vec": "random" },
    { "id": "fruit", "vec": "random" }
  ],
  "processors": [
    {
      "id": "is_a_1",
      "type": "is-a",
      "members": ["apple", "fruit"],
      "params": {"lambda_v": 0.1, "lambda_e": 0.1}
    }
  ],
  "schedule": {
    "order": ["is-a", "part-of", "correlates"],
    "steps": 10,
    "epsilon": 1e-4
  }
}
```

## Implementation Status

See [Honest Status](../../HONEST_STATUS.md) for current implementation progress (~5% complete).

### Working Components

- ✅ Mathematically correct update rules
- ✅ Levi transform conceptual implementation
- ✅ 256D vector operations
- ✅ Basic processor types (IS-A, PART-OF, CORRELATES)
- ✅ Convergence checking

### Not Yet Implemented

- ❌ Full parser for LOOM syntax
- ❌ GPU acceleration
- ❌ Distributed computation
- ❌ Visual debugger
- ❌ Large-scale benchmarks

## Further Reading

- [Complete Theory PDF](../../loom_complete_documentation.pdf) - Original theoretical framework
- [Wiki](../../wiki/) - Practical guides and examples
- [Examples](../../examples/) - LOOM program samples
