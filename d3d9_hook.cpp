#include "d3d9_hook.hpp"
#include "core/fonts.hpp"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

bool init = false;

TextEditor editor;

static WNDPROC ogWndProc = NULL;
static HWND window = NULL;

using _Present = long(__stdcall*)(IDirect3DDevice9*, const RECT*, const RECT*, HWND, const RGNDATA*);
_Present ogPresent = nullptr;

LRESULT __stdcall WndProc(const HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	//std::printf(std::format("[DEBUG] WndProc tick\n").c_str());

	//if (true && ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam))
		//uMsg = WM_NULL;

	static bool lastState = false;
	if (wParam == VK_INSERT)
	{
		if (uMsg == WM_KEYDOWN && lastState == false) {
			lastState = true;
			d3d9hook::menuOpened = !d3d9hook::menuOpened;
		}
		else if (uMsg == WM_KEYUP)
		{
			lastState = false;
		}
	}

	if (d3d9hook::menuOpened) {
		ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam);
		return true;
	}

	return CallWindowProcA(ogWndProc, hWnd, uMsg, wParam, lParam);
}

long __stdcall detouredPresent(IDirect3DDevice9* pDevice, const RECT* rect1, const RECT* rect2, HWND hwnd, const RGNDATA* rgndata) {
	if (!init) {
		spdlog::default_logger()->info("Successfully hooked Direct3D device: {}", (void*)pDevice);

		d3d9hook::renderer = pDevice;

		ImGui::CreateContext();
		spdlog::default_logger()->info("Created ImGui context!");

		ImGuiStyle& style = ImGui::GetStyle();

		style.Alpha = 1.0f;
		style.DisabledAlpha = 0.6000000238418579f;
		style.WindowPadding = ImVec2(8.0f, 8.0f);
		style.WindowRounding = 8.399999618530273f;
		style.WindowBorderSize = 1.0f;
		style.WindowMinSize = ImVec2(32.0f, 32.0f);
		style.WindowTitleAlign = ImVec2(0.0f, 0.5f);
		style.WindowMenuButtonPosition = ImGuiDir_Right;
		style.ChildRounding = 3.0f;
		style.ChildBorderSize = 1.0f;
		style.PopupRounding = 3.0f;
		style.PopupBorderSize = 1.0f;
		style.FramePadding = ImVec2(4.0f, 3.0f);
		style.FrameRounding = 3.0f;
		style.FrameBorderSize = 1.0f;
		style.ItemSpacing = ImVec2(8.0f, 4.0f);
		style.ItemInnerSpacing = ImVec2(4.0f, 4.0f);
		style.CellPadding = ImVec2(4.0f, 2.0f);
		style.IndentSpacing = 21.0f;
		style.ColumnsMinSpacing = 6.0f;
		//style.ScrollbarSize = 5.599999904632568f;
		style.ScrollbarRounding = 18.0f;
		style.GrabMinSize = 10.0f;
		style.GrabRounding = 3.0f;
		style.TabRounding = 3.0f;
		style.TabBorderSize = 0.0f;
		style.ColorButtonPosition = ImGuiDir_Right;
		style.ButtonTextAlign = ImVec2(0.5f, 0.5f);
		style.SelectableTextAlign = ImVec2(0.0f, 0.0f);

		style.Colors[ImGuiCol_Text] = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
		style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.6000000238418579f, 0.6000000238418579f, 0.6000000238418579f, 1.0f);
		style.Colors[ImGuiCol_WindowBg] = ImVec4(0.125490203499794f, 0.125490203499794f, 0.125490203499794f, 1.0f);
		style.Colors[ImGuiCol_ChildBg] = ImVec4(0.125490203499794f, 0.125490203499794f, 0.125490203499794f, 1.0f);
		style.Colors[ImGuiCol_PopupBg] = ImVec4(0.168627455830574f, 0.168627455830574f, 0.168627455830574f, 1.0f);
		style.Colors[ImGuiCol_Border] = ImVec4(0.250980406999588f, 0.250980406999588f, 0.250980406999588f, 1.0f);
		style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
		style.Colors[ImGuiCol_FrameBg] = ImVec4(0.168627455830574f, 0.168627455830574f, 0.168627455830574f, 1.0f);
		style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.2156862765550613f, 0.2156862765550613f, 0.2156862765550613f, 1.0f);
		style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.250980406999588f, 0.250980406999588f, 0.250980406999588f, 1.0f);
		style.Colors[ImGuiCol_TitleBg] = ImVec4(0.125490203499794f, 0.125490203499794f, 0.125490203499794f, 1.0f);
		style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.168627455830574f, 0.168627455830574f, 0.168627455830574f, 1.0f);
		style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.125490203499794f, 0.125490203499794f, 0.125490203499794f, 1.0f);
		style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.168627455830574f, 0.168627455830574f, 0.168627455830574f, 1.0f);
		style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.125490203499794f, 0.125490203499794f, 0.125490203499794f, 1.0f);
		style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.250980406999588f, 0.250980406999588f, 0.250980406999588f, 1.0f);
		style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.3019607961177826f, 0.3019607961177826f, 0.3019607961177826f, 1.0f);
		style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.3490196168422699f, 0.3490196168422699f, 0.3490196168422699f, 1.0f);
		style.Colors[ImGuiCol_CheckMark] = ImVec4(0.0f, 0.4705882370471954f, 0.843137264251709f, 1.0f);
		style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.0f, 0.4705882370471954f, 0.843137264251709f, 1.0f);
		style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.0f, 0.3294117748737335f, 0.6000000238418579f, 1.0f);
		style.Colors[ImGuiCol_Button] = ImVec4(0.168627455830574f, 0.168627455830574f, 0.168627455830574f, 1.0f);
		style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.2156862765550613f, 0.2156862765550613f, 0.2156862765550613f, 1.0f);
		style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.250980406999588f, 0.250980406999588f, 0.250980406999588f, 1.0f);
		style.Colors[ImGuiCol_Header] = ImVec4(0.2156862765550613f, 0.2156862765550613f, 0.2156862765550613f, 1.0f);
		style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.250980406999588f, 0.250980406999588f, 0.250980406999588f, 1.0f);
		style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.3019607961177826f, 0.3019607961177826f, 0.3019607961177826f, 1.0f);
		style.Colors[ImGuiCol_Separator] = ImVec4(0.2156862765550613f, 0.2156862765550613f, 0.2156862765550613f, 1.0f);
		style.Colors[ImGuiCol_SeparatorHovered] = ImVec4(0.250980406999588f, 0.250980406999588f, 0.250980406999588f, 1.0f);
		style.Colors[ImGuiCol_SeparatorActive] = ImVec4(0.3019607961177826f, 0.3019607961177826f, 0.3019607961177826f, 1.0f);
		style.Colors[ImGuiCol_ResizeGrip] = ImVec4(0.2156862765550613f, 0.2156862765550613f, 0.2156862765550613f, 1.0f);
		style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.250980406999588f, 0.250980406999588f, 0.250980406999588f, 1.0f);
		style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.3019607961177826f, 0.3019607961177826f, 0.3019607961177826f, 1.0f);
		style.Colors[ImGuiCol_Tab] = ImVec4(0.168627455830574f, 0.168627455830574f, 0.168627455830574f, 1.0f);
		style.Colors[ImGuiCol_TabHovered] = ImVec4(0.2156862765550613f, 0.2156862765550613f, 0.2156862765550613f, 1.0f);
		style.Colors[ImGuiCol_TabActive] = ImVec4(0.250980406999588f, 0.250980406999588f, 0.250980406999588f, 1.0f);
		style.Colors[ImGuiCol_TabUnfocused] = ImVec4(0.168627455830574f, 0.168627455830574f, 0.168627455830574f, 1.0f);
		style.Colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.2156862765550613f, 0.2156862765550613f, 0.2156862765550613f, 1.0f);
		style.Colors[ImGuiCol_PlotLines] = ImVec4(0.0f, 0.4705882370471954f, 0.843137264251709f, 1.0f);
		style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.0f, 0.3294117748737335f, 0.6000000238418579f, 1.0f);
		style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.0f, 0.4705882370471954f, 0.843137264251709f, 1.0f);
		style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.0f, 0.3294117748737335f, 0.6000000238418579f, 1.0f);
		style.Colors[ImGuiCol_TableHeaderBg] = ImVec4(0.1882352977991104f, 0.1882352977991104f, 0.2000000029802322f, 1.0f);
		style.Colors[ImGuiCol_TableBorderStrong] = ImVec4(0.3098039329051971f, 0.3098039329051971f, 0.3490196168422699f, 1.0f);
		style.Colors[ImGuiCol_TableBorderLight] = ImVec4(0.2274509817361832f, 0.2274509817361832f, 0.2470588237047195f, 1.0f);
		style.Colors[ImGuiCol_TableRowBg] = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
		style.Colors[ImGuiCol_TableRowBgAlt] = ImVec4(1.0f, 1.0f, 1.0f, 0.05999999865889549f);
		style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.0f, 0.4705882370471954f, 0.843137264251709f, 1.0f);
		style.Colors[ImGuiCol_DragDropTarget] = ImVec4(1.0f, 1.0f, 0.0f, 0.8999999761581421f);
		style.Colors[ImGuiCol_NavHighlight] = ImVec4(0.2588235437870026f, 0.5882353186607361f, 0.9764705896377563f, 1.0f);
		style.Colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.0f, 1.0f, 1.0f, 0.699999988079071f);
		style.Colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.800000011920929f, 0.800000011920929f, 0.800000011920929f, 0.2000000029802322f);
		style.Colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.800000011920929f, 0.800000011920929f, 0.800000011920929f, 0.3499999940395355f);

		ImGuiIO& io = ImGui::GetIO();
		io.DeltaTime = 10.0f / 60.0f;
		io.ConfigFlags = ImGuiConfigFlags_NoMouseCursorChange;
		//defaultFont = io.Fonts->Fonts[0];

		// clear old fonts
		io.Fonts->Clear();

		// load your font as the only font = becomes default
		d3d9hook::defaultFont = io.Fonts->AddFontFromMemoryTTF(dataRoboto, sizeof(dataRoboto));
		d3d9hook::editorFont = io.Fonts->AddFontFromMemoryTTF(dataRobotoMono, sizeof(dataRobotoMono));

		ImGui_ImplWin32_Init(window);
		ImGui_ImplDX9_Init(pDevice);

		spdlog::default_logger()->info("Initialized ImGui Win32 and Direct3D implementations!");

		init = true;
	}

	pDevice->SetRenderState(D3DRS_COLORWRITEENABLE, 0xffffffff);
	pDevice->SetRenderState(D3DRS_SRGBWRITEENABLE, false);

	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	int width;
	int height;

	interfaces::engine->GetScreenSize(width, height);

	if (d3d9hook::menuOpened) {
		ImGui::Begin("Leclerc (Advanced Vector Extensions 1)", &d3d9hook::menuOpened, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollWithMouse);
		ImGui::SetWindowSize(ImVec2{ 800, 440 });

		if (ImGui::BeginTabBar("tabs", ImGuiTabBarFlags_None)) {
			if (ImGui::BeginTabItem("Movement")) {
				ImGui::Checkbox("Bunny-Hop", &config::bunnyhop);
				ImGui::Checkbox("Auto Strafe", &config::autostrafe);

				const char* legit_strafe = "Legit Strafing";
				const char* silent_strafe = "Silent Strafing";
				const char* directional_strafe = "Directional Strafing";

				// determine current mode from config
				static int current_strafing_mode = 0;
				if (config::autostrafe_legit)
					current_strafing_mode = 0;
				else if (config::autostrafe_silent)
					current_strafing_mode = 1;
				else if (config::autostrafe_directional)
					current_strafing_mode = 2;

				const char* preview = "";
				switch (current_strafing_mode) {
				case 0: preview = legit_strafe; break;
				case 1: preview = silent_strafe; break;
				case 2: preview = directional_strafe; break;
				}

				if (ImGui::BeginCombo("Strafing Type", preview)) {
					if (ImGui::Selectable(legit_strafe, current_strafing_mode == 0)) {
						current_strafing_mode = 0;
						config::autostrafe_legit = true;
						config::autostrafe_silent = false;
						config::autostrafe_directional = false;
					}
					if (ImGui::Selectable(silent_strafe, current_strafing_mode == 1)) {
						current_strafing_mode = 1;
						config::autostrafe_legit = false;
						config::autostrafe_silent = true;
						config::autostrafe_directional = false;
					}
					if (ImGui::Selectable(directional_strafe, current_strafing_mode == 2)) {
						current_strafing_mode = 2;
						config::autostrafe_legit = false;
						config::autostrafe_silent = false;
						config::autostrafe_directional = true;
					}
					ImGui::EndCombo();
				}

				ImGui::EndTabItem();
			}

			if (ImGui::BeginTabItem("Aiming")) {
				ImGui::Checkbox("Aimbot", &config::aimbot);
				ImGui::Checkbox("Silent Aim", &config::silent);
				ImGui::Checkbox("Autoshoot", &config::autoshoot);
				ImGui::SliderFloat("FOV", &config::fov, 25.f, 160.f);

				// bone labels
				const char* head_text = "Head";
				const char* neck_text = "Neck";
				const char* spine1_text = "Spine1";
				const char* spine2_text = "Spine2";
				const char* lhand_text = "LeftHand";
				const char* rhand_text = "RightHand";
				const char* pelvis_text = "Pelvis";

				// figure out currently selected bone text
				const char* current_bone = "";
				switch (config::bone) {
				case Bones::bone_head:    current_bone = head_text; break;
				case Bones::bone_neck:    current_bone = neck_text; break;
				case Bones::bone_spine_1: current_bone = spine1_text; break;
				case Bones::bone_spine_2: current_bone = spine2_text; break;
				case Bones::bone_hand_l:  current_bone = lhand_text; break;
				case Bones::bone_hand_r:  current_bone = rhand_text; break;
				case Bones::bone_pelvis:  current_bone = pelvis_text; break;
				default: current_bone = "Unknown"; break;
				}

				if (ImGui::BeginCombo("Bone", current_bone)) {
					if (ImGui::Selectable(head_text, config::bone == Bones::bone_head))    config::bone = Bones::bone_head;
					if (ImGui::Selectable(neck_text, config::bone == Bones::bone_neck))    config::bone = Bones::bone_neck;
					if (ImGui::Selectable(spine1_text, config::bone == Bones::bone_spine_1)) config::bone = Bones::bone_spine_1;
					if (ImGui::Selectable(spine2_text, config::bone == Bones::bone_spine_2)) config::bone = Bones::bone_spine_2;
					if (ImGui::Selectable(lhand_text, config::bone == Bones::bone_hand_l))  config::bone = Bones::bone_hand_l;
					if (ImGui::Selectable(rhand_text, config::bone == Bones::bone_hand_r))  config::bone = Bones::bone_hand_r;
					if (ImGui::Selectable(pelvis_text, config::bone == Bones::bone_pelvis))  config::bone = Bones::bone_pelvis;
					ImGui::EndCombo();
				}

				ImGui::EndTabItem();
			}

			if (ImGui::BeginTabItem("ESP")) {
				ImGui::Checkbox("Snapline", &config::snapline);
				ImGui::ColorPicker3("Snapline Color", config::snapline_color);
				ImGui::Checkbox("Chams", &config::chams);
				ImGui::Checkbox("Square ESP", &config::squareesp);
				ImGui::ColorPicker3("Square ESP Color", config::squareesp_color);
				ImGui::EndTabItem();
			}

			if (ImGui::BeginTabItem("Miscellaneous")) {
				ImGui::Checkbox("Chat on Death", &config::chatondeath);
				ImGui::Checkbox("sv_allowcslua (detected, not recommended)", &config::allowcslua);
				ImGui::Checkbox("sv_cheats (detected, not recommended)", &config::allowcheats);
				ImGui::Checkbox("test", &config::chatondeath);
				ImGui::EndTabItem();
			}

			if (ImGui::BeginTabItem("Lua Loader")) {
				static bool editor_initialized = false;
				if (!editor_initialized) {
					editor.SetLanguageDefinition(TextEditor::LanguageDefinition::Lua());
					//editor.SetTabSize(9999);
					editor_initialized = true;
				}

				if (ImGui::SmallButton("Execute")) {
					lualoader::Execute(editor.GetText());
				}
				ImGui::SameLine();
				if (ImGui::SmallButton("Run Anti Screen Grab")) {
					lualoader::Execute(R"(render.Capture = function() return end)");
				}
				ImGui::SameLine();
				ImGui::Text(std::format("Lua State -> {}", (void*)config::luastate).c_str());

				//editor.font = d3d9hook::editorFont;
				editor.Render("codeeditor");
				ImGui::EndTabItem();
			}

			ImGui::EndTabBar();
		}

		ImGui::End();
	}

	if (interfaces::engine->IsInGame()) {
		ImGui::Begin("Overlay", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoInputs | ImGuiWindowFlags_NoBackground);
		for (int i = 0; i < 128; ++i)
		{
			C_BasePlayer* target = (C_BasePlayer*)interfaces::entityList->GetClientEntity(i);

			if (config::snapline || config::squareesp) {
				if (target == nullptr) continue;
				if (target == localPlayer) continue;
				if (!target->IsPlayer()) continue;
				if (!target->IsAlive()) continue;

				player_info_s attackerInfo;
				interfaces::engine->GetPlayerInfo(i, &attackerInfo);

				Matrix3x4 bones[128];
				if (!target->SetupBones(bones, 128, 256, interfaces::globalVars->curtime))
					continue;

				Vector head;
				if (interfaces::debugoverlay->ScreenPosition(bones[Bones::bone_head].GetOrigin(), head))
					continue;

				Vector bottom;
				if (interfaces::debugoverlay->ScreenPosition(target->GetAbsOrigin(), bottom))
					continue;

				const float h = bottom.y - head.y;
				const float w = h * 0.3f;

				const int left = static_cast<int>(head.x - w);
				const int right = static_cast<int>(head.x + w);

				if (config::snapline) {
					ImGui::GetBackgroundDrawList()->AddLine(ImVec2(width / 2, height / 2), ImVec2(bottom.x, bottom.y), ImColor(config::snapline_color[0] * 255, config::snapline_color[1] * 255, config::snapline_color[2] * 255));
				}
				if (config::squareesp) {
					const float healthFraction = target->GetHealth() * 0.02f;

					ImGui::GetBackgroundDrawList()->AddText(d3d9hook::editorFont, 0, ImVec2(head.x, head.y - 40), ImColor(255, 255, 255), attackerInfo.name);
					ImGui::GetBackgroundDrawList()->AddText(d3d9hook::editorFont, 0, ImVec2(head.x, head.y - 20), ImColor((1.f - healthFraction) * 255, 255 * healthFraction, 0.f), std::format("Health: {}", target->GetHealth()).c_str());
					
					ImGui::GetBackgroundDrawList()->AddRect(ImVec2(left, head.y), ImVec2(right, bottom.y), ImColor(config::squareesp_color[0] * 255, config::squareesp_color[1] * 255, config::squareesp_color[2] * 255));
				}
			}
		}

		ImGui::GetBackgroundDrawList()->AddText(d3d9hook::editorFont, 0, ImVec2(25, height - 150), ImColor(0, 255, 0), std::format("VELOCITY: {}", config::currentvelocity).c_str());

		ImGui::End();
	}

	ImGui::EndFrame();
	ImGui::Render();
	ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());

	return ogPresent(pDevice, rect1, rect2, hwnd, rgndata);
}

void d3d9hook::Init() noexcept {
	if (kiero::init(kiero::RenderType::D3D9) == kiero::Status::Success) {
		window = FindWindowA("Valve001", 0);
		ogWndProc = reinterpret_cast<WNDPROC>(SetWindowLongPtrA(window, GWLP_WNDPROC, (LONG_PTR)WndProc));

		auto result = kiero::bind(17, reinterpret_cast<void**>(&ogPresent), &detouredPresent);
		if (!result == kiero::Status::Success) {
			spdlog::default_logger()->error("Failed to hook Direct3D device: {}", (int)result);
			return;
		}
	}
	return;
}

void d3d9hook::Shutdown() noexcept {
	ImGui_ImplDX9_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
	kiero::unbind(17);
	kiero::shutdown();
}