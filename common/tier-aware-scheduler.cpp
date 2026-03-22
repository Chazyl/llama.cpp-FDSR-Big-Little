// llama.cpp/common/tier-aware-scheduler.cpp
// Tier-aware scheduler stub

#include "tier-aware-scheduler.h"
#include <cstdlib>
#include <cstring>

// Simple state
struct TierAwareSchedulerState {
    const struct expert_profile* profile;
    int n_devices;
    // In real impl: per-device load, etc.
    float* device_load;
};

tier_aware_scheduler_t tier_aware_scheduler_init(const struct expert_profile* profile, int n_devices) {
    TierAwareSchedulerState* state = new TierAwareSchedulerState();
    state->profile = profile;
    state->n_devices = n_devices;
    state->device_load = new float[n_devices];
    for (int i = 0; i < n_devices; ++i) {
        state->device_load[i] = 0.0f; // start idle
    }
    return (tier_aware_scheduler_t)state;
}

int tier_aware_scheduler_assign_expert(tier_aware_scheduler_t scheduler, int expert_id) {
    TierAwareSchedulerState* state = (TierAwareSchedulerState*)scheduler;
    // Stub: just assign to device 0 if available, else -1
    if (state->n_devices > 0) {
        // In real impl: consider expert tier and device load
        return 0;
    }
    return -1;
}

void tier_aware_scheduler_free(tier_aware_scheduler_t scheduler) {
    TierAwareSchedulerState* state = (TierAwareSchedulerState*)scheduler;
    if (state->device_load) {
        delete[] state->device_load;
    }
    delete state;
}
