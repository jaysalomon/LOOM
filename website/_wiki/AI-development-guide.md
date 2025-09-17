---
title: AI Development Guide
---

# AI Development Guide

This guide shows how to use AI coding tools to develop LOOM effectively while honoring the theory-first posture.

## Principles
- Treat the LaTeX as the single source of truth for theory.
- Convert theory to small, testable tasks.
- Keep public claims honest: most of LOOM is not implemented yet.

## Workflow
1. Choose a small target (e.g., parse `weave` blocks).
2. Extract relevant theory references.
3. Draft a minimal spec (input/output, data structures, error modes).
4. Write 2–4 tiny tests (happy path + edge cases).
5. Implement iteratively with AI assistance; keep changes small.
6. Document the decision and link back to theory sections.

## Prompt patterns
- “Given this spec, implement a PEG parser rule for `weave { ... }` in Rust (pest/nom), with tests.”
- “Translate this mathematical update rule into a simple Rust function with unit tests.”
- “Refactor this module to separate ‘lens’ interfaces and provide a mock lens for tests.”

## Repo map for AI tools
- Theory: `loom_complete_documentation.tex`
- Language: `src/lang/src/*.rs` (lexer, parser, ast, runtime, repl)
- Kernel: `kernel/src/*.c`, `src/gpu/*.metal`, `src/isa/*`
- Docs: `docs/`, `wiki/`

## Conventions
- Prefer pure functions and small modules.
- Error handling: result types, clear messages.
- Tests before refactors; keep fast.
- Benchmarks later; correctness first.

## Good first AI-assisted tasks
- Minimal tokenizer and tokens enum.
- Parsing `weave` blocks with names and weighted edges.
- In-memory graph representation (nodes, edges, weights).
- Simple visualization exporter (JSON) used by the Pages visualizer.

## Pitfalls to avoid
- Overfitting to speculative features not in theory.
- Large PRs without tests.
- Implicit formats; always specify data shapes.

