#ifndef GAMELOGIC_H
#define GAMELOGIC_H#include <cmath>
#include <vector>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_primitives.h>
#include "BlockyEntity.h"
#include "Ball.h"
#include "Brick.h"
#include "Paddle.h"

enum MYKEYS {
   KEY_SPACE, KEY_ESC
};

struct Fonts {
	ALLEGRO_FONT *Small;
	ALLEGRO_FONT *Medium;
	ALLEGRO_FONT *Big;
};

struct Sounds {
	ALLEGRO_SAMPLE *HitWall;
    ALLEGRO_SAMPLE *HitPaddle;
    ALLEGRO_AUDIO_STREAM *BackgroundMusic;
};

struct Bitmaps {
	ALLEGRO_BITMAP *Logo;
    ALLEGRO_BITMAP *Title;
};

class GameLogic
{
	bool Debug;
	ALLEGRO_DISPLAY *Display;
    ALLEGRO_EVENT_QUEUE *EventQueue;
    ALLEGRO_TIMER *Timer;
    ALLEGRO_VOICE *Voice;
    ALLEGRO_MIXER *Mixer;
    ALLEGRO_FILE *UserDataFile;
	
	Bitmaps Bitmap;
    Fonts Font;
	Sounds Sound;

	std::vector<Brick> Bricks;
	std::vector<Ball> Balls;
	std::vector<Paddle> Paddles;
public:
	GameLogic(int ScreenWidth, int ScreenHeight, float FPS = 60);
	~GameLogic();
	bool InitializeAndLoadAssets();
	void SetDebugMode(bool Verbose);
};
#endif