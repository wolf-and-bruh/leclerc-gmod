#include "lua_loader.hpp"

#define lua_pop(L,n) lua_settop(L, -(n)-1)

#define LUA_GCSTOP		0
#define LUA_GCRESTART		1
#define LUA_GCCOLLECT		2
#define LUA_GCCOUNT		3
#define LUA_GCCOUNTB		4
#define LUA_GCSTEP		5
#define LUA_GCSETPAUSE		6
#define LUA_GCSETSTEPMUL	7

#define LUA_REGISTRYINDEX -10002
#define LUA_GLOBALSINDEX (-10002)

uintptr_t thread;

void lualoader::Init() noexcept
{
	h_luashared = GetModuleHandleA("lua_shared.dll");

	luaL_loadfile = reinterpret_cast<_luaL_loadfile>(GetProcAddress(h_luashared, "luaL_loadfile"));
	lua_tolstring = reinterpret_cast<_lua_tolstring>(GetProcAddress(h_luashared, "lua_tolstring"));
	lua_settop = reinterpret_cast<_lua_settop>(GetProcAddress(h_luashared, "lua_settop"));
	lua_gettop = reinterpret_cast<_lua_gettop>(GetProcAddress(h_luashared, "lua_gettop"));
	lua_newthread = reinterpret_cast<_lua_newthread>(GetProcAddress(h_luashared, "lua_newthread"));
	lua_pushvalue = reinterpret_cast<_lua_pushvalue>(GetProcAddress(h_luashared, "lua_pushvalue"));
	lua_xmove = reinterpret_cast<_lua_xmove>(GetProcAddress(h_luashared, "lua_xmove"));
	lua_setfenv = reinterpret_cast<_lua_setfenv>(GetProcAddress(h_luashared, "lua_setfenv"));
	luaL_loadstring = reinterpret_cast<_luaL_loadstring>(GetProcAddress(h_luashared, "luaL_loadstring"));
	lua_pcall = reinterpret_cast<_lua_pcall>(GetProcAddress(h_luashared, "lua_pcall"));
	lua_gc = reinterpret_cast<_lua_gc>(GetProcAddress(h_luashared, "lua_gc"));
	luaL_loadbuffer = reinterpret_cast<_luaL_loadbuffer>(GetProcAddress(h_luashared, "luaL_loadbuffer"));
}

auto getallthreads() {
	HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, GetCurrentProcessId());

	std::vector<DWORD> threads = {};

	THREADENTRY32 te32 = {};
	te32.dwSize = sizeof(THREADENTRY32);

	while (Thread32Next(snapshot, &te32)) {
		if (te32.th32OwnerProcessID == GetCurrentProcessId()) {
			threads.push_back(te32.th32ThreadID);
		}
	}

	CloseHandle(snapshot);
	return threads;
}

void callbacktest(const void*) {
	spdlog::default_logger()->info("Callback called!");
}

lua_State* execThread = nullptr;
bool lualoader::Execute(std::string script) noexcept
{
	if (interfaces::engine->IsInGame()) {
		//execThread = lua_newthread(config::luastate);
		const auto threads = getallthreads();

		luaL_loadbuffer(config::luastate, script.c_str(), script.size(), "leclerc_lua_loader");
		if (lua_pcall(config::luastate, 0, -1, 0) != 0) {
			lua_pop(config::luastate, 1);
			return false;
		}

		return true;
	}
	else {
		return false;
	}
}

bool lualoader::ExecuteFile(std::string path) noexcept
{
	if (interfaces::engine->IsInGame()) {
		luaL_loadfile(config::luastate, path.c_str());
		if (lua_pcall(config::luastate, 0, -1, 0) != 0) {
			lua_pop(config::luastate, 1);
			return false;
		}

		return true;
	}
	else {
		return false;
	}
}