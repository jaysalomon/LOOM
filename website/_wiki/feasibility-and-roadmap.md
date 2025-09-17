---
title: Feasibility And Roadmap
---

# Feasibility & Roadmap

LOOM is theoretically sound and practically buildable. The core ideas—hypergraph/Levi transforms, Hebbian-style updates, multi-lens projections, and consolidation—map to well-understood data structures and algorithms. What’s needed is a focused, incremental engineering effort.

## Why it’s feasible
- Hypergraphs ↔ Levi graphs are standard transformations.
- Message passing and correlation updates are well-known in GNNs and graph algorithms.
- Unified memory and GPU compute (Metal/CUDA) suit vectorized updates and sparse ops.
- The runtime can start simple (CPU, sparse matrices) and evolve.

## Risks & mitigations
- Scope creep → Strict MVP slices and small PRs.
- Performance unknowns → Start CPU-first; add GPU when correctness is stable.
- Ambiguity → Theory-to-Implementation Map keeps modules grounded.

## Phased roadmap

### Phase 0 — Bootstrapping (2–4 weeks)
- Minimal tokenizer and AST for `weave` and connections
- In-memory topology (nodes, edges, weights)
- REPL with `weave`, `connect`, `:topology`

### Phase 1 — Basic dynamics (4–6 weeks)
- Hebbian update step and simple activation propagation
- JSON export for visualizer; smoke tests
- KaTeX-backed theory snippets embedded in docs

### Phase 2 — Structure and consolidation (6–10 weeks)
- Hyperedge modeling and Levi conversion utilities
- Sleep consolidation pass (prune/strengthen)
- Simple lens trait and mock implementations

### Phase 3 — Performance & kernel (8–12 weeks)
- C/Metal kernels for batched updates
- FFI boundary and benchmarks
- Sparse formats and memory layout improvements

## How to get involved (contributors)
- Pick a task from [[priority-tasks.md|Priority Tasks]]
- Read [[AI Development Guide]] for suggested dev workflow and prompts
- Start small: parser slice, graph data types, or REPL commands

## Milestones (public)
- M1: Parse & REPL demo (text only)
- M2: Topology JSON + visualizer interop
- M3: Hebbian + consolidation passes
- M4: First GPU-accelerated batch update

