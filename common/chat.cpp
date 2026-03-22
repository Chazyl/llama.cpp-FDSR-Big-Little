#include "chat.h"

int chat_completion_create(void *ctx, const chat_message *messages, int n_messages, chat_message *response, int n_response_max, const llama_sampling_params *params) {
    // Stub implementation for foundational phase
    if (response && n_response_max > 0) {
        static const char *role = "assistant";
        static const char *content = "Hello from FDSR-Big-Little! This is a stub response.";
        response->role = role;
        response->content = content;
    }
    return 0;
}
