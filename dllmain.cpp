// dllmain.cpp : Defines the entry point for the DLL application.
#include "includes.hpp"
#include "interfaces/gameevents.hpp"

static FILE* file = {};

int EntryPoint(HMODULE hModule) {
    AllocConsole();

    SetConsoleTitleA("Leclerc-GMOD Console");

    freopen_s(&file, "CONIN$", "r", stdin);
    freopen_s(&file, "CONOUT$", "w", stdout);

    auto errorLogSink = std::make_shared<spdlog::sinks::basic_file_sink_mt>("leclerc_error.log");
    errorLogSink->set_level(spdlog::level::err);

    MH_Initialize();

    d3d9hook::Init();
    interfaces::Prepare();
    hooks::Init();
    lualoader::Init();
    scheduler_system->init();

    scheduler_system->queueRepeating([] {
        //spdlog::default_logger()->info("[scheduler] ConVar tick!");

        const auto sv_cheats = interfaces::cvar->FindCommandBase("sv_cheats");
        const auto sv_allowcslua = interfaces::cvar->FindCommandBase("sv_allowcslua");
        const auto fov_desired = interfaces::cvar->FindCommandBase("fov_desired");

        fov_desired->InternalSetValue(config::fov);

        if (config::allowcheats) {
            sv_cheats->InternalSetValue(1);
        }
        else {
            sv_cheats->InternalSetValue(0);
        }

        if (config::allowcslua) {
            sv_allowcslua->InternalSetValue(1);
        }
        else {
            sv_allowcslua->InternalSetValue(0);
        }
    }, std::chrono::milliseconds(100));

    std::this_thread::sleep_for(std::chrono::milliseconds(10));

    spdlog::default_logger()->info("Got IClientEntityList interface -> {}", (void*)interfaces::entityList);
    spdlog::default_logger()->info("Got IBaseClientDLL interface -> {}", (void*)interfaces::client);
    spdlog::default_logger()->info("Got GlobalVars interface -> {}", (void*)interfaces::globalVars);
    spdlog::default_logger()->info("Got ClientMode interface -> {}", (void*)interfaces::clientMode);
    spdlog::default_logger()->info("Got CLuaShared interface -> {}", (void*)interfaces::cluaShared);
    spdlog::default_logger()->info("Got ClientState interface -> {}", interfaces::clientState);
    spdlog::default_logger()->info("Got IPrediction interface -> {}", (void*)interfaces::prediction);
    spdlog::default_logger()->info("Got IEngineTrace interface -> {}", (void*)interfaces::trace);
    spdlog::default_logger()->info("Got ICvar interface -> {}", (void*)interfaces::cvar);
    spdlog::default_logger()->info("Got IGameEventsManager2 interface -> {}", (void*)interfaces::gameevent);
    spdlog::default_logger()->info("Got CGameMovement interface -> {}", (void*)interfaces::gamemovement);
    spdlog::default_logger()->info("Got IMatRenderContext interface -> {}", (void*)interfaces::matrenderctx);
    spdlog::default_logger()->info("Got CModelRender interface -> {}", (void*)interfaces::modelrender);
    spdlog::default_logger()->info("Got CMaterialSystem interface -> {}", (void*)interfaces::matsystem);
    spdlog::default_logger()->info("Got IMoveHelper interface -> {}", (void*)interfaces::movehelper);
    
    interfaces::engine->ClientCmd("play hl1/ambience/particle_suck1.wav");

    auto dmgevent = new DamageEvent();
    interfaces::gameevent->AddListener(reinterpret_cast<IGameEventListener2*>(dmgevent), "player_hurt", false);

    while (!GetAsyncKeyState(VK_END) & 1) {};

    hooks::Shutdown();
    d3d9hook::Shutdown();
    scheduler_system->stop();

    MH_Uninitialize();

    FreeConsole();
    FreeLibraryAndExitThread(hModule, 0);
    return 0;
}

int __stdcall DllMain(HMODULE hModule,DWORD reason,void*)
{
    DisableThreadLibraryCalls(hModule);
    switch (reason)
    {
    case DLL_PROCESS_ATTACH:
        CreateThread(0, 0, (LPTHREAD_START_ROUTINE)EntryPoint, hModule, 0, 0);
        break;
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}