// llama.cpp/common/expert-profile-db.cpp
// Expert profile database implementation with SQLite

#include "expert-profile-db.h"
#include "sqlite3-wrapper.h"
#include <cstring>
#include <cstdio>

int expert_profile_db_init(struct expert_profile_db* db, const char* db_path) {
    if (!db || !db_path) return -1;
    
    sqlite3* raw_db = nullptr;
    int rc = sqlite3_wrapper_open(db_path, &raw_db);
    if (rc != SQLITE_OK) {
        return rc;
    }

    db->db_handle = raw_db;
    db->db_path = db_path;

    // Create table if not exists
    const char* create_sql =
        "CREATE TABLE IF NOT EXISTS expert_profiles ("
        "expert_id INTEGER PRIMARY KEY,"
        "activation_count INTEGER NOT NULL,"
        "total_tokens INTEGER NOT NULL"
        ");";
    
    char* errmsg = nullptr;
    rc = sqlite3_wrapper_exec(raw_db, create_sql, nullptr, nullptr, &errmsg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQLite error (create table): %s\n", errmsg);
        sqlite3_free(errmsg);
        sqlite3_close(raw_db);
        return rc;
    }

    return SQLITE_OK;
}

int expert_profile_db_save(struct expert_profile_db* db, const struct expert_profile* profile) {
    if (!db || !db->db_handle || !profile) return -1;

    sqlite3* raw_db = static_cast<sqlite3*>(db->db_handle);
    int rc;

    // Begin transaction
    rc = sqlite3_wrapper_exec(raw_db, "BEGIN TRANSACTION", nullptr, nullptr, nullptr);
    if (rc != SQLITE_OK) return rc;

    for (int i = 0; i < profile->n_experts; ++i) {
        // Use INSERT OR REPLACE to upsert
        sqlite3_stmt* stmt = nullptr;
        const char* sql = "INSERT OR REPLACE INTO expert_profiles (expert_id, activation_count, total_tokens) VALUES (?, ?, ?);";
        rc = sqlite3_wrapper_prepare_v2(raw_db, sql, -1, &stmt, nullptr);
        if (rc != SQLITE_OK) break;

        sqlite3_wrapper_bind_int(stmt, 1, i);
        sqlite3_wrapper_bind_int64(stmt, 2, profile->activation_counts[i]);
        sqlite3_wrapper_bind_int64(stmt, 3, profile->total_tokens);
        rc = sqlite3_wrapper_step(stmt);
        sqlite3_wrapper_finalize(stmt);
        if (rc != SQLITE_DONE) break;
    }

    if (rc == SQLITE_DONE || rc == SQLITE_OK) {
        rc = sqlite3_wrapper_exec(raw_db, "COMMIT", nullptr, nullptr, nullptr);
    } else {
        sqlite3_wrapper_exec(raw_db, "ROLLBACK", nullptr, nullptr, nullptr);
    }

    return rc;
}

int expert_profile_db_load(struct expert_profile_db* db, struct expert_profile* profile) {
    if (!db || !db->db_handle || !profile) return -1;

    sqlite3* raw_db = static_cast<sqlite3*>(db->db_handle);
    int rc;

    // Reset profile first
    if (profile->activation_counts) {
        memset(profile->activation_counts, 0, profile->n_experts * sizeof(int64_t));
    }
    profile->total_tokens = 0;

    const char* sql = "SELECT expert_id, activation_count FROM expert_profiles;";
    sqlite3_stmt* stmt = nullptr;
    rc = sqlite3_wrapper_prepare_v2(raw_db, sql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) return rc;

    while ((rc = sqlite3_wrapper_step(stmt)) == SQLITE_ROW) {
        int expert_id = sqlite3_wrapper_column_int(stmt, 0);
        sqlite3_int64 count = sqlite3_wrapper_column_int64(stmt, 1);
        if (expert_id >= 0 && expert_id < profile->n_experts) {
            profile->activation_counts[expert_id] = count;
        }
    }
    sqlite3_wrapper_finalize(stmt);

    if (rc != SQLITE_DONE && rc != SQLITE_ROW) {
        return rc;
    }

    // Sum total tokens for convenience
    for (int i = 0; i < profile->n_experts; ++i) {
        profile->total_tokens += profile->activation_counts[i];
    }

    return SQLITE_OK;
}

void expert_profile_db_close(struct expert_profile_db* db) {
    if (db && db->db_handle) {
        sqlite3_close(static_cast<sqlite3*>(db->db_handle));
        db->db_handle = nullptr;
    }
}
