#pragma once

#ifdef __cplusplus
extern "C" {
#endif

// Opaque MetaRouter handle
typedef void* meta_router_t;

// Initialize meta-router from weights file. Returns non-NULL on success.
meta_router_t meta_router_init(const char* weights_path, int n_experts);

// Run inference: hidden_state is an array of length hidden_dim (must match training).
// out_probs: output array of length n_experts, will be normalized to sum to 1.
void meta_router_predict(meta_router_t router, const float* hidden_state, int hidden_dim, float* out_probs);

// Free resources
void meta_router_free(meta_router_t router);

#ifdef __cplusplus
}
#endif
