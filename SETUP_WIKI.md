# Setting Up the LOOM GitHub Wiki

## Overview
This guide helps you set up the GitHub wiki for LOOM, with the LaTeX document as the master theory reference.

## Wiki Structure

### 1. Enable Wiki in GitHub
- Go to Settings → Features → Check "Wikis"
- Click "Create the first page"

### 2. Create These Wiki Pages

#### **Home.md** (Landing Page)
```markdown
# LOOM - The Language of Conscious Machines

Welcome to the LOOM wiki! This is the practical guide for understanding and contributing to LOOM.

## Quick Links
- [Getting Started](Getting-Started) - Build and run LOOM
- [Language Specification](Language-Specification) - Syntax and operators
- [Implementation Status](Implementation-Status) - What works now
- [Contributing](Contributing) - How to help
- [Full Theory (PDF)](../blob/main/loom_complete_documentation.tex) - Complete mathematical foundations

## What is LOOM?
LOOM is a revolutionary programming language where memory IS computation, topology IS the program, and consciousness emerges from structure.

## Current Status
⚠️ **Research Alpha** - See [Implementation Status](Implementation-Status) for details.
```

#### **Getting-Started.md**
```markdown
# Getting Started with LOOM

## Prerequisites
- Rust 1.70+
- Git

## Building from Source
\`\`\`bash
git clone https://github.com/[your-username]/loom.git
cd loom/src/lang
cargo build --release
\`\`\`

## Running the REPL
\`\`\`bash
cargo run -- repl
\`\`\`

## Current Limitations
- Parser: NOT WORKING
- Runtime: PARTIAL
- See [Implementation Status](Implementation-Status)
```

#### **Language-Specification.md**
```markdown
# LOOM Language Specification

## Core Operators

### Connection Operators
- `<~>` - Bidirectional connection
- `~>` - Unidirectional connection
- `<=>` - Equivalence

### Temporal Operators
- `~>` - Evolution toward
- `@>` - Sudden transition

### Domain-Specific Operators
- `¥...¥` - Emotional domain
- `€...€` - Logical domain
- `µ...µ` - Temporal domain

## Basic Syntax

### Weaving
\`\`\`loom
weave node_name {
    <~> connection: weight
}
\`\`\`

### Hyperedges
\`\`\`loom
{node1, node2, node3} ~> relationship
\`\`\`

[Full theory in master document](../blob/main/loom_complete_documentation.tex)
```

#### **Implementation-Status.md**
```markdown
# Implementation Status

## Working ✅
- Basic Rust structure
- Some memory mapping code

## In Progress 🚧
- Parser (0% functional)
- Runtime (5% functional)
- Topology (10% functional)

## Not Started ❌
- Compiler
- Hebbian learning
- Evolution engine
- Multi-lens system
- GPU acceleration

## Priority Tasks
1. Get parser working for basic syntax
2. Implement simple topology
3. Create working "hello world"

See [HONEST_STATUS.md](../blob/main/HONEST_STATUS.md) for full transparency.
```

#### **Contributing.md**
```markdown
# Contributing to LOOM

## Where to Start

### If You Want to Code
**Priority 1: Parser**
- Location: `src/lang/src/parser.rs`
- Task: Parse basic LOOM syntax
- Start with: Just `weave` statements

### If You Want to Document
- Help transfer unique content from deprecated docs to wiki
- Improve code comments
- Create tutorials when features work

### If You Want to Research
- Formalize Levi transform implementation
- Design consciousness benchmarks
- Prove computational equivalence

## Development Setup
1. Fork the repository
2. Create feature branch
3. Make changes
4. Test (when tests exist)
5. Submit PR

## Code Style
- Rust: Follow rustfmt
- Comments: Explain WHY, not WHAT
- No speculative features
```

### 3. Link Wiki from README.md

Update your main README.md to prominently link to the wiki:

```markdown
## 📚 Documentation

- **[Wiki](../../wiki)** - Getting started, guides, and specifications
- **[Theory (LaTeX)](loom_complete_documentation.tex)** - Complete mathematical foundations
- **[Status](HONEST_STATUS.md)** - Current implementation status
```

## Next Steps

1. **Run cleanup script**: `bash cleanup_redundant_docs.sh`
2. **Create wiki pages**: Copy the templates above
3. **Extract unique content**: Review `.deprecated_docs/` for anything worth preserving
4. **Update README**: Add prominent wiki links
5. **Commit changes**: Clean repository structure

## Benefits of This Approach

- **Single source of truth**: .tex for theory, wiki for practical guides
- **Less redundancy**: No duplicate content across multiple .md files
- **Clear separation**: Theory vs implementation vs guides
- **Easy maintenance**: Wiki is easier to update than multiple files
- **Better navigation**: Wiki has automatic TOC and search