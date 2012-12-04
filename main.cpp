#include <cstdio>
#include <cstdlib>
#include "main.h"
#include "GameLogic.h"

using namespace std;

const int SCREEN_W = 640;
const int SCREEN_H = 480;
const int BALL_SIZE = 16;
const int DIFFICULTY = 4;

int main(int argc, char **argv)
{
	GameLogic Game = GameLogic(SCREEN_W, SCREEN_H);
	
	if(argc > 1) {
		// assume switch is for debug mode
		Game.SetDebugMode(true);
	}

	return 0;
}
