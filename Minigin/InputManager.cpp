#include <SDL.h>
#include "InputManager.h"
#include "SceneManager.h"
#include "Scene.h"

bool diji::InputManager::ProcessInput()
{
	//const auto& sceneManager = SceneManager::GetInstance();
	//for (const auto& scene : sceneManager.GetScenes())
	//{
	//	const auto& gameObjectPtrVec = scene->GetObjects();
	//	for (const auto& gameObjectPtr : gameObjectPtrVec)
	//	{
	//		//if (gameObjectPtr) //isPLayableCaracter //maybe when creating component Playable call an initialiwation to this instance to set gameObj
	//		//{

	//		//}
	//	}
	//}

	SDL_Event e;
	while (SDL_PollEvent(&e)) {
		if (e.type == SDL_QUIT) {
			return false;
		}
		if (e.type == SDL_KEYDOWN) {
			
		}
		if (e.type == SDL_MOUSEBUTTONDOWN) {
			
		}
		if (e.type == SDLK_w)
		{

		}
		if (e.type == SDLK_a)
		{

		}
		if (e.type == SDLK_s)
		{

		}
		if (e.type == SDLK_d)
		{

		}
	}

	return true;
}
