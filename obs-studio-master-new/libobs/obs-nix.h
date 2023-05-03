
#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "obs-internal.h"

struct obs_nix_hotkeys_vtable {
	bool (*init)(struct obs_core_hotkeys *hotkeys);

	void (*free)(struct obs_core_hotkeys *hotkeys);

	bool (*is_pressed)(obs_hotkeys_platform_t *context, obs_key_t key);

	void (*key_to_str)(obs_key_t key, struct dstr *dstr);

	obs_key_t (*key_from_virtual_key)(int sym);

	int (*key_to_virtual_key)(obs_key_t key);
};

#ifdef __cplusplus
}
#endif
