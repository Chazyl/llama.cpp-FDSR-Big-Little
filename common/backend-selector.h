#pragma once

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef void* backend_handle_t;

/* Initialize backend selector */
backend_handle_t backend_selector_init();

/* Select backend for an expert based on tier and device capabilities.
   Returns handle to backend (opaque) or NULL if none available */
backend_handle_t backend_selector_select(int expert_id, int tier, int device_id);

/* Free resources */
void backend_selector_free(backend_handle_t selector);

#ifdef __cplusplus
}
#endif
