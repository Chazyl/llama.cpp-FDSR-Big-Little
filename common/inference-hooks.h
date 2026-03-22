#pragma once

#include <stddef.h>
#include "hidden-state.h"

#ifdef __cplusplus
extern "C" {
#endif

// Hook function types for different inference stages
typedef void (*hook_fn_pre_attention)(void* llama_ctx, struct hidden_state_context* state, void* user_data);
typedef void (*hook_fn_post_attention)(void* llama_ctx, struct hidden_state_context* state, void* user_data);
typedef void (*hook_fn_pre_feedforward)(void* llama_ctx, struct hidden_state_context* state, void* user_data);
typedef void (*hook_fn_post_feedforward)(void* llama_ctx, struct hidden_state_context* state, void* user_data);

// Opaque context for llama (use void* to avoid heavy dependencies here)
typedef void* llama_hook_context_t;

// Registration functions - return 0 on success, -1 on error (e.g., no space)
int hook_register_pre_attention(hook_fn_pre_attention fn, void* user_data);
int hook_register_post_attention(hook_fn_post_attention fn, void* user_data);
int hook_register_pre_feedforward(hook_fn_pre_feedforward fn, void* user_data);
int hook_register_post_feedforward(hook_fn_post_feedforward fn, void* user_data);

// Hook invocation functions - called from inference loop
void hook_invoke_pre_attention(llama_hook_context_t ctx, struct hidden_state_context* state);
void hook_invoke_post_attention(llama_hook_context_t ctx, struct hidden_state_context* state);
void hook_invoke_pre_feedforward(llama_hook_context_t ctx, struct hidden_state_context* state);
void hook_invoke_post_feedforward(llama_hook_context_t ctx, struct hidden_state_context* state);

// Hook management: clear all registered hooks (optional, for testing/reset)
void hook_clear_all(void);

#ifdef __cplusplus
}
#endif
