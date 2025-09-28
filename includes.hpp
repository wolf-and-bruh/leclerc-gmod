#pragma once

typedef struct lua_State lua_State;

#include <vector>
#include <random>
#include <iostream>
#include <cstdint>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <functional>
#include <memory>
#include <atomic>
#include <cmath>

#include <Windows.h>
#include <TlHelp32.h>
#include <Psapi.h>

#include "core/memory.hpp"
#include "interfaces/mathlib.hpp"

#include "dependencies/imgui/imgui.h"
#pragma comment(lib, "dependencies/freetype.lib")
#include "dependencies/kiero/kiero.h"
#include "dependencies/minhook/include/MinHook.h"
#include "dependencies/spdlog/spdlog.h"
#include "dependencies/spdlog/sinks/basic_file_sink.h"

#include "interfaces/icliententitylist.hpp"
#include "interfaces/ibaseclientdll.hpp"
#include "interfaces/iglobalvars.hpp"
#include "interfaces/cbaseplayer.hpp"
#include "interfaces/clientmodenormal.hpp"
#include "interfaces/ivengineclient.hpp"
#include "interfaces/cluashared.hpp"
#include "interfaces/ivdebugoverlay.hpp"
#include "interfaces/iprediction.hpp"
#include "interfaces/cenginetrace.hpp"
#include "interfaces/predictionengine.hpp"
#include "interfaces/iconvar.hpp"
#include "interfaces/igameevents.hpp"
#include "interfaces/igamemovement.hpp"
#include "interfaces/imaterial.hpp"
#include "interfaces/imatrendercontext.hpp"
#include "interfaces/cmaterialsystem.hpp"
#include "interfaces/cmodelrender.hpp"

#include "core/config.hpp"
#include "core/interfaces.hpp"
#include "core/hooks.hpp"
#include "core/lua_loader.hpp"
#include "core/scheduler.hpp"

#include "../d3d9_hook.hpp"