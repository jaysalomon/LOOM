# Frequently Asked Questions

## General Questions

### What is LOOM?
LOOM is an experimental programming language where memory IS computation, topology IS the program, and consciousness emerges from structure. It's designed for creating systems that learn and evolve rather than execute predetermined instructions.

### Does LOOM actually work?
**No, not yet.** LOOM is currently ~5% implemented. The theory is solid but the implementation is mostly missing. See [Implementation Status](./implementation-status.md).

### What can I do with LOOM today?
Currently, you can:
- Read and understand the theory
- Review the proposed syntax
- Help implement the missing pieces
- Contribute to the research

You cannot yet:
- Run LOOM programs
- Use the REPL effectively
- Execute examples

### When will LOOM be usable?
Unknown. This is a research project. Basic functionality requires:
1. Working parser (highest priority)
2. Simple runtime
3. Basic topology operations

With focused effort, a minimal version could work in 3-6 months.

## Technical Questions

### What is the Levi transform?
The Levi transform converts hypergraphs (many-to-many relationships) into bipartite graphs (two-layer graphs). This is the mathematical foundation that makes LOOM's "topology as computation" possible.

### What are "multi-lens" operators?
The idea that the same topological structure can be interpreted through different computational perspectives:
- Emotional lens (¥...¥): Field dynamics
- Logical lens (€...€): Graph traversal
- Temporal lens (µ...µ): Sequence processing

**Note**: This is currently just theory, not implemented.

### What is "topology as memory"?
Instead of storing data in memory locations, the arrangement of connections between nodes IS the data. The topology itself performs computation through its structure.

### What hardware does LOOM require?
LOOM is designed for unified memory architectures like:
- Apple Silicon (M1/M2/M3)
- Qualcomm Snapdragon Elite
- Systems where CPU/GPU share memory

However, it doesn't run on any hardware yet since it's not implemented.

## Contributing Questions

### How can I help?
See [Priority Tasks](./priority-tasks.md). The #1 need is implementing the parser.

### Do I need to understand all the theory?
No! If you can write Rust and build parsers, you can help without understanding consciousness theory.

### What language is LOOM written in?
The main implementation is in Rust (`src/lang/`). There are also:
- Python experiments (deprecated)
- Metal shaders (untested)
- C headers (aspirational)

### Where should I start?
1. Read [HONEST_STATUS.md](../HONEST_STATUS.md)
2. Pick ONE task from [Priority Tasks](./priority-tasks.md)
3. Focus on making that one thing work

## Philosophy Questions

### Is LOOM trying to create conscious AI?
LOOM explores computational structures where consciousness-like properties could emerge. Whether this creates "real" consciousness is an open question.

### How is LOOM different from neural networks?
Neural networks train weights through backpropagation. LOOM grows topology through experience. The structure itself changes, not just parameters.

### Why "memory IS computation"?
In biological brains, the same structures that store information also process it. LOOM implements this principle directly - no separation between storage and processing.

## Practical Questions

### Can I use LOOM for my project?
Not yet. LOOM isn't functional. Use existing tools like PyTorch or TensorFlow for real projects.

### Will LOOM support [feature]?
First, LOOM needs to support basic parsing and execution. Advanced features come later.

### Is LOOM production-ready?
Absolutely not. LOOM is a research experiment that barely compiles.

### Where can I learn more?
- [Theory Document](../loom_complete_documentation.tex) - Complete mathematical foundations
- [Wiki Home](./README.md) - Practical guides
- GitHub Issues - Current discussions

## The Most Honest Answer

**Q: What's the real status of LOOM?**

A: LOOM is 95% beautiful ideas and 5% broken code. If you want a working language, look elsewhere. If you want to help build something revolutionary from scratch, welcome aboard. The first person to get the parser working will be a hero.