#ifndef CHAT_H
#define CHAT_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    const char * role;
    const char * content;
} chat_message;

typedef struct {
    float temperature;
    int max_tokens;
} llama_sampling_params;

int chat_completion_create(
    void *ctx,
    const chat_message * messages,
    int n_messages,
    chat_message * response,
    int n_response_max,
    const llama_sampling_params * params);

#ifdef __cplusplus
}
#endif

#endif
