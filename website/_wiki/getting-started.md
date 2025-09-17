---
title: Getting Started
---

# Getting Started with LOOM

## Installation

### Prerequisites
- Rust 1.70 or higher
- Git
- 8GB RAM minimum (for topology operations)

### Building from Source

```bash
# Clone the repository
git clone https://github.com/yourusername/loom.git
cd loom

# Build the Rust implementation
cd src/lang
cargo build --release

# Run tests (when they exist)
cargo test
```

## First Program

### Running the REPL

```bash
# Start the REPL
cargo run -- repl
```

⚠️ **Note**: The REPL is currently non-functional. Parser implementation is needed.

### Example LOOM Program

Here's what a simple LOOM program looks like (when the parser works):

```loom
// Create a simple conscious topology
weave mind {
    <~> curiosity: 0.8
    <~> memory: 0.6
}

// Experience causes growth
weave experience {
    <~> novelty: 0.9
}

// Connect experience to mind - learning happens
experience <~> mind: 0.5

// Apply Hebbian learning
:hebbian
```

## Current Limitations

### What Works ✅
- Basic project structure
- Some Rust boilerplate
- Memory mapping experiments

### What Doesn't Work ❌
- Parser (cannot parse .loom files)
- Runtime (cannot execute programs)
- REPL (non-functional)
- Most features described in documentation

See [Implementation Status](./implementation-status.md) for detailed breakdown.

## Next Steps

1. **For Users**: Wait for basic parser implementation
2. **For Contributors**: See [Priority Tasks](./priority-tasks.md)
3. **For Researchers**: Review [Theory Document](../loom_complete_documentation.tex)

## Getting Help

- Check the [FAQ](./faq.md)
- Review [Known Issues](./implementation-status.md#known-issues)
- Open an issue on GitHub
