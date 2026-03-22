// llama.cpp/common/inference-hooks.cpp
// Hook infrastructure implementation

#include "inference-hooks.h"
#include <cstring>

// Maximum hooks per type
#define MAX_HOOKS_PER_TYPE 64

struct hook_entry {
    void (*fn)(void*, struct hidden_state_context*, void*);
    void* user_data;
};

// Separate arrays for each hook type
static hook_entry g_pre_attention[MAX_HOOKS_PER_TYPE];
static int g_n_pre_attention = 0;

static hook_entry g_post_attention[MAX_HOOKS_PER_TYPE];
static int g_n_post_attention = 0;

static hook_entry g_pre_feedforward[MAX_HOOKS_PER_TYPE];
static int g_n_pre_feedforward = 0;

static hook_entry g_post_feedforward[MAX_HOOKS_PER_TYPE];
static int g_n_post_feedforward = 0;

int hook_register_pre_attention(hook_fn_pre_attention fn, void* user_data) {
    if (g_n_pre_attention >= MAX_HOOKS_PER_TYPE) return -1;
    g_pre_attention[g_n_pre_attention].fn = reinterpret_cast<void (*)(void*, struct hidden_state_context*, void*)>(fn);
    g_pre_attention[g_n_pre_attention].user_data = user_data;
    g_n_pre_attention++;
    return 0;
}

int hook_register_post_attention(hook_fn_post_attention fn, void* user_data) {
    if (g_n_post_attention >= MAX_HOOKS_PER_TYPE) return -1;
    g_post_attention[g_n_post_attention].fn = reinterpret_cast<void (*)(void*, struct hidden_state_context*, void*)>(fn);
    g_post_attention[g_n_post_attention].user_data = user_data;
    g_n_post_attention++;
    return 0;
}

int hook_register_pre_feedforward(hook_fn_pre_feedforward fn, void* user_data) {
    if (g_n_pre_feedforward >= MAX_HOOKS_PER_TYPE) return -1;
    g_pre_feedforward[g_n_pre_feedforward].fn = reinterpret_cast<void (*)(void*, struct hidden_state_context*, void*)>(fn);
    g_pre_feedforward[g_n_pre_feedforward].user_data = user_data;
    g_n_pre_feedforward++;
    return 0;
}

int hook_register_post_feedforward(hook_fn_post_feedforward fn, void* user_data) {
    if (g_n_post_feedforward >= MAX_HOOKS_PER_TYPE) return -1;
    g_post_feedforward[g_n_post_feedforward].fn = reinterpret_cast<void (*)(void*, struct hidden_state_context*, void*)>(fn);
    g_post_feedforward[g_n_post_feedforward].user_data = user_data;
    g_n_post_feedforward++;
    return 0;
}

void hook_invoke_pre_attention(llama_hook_context_t ctx, struct hidden_state_context* state) {
    for (int i = 0; i < g_n_pre_attention; ++i) {
        auto fn = reinterpret_cast<hook_fn_pre_attention>(g_pre_attention[i].fn);
        fn(ctx, state, g_pre_attention[i].user_data);
    }
}

void hook_invoke_post_attention(llama_hook_context_t ctx, struct hidden_state_context* state) {
    for (int i = 0; i < g_n_post_attention; ++i) {
        auto fn = reinterpret_cast<hook_fn_post_attention>(g_post_attention[i].fn);
        fn(ctx, state, g_post_attention[i].user_data);
    }
}

void hook_invoke_pre_feedforward(llama_hook_context_t ctx, struct hidden_state_context* state) {
    for (int i = 0; i < g_n_pre_feedforward; ++i) {
        auto fn = reinterpret_cast<hook_fn_pre_feedforward>(g_pre_feedforward[i].fn);
        fn(ctx, state, g_pre_feedforward[i].user_data);
    }
}

void hook_invoke_post_feedforward(llama_hook_context_t ctx, struct hidden_state_context* state) {
    for (int i = 0; i < g_n_post_feedforward; ++i) {
        auto fn = reinterpret_cast<hook_fn_post_feedforward>(g_post_feedforward[i].fn);
        fn(ctx, state, g_post_feedforward[i].user_data);
    }
}

void hook_clear_all(void) {
    g_n_pre_attention = 0;
    g_n_post_attention = 0;
    g_n_pre_feedforward = 0;
    g_n_post_feedforward = 0;
}
