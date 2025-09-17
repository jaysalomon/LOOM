---
title: Key Insight
---

# Key Insight: Hyperedges Are Processors (Levi Transform)

In traditional graph representations, edges are passive pointers or weights. In LOOM, every hyperedge is promoted to a
processor node in the Levi graph that actively computes over its member nodes.

```
Traditional:  Node A --edge--> Node B     (edge is passive)
LOOM (Levi):  Node A ---> [Hyperedge Processor] ---> Node B
                           (actively computes)
```

What a hyperedge processor does:

1) Has its own 256D vector (it’s a node in the Levi graph)
2) Actively processes its member nodes’ vectors
3) Extracts patterns, enforces relationships, builds abstractions
4) Back‑propagates changes to member vectors (consistency maintenance)

## Example — Apple IS‑A Fruit

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

See also:

- [[syntax-reference.md|Syntax Reference]]
- [[operators.md|Operators]]
- [[theory-to-implementation.md|Theory → Implementation Map]]
