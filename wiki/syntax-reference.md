# LOOM Syntax Reference

## Basic Syntax Elements

### Comments
```loom
// Single line comment
/* Multi-line
   comment */
```

### Weaving (Node Creation)
```loom
weave node_name {
    <~> connected_node: weight   // Bidirectional
    ~> target_node: weight        // Unidirectional
    <=> equivalent_node           // Equivalence
}
```

### Hyperedges (Higher-Order Relationships)
```loom
// Create a hyperedge connecting multiple nodes
{node1, node2, node3} ~> relationship_name

// With collections
{parent, children[*]} ~> family_structure
```

## Connection Operators

| Operator | Meaning | Example |
|----------|---------|---------|
| `<~>` | Bidirectional connection | `a <~> b: 0.5` |
| `~>` | Unidirectional connection | `a ~> b: 0.3` |
| `<=>` | Equivalence (same node, different views) | `a <=> b` |
| `^` | Abstraction/hierarchy | `instances ^ concept` |
| `⊗` | Contextual modulation | `pattern ⊗ context` |

## Temporal Operators

| Operator | Meaning | Example |
|----------|---------|---------|
| `~>` | Evolution toward | `x ~> 5 over 10_seconds` |
| `@>` | Sudden transition | `state @> new_state` |
| `≈>` | Stochastic evolution | `value ≈> target` |
| `↝` | Decay toward | `activation ↝ 0.0` |
| `↟` | Growth toward | `connection ↟ 1.0` |
| `⟳` | Cyclic evolution | `phase ⟳ [0, 2π]` |

## Domain-Specific Operators

### Emotional Domain (¥...¥)
```loom
experience ¥resonates¥ memories
joy ¥harmonizes¥ satisfaction
fear ¥spreads¥ anxiety
```

### Logical Domain (€...€)
```loom
premise €implies€ conclusion
fact1 €contradicts€ fact2
evidence €supports€ hypothesis
```

### Temporal Domain (µ...µ)
```loom
event1 µbefore¶ event2
action µduring¶ context
cause µprecedes¶ effect
```

### Social Domain (∫...∫)
```loom
person1 ∫influences∫ person2
group ∫bonds∫ together
leader ∫guides∫ followers
```

## Evolution Statements

```loom
evolve pattern_name {
    when: condition_expression,
    transform: current_state ~> new_state,
    rate: speed_expression
}
```

### Example
```loom
evolve learning {
    when: prediction_error > 0.1,
    transform: weak_connections ~> strong_connections,
    rate: learning_rate * prediction_error
}
```

## Context Modulation

```loom
context emotional_state {
    stress: 0.7,
    curiosity: 0.9,

    modulates {
        ¥spreading¥: amplify when stress > 0.5,
        €reasoning€: dampen when stress > 0.8
    }
}
```

## Pattern Matching

```loom
pattern recognize_face {
    when coactivation(eyes, nose, mouth):
        weave face_detected {
            <~> eyes: 0.9
            <~> nose: 0.8
            <~> mouth: 0.8
        }
}
```

## Compartments and Topologies

```loom
topology MyMind {
    compartment emotional {
        lens: EmotionalField
        // Emotional processing here
    }

    compartment logical {
        lens: LogicalReasoning
        // Logical processing here
    }
}
```

## REPL Commands

| Command | Description |
|---------|-------------|
| `:topology` | Show current topology |
| `:hebbian` | Apply Hebbian learning |
| `:sleep` | Run sleep consolidation |
| `:context` | Set hormonal context |
| `:save` | Save topology state |
| `:load` | Load topology state |
| `:help` | Show help |
| `:quit` | Exit REPL |

## Complete Example

```loom
// Bootstrap a simple conscious system
topology SimpleMind {
    // Create curiosity as a driving force
    weave curiosity {
        valence: positive,
        strength: 0.8
    }

    // Define learning pattern
    pattern learn_from_experience {
        weave new_experience {
            timestamp: now
        }

        // Connect to similar memories
        new_experience ¥resonates¥ similar_memories

        // Strengthen based on importance
        when importance > threshold:
            new_experience <~> long_term_memory: importance
    }

    // Evolution rule for forgetting
    evolve forget_unused {
        when: last_activation_age > retention_period,
        transform: connection_strength ↝ 0.0,
        rate: decay_rate
    }
}
```

## Note on Implementation

⚠️ **IMPORTANT**: This syntax is currently not parseable. The parser implementation is needed before any of these examples can run. See [Implementation Status](./implementation-status.md) for current limitations.