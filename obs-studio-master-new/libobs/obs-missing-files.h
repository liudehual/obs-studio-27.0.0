
#pragma once

#include "util/c99defs.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef void (*obs_missing_file_cb)(void *src, const char *new_path,
				    void *data);

struct obs_missing_file;
struct obs_missing_files;
typedef struct obs_missing_file obs_missing_file_t;
typedef struct obs_missing_files obs_missing_files_t;

enum obs_missing_file_src { OBS_MISSING_FILE_SOURCE, OBS_MISSING_FILE_SCRIPT };

EXPORT obs_missing_files_t *obs_missing_files_create();
EXPORT obs_missing_file_t *obs_missing_file_create(const char *path,
						   obs_missing_file_cb callback,
						   int src_type, void *src,
						   void *data);

EXPORT void obs_missing_files_add_file(obs_missing_files_t *files,
				       obs_missing_file_t *file);
EXPORT size_t obs_missing_files_count(obs_missing_files_t *files);
EXPORT obs_missing_file_t *
obs_missing_files_get_file(obs_missing_files_t *files, int idx);
EXPORT void obs_missing_files_destroy(obs_missing_files_t *files);
EXPORT void obs_missing_files_append(obs_missing_files_t *dst,
				     obs_missing_files_t *src);

EXPORT void obs_missing_file_issue_callback(obs_missing_file_t *file,
					    const char *new_path);
EXPORT const char *obs_missing_file_get_path(obs_missing_file_t *file);
EXPORT const char *obs_missing_file_get_source_name(obs_missing_file_t *file);
EXPORT void obs_missing_file_release(obs_missing_file_t *file);
EXPORT void obs_missing_file_destroy(obs_missing_file_t *file);

#ifdef __cplusplus
}
#endif
