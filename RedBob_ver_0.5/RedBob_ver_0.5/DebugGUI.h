#pragma once

#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_sdlrenderer2.h"

// Setup Dear ImGui context
ImGuiContext* context = ImGui::CreateContext();
ImGuiIO& io = ImGui::GetIO();

struct DebugGUI
{
	DebugGUI() = default;

	void Init() {
		// ImGUI From 2.0.18: Enable native IME.
		#ifdef SDL_HINT_IME_SHOW_UI
				SDL_SetHint(SDL_HINT_IME_SHOW_UI, "1");
		#endif	

		// Setup Dear ImGui context
		IMGUI_CHECKVERSION();
		ImGui::SetCurrentContext(context);
		(void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

		// Setup Dear ImGui style
		ImGui::StyleColorsDark();

	}
	

	void RenderDebugDemo()
	{
		// Start the Dear ImGui frame
		ImGui::CreateContext();
		ImGui_ImplSDLRenderer2_NewFrame();
		ImGui_ImplSDL2_NewFrame();
		ImGui::NewFrame();

		// This is where you create your window and all UI elements
		ImGui::ShowDemoWindow();

		
	
		// Rendering
		ImGui::Render();
		ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData());

	}

	void DebugInput()
	{
		// Start the Dear ImGui frame
		ImGui::CreateContext();
		ImGui_ImplSDLRenderer2_NewFrame();
		ImGui_ImplSDL2_NewFrame();
		ImGui::NewFrame();


		ImGui::Begin("Debug Input");
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
		ImGui::Text("Mouse delta: (%g, %g)", io.MouseDelta.x, io.MouseDelta.y);
		ImGui::Text("Mouse down:");
		for (int i = 0; i < IM_ARRAYSIZE(io.MouseDown); i++) if (ImGui::IsMouseDown(i)) { ImGui::SameLine(); ImGui::Text("b%d (%.02f secs)", i, io.MouseDownDuration[i]); }
		ImGui::Text("Mouse wheel: %.1f", io.MouseWheel);

		ImGui::End();
	
		// Rendering
		ImGui::Render();
		ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData());
	}

};