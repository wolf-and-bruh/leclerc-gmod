#pragma once
#include "../includes.hpp"

namespace interfaces {
	inline IClientEntityList* entityList = nullptr;
	inline IBaseClientDLL* client = nullptr;
	inline GlobalVars* globalVars = nullptr;
	inline IClientModeNormal* clientMode = nullptr;
	inline CEngineClient* engine = nullptr;
	inline CLuaShared* cluaShared = nullptr;
	inline IVDebugOverlay* debugoverlay = nullptr;
	inline void* clientState = nullptr;
	inline IPrediction* prediction = nullptr;
	inline IEngineTrace* trace = nullptr;
	inline ICVar* cvar = nullptr;
	inline IGameEventManager2* gameevent = nullptr;
	inline CGameMovement* gamemovement = nullptr;
	inline IMatRenderContext* matrenderctx = nullptr;
	inline CModelRender* modelrender = nullptr;
	inline CMaterialSystem* matsystem = nullptr;
	inline IMoveHelper* movehelper = nullptr;

	template <typename T>
	T* Capture(const char* moduleName, const char* interfaceName) noexcept;

	void Prepare() noexcept;
}