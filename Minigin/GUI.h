#pragma once
#include "Singleton.h"

struct SDL_Window;
struct SDL_Renderer;
union SDL_Event;

namespace diji
{
    class GUI final : public Singleton<GUI>
    {
    public:
        void Init(SDL_Window* windowPtr, SDL_Renderer* rendererPtr);

        void NewFrame() const;
        void Render() const;
        void Destroy();

        bool ProcessEvent(SDL_Event* eventPtr);
    };
}