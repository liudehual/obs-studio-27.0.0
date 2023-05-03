#include "obs-internal.h"
#include "obs-nix-platform.h"
#include "obs-nix-wayland.h"

#include <wayland-client.h>

void obs_nix_wayland_log_info(void)
{
	struct wl_display *display = obs_get_nix_platform_display();
	if (display == NULL) {
		blog(LOG_INFO, "Unable to connect to Wayland server");
		return;
	}
	//TODO: query some information about the wayland server if possible
	blog(LOG_INFO, "Connected to Wayland server");
}

static bool
obs_nix_wayland_hotkeys_platform_init(struct obs_core_hotkeys *hotkeys)
{
	UNUSED_PARAMETER(hotkeys);
	return true;
}

static void
obs_nix_wayland_hotkeys_platform_free(struct obs_core_hotkeys *hotkeys)
{
	UNUSED_PARAMETER(hotkeys);
}

static bool
obs_nix_wayland_hotkeys_platform_is_pressed(obs_hotkeys_platform_t *context,
					    obs_key_t key)
{
	UNUSED_PARAMETER(context);
	UNUSED_PARAMETER(key);
	return false;
}

static void obs_nix_wayland_key_to_str(obs_key_t key, struct dstr *dstr)
{
	UNUSED_PARAMETER(key);
	UNUSED_PARAMETER(dstr);
}

static obs_key_t obs_nix_wayland_key_from_virtual_key(int sym)
{
	UNUSED_PARAMETER(sym);
	return OBS_KEY_NONE;
}

static int obs_nix_wayland_key_to_virtual_key(obs_key_t key)
{
	UNUSED_PARAMETER(key);
	return 0;
}

static const struct obs_nix_hotkeys_vtable wayland_hotkeys_vtable = {
	.init = obs_nix_wayland_hotkeys_platform_init,
	.free = obs_nix_wayland_hotkeys_platform_free,
	.is_pressed = obs_nix_wayland_hotkeys_platform_is_pressed,
	.key_to_str = obs_nix_wayland_key_to_str,
	.key_from_virtual_key = obs_nix_wayland_key_from_virtual_key,
	.key_to_virtual_key = obs_nix_wayland_key_to_virtual_key,

};

const struct obs_nix_hotkeys_vtable *obs_nix_wayland_get_hotkeys_vtable(void)
{
	return &wayland_hotkeys_vtable;
}
