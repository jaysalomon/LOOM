---
title: Operators
---

# LOOM Operators

This page defines LOOM’s operators with their intended semantics and examples. Note: the current implementation is incomplete; this is the reference for builders.

## Connection Operators

- `<~>` — Bidirectional connection (undirected or symmetric pair)  
  Example: `a <~> b: 0.6`
- `~>` — Unidirectional connection (directed)  
  Example: `sensor ~> perception: 0.9`
- `<=>` — Equivalence / aliasing (two names, same identity)  
  Example: `grandma <=> grandmother`
- `^` — Abstraction / “is-a” relationship (instances to concept)  
  Example: `{sparrow, robin} ^ bird`

## Hyperedges

- `{a, b, c} ~> relation` — Higher-order relation from a set to a node  
  Example: `{subject, verb, object} ~> sentence`

## Temporal & Evolution Operators

- `~>` (in evolve) — Transform toward state/value  
  Example:
  ```loom
  evolve strengthening {
    transform: weak_connections ~> strong_connections,
    rate: learning_rate
  }
  ```
- `@>` — Sudden transition  
  Example: `state @> reboot`
- `↝` — Decay toward target  
  Example: `activation ↝ 0.0`
- `↟` — Growth toward target  
  Example: `connection ↟ 1.0`
- `⟳` — Cyclic evolution  
  Example: `phase ⟳ [0, 2π]`

## Lens / Domain Annotations

Lens markers scope relationships under a perspective. They can be read as operators with lens context.

- `¥ ... ¥` — Emotional lens  
  Example: `experience ¥resonates¥ memories`
- `€ ... €` — Logical lens  
  Example: `premise €implies€ conclusion`
- `µ ... µ` — Temporal lens  
  Example: `event1 µprecedesµ event2`

## Context Modulation

Inside `context` blocks, modulation verbs adjust lens-specific propagation:
```loom
context emotional_state {
  stress: 0.7,
  curiosity: 0.9,

  modulates {
    ¥spreading¥: amplify when stress > 0.5,
    €reasoning€: dampen when stress > 0.8
  }
}
```

## Precedence (proposed)
1. Lens annotations (tightest)
2. Hyperedge construction `{...} ~> x`
3. Connection operators `<~>`, `~>`, `<=>`, `^`
4. Evolution operators inside `evolve` blocks

## Reserved punctuation (proposed)
- Braces `{}` for hyperedge sources and blocks
- `:` for weights and key/value pairs
- `[]` for sequences and ranges (e.g., cycles)

---

If you find ambiguities, open an issue and propose a precise rule or example. This page is the contract for parser and runtime work.

