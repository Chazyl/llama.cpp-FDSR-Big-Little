#pragma once

#include <cstdint>
#include "expert-profile.h"

#ifdef __cplusplus
extern "C" {
#endif

struct expert_profile_db {
    // Opaque database handle from wrapper
    void* db_handle;  // actually sqlite3*, but opaque
    const char* db_path;
};

int expert_profile_db_init(struct expert_profile_db* db, const char* db_path);
int expert_profile_db_save(struct expert_profile_db* db, const struct expert_profile* profile);
int expert_profile_db_load(struct expert_profile_db* db, struct expert_profile* profile);
void expert_profile_db_close(struct expert_profile_db* db);

#ifdef __cplusplus
}
#endif
