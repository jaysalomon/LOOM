"""
Hebbian Learning Implementation for Loom
Connections that fire together wire together - the fundamental growth mechanism
"""

import numpy as np
from typing import List, Tuple, Dict, Set
from dataclasses import dataclass
import time
from .topology import LoomTopology, TopologicalNode

@dataclass
class ActivationEvent:
    """Record of node activation for correlation detection"""
    node_id: int
    timestamp: float
    strength: float

class HebbianPlasticity:
    """
    Implements Hebbian learning: simultaneous activation strengthens connections
    This is how the topology grows and learns from experience
    """
    def __init__(self, topology: LoomTopology):
        self.topology = topology

        # Learning parameters
        self.strengthening_rate = 0.1
        self.weakening_rate = 0.05
        self.pruning_threshold = 0.001
        self.sprouting_threshold = 0.8

        # Correlation detection window (in seconds)
        self.correlation_window = 0.05  # 50ms window

        # Track recent activations for correlation
        self.activation_history: List[ActivationEvent] = []
        self.coactivation_counts: Dict[Tuple[int, int], int] = {}

    def record_activation(self, node_id: int, strength: float = 1.0):
        """Record a node activation event"""
        event = ActivationEvent(
            node_id=node_id,
            timestamp=time.time(),
            strength=strength
        )
        self.activation_history.append(event)

        # Clean old events outside correlation window
        current_time = time.time()
        self.activation_history = [
            e for e in self.activation_history
            if current_time - e.timestamp < self.correlation_window * 2
        ]

    def detect_coactivations(self) -> List[Tuple[int, int, float]]:
        """
        Detect which nodes fired together within the correlation window
        Returns list of (node1, node2, correlation_strength)
        """
        coactivations = []
        current_time = time.time()

        # Group events by time window
        window_events = [
            e for e in self.activation_history
            if current_time - e.timestamp < self.correlation_window
        ]

        # Find all pairs that fired together
        for i, event1 in enumerate(window_events):
            for event2 in window_events[i+1:]:
                if event1.node_id != event2.node_id:
                    time_diff = abs(event1.timestamp - event2.timestamp)
                    if time_diff < self.correlation_window:
                        # Correlation strength inversely proportional to time difference
                        correlation = 1.0 - (time_diff / self.correlation_window)
                        correlation *= (event1.strength * event2.strength)
                        coactivations.append((event1.node_id, event2.node_id, correlation))

                        # Track coactivation frequency
                        key = tuple(sorted([event1.node_id, event2.node_id]))
                        self.coactivation_counts[key] = self.coactivation_counts.get(key, 0) + 1

        return coactivations

    def apply_hebbian_update(self):
        """
        Main Hebbian learning step
        Strengthens connections between coactivated nodes
        """
        coactivations = self.detect_coactivations()

        for node1_id, node2_id, correlation in coactivations:
            # Check if connection exists
            existing_strength = self.topology.edges.get_edge_strength(node1_id, node2_id)

            if existing_strength > 0:
                # Strengthen existing connection
                new_strength = existing_strength + (self.strengthening_rate * correlation)
                self.topology.connect(node1_id, node2_id, self.strengthening_rate * correlation)
            else:
                # Sprout new connection if correlation is strong enough
                if correlation > self.sprouting_threshold:
                    self.topology.connect(node1_id, node2_id, 0.01, bidirectional=True)

    def apply_synaptic_decay(self):
        """
        Weaken unused connections over time
        Implements forgetting and pruning
        """
        edges_to_prune = []

        for (source, target), strength in list(self.topology.edges.edges.items()):
            # Check coactivation frequency
            key = tuple(sorted([source, target]))
            coactivation_freq = self.coactivation_counts.get(key, 0)

            if coactivation_freq == 0:
                # No recent coactivation - apply decay
                new_strength = strength * (1.0 - self.weakening_rate)

                if new_strength < self.pruning_threshold:
                    edges_to_prune.append((source, target))
                else:
                    self.topology.edges.edges[(source, target)] = new_strength

        # Prune very weak connections
        for source, target in edges_to_prune:
            del self.topology.edges.edges[(source, target)]
            if target in self.topology.edges.reverse_edges:
                self.topology.edges.reverse_edges[target].remove(source)

        # Reset coactivation counts periodically
        self.coactivation_counts = {}

    def induce_sprouting(self, active_nodes: Set[int]):
        """
        Active nodes spawn new connections
        Explores new topological patterns
        """
        for node_id in active_nodes:
            if node_id not in self.topology.nodes:
                continue

            # Sprouting probability depends on context
            sprout_prob = 0.1 * self.topology.context['curiosity']

            if np.random.random() < sprout_prob:
                # Choose random target for new connection
                target_id = np.random.randint(0, self.topology.next_node_id)
                if target_id != node_id and target_id in self.topology.nodes:
                    # Create weak exploratory connection
                    self.topology.connect(node_id, target_id, 0.001)

class CriticalPeriod:
    """
    Manages developmental critical periods where plasticity changes over time
    Young topologies are highly plastic, mature ones are more stable
    """
    def __init__(self, topology: LoomTopology):
        self.topology = topology
        self.experience_count = 0
        self.phase = "infant"

    def update_phase(self):
        """Update developmental phase based on experience"""
        if self.experience_count < 1000:
            self.phase = "infant"
            self.plasticity_multiplier = 2.0
            self.pruning_resistance = 0.1
        elif self.experience_count < 10000:
            self.phase = "juvenile"
            self.plasticity_multiplier = 1.5
            self.pruning_resistance = 0.3
        elif self.experience_count < 100000:
            self.phase = "adolescent"
            self.plasticity_multiplier = 1.2
            self.pruning_resistance = 0.5
        else:
            self.phase = "adult"
            self.plasticity_multiplier = 0.8
            self.pruning_resistance = 0.8

    def record_experience(self):
        """Count experiences to track developmental stage"""
        self.experience_count += 1
        self.update_phase()

    def modulate_plasticity(self, hebbian: HebbianPlasticity):
        """Adjust learning rates based on developmental phase"""
        hebbian.strengthening_rate *= self.plasticity_multiplier
        hebbian.weakening_rate *= (2.0 - self.plasticity_multiplier)
        hebbian.pruning_threshold *= self.pruning_resistance

class ExperienceReplay:
    """
    Replay past experiences to consolidate learning
    Similar to hippocampal replay during sleep
    """
    def __init__(self, capacity: int = 10000):
        self.capacity = capacity
        self.experiences: List[List[int]] = []  # Lists of coactivated nodes

    def store_experience(self, active_nodes: List[int]):
        """Store pattern of active nodes"""
        self.experiences.append(active_nodes.copy())
        if len(self.experiences) > self.capacity:
            self.experiences.pop(0)

    def replay_experiences(self, topology: LoomTopology, hebbian: HebbianPlasticity,
                         num_replays: int = 10):
        """
        Replay stored experiences to strengthen important patterns
        This happens during consolidation/sleep cycles
        """
        if not self.experiences:
            return

        for _ in range(num_replays):
            # Sample random experience
            experience = self.experiences[np.random.randint(len(self.experiences))]

            # Reactivate the pattern
            for node_id in experience:
                hebbian.record_activation(node_id, strength=0.5)  # Weaker replay

            # Apply Hebbian learning on replayed pattern
            hebbian.apply_hebbian_update()

class StructuralPlasticity:
    """
    Manages large-scale topological reorganization
    Creates new nodes and removes unused ones
    """
    def __init__(self, topology: LoomTopology):
        self.topology = topology
        self.node_usage: Dict[int, float] = {}
        self.usage_decay = 0.99

    def track_usage(self, active_nodes: Set[int]):
        """Track which nodes are actively used"""
        # Decay all usage scores
        for node_id in self.node_usage:
            self.node_usage[node_id] *= self.usage_decay

        # Increase usage for active nodes
        for node_id in active_nodes:
            self.node_usage[node_id] = self.node_usage.get(node_id, 0.0) + 1.0

    def spawn_new_nodes(self, num_nodes: int = 1):
        """
        Create new nodes when learning pressure is high
        This expands the topology's capacity
        """
        for _ in range(num_nodes):
            # Create node with random initial embedding
            semantic_embedding = np.random.randn(64) * 0.1
            new_node_id = self.topology.weave_node(semantic_content=semantic_embedding)

            # Connect to a few random existing nodes
            if self.topology.next_node_id > 10:
                num_connections = np.random.randint(1, 4)
                targets = np.random.choice(
                    self.topology.next_node_id - 1,
                    size=num_connections,
                    replace=False
                )
                for target in targets:
                    self.topology.connect(new_node_id, int(target), 0.001, bidirectional=True)

    def prune_unused_nodes(self, usage_threshold: float = 0.01):
        """
        Remove nodes that haven't been used recently
        This prevents unlimited growth
        """
        nodes_to_remove = [
            node_id for node_id, usage in self.node_usage.items()
            if usage < usage_threshold
        ]

        for node_id in nodes_to_remove:
            # Remove from tracking
            del self.node_usage[node_id]
            # Note: In full implementation, would remove from topology
            # For now, just mark as inactive