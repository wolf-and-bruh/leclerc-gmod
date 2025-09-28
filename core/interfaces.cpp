#include "interfaces.hpp"

//thanks to gmod-sdk code, it was hard to me understand how this works
template<typename T>
T* GetVMT(uintptr_t address, int index, uintptr_t offset) // Address must be a VTable pointer, not a VTable !
{
	uintptr_t step = 3;
	uintptr_t instructionSize = 7;
	uintptr_t instruction = ((*(uintptr_t**)(address))[index] + offset);

	uintptr_t relativeAddress = *(DWORD*)(instruction + step);
	uintptr_t realAddress = instruction + instructionSize + relativeAddress;
	return *(T**)(realAddress);
}

template<typename Interface>
Interface* interfaces::Capture(const char* moduleName, const char* interfaceName) noexcept
{
	const auto handle = GetModuleHandleA(moduleName);
	if (!handle) { return nullptr; }

	using CreateInterfaceFn = Interface * (__cdecl*)(const char*, int*);
	const CreateInterfaceFn createInterface = reinterpret_cast<CreateInterfaceFn>(GetProcAddress(handle, "CreateInterface"));

	return createInterface(interfaceName, nullptr);
}

void interfaces::Prepare() noexcept
{
	entityList = Capture<IClientEntityList>("client.dll", "VClientEntityList003");
	client = Capture<IBaseClientDLL>("client.dll", "VClient017");
	globalVars = GetVMT<GlobalVars>(
		reinterpret_cast<uintptr_t>(client),
		0,
		0x94
	);
	clientMode = GetVMT<IClientModeNormal>(
		reinterpret_cast<uintptr_t>(client),
		10,
		0x0
	);
	engine = Capture<CEngineClient>("engine.dll", "VEngineClient015");
	cluaShared = Capture<CLuaShared>("lua_shared.dll", "LUASHARED003");
	debugoverlay = Capture<IVDebugOverlay>("engine.dll", "VDebugOverlay003");
	//clientState = mem::PatternScan("engine.dll", "48 8D 0D ? ? ? ? E9") + 3; // test
	prediction = Capture<IPrediction>("client.dll", "VClientPrediction001");
	trace = Capture<IEngineTrace>("engine.dll", "EngineTraceClient003");
	cvar = Capture<ICVar>("vstdlib.dll", "VEngineCvar007");
	gameevent = Capture<IGameEventManager2>("engine.dll", "GAMEEVENTSMANAGER002");
	gamemovement = Capture<CGameMovement>("client.dll", "GameMovement001");
	matrenderctx = Capture<IMatRenderContext>("materialsystem.dll", "VMaterialSystem080");
	modelrender = Capture<CModelRender>("engine.dll", "VEngineModel016");
	matsystem = Capture<CMaterialSystem>("materialsystem.dll", "VMaterialSystem080");
	movehelper = *reinterpret_cast<IMoveHelper**>(mem::GetAbsAddress(mem::PatternScan("client.dll", "48 8B 0D ?? ?? ?? ?? 48 8B 01 FF 50 ?? 48 8B 03 48 8B CB FF 90 ?? ?? ?? ?? 48 8B 4B")));
}