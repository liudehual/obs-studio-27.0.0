#include "obs-nix-platform.h"

static enum obs_nix_platform_type obs_nix_platform = OBS_NIX_PLATFORM_X11_GLX;

static void *obs_nix_platform_display = NULL;

void obs_set_nix_platform(enum obs_nix_platform_type platform)
{
	obs_nix_platform = platform;
}

enum obs_nix_platform_type obs_get_nix_platform(void)
{
	return obs_nix_platform;
}

void obs_set_nix_platform_display(void *display)
{
	obs_nix_platform_display = display;
}

void *obs_get_nix_platform_display(void)
{
	return obs_nix_platform_display;
}
