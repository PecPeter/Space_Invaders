#include "2D-Engine/engine.hpp"

#include "handler.hpp"

int main (void)
{
	cEngine engine;
	setSettings(60,200,10);

	char title[] = "SPACE INVADERS";
	cHandler stateHandler;

	if (engine.init (640,480,title,&stateHandler) == false) {
		std::cerr << "Could not initialize engine properly. Now exiting." << std::endl;
		return 1;
	}
	engine.mainLoop();
	engine.quit();

	return 0;
}
