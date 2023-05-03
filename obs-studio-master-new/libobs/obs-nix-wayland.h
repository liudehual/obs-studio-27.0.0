
#pragma once

#include "obs-nix.h"

void obs_nix_wayland_log_info(void);

const struct obs_nix_hotkeys_vtable *obs_nix_wayland_get_hotkeys_vtable(void);
