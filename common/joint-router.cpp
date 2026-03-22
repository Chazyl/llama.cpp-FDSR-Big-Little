// llama.cpp/common/joint-router.cpp
// Joint routing decision stub

#include "joint-router.h"
#include <cstdlib>
#include <cstring>

// Simple state
struct JointRouterState {
    // In real impl: correlation matrix, etc.
    int dummy;
};

joint_router_t joint_router_init(const char* correlation_data_path) {
    // Stub: just allocate state
    JointRouterState* state = new JointRouterState();
    state->dummy = 0;
    return (joint_router_t)state;
}

int joint_router_decide(joint_router_t router, 
                       int* layer_indices, int n_layers,
                       float* hidden_states,  /* [n_layers * hidden_dim] */
                       int* out_expert_ids, int out_capacity, int* out_count) {
    // Stub: return expert 0 for all layers
    int n = (n_layers < out_capacity) ? n_layers : out_capacity;
    for (int i = 0; i < n; ++i) {
        out_expert_ids[i] = 0;
    }
    *out_count = n;
    return 0; // success
}

void joint_router_free(joint_router_t router) {
    JointRouterState* state = (JointRouterState*)router;
    delete state;
}
