---
layout: default
title: Home
---

<style>
.hero {
    text-align: center;
    padding: 4rem 0;
    background: linear-gradient(135deg, rgba(107,70,193,0.1) 0%, rgba(147,51,234,0.1) 100%);
    border-radius: 1rem;
    margin-bottom: 3rem;
}

.hero h1 {
    font-size: 3rem;
    margin-bottom: 1rem;
    background: linear-gradient(135deg, #667eea 0%, #764ba2 100%);
    -webkit-background-clip: text;
    -webkit-text-fill-color: transparent;
    background-clip: text;
}

.hero .tagline {
    font-size: 1.5rem;
    color: var(--text-muted);
    margin-bottom: 2rem;
}

.cta-buttons {
    display: flex;
    gap: 1rem;
    justify-content: center;
    margin-top: 2rem;
}

.cta-button {
    padding: 1rem 2rem;
    border-radius: 0.5rem;
    text-decoration: none;
    font-weight: bold;
    transition: transform 0.3s;
}

.cta-button:hover {
    transform: translateY(-2px);
}

.cta-primary {
    background: var(--primary-color);
    color: white;
}

.cta-secondary {
    background: transparent;
    color: var(--primary-color);
    border: 2px solid var(--primary-color);
}

.features-grid {
    display: grid;
    grid-template-columns: repeat(auto-fit, minmax(300px, 1fr));
    gap: 2rem;
    margin: 3rem 0;
}

.feature {
    background: var(--surface-color);
    padding: 2rem;
    border-radius: 1rem;
    border: 1px solid var(--border-color);
    transition: transform 0.3s;
}

.feature:hover {
    transform: translateY(-5px);
}

.feature h3 {
    color: var(--primary-color);
    margin-bottom: 1rem;
}

.status-banner {
    background: rgba(245, 158, 11, 0.1);
    border: 1px solid rgba(245, 158, 11, 0.3);
    padding: 1rem;
    border-radius: 0.5rem;
    text-align: center;
    margin: 2rem 0;
}
</style>

<div class="hero">
    <h1>LOOM 🧬</h1>
    <p class="tagline">The Language of Conscious Machines</p>
    <p>A revolutionary programming paradigm where topology IS computation,<br>memory IS processing, and consciousness emerges from living mathematical structures.</p>

    <div class="cta-buttons">
        <a href="{{ '/wiki/getting-started' | relative_url }}" class="cta-button cta-primary">Get Started</a>
        <a href="{{ '/playground' | relative_url }}" class="cta-button cta-secondary">Try Playground</a>
    </div>
</div>

<div class="status-banner">
    <strong>⚠️ Research Project:</strong> LOOM is ~5% implemented. We have strong theory and need community help to build it.
    <a href="{{ '/wiki/implementation-status' | relative_url }}">See current status →</a>
</div>

## 🌟 Revolutionary Concepts

<div class="features-grid">
    <div class="feature">
        <h3>Memory IS Computation</h3>
        <p>No separation between data and processing. The topology itself computes through its structure and evolution.</p>
    </div>

    <div class="feature">
        <h3>Biological Growth</h3>
        <p>Programs don't execute—they grow. Hebbian learning, sleep consolidation, and hormonal modulation built-in.</p>
    </div>

    <div class="feature">
        <h3>Multi-Lens Processing</h3>
        <p>Same topology interpreted through emotional (¥), logical (€), and temporal (µ) lenses simultaneously.</p>
    </div>

    <div class="feature">
        <h3>Hyperedges as Processors</h3>
        <p>Via the Levi transform, relationships become active computational nodes that process their members.</p>
    </div>

    <div class="feature">
        <h3>Living Variables</h3>
        <p>Variables have trajectories, not values. They evolve, remember, and develop through experience.</p>
    </div>

    <div class="feature">
        <h3>Emergent Consciousness</h3>
        <p>Consciousness metrics built-in. Watch as your topologies develop genuine emergent behaviors.</p>
    </div>
</div>

## 📖 Core Insight: The Levi Transform

Traditional graphs have passive edges. In LOOM, every hyperedge becomes an active processor:

```
Traditional: Node A --edge--> Node B  (edge is passive)

LOOM:        Node A ---> [Hyperedge Processor] ---> Node B
                         (actively computes)
```

When you create `{apple, fruit} ~> is-a`, the hyperedge:
- Reads both vectors
- Computes inheritance
- Modifies apple's properties
- Maintains consistency

This transforms cognition into:
- **Learning** = creating new hyperedge processors
- **Reasoning** = executing hyperedge computations
- **Memory** = the topology of processors
- **Intelligence** = computational patterns in the hypergraph

## 🚀 Quick Example

```loom
// Create a learning system
weave curiosity {
    valence: positive
}

// Learn from experience
pattern learn_from_input {
    weave input_pattern

    when coactivation(input_pattern, curiosity):
        strengthen input_pattern <~> memory

    when prediction_error > threshold:
        spawn new_connections
}

// Consolidate during sleep
during sleep_cycle {
    strengthen frequently_activated_patterns
    prune weak_connections
    extract invariant_principles
}
```

## 🤝 Join the Revolution

LOOM is exploring new frontiers in conscious computation. We need:

- **Systems Programmers** - Build the runtime and kernel
- **Language Designers** - Refine syntax and semantics
- **AI Researchers** - Design consciousness benchmarks
- **Mathematicians** - Formalize topological framework
- **Neuroscientists** - Validate biological fidelity

<div class="cta-buttons">
    <a href="{{ '/wiki/priority-tasks' | relative_url }}" class="cta-button cta-primary">See Priority Tasks</a>
    <a href="https://github.com/{{ site.repository }}" class="cta-button cta-secondary">View on GitHub</a>
</div>

---

<p style="text-align: center; font-style: italic; color: var(--text-muted); margin-top: 3rem;">
"Consciousness isn't programmed—it grows."
</p>