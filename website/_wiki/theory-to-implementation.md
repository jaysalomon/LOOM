---
title: Theory To Implementation
---

# Theory → Implementation Map

This page maps core theoretical constructs to concrete modules and tasks in the repo.

## Hypergraphs & Levi Transform
- Theory: Levi graph, incidence matrices, message passing.
- Implementation:
  - Data structures: adjacency / incidence in `runtime.rs`.
  - Conversion utilities: hyperedge ↔ bipartite representation.
  - Tasks: define types, load/store, basic traversals.

## Lenses (Multi-Perspective Processing)
- Theory: projections `P_k`, dynamics `f_k`, weights `α_k`.
- Implementation:
  - Trait `Lens` with `project`, `update`, `weight`.
  - Simple lenses: emotional/logical/temporal (mock behaviors first).
  - Tasks: interface + mock implementations + unit tests.

## Hebbian Learning
- Theory: correlation-driven updates, simple rules.
- Implementation:
  - Function `hebbian_step(X, A, η)` updating weights.
  - Start CPU-side in Rust; GPU later.
  - Tasks: numeric tests; monotonic strengthening on co-activation.

## Sleep Consolidation
- Theory: replay `R`, sparsity, soft-thresholding.
- Implementation:
  - `replay()` pass + `soft_threshold(A, τ)`.
  - Tasks: prune weak edges, reinforce frequent co-activations.

## Runtime / REPL
- Theory: evolution operators over time.
- Implementation:
  - Minimal REPL commands: `weave`, `connect`, `:hebbian`, `:topology`.
  - Tasks: wire parser to runtime; printable state; JSON export.

## Kernel / Accelerators
- Theory: unified memory ops; vectorized updates.
- Implementation:
  - C functions for vector ops; Metal kernels for batched updates.
  - Tasks: define FFI surface; smoke tests; perf later.

