#pragma once

namespace diji
{
	class Scene;
}

namespace Loader 
{
	void PacmanMenuReload(bool isFirstLoad = false);
	void PacmanIntro();
	void PacmanMenu();
	void CommonGameAssets(diji::Scene* &scene);
	void PacmanLevel();
	void VersusLevel();
	void CoopLevel();
	void FreemodeLevel();
	void HighScoreMenu(int score = 0);
	void Load();

}

