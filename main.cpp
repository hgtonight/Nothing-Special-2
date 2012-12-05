using namespace std;

#include "main.h"

const float FPS = 60.0;
const float TICKTIME = 1.0 / FPS;
const int SCREEN_W = 640;
const int SCREEN_H = 480;
const int BALL_SIZE = 16;
const int DIFFICULTY = 4;

int main(int argc, char **argv)
{
	GameLogic Game = GameLogic(SCREEN_W, SCREEN_H, FPS);
	
	if(argc > 1) {
		// assume switch is for debug mode
		Game.SetDebugMode(true);
	}
	
	// set up allegro
	if(Game.InitializeAllegro()) {
		return -1;
	}

	// set up timer

	// show intro screen

		// let it be skippable

	// show menu

		// start game

			// load first level

			// Play game
				
				// Show Pause menu

					// Restart 

					// Go to main menu
	
		// change options

		// exit program


	
	
	

	return 0;
}
