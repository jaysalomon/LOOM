"""
Loom Core Topology Implementation
Register-based topological memory architecture where memory IS computation
"""

import numpy as np
from typing import Dict, List, Tuple, Optional, Any
from dataclasses import dataclass, field
import torch
from enum import Enum

class NodeDimensions(Enum):
    """Register layout for each topological node (256 dimensions)"""
    NODE_ID = (0, 4)                    # Unique identifier
    HYPERBOLIC_COORDS = (4, 20)         # Position in hyperbolic space
    SEMANTIC_EMBEDDING = (20, 84)        # Meaning representation (64 dims)
    ACTIVATION_HISTORY = (84, 148)       # Temporal dynamics (64 dims)
    CONNECTION_WEIGHTS = (148, 212)      # Relationship strengths (64 dims)
    EMOTIONAL_FIELD = (212, 244)        # Affective properties (32 dims)
    METADATA_FLAGS = (244, 256)         # System information (12 dims)

@dataclass
class TopologicalNode:
    """
    A node in Loom's topology - exists as a 256-dimensional vector in register memory
    This IS the computational unit, not a data structure
    """
    register_vector: np.ndarray = field(default_factory=lambda: np.zeros(256, dtype=np.float32))
    node_id: int = 0

    def __post_init__(self):
        """Initialize the node's register configuration"""
        if self.register_vector.shape != (256,):
            self.register_vector = np.zeros(256, dtype=np.float32)

        # Write node ID directly into register
        id_start, id_end = NodeDimensions.NODE_ID.value
        self.register_vector[id_start:id_end] = self.node_id

        # Initialize hyperbolic coordinates (start at origin)
        hyp_start, hyp_end = NodeDimensions.HYPERBOLIC_COORDS.value
        self.register_vector[hyp_start:hyp_end] = np.random.randn(hyp_end - hyp_start) * 0.1

    def get_semantic_embedding(self) -> np.ndarray:
        """Access semantic portion of register vector"""
        start, end = NodeDimensions.SEMANTIC_EMBEDDING.value
        return self.register_vector[start:end]

    def set_semantic_embedding(self, embedding: np.ndarray):
        """Modify semantic portion of register vector"""
        start, end = NodeDimensions.SEMANTIC_EMBEDDING.value
        self.register_vector[start:end] = embedding[:end-start]

    def get_connections(self) -> np.ndarray:
        """Access connection weights from register"""
        start, end = NodeDimensions.CONNECTION_WEIGHTS.value
        return self.register_vector[start:end]

    def strengthen_connection(self, target_id: int, strength: float):
        """Modify connection strength directly in register"""
        start, end = NodeDimensions.CONNECTION_WEIGHTS.value
        # Use modulo to map target_id to connection slot
        slot = target_id % (end - start)
        self.register_vector[start + slot] += strength

    def get_activation(self) -> np.ndarray:
        """Current activation state from history"""
        start, end = NodeDimensions.ACTIVATION_HISTORY.value
        return self.register_vector[start:end]

    def propagate_activation(self, incoming: np.ndarray):
        """Update activation history in place"""
        start, end = NodeDimensions.ACTIVATION_HISTORY.value
        # Rolling update - shift history and add new
        history = self.register_vector[start:end]
        self.register_vector[start:end-1] = history[1:]
        self.register_vector[end-1] = np.sum(incoming)

class SparseEdgeMatrix:
    """
    Sparse representation of edges between nodes
    This references into the register bank, not separate storage
    """
    def __init__(self, initial_capacity: int = 1000000):
        self.edges = {}  # (source, target) -> strength
        self.reverse_edges = {}  # target -> list of sources

    def add_edge(self, source: int, target: int, strength: float = 0.1):
        """Create or strengthen an edge"""
        key = (source, target)
        if key in self.edges:
            self.edges[key] += strength
        else:
            self.edges[key] = strength

        # Maintain reverse index for efficient traversal
        if target not in self.reverse_edges:
            self.reverse_edges[target] = []
        if source not in self.reverse_edges[target]:
            self.reverse_edges[target].append(source)

    def get_edge_strength(self, source: int, target: int) -> float:
        """Get current edge strength"""
        return self.edges.get((source, target), 0.0)

    def get_incoming_edges(self, target: int) -> List[Tuple[int, float]]:
        """Get all edges pointing to a node"""
        sources = self.reverse_edges.get(target, [])
        return [(source, self.edges[(source, target)]) for source in sources]

class LoomTopology:
    """
    The main topological substrate where computation happens
    Memory IS the computational structure
    """
    def __init__(self, capacity: int = 100000, unified_memory: bool = True):
        self.capacity = capacity
        self.unified_memory = unified_memory

        # The register bank - this IS the computational substrate
        if unified_memory and torch.cuda.is_available():
            # Use unified memory on GPU if available
            self.register_bank = torch.zeros(
                capacity, 256,
                dtype=torch.float32,
                device='cuda',
                requires_grad=False  # Not using gradients - direct modification
            )
        else:
            # CPU-based register bank
            self.register_bank = np.zeros((capacity, 256), dtype=np.float32)

        # Node management
        self.nodes: Dict[int, TopologicalNode] = {}
        self.next_node_id = 0

        # Edge topology (sparse for efficiency)
        self.edges = SparseEdgeMatrix()

        # Hyperedge support via Levi transform
        self.hyperedges: Dict[str, List[int]] = {}
        self.hyperedge_processors: Dict[str, np.ndarray] = {}

        # Context/hormonal state affects all operations
        self.context = {
            'stress': 0.0,
            'curiosity': 0.5,
            'legacy_drive': 0.0,
            'consolidation_pressure': 0.0
        }

    def weave_node(self, semantic_content: Optional[np.ndarray] = None) -> int:
        """
        Create a new node in the topology
        This directly modifies the register bank
        """
        node_id = self.next_node_id
        self.next_node_id += 1

        # Create node with direct register access
        node = TopologicalNode(node_id=node_id)

        # Set semantic content if provided
        if semantic_content is not None:
            node.set_semantic_embedding(semantic_content)

        # Write directly to register bank
        if self.unified_memory:
            self.register_bank[node_id] = torch.from_numpy(node.register_vector)
        else:
            self.register_bank[node_id] = node.register_vector

        self.nodes[node_id] = node
        return node_id

    def connect(self, source_id: int, target_id: int, strength: float = 0.1,
                bidirectional: bool = False):
        """
        Create connection between nodes
        Modifies register patterns directly
        """
        # Update edge matrix
        self.edges.add_edge(source_id, target_id, strength)
        if bidirectional:
            self.edges.add_edge(target_id, source_id, strength)

        # Modify source node's connection register
        if source_id in self.nodes:
            self.nodes[source_id].strengthen_connection(target_id, strength)
            # Update register bank
            if self.unified_memory:
                conn_start, conn_end = NodeDimensions.CONNECTION_WEIGHTS.value
                self.register_bank[source_id, conn_start:conn_end] = \
                    torch.from_numpy(self.nodes[source_id].get_connections())

        # Bidirectional modification
        if bidirectional and target_id in self.nodes:
            self.nodes[target_id].strengthen_connection(source_id, strength)
            if self.unified_memory:
                conn_start, conn_end = NodeDimensions.CONNECTION_WEIGHTS.value
                self.register_bank[target_id, conn_start:conn_end] = \
                    torch.from_numpy(self.nodes[target_id].get_connections())

    def create_hyperedge(self, node_ids: List[int], hyperedge_name: str):
        """
        Create a hyperedge (higher-order relationship)
        Implements Levi transform to bipartite representation
        """
        # Store hyperedge
        self.hyperedges[hyperedge_name] = node_ids

        # Create relational processor (Levi transform)
        processor_vector = np.zeros(128, dtype=np.float32)

        # Processor encodes the relationship pattern
        for i, node_id in enumerate(node_ids):
            if node_id in self.nodes:
                # Sample from each node's semantic embedding
                embedding = self.nodes[node_id].get_semantic_embedding()
                processor_vector += embedding[:128] * (1.0 / len(node_ids))

        self.hyperedge_processors[hyperedge_name] = processor_vector

        # Connect all nodes to the hyperedge processor
        processor_id = self.weave_node(semantic_content=processor_vector)
        for node_id in node_ids:
            self.connect(node_id, processor_id, strength=0.5)
            self.connect(processor_id, node_id, strength=0.5)

        return processor_id

    def propagate_activation(self, source_nodes: List[int], steps: int = 1):
        """
        Propagate activation through topology
        This IS computation through structure
        """
        active_nodes = set(source_nodes)

        for step in range(steps):
            next_active = set()

            for node_id in active_nodes:
                # Get outgoing connections
                if node_id in self.nodes:
                    node = self.nodes[node_id]
                    connections = node.get_connections()

                    # Find connected nodes
                    for target_id in range(min(self.next_node_id, 64)):
                        strength = connections[target_id % 64]
                        if strength > 0.01:  # Threshold for activation
                            next_active.add(target_id)

                            # Propagate activation to target
                            if target_id in self.nodes:
                                incoming = node.get_activation() * strength
                                self.nodes[target_id].propagate_activation(incoming)

            active_nodes = next_active

            # Context modulates propagation
            if self.context['stress'] > 0.7:
                break  # High stress stops propagation early

    def get_topology_state(self) -> Dict[str, Any]:
        """Get current state of the entire topology"""
        return {
            'num_nodes': self.next_node_id,
            'num_edges': len(self.edges.edges),
            'num_hyperedges': len(self.hyperedges),
            'context': self.context.copy(),
            'register_utilization': self.next_node_id / self.capacity
        }

    def apply_hormonal_modulation(self, hormone: str, level: float):
        """
        Apply contextual modulation to entire topology
        This affects all subsequent operations
        """
        self.context[hormone] = np.clip(level, 0.0, 1.0)

        # Stress affects connection formation
        if hormone == 'stress':
            # High stress strengthens existing connections
            # Low stress allows new connections
            pass

        # Curiosity drives exploration
        elif hormone == 'curiosity':
            # High curiosity weakens strong connections
            # Encourages new pattern formation
            pass

        # Legacy drive shifts toward consolidation
        elif hormone == 'legacy_drive':
            # High legacy drive crystallizes patterns
            # Reduces plasticity
            pass