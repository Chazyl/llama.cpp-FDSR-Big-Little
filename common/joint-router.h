#pragma once

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef void* joint_router_t;

/* Initialize joint router with correlation data */
joint_router_t joint_router_init(const char* correlation_data_path);

/* Make joint routing decision for a set of layers
   Returns array of expert IDs (one per layer) and count via out_count */
int joint_router_decide(joint_router_t router, 
                       int* layer_indices, int n_layers,
                       float* hidden_states,  /* [n_layers * hidden_dim] */
                       int* out_expert_ids, int out_capacity, int* out_count);

/* Free resources */
void joint_router_free(joint_router_t router);

#ifdef __cplusplus
}
#endif
