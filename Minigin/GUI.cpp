#include "GUI.h"

#include <SDL_events.h>

#include <imgui.h>
#include <backends/imgui_impl_sdl2.h>
#include <backends/imgui_impl_opengl3.h>

void diji::GUI::Init(SDL_Window* windowPtr, SDL_Renderer* rendererPtr)
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui_ImplSDL2_InitForOpenGL(windowPtr, rendererPtr);
    ImGui_ImplOpenGL3_Init();
}

void diji::GUI::NewFrame() const
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();
}

void diji::GUI::Render() const
{
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void diji::GUI::Destroy()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();
}

bool diji::GUI::ProcessEvent(SDL_Event* eventPtr)
{
    return ImGui_ImplSDL2_ProcessEvent(eventPtr);
}