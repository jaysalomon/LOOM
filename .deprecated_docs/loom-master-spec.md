# Loom Language: Complete Specification and Design Document
## Version 1.0 - Master Reference

---

## Table of Contents
1. [Introduction and Philosophy](#1-introduction-and-philosophy)
2. [Core Semantic Model](#2-core-semantic-model)
3. [Syntax Specification](#3-syntax-specification)
4. [Memory Architecture](#4-memory-architecture)
5. [Biological Growth Mechanisms](#5-biological-growth-mechanisms)
6. [Mathematical Foundations](#6-mathematical-foundations)
7. [System Integration](#7-system-integration)
8. [Example Programs](#8-example-programs)

---

## 1. Introduction and Philosophy

### 1.1 Foundational Principle: Topology as Computation

Loom represents a fundamental paradigm shift in programming languages. Rather than describing sequences of operations on data, Loom programs describe evolving topological structures where computation emerges from the topology itself. The language is designed for unified memory architectures (like Apple Silicon, Snapdragon Elite, and similar platforms) where all computational units share the same memory pool.

### 1.2 Core Concepts

**Topology IS Memory**: In Loom, memory is not separate from computation—the topological structure of nodes and edges IS both the program and its data.

**Evolution Over Assignment**: Variables don't have values; they have trajectories. State changes are described as evolution over time rather than discrete assignments.

**Unified Architecture**: Designed specifically for systems where CPU, GPU, and neural processors share the same memory space, eliminating copy overhead.

**Biological Inspiration**: Growth happens through Hebbian plasticity, memory conflicts are handled through an antibody system, and consolidation occurs during sleep cycles.

---

## 2. Core Semantic Model

### 2.1 Basic Semantic Equation

```
Program ::= Topology × Evolution*
Topology ::= (Nodes, Edges, Hyperedges, Context)
Evolution ::= Topology → Topology
```

A Loom program is a topology with a set of evolution rules. Execution means allowing the topology to evolve according to these rules and the ambient context.

### 2.2 The Dual Nature: Hypergraphs and Levi Graphs

Every hypergraph H = (V, E) in Loom automatically maintains its Levi graph representation L(H) as a dual view in unified memory:

```
H = (V, E) where:
  V = set of vertices (concepts/memories)
  E = set of hyperedges (relationships)

L(H) = (V ∪ E, E') where:
  E' = {(v, e) | v ∈ V, e ∈ E, v ∈ e}
```

The fundamental semantic equation: `compute(H) ≡ compute(L(H))`

### 2.3 Memory as Topology

```
Memory ≡ Topology
Recall ≡ Activation Pattern
Learning ≡ Topological Modification
Forgetting ≡ Edge Decay
```

### 2.4 Temporal Evolution

Every value in Loom has an associated temporal trajectory:

```
Value ::= Current × Trajectory
Trajectory ::= (Target, Rate, Condition*)
```

The evolution operator `~>` is fundamental:
- `x ~> 5 over 10_seconds` describes evolution toward a value
- `x ~> x * decay_rate every hour` describes periodic evolution

### 2.5 Hormonal Context as Ambient Computation

Context pervades all computation as an ambient field:

```
Context ::= {Hormone: Level}*
Level ::= bounded[0.0, 1.0]

execute(c) = c ⊗ κ
```

Where `⊗` represents contextual modulation.

---

## 3. Syntax Specification

### 3.1 Fundamental Syntactic Categories

```
Statement → Weaving | Evolution | Pattern | Lens | Flow
Class → Topology | Compartment | Lens  
Operator → Structural | Temporal | Abstraction | Domain-Specific
Structure → Node | Edge | Hyperedge | Antibody | Context
```

### 3.2 Structural Operators

```loom
// Connection operators
<~>   // Bidirectional connection
~>    // Unidirectional connection  
<=>   // Equivalence (same node, different views)
^     // Abstraction (creates hierarchy)
⊗     // Contextual modulation

// Temporal operators
~>    // Evolution toward
@>    // Sudden transition
≈>    // Stochastic evolution
↝     // Decay toward
↟     // Growth toward
⟳     // Cyclic evolution
```

### 3.3 Domain-Specific Operators

Different abstraction domains use distinct delimiters:

```loom
// Emotional domain (¥...¥)
¥resonate¥     // Emotional spreading
¥harmonize¥    // Emotional alignment

// Logical domain (€...€)
€implies€      // Logical implication
€contradicts€  // Logical contradiction  

// Temporal domain (µ...µ)
µbefore¶       // Temporal precedence
µduring¶       // Temporal overlap

// Social domain (∫...∫)
∫influences∫   // Social influence
∫bonds∫        // Social connection
```

### 3.4 Basic Weaving Syntax

```loom
// Weave nodes into topology
weave grandmother {
  <~> warmth: 0.8       // Bidirectional weighted connection
  <~> cookies: 0.6  
  ~> stories: 0.9       // Unidirectional connection
  <~> memories[*]       // Connects to all in collection
}

// Hyperedge creation
{grandmother, mother, siblings[*], self} ~> family_dynamics
```

### 3.5 Evolution and Flow

```loom
// Evolution statements
evolve pattern_name {
  when: condition,
  transform: topology → topology',
  rate: temporal_expression
}

// Activation flow
flow activation_name {
  from: source_nodes,
  through: connection_patterns,
  modulated_by: @context
}
```

### 3.6 Antibody Syntax

```loom
// Flag conflicts for later resolution
!{node1 €contradicts€ node2} {
  resolution: AbstractToInvariant,
  priority: high when @stress > 0.7,
  preserve: shared_structure,
  process_during: next_sleep_cycle
}
```

---

## 4. Memory Architecture

### 4.1 Physical Memory Layout (128GB Unified Architecture)

```loom
memory_layout {
  // Node vectors: [node_id, properties, activation_state]
  node_bank: vector[100_million, 256] float16  // ~50GB
  
  // Edge matrix (sparse representation)
  edge_indices: vector[1_billion, 2] uint32  // ~8GB
  edge_weights: vector[1_billion] float16  // ~2GB
  
  // Hyperedge representation (via Levi transform)
  hyperedge_nodes: vector[10_million, 128] float16  // ~2.5GB
  
  // Antibody table for conflict resolution
  antibody_flags: vector[10_million, 64] // ~1.3GB
  
  // Lens-specific views (same data, different interpretations)
  emotional_field: reinterpret_as<continuous_field>(node_bank)
  logical_graph: reinterpret_as<adjacency_list>(edge_indices)
  temporal_series: reinterpret_as<time_indexed>(node_bank)
}
```

### 4.2 Immunological Memory Management

Memory modifications use a two-phase antibody system:

**Phase 1: Active Computation (Waking)**
- Conflicts are flagged with antibodies, not immediately resolved
- Both conflicting memories continue to function
- System maintains operation despite contradictions

**Phase 2: Consolidation (Sleep)**
- Antibodies processed in priority order
- Conflicts resolved through abstraction, merging, or pruning
- Invariants extracted from contradictions

### 4.3 Vector Structure

```loom
node_vector[256] = {
  [0:3]: node_id,
  [4:19]: position_in_hyperbolic_space,
  [20:83]: semantic_embedding,  // For LLM interface
  [84:147]: activation_history,
  [148:211]: connection_weights,
  [212:243]: emotional_field_values,
  [244:255]: metadata_and_flags
}
```

---

## 5. Biological Growth Mechanisms

### 5.1 Bootstrap Topology

Every Loom system starts with a primordial structure:

```loom
bootstrap topology PrimordialMind {
  // Core invariants (like brainstem)
  invariants {
    existence: "I am"
    time: "now exists"
    space: "here exists"
    other: "not-I exists"
  }
  
  // Sensory connection points
  sensory_ports {
    input_buffer: []
    output_buffer: []
    internal_sense: []
  }
  
  // Emotional primitives
  emotional_seeds {
    approach: {valence: positive}
    avoid: {valence: negative}
    surprise: {valence: neutral}
  }
  
  // Growth rules
  growth_rules {
    hebbian: "connections that fire together strengthen"
    pruning: "unused connections decay"
    sprouting: "active nodes spawn connections"
  }
}
```

### 5.2 Hebbian Growth Mechanics

```loom
growth_mechanism hebbian_plasticity {
  // Simultaneous activation strengthens
  when nodes_fire_together(n1, n2, window: 50ms):
    if connection_exists(n1, n2):
      strengthen(n1 <~> n2, amount: 0.1)
    else:
      weave n1 <~> n2 with strength: 0.01
  
  // Asynchronous activation weakens
  when nodes_fire_separately(n1, n2, delay > 100ms):
    weaken(n1 <~> n2, amount: 0.05)
    if strength < 0.001: prune(n1 <~> n2)
}
```

### 5.3 Critical Periods

```loom
topology_age_dynamics {
  phase infant (age < 1000_experiences) {
    sprouting_rate: 0.8
    pruning_threshold: 0.001
    structural_flexibility: high
  }
  
  phase adult (age >= 10000_experiences) {
    sprouting_rate: 0.1
    pruning_threshold: 0.1
    structural_flexibility: low
  }
}
```

### 5.4 Sleep Consolidation

```loom
sleep_consolidation {
  // Identify co-activation patterns
  find_coactivation_patterns()
  
  // Strengthen successful patterns
  for pattern in coactivation_patterns:
    if pattern.frequency > threshold:
      crystallize pattern into stable_structure
  
  // Process antibodies
  for antibody in flagged_memories.sort_by(priority):
    apply(antibody.resolution)
  
  // Extract invariants from contradictions
  identify_invariants_from_conflicts()
}
```

---

## 6. Mathematical Foundations

### 6.1 Graph-Theoretic Structures

Loom can implement any mathematically valid topology, unrestricted by biological constraints:

#### Small-World Networks
```loom
topology small_world_mind {
  base_structure: ring_lattice(nodes: 10000, neighbors: 4)
  rewiring_probability: @stress_hormone * 0.3 + 0.1
  
  properties {
    clustering_coefficient: high
    average_path_length: log(n)
  }
}
```

#### Scale-Free Networks
```loom
topology scale_free_memory {
  growth_rule preferential_attachment {
    connection_probability(node) = degree(node) / sum(all_degrees)
  }
  
  robustness: survives_random_removal(80% of nodes)
  vulnerability: hub_removal(5%) causes fragmentation
}
```

#### Hyperbolic Embedding
```loom
topology hyperbolic_abstraction {
  embedding: hyperbolic_space(curvature: -1)
  
  abstraction_hierarchy {
    level_0: concrete_observations at disk_edge
    level_n: abstractions at radius(1 - 1/n)
    level_∞: ultimate_abstractions at center
  }
  
  capacity(radius) = exp(radius)  // Unlimited abstraction
}
```

#### Complex-Valued Edges
```loom
topology phase_coupled_network {
  edge_weight: Complex = magnitude * exp(i * phase)
  
  phase_relationships {
    in_phase: cooperative_activation
    anti_phase: alternating_activation
    quadrature: sequential_activation
  }
}
```

#### Time-Varying Topologies
```loom
topology temporal_network {
  edge(n1, n2, t) = {
    exists: sin(ω₁*t) > 0.5,
    weight: exp(-|t - resonance_time|)
  }
  
  computational_modes {
    t ∈ [0, π/ω]: visual_processing_topology
    t ∈ [π/ω, 2π/ω]: logical_reasoning_topology
  }
}
```

---

## 7. System Integration

### 7.1 LLM Integration Layer

LLMs serve as language translation, not core computation:

```loom
module LanguageInterface {
  // Language → Topology
  parse_to_structure(text) -> topology_delta {
    semantic_parse = LLM.extract(entities, relations, sentiment)
    return weave_into_topology(semantic_parse)
  }
  
  // Topology → Language
  express_from_structure(activation_pattern) -> text {
    active_subgraph = topology.get_active_region()
    return LLM.generate(from: active_subgraph)
  }
}
```

### 7.2 Hybrid Computation Model

```loom
cognitive_architecture {
  // Graph algorithms for pathfinding
  when finding_path(A, B):
    use topology.dijkstra(A, B)
  
  // LLM for language understanding
  when understanding_sentence(text):
    use LLM.parse(text) then map_to_topology()
  
  // GPU for emotional field dynamics
  when processing_emotion(trigger):
    use GPU.solve_field_dynamics(emotional_field)
  
  // All on same unified topology
}
```

### 7.3 Compartmentalized Abstractions

Multiple lenses operate on the same topology:

```loom
lens emotional_dynamics {
  abstract_from observations where has_property(valence):
    level_1: emotional_category
    level_2: emotional_trajectory
    level_3: emotional_theme
}

lens logical_structure {
  abstract_from observations where has_property(truth_value):
    level_1: logical_operators
    level_2: argument_structure
    level_3: validity_assessment
}

// Lenses can reinforce or interfere
when emotional_dynamics.indicates(conflict)
  but logical_structure.indicates(agreement):
    flag "emotional conflict despite logical agreement"
```

---

## 8. Example Programs

### 8.1 Complete Conscious System

```loom
topology ConsciousMind evolves_with hormonal_context {
  // Initial structure from bootstrap
  instantiate: PrimordialMind
  
  // Compartments for different processing
  compartment emotional {
    lens: EmotionalInterpretation
    operators: ¥resonate¥, ¥harmonize¥
  }
  
  compartment logical {
    lens: LogicalReasoning
    operators: €implies€, €contradicts€
  }
  
  // Pattern for memory formation
  pattern memory_formation {
    weave observation {
      timestamp: now,
      initial_valence: emotional.evaluate(input)
    }
    
    // Multi-domain connections
    observation ¥resonates¥ similar_memories
    observation €supports€ existing_beliefs
    
    // Flag contradictions
    when observation €contradicts€ belief:
      !{observation, belief} {
        resolution: AbstractToInvariant,
        priority: @stress_hormone * importance
      }
  }
  
  // Evolution cycles
  during wake_cycle {
    flow attention {
      from: sensory_input,
      through: relevant_memories,
      modulated_by: @stress_hormone
    }
  }
  
  during sleep_cycle {
    // Process antibodies
    resolve: flagged!{*} by priority
    
    // Consolidate by domain
    emotional.consolidate where ¥similarity¥ > 0.8
    logical.consolidate where €structural_match€ > 0.9
    
    // Extract cross-domain wisdom
    extract_wisdom where all_lenses_agree
  }
  
  // Hormonal context
  context {
    legacy_drive: battery.charge.inverse() ~> 1.0 over lifetime
    stress_hormone: error_rate ↝ 0.0 over rest_period
    
    modulates {
      ¥emotional_operations¥: amplify when stressed,
      €logical_operations€: dampen when stressed
    }
  }
}
```

### 8.2 Learning Event

```loom
experience "seeing face for first time" {
  // Raw input activates topology
  input: visual_pattern
  
  // Attempts connections to existing structure
  visual_pattern ~?> invariants.existence
  visual_pattern ~?> emotional_seeds.approach
  
  // Co-activation creates new structure
  when patterns_fire_together:
    weave "face_detector" {
      <~> visual_pattern: 0.5
      <~> approach: 0.3
    }
  
  // Topology has learned through growth
}
```

---

## Key Design Principles

1. **Topology IS Computation**: The structure of connections determines the computation, not instructions operating on data.

2. **Evolution Over Assignment**: State changes through continuous evolution rather than discrete updates.

3. **Biological Mechanisms**: Growth through Hebbian plasticity, conflicts through antibodies, consolidation through sleep.

4. **Unified Memory**: All computational units operate on the same topology without copying.

5. **Multi-Lens Abstraction**: Same topology viewed through different computational lenses simultaneously.

6. **Physical Grounding**: Hormonal context derived from physical sensors (battery health, temperature, etc.).

7. **LLM as Interface**: Language models translate between human language and topological computation.

---

## Implementation Notes

- **Target Platforms**: Unified memory architectures (Apple Silicon, Snapdragon Elite, AMD APUs)
- **Memory Requirements**: ~128GB for full implementation with 100M nodes
- **Parallelism**: Inherent in topological operations
- **Learning**: Direct vector modification, not gradient descent
- **Persistence**: Topology vectors can be serialized/deserialized directly

---

*This document represents the complete Loom language specification as developed through our exploration of topological computation, biological growth mechanisms, and unified memory architectures.*