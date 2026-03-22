// llama.cpp/common/moe-inference.cpp
// MoE inference implementation integrated with FDSR components

#include "moe-inference.h"
#include "expert-profile.h"
#include "meta-router.h"
#include "confidence-gate.h"
#include "fdsr.h"
#include "top2-fallback.h"
#include "expert-dispatcher.h"
#include "weight-prefetcher.h"

#include <cstring>

// Global prefetcher state for simple single-threaded inference
static prefetch_handle_t g_prefetch_handle = nullptr;

void moe_inference_init(struct moe_inference_context* ctx, const struct expert_profile* profile, int n_experts, int n_layers) {
    ctx->profile = profile;
    ctx->n_experts = n_experts;
    ctx->n_layers = n_layers;
    ctx->layer_idx = 0;

    // Initialize the expert dispatcher
    dispatcher_init(n_experts, n_layers);
}

int moe_get_expert_for_token(struct moe_inference_context* ctx, int token_id, const float* routing_weights, int top_k) {
    // Current layer index
    int layer_idx = ctx->layer_idx;

    // 1. Get hidden state for this token at this layer.
    // In a full implementation, this would be passed from the caller.
    // Stub: we'll allocate a zeroed array.
    float hidden_state[512] = {0};

    // 2. Ask meta-router for predictions (top-2)
    // In a real system, we would call meta_router_predict_topk with the actual hidden state.
    // Here we use placeholder values.
    int predicted_experts[2] = {0, 1};
    float probs[2] = {0.6f, 0.4f};

    // 3. Confidence gating
    float confidence = probs[0];
    float threshold = 0.7f; // default
    bool use_speculation = (confidence >= threshold);

    // 4. FDSR: if we are at layer k and want to speculate for layer k+1, initiate prefetch
    if (use_speculation && layer_idx < ctx->n_layers - 1) {
        // Trigger prefetch for next layer's expert (top-1)
        int next_expert = predicted_experts[0];
        g_prefetch_handle = prefetcher_start(next_expert, 1);
        // Record prediction for later verification
        fdsr_record_prediction(token_id, next_expert);
    }

    // 5. Wait for prefetch if any, or fallback
    int chosen_expert = -1;
    if (g_prefetch_handle) {
        prefetcher_wait(g_prefetch_handle);
        g_prefetch_handle = nullptr;
        chosen_expert = predicted_experts[0];
    } else {
        // Use dispatcher with fallback logic (top-2)
        int out_experts[2];
        int out_count = 0;
        dispatcher_route_token(layer_idx, token_id, hidden_state, out_experts, &out_count, 2);
        chosen_expert = out_experts[0];
    }

    // 6. Grade prediction (actual expert chosen)
    fdsr_record_actual(layer_idx, token_id, chosen_expert);

    // 7. Record usage
    expert_profile_record_activation(const_cast<struct expert_profile*>(ctx->profile), chosen_expert, 1);

    // Advance layer index for next token
    ctx->layer_idx = (ctx->layer_idx + 1) % ctx->n_layers;

    return chosen_expert;
}

void moe_record_expert_usage(struct moe_inference_context* ctx, int expert_id, int token_count) {
    if (ctx->profile) {
        expert_profile_record_activation(const_cast<struct expert_profile*>(ctx->profile), expert_id, token_count);
    }
    // For consistency, also advance layer (though moe_get_expert_for_token already does)
    ctx->layer_idx = (ctx->layer_idx + 1) % ctx->n_layers;
}

void moe_inference_free(struct moe_inference_context* ctx) {
    prefetcher_shutdown();
    ctx->profile = nullptr;
}
