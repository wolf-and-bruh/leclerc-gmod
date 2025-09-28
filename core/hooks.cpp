#include "hooks.hpp"
#include "../interfaces/cmovehelper.hpp"
#include "../interfaces/cbasecombatweapon.hpp"

using _CreateMove = bool(__thiscall*)(IClientModeNormal*, float, CUserCmd*);
_CreateMove ogCreateMove = nullptr;

using _LuaHook = lua_State * (__cdecl*)(void*, void*);
_LuaHook ogLuaHook = nullptr;

using _ReadPixels = void(__thiscall*)(IMatRenderContext*, int, int, int, int, unsigned char*, ImageFormat);
_ReadPixels ogReadPixels = nullptr;

using _ReadPixelsAndStretch = void(__thiscall*)(IMatRenderContext*, void*, void*, unsigned char*, ImageFormat, int);
_ReadPixelsAndStretch ogReadPixelsAndStretch = nullptr;

using _DrawModelExecute = void(__thiscall*)(CModelRender*, const DrawModelState_t&, const ModelRenderInfo_t&, Matrix3x4*);
_DrawModelExecute ogDrawModelExecute = nullptr;

static float cl_sidespeed = 10000;

CMoveData moveData;

bool CanHit(C_BasePlayer* target, Vector from, Vector to)
{
	if (!localPlayer)
		return false;
	if (!target)
		return false;

	CTrace trace;
	auto filter = new TraceFilterSimple(localPlayer);

	Ray_t Ray;

	Ray.Init(from, to);


	interfaces::trace->TraceRay(Ray, MASK_SHOT, filter, &trace);
	return trace.entity == target || trace.fraction >= 0.98f;
}

bool __stdcall detourCreateMove(void*, float flInputSampleTime, CUserCmd* cmd) {
	localPlayer = (C_BasePlayer*)interfaces::entityList->GetClientEntity(interfaces::engine->GetLocalPlayer());

	const auto result = ogCreateMove(interfaces::clientMode, flInputSampleTime, cmd);
	if (!cmd || !cmd->command_number)
		return result;

	// need flags and is on ground
	if (localPlayer) {
		float oldCurtime = interfaces::globalVars->curtime;
		float oldFrametime = interfaces::globalVars->frametime;

		interfaces::globalVars->curtime = localPlayer->GetTickBase() * interfaces::globalVars->interval_per_tick;
		interfaces::globalVars->frametime = interfaces::globalVars->interval_per_tick;

		config::currentvelocity = localPlayer->GetVelocity().Length();
		//spdlog::default_logger()->info(std::format("{}", interfaces::cvar->FindCommandBase("cl_sidespeed")->intValue));
		if (config::bunnyhop && !(localPlayer->GetFlags() & FL_ONGROUND)){
			cmd->buttons &= ~CUserCmd::IN_JUMP;

			if (config::autostrafe && config::autostrafe_legit) {
				if (!(localPlayer->GetFlags() & FL_ONGROUND))
				{
					if (cmd->mousedx > 0.f)
						cmd->sidemove = 10000.f;
					else if (cmd->mousedx < 0.f) cmd->sidemove = -10000.f;
				}
			}
			else if (config::autostrafe && config::autostrafe_silent) {
				if (!(localPlayer->GetFlags() & FL_ONGROUND))
				{
					if (fabsf(cmd->mousedx > 2)) {
						cmd->sidemove = (cmd->mousedx < 0.f) ? -cl_sidespeed : cl_sidespeed;
					}

					cmd->forwardmove = std::clamp(5850.f / localPlayer->GetVelocity().Length2D(), -99999.f, 99999.f);
					if ((cmd->forwardmove < -99999 || cmd->forwardmove > 99999))
						cmd->forwardmove = 0;

					const auto vel = localPlayer->GetVelocity();
					const float y_vel = rad2deg(atan2(vel.y, vel.x));
					const float diff_ang = normalize_yaw(cmd->viewangles.y - y_vel);

					cmd->sidemove = (diff_ang > 0.0) ? -cl_sidespeed : cl_sidespeed;
					cmd->viewangles.y = normalize_yaw(cmd->viewangles.y - diff_ang);
				}
			}
			else if (config::autostrafe && config::autostrafe_directional) {
				if (!(localPlayer->GetFlags() & FL_ONGROUND || localPlayer->GetFlags() & FL_INRAIN))
				{
					if (fabsf(cmd->mousedx > 2)) {
						cmd->sidemove = (cmd->mousedx < 0.f) ? -cl_sidespeed : cl_sidespeed;
					}

					if (GetAsyncKeyState('S')) {
						cmd->viewangles.y -= 180;
					}
					else if (GetAsyncKeyState('A')) {
						cmd->viewangles.y += 90;
					}
					else if (GetAsyncKeyState('D')) {
						cmd->viewangles.y -= 90;
					}

					if (!localPlayer->GetVelocity().Length2D() > 0.5 || localPlayer->GetVelocity().Length2D() == NAN || localPlayer->GetVelocity().Length2D() == INFINITE)
					{
						cmd->forwardmove = 99999;
					}

					cmd->forwardmove = std::clamp(5850.f / localPlayer->GetVelocity().Length2D(), -99999.f, 99999.f);
					if ((cmd->forwardmove < -99999 || cmd->forwardmove > 99999))
						cmd->forwardmove = 0;

					const auto vel = localPlayer->GetVelocity();
					const float y_vel = rad2deg(atan2(vel.y, vel.x));
					const float diff_ang = normalize_yaw(cmd->viewangles.y - y_vel);

					cmd->sidemove = (diff_ang > 0.0) ? -cl_sidespeed : cl_sidespeed;
					cmd->viewangles.y = normalize_yaw(cmd->viewangles.y - diff_ang);
				}
			}
		}

		if (config::aimbot) {
			interfaces::gamemovement->StartTrackPredictionErrors(localPlayer);

			interfaces::movehelper->SetHost(localPlayer);
			interfaces::prediction->SetupMove(localPlayer, cmd, interfaces::movehelper, &moveData);
			interfaces::gamemovement->ProcessMovement(localPlayer, &moveData);
			interfaces::prediction->FinishMove(localPlayer, cmd, &moveData);

			C_BasePlayer* bestTarget = nullptr;

			double closestDist = 3.402823e+38;
			Vector bestTargetPos{0,0,0};
			bool canHit = false;

			trace_t* trace{};
			Vector eyePos = localPlayer->GetEyePosition();

			for (int i = 0; i < interfaces::globalVars->maxClients; ++i) {
				C_BasePlayer* target = (C_BasePlayer*)interfaces::entityList->GetClientEntity(i);

				if (target == nullptr) continue;
				if (target == localPlayer) continue;
				if (target->IsDormant()) continue;
				if (!target->IsPlayer()) continue;
				if (!target->IsAlive()) continue;

				Matrix3x4 bones[128];
				if (!target->SetupBones(bones, 128, 256, interfaces::globalVars->curtime)) continue;

				Vector bonePos = bones[config::bone].GetOrigin();

				float dist = (bonePos - localPlayer->GetAbsOrigin()).Length();
				if (dist < closestDist) {
					closestDist = dist;
					bestTarget = target;
					bestTargetPos = bonePos;
					canHit = CanHit(target, eyePos, bonePos);
				}
			}

			if (bestTarget) {
				if (canHit) {
					Angle aimAngles = Angle::FromVector((bestTargetPos - eyePos).Scale(100.f));

					if (config::silent) {
						// Silent aim
						cmd->viewangles = aimAngles;
						interfaces::prediction->SetLocalViewAngles(cmd->viewangles);
					}
					else {
						// Normal aim
						cmd->viewangles = aimAngles;
						interfaces::prediction->SetLocalViewAngles(cmd->viewangles);
						interfaces::engine->SetViewAngles(cmd->viewangles);
					}

					if (config::autoshoot) {
						cmd->buttons |= CUserCmd::ButtonFlags::IN_ATTACK;
					}
				}
			}

			interfaces::gamemovement->FinishTrackPredictionErrors(localPlayer);
			interfaces::movehelper->SetHost(0);

			interfaces::globalVars->curtime = oldCurtime;
			interfaces::globalVars->frametime = oldFrametime;
		}
	}

	return false;
}

lua_State* __stdcall detourLuaHook(void* luaAlloc, void* ud) {
	lua_State* luaState = ogLuaHook(luaAlloc, ud);
	config::luastate = luaState;
	spdlog::default_logger()->info("Hooked new LUA state: {}", (void*)luaState);
	return luaState;
}

void __stdcall detourReadPixels(void*, int x, int y, int width, int height, unsigned char* data, ImageFormat dstFormat) {
	spdlog::default_logger()->info("Hooked MatRenderContext::ReadPixels (screengrab)! Width: {} Height: {} Destination format: {}", width, height, (int)dstFormat);

	ogReadPixels(interfaces::matrenderctx, x, y, width, height, data, dstFormat);
}

void __stdcall detourReadPixelsAndStretch(void*, void* pSrcRect, void* pDstRect, unsigned char* pBuffer, ImageFormat dstFormat, int nDstStride) {
	spdlog::default_logger()->info("Hooked MatRenderContext::ReadPixelsAndStretch (screengrab)! dstFormat: {}",(int)(dstFormat));

	ogReadPixelsAndStretch(interfaces::matrenderctx, pSrcRect, pDstRect, pBuffer, dstFormat, nDstStride);
}

void __stdcall detourDrawModelExecute(void*, DrawModelState_t& state, ModelRenderInfo_t& pInfo, Matrix3x4* pCustomBoneToWorld = NULL) {
	//spdlog::default_logger()->info("Hooked DrawModelExecute!");
	//state.
	//interfaces::modelrender->SuppressEngineLighting(true);
	if (pInfo.entity_index) {
		C_BasePlayer* entity = (C_BasePlayer*)interfaces::entityList->GetClientEntity(pInfo.entity_index);

		if (config::chams) {
			if (entity == nullptr) ogDrawModelExecute(interfaces::modelrender, state, pInfo, pCustomBoneToWorld);
			if (entity == localPlayer) ogDrawModelExecute(interfaces::modelrender, state, pInfo, pCustomBoneToWorld);
			//if (!entity->IsPlayer()) ogDrawModelExecute(interfaces::modelrender, state, pInfo, pCustomBoneToWorld);
			//if (!entity->IsAlive()) ogDrawModelExecute(interfaces::modelrender, state, pInfo, pCustomBoneToWorld);

			IMaterial* chamsMat = interfaces::matsystem->FindMaterial("models/wireframe", TEXTURE_GROUP_MODEL);
			chamsMat->AddRef();
			chamsMat->SetMaterialVarFlag(MATERIAL_VAR_SELFILLUM, true);
			chamsMat->SetMaterialVarFlag(MATERIAL_VAR_IGNOREZ, true);

			//implement colors and renderview and inputsystem
			if (std::string(pInfo.pModel->name).contains("models/player/"))
			{
				spdlog::default_logger()->info("Chams on: {}", pInfo.pModel->name);
				interfaces::modelrender->SuppressEngineLighting(true);
				interfaces::modelrender->ForcedMaterialOverride(chamsMat, 0);
			}
			else {
				interfaces::modelrender->SuppressEngineLighting(false);
				interfaces::modelrender->ForcedMaterialOverride(nullptr, 0);
			}
		}
	}
	ogDrawModelExecute(interfaces::modelrender, state, pInfo, pCustomBoneToWorld);
}

void hooks::Init() noexcept
{
	MH_CreateHook(
		mem::Get(interfaces::clientMode, 21),
		&detourCreateMove,
		reinterpret_cast<void**>(&ogCreateMove)
	);

	MH_CreateHook(
		reinterpret_cast<void*>(GetProcAddress(GetModuleHandleA("lua_shared.dll"), "lua_newstate")),
		&detourLuaHook,
		reinterpret_cast<void**>(&ogLuaHook)
	);

	/*MH_CreateHook(
		mem::Get(interfaces::matrenderctx, 12), // 11 or 12 
		&detourReadPixels,
		reinterpret_cast<void**>(&ogReadPixels)
	);

	MH_CreateHook(
		mem::Get(interfaces::matrenderctx, 99), //  99 or 98
		&detourReadPixelsAndStretch,
		reinterpret_cast<void**>(&ogReadPixelsAndStretch)
	);*/

	MH_CreateHook(
		mem::Get(interfaces::modelrender, 20), //  99 or 98
		&detourDrawModelExecute,
		reinterpret_cast<void**>(&ogDrawModelExecute)
	);

	MH_EnableHook(MH_ALL_HOOKS);
}

void hooks::Shutdown() noexcept
{
	MH_DisableHook(MH_ALL_HOOKS);
	MH_RemoveHook(MH_ALL_HOOKS);
}