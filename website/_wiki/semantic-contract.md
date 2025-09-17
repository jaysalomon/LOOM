---
title: Semantic Contract
---

# LOOM Semantic Contract (Syntax-Agnostic)

This document specifies the core semantics of LOOM independent of surface syntax. It defines the data model, update
schedule, invariants, and initial processor types so multiple syntaxes can target the same runtime correctly.

## Data Model

- Nodes V: conceptual entities, each with an embedding vector x ∈ R^d (default d=256)
- Processors (Hyperedges) E: relation/computation units, each with an embedding w ∈ R^d
- Incidence (Levi) structure: bipartite connections between V and E (A_VE, A_EV)
- Types: each processor e ∈ E has a type t(e) ∈ {is-a, part-of, correlates, ...} with parameters θ_e
- Lenses: optional projections P_k: R^d → R^{d_k} with dynamics f_k
- Context: scalar or vector signals influencing weights (damping, learning rate, lens α)

## Update Schedule (single step)

Deterministic order per iteration ensures reproducibility.

1) Optional lens projections: X_k = P_k(X)
2) Processor message pass (typed):
   - v-to-e: M_ve = A_EV X (with type-specific transforms)
   - e update: W' = W + η g_t(M_ve, W, θ)
   - e-to-v: M_ev = A_VE W (type-specific transforms)
   - v update: X' = X + η f_t(M_ev, X, θ)
3) Regularization/consolidation: sparsify/prune edges; normalize embeddings
4) Stopping: iterate until ||X'−X||∞ < ε and ||W'−W||∞ < ε or steps ≥ max_steps

All operations may include damping λ ∈ (0,1] per type to prevent oscillation.

## Invariants and Safety

- Boundedness: ||x_i|| ≤ B_v and ||w_e|| ≤ B_e via normalization/clipping
- Monotonic constraints per type (when applicable): e.g., is-a inheritance should not remove essential parent properties
- Idempotence near fixed points: second step should produce Δ below ε
- Sparsity budget: edge/processors pruned if utility below τ for K steps

## Initial Processor Types (typed message functions)

Let ⊙ denote elementwise operations; LN denote layer norm; σ a smooth nonlinearity; and ⟨·,·⟩ an alignment score.

1) is-a (inheritance)
- Intent: child inherits properties of parent with damping
- e update: w' = (1−λ_e) w + λ_e LN(w + U[x_child, x_parent])
- v update (child): x_c' = (1−λ_v) x_c + λ_v LN(x_c + A x_parent)
- Params: λ_v, λ_e ∈ (0,1]; A,U learned or fixed small matrices

2) part-of (aggregation)
- Intent: whole aggregates parts; parts align with whole
- e update: w' = (1−λ_e) w + λ_e mean({x_part})
- v update: x_whole' = (1−λ_v) x_whole + λ_v LN(x_whole + B·mean({x_part}))
- Params: λ_v, λ_e; optional weights per part

3) correlates-with (coupling)
- Intent: co-activated nodes attract in subspace
- e update: w' = w + η σ(C[x_a, x_b])
- v update: x_a' = x_a + η D w,  x_b' = x_b + η D w
- Params: η, subspace projector D

## Scheduling Notes

- Type order: process (is-a → part-of → correlates) for stability (inherit → compose → correlate)
- Damping: default λ_v=0.1, λ_e=0.1; global η=0.05; adjust per dataset
- Consolidation runs every N steps (e.g., N=10), pruning low-utility processors and renormalizing embeddings

## Contract for Implementations

- Given (V,E,A,X,W,Types,θ,Schedule), any conforming runtime must produce equivalent trajectories up to measure-zero
  differences from numerical precision and normalization choices.
- Surface syntax MUST compile/transpile to this structure; adding syntax cannot change these core meanings.

---

See also: [[key-insight.md|Key Insight: Hyperedges Are Processors]] • [[intermediate-representation.md|Intermediate Representation]]
