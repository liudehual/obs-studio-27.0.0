#pragma once

#include "util/c99defs.h"

#ifdef __cplusplus
extern "C" {
#endif

enum obs_nix_platform_type {
	OBS_NIX_PLATFORM_X11_GLX,
	OBS_NIX_PLATFORM_X11_EGL,
#ifdef ENABLE_WAYLAND
	OBS_NIX_PLATFORM_WAYLAND,
#endif

};

/**
 * Sets the Unix platform.
 * @param platform The platform to select.
 */
EXPORT void obs_set_nix_platform(enum obs_nix_platform_type platform);
/**
 * Gets the host platform.
 */
EXPORT enum obs_nix_platform_type obs_get_nix_platform(void);
/**
 * Sets the host platform's display connection.
 * @param display The host display connection.
 */
EXPORT void obs_set_nix_platform_display(void *display);
/**
 * Gets the host platform's display connection.
 */
EXPORT void *obs_get_nix_platform_display(void);

#ifdef __cplusplus
}
#endif
