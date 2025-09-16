// Simple test harness to exercise loom kernel API (CPU only)
#include <stdio.h>
#include "loom_kernel.h"

int main(int argc, char** argv) {
    (void)argc; (void)argv;
    printf("LOOM kernel test harness\n");
    LoomTopology* topo = loom_init(1024);
    if (!topo) { fprintf(stderr, "Failed to init topology\n"); return 1; }

    uint32_t a = loom_weave_node(topo, "node_a");
    uint32_t b = loom_weave_node(topo, "node_b");
    printf("Weaved nodes: %u, %u\n", a, b);

    uint32_t participants[3] = {a, b, 0};
    uint32_t hid = loom_create_hyperedge(topo, participants, 2, "demo_hyperedge");
    printf("Created hyperedge id: %u\n", hid);

    loom_compute_hyperedge(topo, hid);

    loom_destroy(topo);
    printf("Test harness completed\n");
    return 0;
}
