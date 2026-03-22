#ifndef LLAMA_FDSR_H
#define LLAMA_FDSR_H

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Initialize FDSR subsystem.
   Returns a handle on success, NULL on failure. */
void* llama_fdsr_init(const char* model_path, const char* fdsr_config_path);

/* Predict expert activation for the next layer given hidden state.
   hidden_state: array of length hidden_dim (must match model).
   out_probs: output array of length n_experts (will be normalized).
   Returns 0 on success, -1 on error. */
int llama_fdsr_predict(void* handle, const float* hidden_state, int hidden_dim, float* out_probs, int n_experts);

/* Free resources */
void llama_fdsr_free(void* handle);

#ifdef __cplusplus
}
#endif

#endif /* LLAMA_FDSR_H */