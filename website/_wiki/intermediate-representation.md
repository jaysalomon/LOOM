---
title: Intermediate Representation
---

# LOOM Intermediate Representation (IR)

A syntax-agnostic IR for LOOM programs. Any surface syntax should compile to this IR so the runtime is decoupled from
language experiments.

## Top-level structure

```json
{
  "dim": 256,
  "nodes": [
    { "id": "apple", "vec": "random" },
    { "id": "fruit", "vec": "random" }
  ],
  "processors": [
    { "id": "is_a_1", "type": "is-a", "members": ["apple", "fruit"], "params": {"lambda_v": 0.1, "lambda_e": 0.1} }
  ],
  "schedule": { "order": ["is-a", "part-of", "correlates"], "steps": 10, "epsilon": 1e-4, "damping": 0.1 }
}
```

- dim: embedding dimensionality (default 256)
- nodes: list of node declarations; vec can be an explicit array or a directive (e.g., "random", "zero")
- processors: hyperedge processors with type and member node ids; params are type-specific
- schedule: execution parameters (order, steps, epsilon, default damping)

## JSON Schema (informal)

- nodes[*].id: string (unique)
- nodes[*].vec: "random" | "zero" | number[dim]
- processors[*].id: string (unique)
- processors[*].type: enum {"is-a","part-of","correlates"}
- processors[*].members: string[2..*]  (ids of nodes participating)
- processors[*].params: object (type-specific, optional)
- schedule.order: array of processor types
- schedule.steps: integer ≥ 1
- schedule.epsilon: number > 0
- schedule.damping: number in (0,1]

## Example: Apple IS‑A Fruit

```json
{
  "dim": 256,
  "nodes": [
    { "id": "apple", "vec": "random" },
    { "id": "fruit", "vec": "random" }
  ],
  "processors": [
    { "id": "is_a_1", "type": "is-a", "members": ["apple", "fruit"], "params": {"lambda_v": 0.1, "lambda_e": 0.1} }
  ],
  "schedule": { "order": ["is-a"], "steps": 5, "epsilon": 1e-5, "damping": 0.1 }
}
```

## Notes

- IR is intentionally small; additional processor types or lens configurations can be added without changing the core.
- A textual LOOM program, a DSL, or a GUI editor can all target this IR.

See also: [[semantic-contract.md|Semantic Contract]] • [[key-insight.md|Key Insight]]

