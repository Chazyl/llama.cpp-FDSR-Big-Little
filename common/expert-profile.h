#pragma once

#include <cstddef>
#include <cstdint>

#ifdef __cplusplus
extern "C" {
#endif

struct expert_profile {
    int64_t* activation_counts;  // per-expert activation count (allocated size: n_experts)
    int64_t total_tokens;        // total tokens processed
    int n_experts;               // number of experts
    float hot_threshold;         // fraction for hot experts (e.g., 0.2)
    float warm_threshold;        // fraction for warm experts (e.g., 0.1)
};

void expert_profile_init(struct expert_profile* profile, int n_experts, float hot_thresh, float warm_thresh);
void expert_profile_free(struct expert_profile* profile);
void expert_profile_record_activation(struct expert_profile* profile, int expert_id, int token_count);
void expert_profile_classify_experts(const struct expert_profile* profile, int* expert_types); // output: 0=cold,1=warm,2=hot
float expert_profile_get_activation_frequency(const struct expert_profile* profile, int expert_id);
void expert_profile_reset(struct expert_profile* profile);

#ifdef __cplusplus
}
#endif
