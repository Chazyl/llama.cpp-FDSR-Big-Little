#pragma once
#include <sqlite3.h>

#ifdef __cplusplus
extern "C" {
#endif

// Wrapper functions for SQLite - simple pass-through to SQLite API
// These provide a consistent interface and allow future abstraction if needed.

static inline int sqlite3_wrapper_open(const char *filename, sqlite3 **db) {
    return sqlite3_open(filename, db);
}

static inline int sqlite3_wrapper_exec(sqlite3 *db, const char *sql, sqlite3_callback callback, void *data, char **errmsg) {
    return sqlite3_exec(db, sql, callback, data, errmsg);
}

static inline int sqlite3_wrapper_prepare_v2(sqlite3 *db, const char *sql, int sql_len, sqlite3_stmt **stmt, const char **tail) {
    return sqlite3_prepare_v2(db, sql, sql_len ? sql_len : -1, stmt, tail);
}

static inline int sqlite3_wrapper_step(sqlite3_stmt *stmt) {
    return sqlite3_step(stmt);
}

static inline int sqlite3_wrapper_bind_int(sqlite3_stmt *stmt, int idx, int val) {
    return sqlite3_bind_int(stmt, idx, val);
}

static inline int sqlite3_wrapper_bind_int64(sqlite3_stmt *stmt, int idx, sqlite3_int64 val) {
    return sqlite3_bind_int64(stmt, idx, val);
}

static inline int sqlite3_wrapper_bind_double(sqlite3_stmt *stmt, int idx, double val) {
    return sqlite3_bind_double(stmt, idx, val);
}

static inline const unsigned char* sqlite3_wrapper_column_text(sqlite3_stmt *stmt, int iCol) {
    return sqlite3_column_text(stmt, iCol);
}

static inline int sqlite3_wrapper_column_int(sqlite3_stmt *stmt, int iCol) {
    return sqlite3_column_int(stmt, iCol);
}

static inline sqlite3_int64 sqlite3_wrapper_column_int64(sqlite3_stmt *stmt, int iCol) {
    return sqlite3_column_int64(stmt, iCol);
}

static inline double sqlite3_wrapper_column_double(sqlite3_stmt *stmt, int iCol) {
    return sqlite3_column_double(stmt, iCol);
}

static inline void sqlite3_wrapper_finalize(sqlite3_stmt *stmt) {
    sqlite3_finalize(stmt);
}

#ifdef __cplusplus
}
#endif
