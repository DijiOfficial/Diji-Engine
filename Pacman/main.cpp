#if _DEBUG
#if __has_include(<vld.h>)
#include <vld.h>
#endif
#endif

#include "Minigin.h"
#include "GameLoader.h"

int main(int, char*[]) {
	diji::Minigin engine("../Data/");
	engine.Run(Loader::Load);

	return 0;
}