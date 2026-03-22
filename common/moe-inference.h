#pragma once

#include <cstddef>
#include "expert-profile.h"

#ifdef __cplusplus
extern "C" {
#endif

struct moe_inference_context {
    const struct expert_profile* profile;  // reference to expert profile (non-owning)
    int n_experts;
    int n_layers;           // total MoE layers
    int layer_idx;          // current layer index
};

void moe_inference_init(struct moe_inference_context* ctx, const struct expert_profile* profile, int n_experts, int n_layers);
int moe_get_expert_for_token(struct moe_inference_context* ctx, int token_id, const float* routing_weights, int top_k);
void moe_record_expert_usage(struct moe_inference_context* ctx, int expert_id, int token_count);
void moe_inference_free(struct moe_inference_context* ctx);

#ifdef __cplusplus
}
#endif
