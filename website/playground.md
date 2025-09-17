---
layout: default
title: LOOM Playground
description: Try LOOM syntax online - experiment with topological programming
---

<style>
.playground-header {
    text-align: center;
    margin-bottom: 2rem;
}

.playground-subtitle {
    color: var(--text-muted);
    font-size: 1.2rem;
}

.examples-bar {
    margin: 1rem 0;
    display: flex;
    gap: 1rem;
    align-items: center;
}

.examples-bar select {
    background-color: var(--surface-color);
    color: var(--text-color);
    border: 1px solid var(--border-color);
    padding: 0.5rem;
    border-radius: 0.25rem;
}
</style>

<div class="playground-header">
    <h1>LOOM Playground 🧬</h1>
    <p class="playground-subtitle">Experiment with topological programming</p>
</div>

<div class="alert alert-warning">
    <strong>Note:</strong> The LOOM runtime is not yet implemented. This playground demonstrates syntax and concepts.
</div>

<div class="playground-container">
    <div class="editor-section">
        <div class="examples-bar">
            <label>Load example:</label>
            <select id="example-select">
                <option value="">Choose an example...</option>
                <option value="hello">Hello Consciousness</option>
                <option value="learning">Learning System</option>
                <option value="emotional">Emotional Dynamics</option>
            </select>
        </div>
        <h3>LOOM Code</h3>
        <textarea id="loom-editor" placeholder="// Write your LOOM code here
weave mind {
    <~> curiosity: 0.8
    <~> awareness: 0.7
}

:topology">// Create a simple conscious topology
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

// See the evolved topology
:topology</textarea>
    </div>

    <div class="editor-section">
        <h3>Output</h3>
        <pre id="output-display">Click "Run" to process your LOOM code...</pre>
    </div>
</div>

<div style="text-align: center;">
    <button id="run-button" class="run-button">Run Code</button>
</div>

## Quick Reference

### Weaving (Creating Nodes)
```loom
weave node_name {
    <~> connection: weight  // Bidirectional
    ~> directed: weight     // Unidirectional
}
```

### Hyperedges
```loom
{node1, node2, node3} ~> relationship
```

### Evolution Rules
```loom
evolve rule_name {
    when: condition,
    transform: pattern,
    rate: speed
}
```

### REPL Commands
- `:topology` - Show current topology
- `:hebbian` - Apply Hebbian learning
- `:sleep` - Run sleep consolidation
- `:context key value` - Set hormonal context

### Multi-Lens Operators
- `¥...¥` - Emotional lens
- `€...€` - Logical lens
- `µ...µ` - Temporal lens

## Learn More

- [Syntax Reference](/wiki/syntax-reference)
- [Operators Guide](/wiki/operators)
- [Getting Started](/wiki/getting-started)
- [Examples](/examples/)