#pragma once

#include <cstddef>

#ifdef __cplusplus
extern "C" {
#endif

struct hidden_state_context {
    float* buffer;          // Buffer for hidden state data (owned by caller)
    size_t size;            // Size of buffer in floats
    int layer_id;           // Which layer this state is from
    int token_pos;          // Position in sequence
    void* user_data;        // Optional user context
};

void hidden_state_init(struct hidden_state_context* ctx, float* buffer, size_t size, int layer_id, int token_pos);
const float* hidden_state_get(const struct hidden_state_context* ctx);
float* hidden_state_get_mutable(struct hidden_state_context* ctx);
void hidden_state_set(struct hidden_state_context* ctx, const float* data, size_t size);
void hidden_state_free(struct hidden_state_context* ctx);

#ifdef __cplusplus
}
#endif
