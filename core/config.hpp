#pragma once
#include "../includes.hpp"

namespace config {
	// movement
	inline bool bunnyhop = false;
	inline bool autostrafe = false;
	inline bool autostrafe_legit = true;
	inline bool autostrafe_silent = false;
	inline bool autostrafe_directional = false;

	// aiming
	inline bool aimbot = false;
	inline bool silent = false;
	inline bool autoshoot = false;
	inline int bone = Bones::bone_head;
	inline float fov = 100;

	// esp
	inline bool snapline = false;
	inline bool squareesp = false;
	inline bool chams = false;
	inline float snapline_color[3] = { 0.f, 0.f, 0.f };
	inline float squareesp_color[3] = { 0.f, 0.f, 0.f };

	// misc
	inline bool chatondeath = false;
	inline bool allowcslua = false;
	inline bool allowcheats = false;

	// access
	inline float currentvelocity = 0.f;
	inline lua_State* luastate = nullptr;
}