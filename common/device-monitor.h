#pragma once

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Get current load for a device (0.0 to 1.0) */
float get_device_load(int device_id);

/* Check if device is ready to accept new work (e.g., not overheating, has memory) */
int is_device_ready(int device_id);

/* Get total compute capacity of device in TFLOPS (stub) */
float get_device_capacity_tflops(int device_id);

#ifdef __cplusplus
}
#endif
