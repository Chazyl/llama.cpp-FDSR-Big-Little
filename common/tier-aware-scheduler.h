#pragma once

#include "expert-profile.h"
#include "device-monitor.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef void* tier_aware_scheduler_t;

/* Initialize scheduler with expert profile and device count */
tier_aware_scheduler_t tier_aware_scheduler_init(const struct expert_profile* profile, int n_devices);

/* Assign an expert to a device based on tier and device load.
   Returns device_id (0..n_devices-1) or -1 if no device available */
int tier_aware_scheduler_assign_expert(tier_aware_scheduler_t scheduler, int expert_id);

/* Free resources */
void tier_aware_scheduler_free(tier_aware_scheduler_t scheduler);

#ifdef __cplusplus
}
#endif
