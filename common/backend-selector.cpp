// llama.cpp/common/backend-selector.cpp
// Backend selector stub

#include "backend-selector.h"
#include <cstdlib>
#include <cstring>

// Simple state
struct BackendSelectorState {
    // In real impl: array of backend handles, etc.
    int dummy;
};

backend_handle_t backend_selector_init() {
    BackendSelectorState* state = new BackendSelectorState();
    state->dummy = 0;
    return (backend_handle_t)state;
}

backend_handle_t backend_selector_select(int expert_id, int tier, int device_id) {
    // Stub: always return the same handle (non-null) for simplicity
    // In real impl: would select backend based on tier and device_id
    static BackendSelectorState global_state;
    return (backend_handle_t)&global_state;
}

void backend_selector_free(backend_handle_t selector) {
    // Stub: if it's our static global, do nothing; otherwise delete
    // For simplicity, we assume it's not the static one in this stub.
    // In a real implementation, we would track ownership.
    // Since we are returning a pointer to a static global in select, we should not delete it.
    // To avoid complexity, we'll just do nothing in this stub.
    // Note: This is a limitation of the stub.
}
