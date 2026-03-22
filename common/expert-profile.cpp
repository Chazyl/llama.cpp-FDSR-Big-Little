// llama.cpp/common/expert-profile.cpp
// Expert profiling and classification implementation

#include "expert-profile.h"
#include <cstring>
#include <algorithm>
#include <vector>
#include <utility>

void expert_profile_init(struct expert_profile* profile, int n_experts, float hot_thresh, float warm_thresh) {
    profile->n_experts = n_experts;
    profile->hot_threshold = hot_thresh;
    profile->warm_threshold = warm_thresh;
    profile->total_tokens = 0;
    profile->activation_counts = new int64_t[n_experts];
    memset(profile->activation_counts, 0, n_experts * sizeof(int64_t));
}

void expert_profile_free(struct expert_profile* profile) {
    if (profile->activation_counts) {
        delete[] profile->activation_counts;
        profile->activation_counts = nullptr;
    }
}

void expert_profile_record_activation(struct expert_profile* profile, int expert_id, int token_count) {
    if (expert_id >= 0 && expert_id < profile->n_experts) {
        profile->activation_counts[expert_id] += token_count;
        profile->total_tokens += token_count;
    }
}

float expert_profile_get_activation_frequency(const struct expert_profile* profile, int expert_id) {
    if (expert_id >= 0 && expert_id < profile->n_experts && profile->total_tokens > 0) {
        return static_cast<float>(profile->activation_counts[expert_id]) / profile->total_tokens;
    }
    return 0.0f;
}

void expert_profile_classify_experts(const struct expert_profile* profile, int* expert_types) {
    if (!profile->n_experts) return;

    // Create array of (expert_id, frequency) pairs
    std::vector<std::pair<int, float>> freqs;
    for (int i = 0; i < profile->n_experts; ++i) {
        float freq = expert_profile_get_activation_frequency(profile, i);
        freqs.emplace_back(i, freq);
    }

    // Sort by frequency descending
    std::sort(freqs.begin(), freqs.end(), [](const auto& a, const auto& b) {
        return a.second > b.second;
    });

    // Determine tier boundaries
    int n_hot = static_cast<int>(profile->n_experts * profile->hot_threshold);
    int n_warm = static_cast<int>(profile->n_experts * profile->warm_threshold);
    n_hot = std::max(1, n_hot);
    n_warm = std::max(0, n_warm);
    n_warm = std::min(n_warm, profile->n_experts - n_hot);

    // Assign types: 2=hot, 1=warm, 0=cold
    for (int i = 0; i < profile->n_experts; ++i) {
        expert_types[i] = 0;
    }
    for (int i = 0; i < n_hot; ++i) {
        expert_types[freqs[i].first] = 2;
    }
    for (int i = n_hot; i < n_hot + n_warm; ++i) {
        expert_types[freqs[i].first] = 1;
    }
}

void expert_profile_reset(struct expert_profile* profile) {
    if (profile->activation_counts) {
        memset(profile->activation_counts, 0, profile->n_experts * sizeof(int64_t));
    }
    profile->total_tokens = 0;
}
