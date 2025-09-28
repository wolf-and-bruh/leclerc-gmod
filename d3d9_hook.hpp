#pragma once
#include <Windows.h>
#include <d3d9.h>

#include "core/config.hpp"
#include "../core/lua_loader.hpp"

#include <kiero/kiero.h>

#include <imgui/imgui.h>
#include <imgui/imgui_impl_dx9.h>
#include <imgui/imgui_impl_win32.h>
#include <imgui/TextEditor.h>

#include <spdlog/spdlog.h>

#include "interfaces/cbaseplayer.hpp"

namespace d3d9hook {
	inline IDirect3DDevice9* renderer = nullptr;

	inline bool menuOpened = false;

	inline ImFont* defaultFont = nullptr;
	inline ImFont* editorFont = nullptr;

	void Init() noexcept;
	void Shutdown() noexcept;
}