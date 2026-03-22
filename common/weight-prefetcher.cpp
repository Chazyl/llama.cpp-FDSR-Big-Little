// llama.cpp/common/weight-prefetcher.cpp
// Async weight prefetcher stub implementation

#include "weight-prefetcher.h"
#include <cstdlib>
#include <cstring>

struct PrefetchRequest {
    int expert_id;
    int priority;
    // In real implementation: CUDA stream, event, etc.
};

static PrefetchRequest* request_pool = nullptr;
static int pool_capacity = 0;
static int next_free = 0;
static int in_use = 0;

// Simple mutex stub (single-threaded, not needed)
static int prefetcher_initialized = 0;

int prefetcher_init(int n_experts, int device_id) {
    if (prefetcher_initialized) return 0;
    pool_capacity = 4; // Max concurrent prefetches
    request_pool = new PrefetchRequest[pool_capacity];
    memset(request_pool, 0, sizeof(PrefetchRequest) * pool_capacity);
    prefetcher_initialized = 1;
    return 0;
}

prefetch_handle_t prefetcher_start(int expert_id, int priority) {
    if (!prefetcher_initialized) return nullptr;
    // Find a free slot (simple linear scan)
    for (int i = 0; i < pool_capacity; ++i) {
        if (request_pool[i].expert_id == 0) {
            request_pool[i].expert_id = expert_id;
            request_pool[i].priority = priority;
            return (prefetch_handle_t)(&request_pool[i]);
        }
    }
    // Pool full - could block or fail; for stub return nullptr
    return nullptr;
}

void prefetcher_wait(prefetch_handle_t handle) {
    if (!handle) return;
    PrefetchRequest* req = (PrefetchRequest*)handle;
    // In stub, mark as completed by clearing expert_id
    req->expert_id = 0;
}

void prefetcher_cancel(prefetch_handle_t handle) {
    if (!handle) return;
    PrefetchRequest* req = (PrefetchRequest*)handle;
    req->expert_id = 0;
}

void prefetcher_set_callback(prefetch_callback cb, void* user_data) {
    // Not implemented in stub
}

void prefetcher_shutdown(void) {
    if (request_pool) {
        delete[] request_pool;
        request_pool = nullptr;
    }
    prefetcher_initialized = 0;
}
