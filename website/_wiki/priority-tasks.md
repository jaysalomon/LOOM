---
title: Priority Tasks
---

# Priority Tasks for Contributors

## 🔴 Critical Path to "Hello World"

These tasks are ordered by dependency. Each enables the next.

### Task 1: Implement Basic Parser
**Difficulty**: Medium
**Location**: `src/lang/src/parser.rs`
**Goal**: Parse minimal LOOM syntax

```rust
// Minimum viable parser should handle:
weave node_name {
    <~> other_node: 0.5
}
```

**Success Criteria**: Can parse simple .loom file into AST

---

### Task 2: Create Simple Topology Structure
**Difficulty**: Easy
**Location**: `src/lang/src/runtime.rs`
**Goal**: Store nodes and edges in memory

```rust
// Need basic:
struct Node { id: u32, name: String }
struct Edge { from: u32, to: u32, weight: f32 }
struct Topology { nodes: Vec<Node>, edges: Vec<Edge> }
```

**Success Criteria**: Can create and query topology

---

### Task 3: Connect Parser to Runtime
**Difficulty**: Medium
**Location**: `src/lang/src/interpreter.rs`
**Goal**: Execute parsed AST to build topology

**Success Criteria**: Running `weave` creates actual nodes

---

### Task 4: Implement REPL Commands
**Difficulty**: Easy
**Location**: `src/lang/src/repl.rs`
**Goal**: Make `:topology` command work

**Success Criteria**: Can see created nodes in REPL

---

## 🟡 High-Value Improvements

### Documentation Tasks
- Extract unique content from deprecated docs
- Add inline code documentation
- Create working example when parser works

### Testing Infrastructure
- Set up basic test framework
- Create parser test cases
- Add topology validation tests

### Refactoring
- Remove Python prototype code
- Consolidate Rust implementations
- Clean up unused dependencies

---

## 🟢 Research & Advanced Features

### Mathematical Formalization
- Implement actual Levi transform
- Prove computational equivalence
- Create consciousness metrics

### Advanced Features (AFTER basics work)
- Hebbian learning implementation
- Evolution operators
- Multi-lens processing
- GPU acceleration

---

## How to Claim a Task

1. Check if someone's already working on it (see Issues)
2. Comment on the issue or create one
3. Fork the repository
4. Create a feature branch
5. Submit PR when ready

## Current Blockers

⚠️ **Nothing works without the parser!** Priority #1 is getting basic parsing functional.

## Questions?

- Review [Architecture](./architecture.md) for system design
- Check [Development Setup](./development-setup.md) for environment
- Ask in GitHub Discussions
