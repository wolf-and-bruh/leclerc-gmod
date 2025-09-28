#pragma once
#include "../includes.hpp"

enum Bones : int {
	bone_head = 6,
	bone_neck = 5,
	bone_spine_1 = 4,
	bone_spine_2 = 2,
	bone_pelvis = 0,
	bone_arm_top_l = 8,
	bone_arm_top_r = 13,
	bone_arm_bot_l = 9,
	bone_arm_bot_r = 14,
	bone_hand_l = 10,
	bone_hand_r = 15,
	bone_leg_top_l = 22,
	bone_leg_top_r = 25,
	bone_leg_bot_l = 23,
	bone_leg_bot_r = 26,
	bone_ANKLE_l = 24,
	bone_ANKLE_r = 27,
};

class C_BasePlayer {
public:

	const int& GetHealth() noexcept {
		return *reinterpret_cast<int*>(uintptr_t(this) + 0xC8);
	}

	const int& GetFlags() noexcept {
		return *reinterpret_cast<int*>(uintptr_t(this) + 0x440);
	}

	const Vector& GetAbsOrigin() noexcept {
		return mem::Call<const Vector&>(this, 9);
	}

	const bool IsAlive() noexcept {
		return mem::Call<bool>(this, 130);
	}

	const bool IsPlayer() noexcept {
		return mem::Call<bool>(this, 131);
	}

	const Vector& GetViewOffset() noexcept {
		return *reinterpret_cast<Vector*>(uintptr_t(this) + 0x13C);
	}

	const Vector& GetVelocity() noexcept {
		return *reinterpret_cast<Vector*>(uintptr_t(this) + 0x148);
	}

	constexpr bool SetupBones(Matrix3x4* pBoneToWorldOut, int nMaxBones, int boneMask, float currentTime) noexcept {
		return mem::Call<bool>(this + 0x8, 16, pBoneToWorldOut, nMaxBones, boneMask, currentTime);
	}

	constexpr void SetMaterialOverridePointer(void* mat) noexcept {
		mem::Call<void>(this, 161, mat);
	}

	const bool IsDormant(void) noexcept {
		return *reinterpret_cast<bool*>(uintptr_t(this) + 0x1FA);
	}

	const Vector& GetEyePosition() noexcept {
		return GetAbsOrigin() + GetViewOffset();
	}

	const int GetTickBase() noexcept {
		return *reinterpret_cast<int*>(uintptr_t(this) + 0x2D48);
	}

	const Angle& GetViewPunch() noexcept {
		return *reinterpret_cast<Angle*>(uintptr_t(this) + 0x2A00);
	}

	const Angle& GetAimPunch() noexcept {
		return *reinterpret_cast<Angle*>(uintptr_t(this) + 0x250C);
	}

	const void* GetActiveWeapon() noexcept {
		return mem::Call<void*>(this, 280);
	}
};

inline C_BasePlayer* localPlayer = nullptr;