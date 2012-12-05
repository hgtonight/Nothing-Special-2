#include "GameLogic.h"

GameLogic::GameLogic(int Width, int Height, float LogicFPS) {
	Debug = false;
	
	Display = NULL;
    EventQueue = NULL;
    Timer = NULL;
    Bitmap.Logo = NULL;
	Bitmap.Title = NULL;
    Font.Small = NULL;
    Font.Medium = NULL;
    Font.Big = NULL;
    Sound.HitWall = NULL;
    Sound.HitPaddle = NULL;
    Sound.BackgroundMusic = NULL;
    UserDataFile = NULL;
	DisplayWidth = Width;
	DisplayHeight = Height;
	TimerStep = 1.0 / LogicFPS;
    
	bool key[2] = { false, false };
    float alpha = 0;
    float colr = 0;
    float colg = 0;
    float colb = 0;
    int tick = 0;
    int menu = 0;
    int high_score = 0;
    bool restart = true;
    int score = 0, multi = 1, final_score = 1;
    bool game_over, redraw;
    char scoretxt[8] = "";
    char multitxt[4] = "";
    char final_scoretxt[16] = "";
    char hightxt[16] = "";

    // intro screen
    while (1) {
        ALLEGRO_EVENT ev;
        al_wait_for_event(EventQueue, &ev);

        if(ev.type == ALLEGRO_EVENT_TIMER) {
            // trigger redraw
            tick++;
            if(tick < 180) {
                colr = colg = colb = (0.5 * sin( (tick - 90) * ALLEGRO_PI / 180.0) + 0.5);
                alpha = 0;
            }
            else if(tick < 360) {
                alpha = (0.5 * sin( (tick + 90) * ALLEGRO_PI / 180.0) + 0.5);
                colr = colg = colb = 1;
            }
            else if(tick > 450 && tick < 630) {
                colr = colg = colb = alpha = (0.5 * cos( (tick - 90) * ALLEGRO_PI / 180.0) + 0.5);
            }
            else if(tick > 632) {
                break;
            }
            redraw = true;
        }
        else if(ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            return;
        }
        else if(ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP) {
            break;
        }

        if(redraw && al_is_event_queue_empty(EventQueue)) {
            redraw = false;
            al_clear_to_color(al_map_rgb_f(colr,colg,colb));
            al_draw_tinted_bitmap(Bitmap.Logo, al_map_rgba_f(alpha, alpha, alpha, alpha), (DisplayWidth - al_get_bitmap_width(Bitmap.Logo) ) / 2, (DisplayHeight - al_get_bitmap_height(Bitmap.Logo) )/ 2, 0);
            al_flip_display();
        }
    }

    al_flip_display();
    al_clear_to_color(al_map_rgb(0,0,0));

	// load hiscore info
    UserDataFile = al_fopen("assets/hiscore.txt", "r");
    if(UserDataFile) {
        char *line = (char*) malloc (17);
        line = al_fgets(UserDataFile, line, 17);
        high_score = atoi(line);
        free(line);
    }
    else {
        high_score = 0;
    }
	final_score = 0;

    sprintf(hightxt, "High: %d", high_score);
    al_fclose(UserDataFile);

    // main menu loop
    while (1) {
        ALLEGRO_EVENT ev;
        al_wait_for_event(EventQueue, &ev);

        if(ev.type == ALLEGRO_EVENT_TIMER) {
            // trigger redraw
            redraw = true;
        }
        else if(ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
			// exit if the Display is closed
            return;
        }
        else if(ev.type == ALLEGRO_EVENT_MOUSE_AXES || ev.type == ALLEGRO_EVENT_MOUSE_ENTER_DISPLAY) {
            // track mouse movement
			if((ev.mouse.y > 350) && (ev.mouse.y < 388) && (ev.mouse.x > 250) && (ev.mouse.x < 388)) {
				menu = 1;
			}
			else {
				menu = 0;
			}
        }
        else if(ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP) {
			// track mouse clicks
			if(menu == 1) {
				break;
			}
        }

        if(redraw && al_is_event_queue_empty(EventQueue)) {
			// process entire queue and only render if the Timer has ticked
            redraw = false;
            al_clear_to_color(al_map_rgb(0,0,0));
			al_draw_bitmap(Bitmap.Title, (DisplayWidth - al_get_bitmap_width(Bitmap.Title) ) / 2, 16, 0);
            al_draw_text(Font.Small, al_map_rgb(128,128,0), DisplayWidth / 2, 250, ALLEGRO_ALIGN_CENTRE, "Version 0.1");
			if(menu == 1) {
				al_draw_text(Font.Medium, al_map_rgb(128,0,0), DisplayWidth / 2, 350, ALLEGRO_ALIGN_CENTRE, "Play Now!");
			}
			else {
				al_draw_text(Font.Medium, al_map_rgb(128,128,0), DisplayWidth / 2, 350, ALLEGRO_ALIGN_CENTRE, "Play Now!");
			}
            al_flip_display();
        }
    }

    // seed rand function
    srand(al_get_time());

    // game wrapper lets us restart the game
    while(restart) {
		// hide the cursor
        //al_hide_mouse_cursor(Display);

		// assume we don't want to play again
        restart = false;
		
		// Base objects
		Ball StarterBall = Ball();
		Paddle StarterPaddle = Paddle(0);
		Paddle StarterPaddle2 = Paddle(DisplayHeight - 8);
		Paddle StarterPaddle3 = Paddle(0, true);
		Paddle StarterPaddle4 = Paddle(DisplayWidth - 8, true);
		        
		// TODO: load the right map

			// set Bitmap.Title
			// set base speed
			// create list of block entities
		

		// Populate entity lists
		// Remove static data when we get map loading done
		for(int i=0; i < 64; i++) {
			Brick SingleBlockyEntity = Brick((i % 16) * 40, 80 + ( (i / 16) * 10), i % 8);
			Bricks.push_back(SingleBlockyEntity);
		}
		Paddles.push_back(StarterPaddle);
		Paddles.push_back(StarterPaddle2);
		Paddles.push_back(StarterPaddle3);
		Paddles.push_back(StarterPaddle4);
		Balls.push_back(StarterBall);

		// save high score if the final score is higher
        if(final_score > high_score) {
            high_score = final_score;
            sprintf (hightxt, "High: %d", high_score);
        }

		// reset score if not a continuation

		// init hud
		
		// force initial draw
		redraw = true;
        
		// reset physics
		game_over = false;

        // main game loop
        while(1) {
            ALLEGRO_EVENT ev;
            al_wait_for_event(EventQueue, &ev);

            if(ev.type == ALLEGRO_EVENT_TIMER) {
                // game logic loop
                if(!game_over) {
					// calculate each ball's new position and check for...
					Path CurrentPath;
					for(int i = 0; i < Balls.size(); i++) {
						CurrentPath = Balls[i].GetPath(TimerStep);

						// distance traveled in axis directions in time delta t
						float NX = Balls[i].Position.DeltaX * TimerStep;
						float NY = Balls[i].Position.DeltaY * TimerStep;
						// new unencombered position
						float NewPositionX = Balls[i].Position.X + NX;
						float NewPositionY = Balls[i].Position.Y + NY;
						ClosestEntity Closest;
						Closest.Magnitude = 10000;
						// look for collisions
						for(int j = 0; j < (int)Bricks.size(); j++) {
							// only check items that haven't been hit in the most recent update
							if (!Bricks[j].HasBeenHit()) {
								Intersection Point = Bricks[j].PathIntersect(CurrentPath);
								if (Point.Type == Intersecting) {
									// mark this hit so we don't check for it again...
									// until the next update frame.
									Bricks[i].Hit = true;
									// see how far the collision is
									float Magnitude = sqrt(pow(Point.X - Balls[i].Position.X, 2) + pow(Point.Y - Balls[i].Position.Y, 2)); 
									if (Magnitude <  Closest.Magnitude) {
										// nab the closest collision so we can handle multiple collisions
										Closest.Magnitude = Magnitude;
										Closest.EntityIndex = j;
										Closest.CollisionPoint = Point;
									}
								}
							}
						}

						if (Closest.CollisionPoint.Type == Intersecting) {
							if(Bricks[Closest.EntityIndex].Harm(Balls[i].CollisionDamage)) {
		
							// TODO: play a sound

							// Update the ball to the point of collision
							Balls[i].Position.X = Closest.CollisionPoint.X;
							Balls[i].Position.Y = Closest.CollisionPoint.Y;

							// Reflect the ball in the appropriate direction
							switch(Closest.CollisionPoint.From) {
							default:
							case Top:
							case Bottom:
								Balls[i].ReflectY();
								break;
							case Left:
							case Right:
								Balls[i].ReflectX();
								break;
							}
		
							/* Find the fraction of DeltaT that had to occur before this collision
							float UnknownT = TimerStep * (Closest.Magnitude / sqrt(pow(NX,2) + pow(NY, 2)));
							// simlulate the ball movement after the collision
							Balls[i].GetPath(TimerStep - UnknownT);
							break;*/
						}

	
						if(NewPositionX > 0.0f && NewPositionX < DisplayWidth - Balls[i].Width && NewPositionY > 0.0f && NewPositionY < DisplayHeight - Balls[i].Height) {
							Balls[i].Live = true;
							Balls[i].Position.X = NewPositionX;
							Balls[i].Position.Y = NewPositionY;
						}
						else {
							Balls[i].Live = false;
						}
					}

					// make sure at least one ball is in play
					if(Balls[0].BallCount < 1) {
						game_over = true;
					}
					}
                }
                else {
                    // end game animation logic
                }
                // trigger a render every tick
				redraw = true;
            }
            else if(ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
                break;
            }
            else if(ev.type == ALLEGRO_EVENT_MOUSE_AXES || ev.type == ALLEGRO_EVENT_MOUSE_ENTER_DISPLAY) {
                if(!game_over) {
                    // update each paddle
					for(int i = 0; i < Paddles.size(); i++) {
						// horizontal clip
						if(Paddles[i].IsHorizontal()) {
							if(ev.mouse.x < (Paddles[i].Width / 2) ) {
								Paddles[i].Position.X = 0;
							}
							else if(ev.mouse.x > DisplayWidth - (Paddles[i].Width / 2) ) {
								Paddles[i].Position.X = DisplayWidth - Paddles[i].Width;
							}
							else {
								Paddles[i].Position.X = ev.mouse.x - (Paddles[i].Width / 2);
							}
						}
						else {
						// vertical clip
							if(ev.mouse.y < (Paddles[i].Height / 2) ) {
								Paddles[i].Position.Y = 0;
							}
							else if(ev.mouse.y > DisplayHeight - (Paddles[i].Height / 2) ) {
								Paddles[i].Position.Y = DisplayHeight - Paddles[i].Height;
							}
							else {
								Paddles[i].Position.Y = ev.mouse.y - (Paddles[i].Height / 2);
							}
						}
					}
                }
                else {
                    // track mouse movement for retry menu logic
                }
            }
            else if(ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP) {
                if(game_over) {
                    // track mouse clicks for retry menu logic
                }
				else {
					// Debug
					// add a ball at the current mouse position
				}
            }
            else if(ev.type == ALLEGRO_EVENT_KEY_DOWN) {
                // set keys we are interested in if pressed
                switch(ev.keyboard.keycode) {
                    case ALLEGRO_KEY_ESCAPE:
                        key[KEY_ESC] = true;
                        break;
					case ALLEGRO_KEY_SPACE:
                        key[KEY_SPACE] = true;
                        break;
                }
            }
            else if(ev.type == ALLEGRO_EVENT_KEY_UP) {
                // release keys we are interested in if let go
                switch(ev.keyboard.keycode) {
                    case ALLEGRO_KEY_ESCAPE:
                        key[KEY_ESC] = false;
                        break;
					case ALLEGRO_KEY_SPACE:
                        key[KEY_SPACE] = false;
                        break;
                }
            }

            if(redraw && al_is_event_queue_empty(EventQueue)) {
                //The render loop
                redraw = false;
                if(!game_over) {
					// game play
                    // blank
                    al_clear_to_color(al_map_rgb(128,128,128));
                    
					// render each brick
					for(int i = 0; i < Bricks.size(); i++)
					{
						Bricks[i].Render();
					}

					// render each ball
					for(int i = 0; i < Balls.size(); i++)
					{
						Balls[i].Render();
					}

					// render each paddle
					for(int i = 0; i < Paddles.size(); i++)
					{
						Paddles[i].Render();
					}
					
					// render HUD
					sprintf (scoretxt, "%d", score);
					sprintf (multitxt, "%d", multi);
                    al_draw_text(Font.Medium, al_map_rgb(0,0,0), 8, 8, ALLEGRO_ALIGN_LEFT, "Score");
                    al_draw_text(Font.Medium, al_map_rgb(255,255,255), 128, 8, ALLEGRO_ALIGN_LEFT, scoretxt);
                    al_draw_text(Font.Medium, al_map_rgb(0,0,0), 300, 8, ALLEGRO_ALIGN_LEFT, "x");
                    al_draw_text(Font.Medium, al_map_rgb(255,255,255), 316, 8, ALLEGRO_ALIGN_LEFT, multitxt);
                    al_draw_text(Font.Medium, al_map_rgb(0,0,0), 424, 8, ALLEGRO_ALIGN_LEFT, hightxt);
                }
                else {
					// render retry menu
                    al_clear_to_color(al_map_rgb(128,128,128));
                }
                // always flip Display
                al_flip_display();
            }
        }
    }
    // save high score if the final score is higher
    if(final_score > high_score) {
        high_score = final_score;
    }
    sprintf (hightxt, "%d", high_score);
    // write highscore to file
    UserDataFile = al_fopen("assets/hiscore.txt", "w");
    if(UserDataFile) {
        al_fputs(UserDataFile, hightxt);
        al_fclose(UserDataFile);
    }

	// proper cleanup is important!
    al_destroy_bitmap(Bitmap.Title);
    al_destroy_bitmap(Bitmap.Logo);
    al_destroy_timer(Timer);
    al_destroy_display(Display);
    al_destroy_event_queue(EventQueue);
}

GameLogic::~GameLogic() {

}

bool GameLogic::InitializeAllegro() {
	if(!al_init()) {
        fprintf(stderr, "Failed to initialize allegro!\n");
        return false;
    }
    if(Debug) { fprintf(stderr, "Allegro 5 initialized.\n"); }

	if(!al_install_keyboard()) {
      fprintf(stderr, "Failed to initialize the keyboard!\n");
      return false;
    }
    if(Debug) { fprintf(stderr, "Keyboard initialized.\n"); }

    if(!al_install_mouse()) {
        fprintf(stderr, "Failed to initialize the mouse!\n");
        return false;
    }
    if(Debug) { fprintf(stderr, "Mouse initialized.\n"); }

    Timer = al_create_timer(TimerStep);
    if(!Timer) {
        fprintf(stderr, "Failed to create Timer!\n");
        return false;
    }
    if(Debug) { fprintf(stderr, "Timer initialized.\n"); }

    if(!al_init_image_addon()) {
        fprintf(stderr, "Failed to initialize images loading!\n");
        return false;
    }
    if(Debug) { fprintf(stderr, "Image loading initialized.\n"); }

	if(!al_init_primitives_addon()) {
        fprintf(stderr, "Failed to initialize primitives!\n");
        return false;
    }
    if(Debug) { fprintf(stderr, "Primitives addon initialized.\n"); }

    al_init_font_addon();
	
    if(!al_init_ttf_addon()) {
        fprintf(stderr, "Failed to initialize TrueType!\n");
        return false;
    }
    if(Debug) { fprintf(stderr, "TTF font loading initialized.\n"); }
    
    if(!al_install_audio()) {
        fprintf(stderr, "Failed to initialize audio!\n");
        return false;
    }
    if(Debug) { fprintf(stderr, "Audio initialized.\n"); }

    if(!al_init_acodec_addon()) {
        fprintf(stderr, "Failed to initialize audio codecs!\n");
        return false;
    }
    if(Debug) { fprintf(stderr, "Ogg/FLAC initialized.\n"); }

    // create and confirm allegro Display
    Display = al_create_display(DisplayWidth, DisplayHeight);
    if(!Display) {
        fprintf(stderr, "Failed to create Display!\n");
        return false;
    }
    if(Debug) { fprintf(stderr, "Display initialized.\n"); }

    // Make sure we are on the backbuffer
    al_set_target_bitmap(al_get_backbuffer(Display));
    
    Voice = al_create_voice(44100, ALLEGRO_AUDIO_DEPTH_INT16, ALLEGRO_CHANNEL_CONF_2);
    if (!Voice) {
        fprintf(stderr, "Could not create ALLEGRO_VOICE.\n");
        return 1;
    }
    if(Debug) { fprintf(stderr, "Voice created.\n"); }

    Mixer = al_create_mixer(44100, ALLEGRO_AUDIO_DEPTH_FLOAT32,ALLEGRO_CHANNEL_CONF_2);
    if (!Mixer) {
        fprintf(stderr, "Could not create ALLEGRO_MIXER.\n");
        return 1;
    }
    if(Debug) { fprintf(stderr, "Mixer created.\n"); }

    if (!al_attach_mixer_to_voice(Mixer, Voice)) {
        fprintf(stderr, "Attaching Mixer to Voice failed.\n");
        return 1;
    }
    if(Debug) { fprintf(stderr, "Mixer attached to Voice.\n"); }
    
    // create and confirm event queue
    EventQueue = al_create_event_queue();
    if(!EventQueue) {
        fprintf(stderr, "Failed to create EventQueue!\n");
        return false;
    }
    if(Debug) { fprintf(stderr, "Event queue created successfully.\n"); }

    al_register_event_source(EventQueue, al_get_display_event_source(Display));
    al_register_event_source(EventQueue, al_get_timer_event_source(Timer));
    al_register_event_source(EventQueue, al_get_mouse_event_source());
    al_register_event_source(EventQueue, al_get_keyboard_event_source());

    // blank the Display
    al_clear_to_color(al_map_rgb(0,0,0));
    al_flip_display();

	// actually start our Timer
    al_start_timer(Timer);

	return true;
}

bool GameLogic::LoadAssets() {

    Bitmap.Logo = al_load_bitmap("assets/images/daklutz.png");
    Bitmap.Title = al_load_bitmap("assets/images/Bitmap.Title.png");
    if(!Bitmap.Title || !Bitmap.Logo) {
        fprintf(stderr, "Failed to load image files!\n");
        return false;
    }
	if(Debug) { fprintf(stderr, "Bitmaps loaded.\n"); }

	// actually load the font, I loaded the same font at different sizes.
    Font.Small = al_load_font("assets/fonts/Geo-Regular.ttf", 24, 0);
    Font.Medium = al_load_font("assets/fonts/Geo-Regular.ttf", 38, 0);
    Font.Big = al_load_font("assets/fonts/Geo-Regular.ttf", 70, 0);
	if(!Font.Small || !Font.Medium || !Font.Big) {
		fprintf(stderr, "Failed to load fonts!\n");
        return false;
    }

	if (!al_reserve_samples(2)) {
        fprintf(stderr, "Could not reserve samples!\n");
        return false;
    }
    if(Debug) { fprintf(stderr, "Samples reserved.\n"); }

	Sound.HitWall = al_load_sample("assets/sounds/wall_collision.ogg");
    Sound.HitPaddle = al_load_sample("assets/sounds/paddle_collision.ogg");
    Sound.BackgroundMusic = al_load_audio_stream("assets/sounds/Sound.BackgroundMusic-tech.ogg", 4, 2048);
    if (!Sound.HitWall || !Sound.HitPaddle || !Sound.BackgroundMusic) {
        fprintf(stderr, "Failed to load sound files!\n");
        return false;
    }
	if(Debug) { fprintf(stderr, "Sound files loaded.\n"); }

	if(!al_attach_audio_stream_to_mixer(Sound.BackgroundMusic, Mixer) ) {
        fprintf(stderr, "Failed to attach stream to default Mixer!\n");
        return 1;
    }
    if(Debug) { fprintf(stderr, "Stream attached to Mixer.\n"); }

    if(!al_set_mixer_playing(Mixer, true)) {
        fprintf(stderr, "Failed to play Mixer!\n");
    }
    if(Debug) { fprintf(stderr, "Mixer playing.\n"); }

    if(!al_set_audio_stream_playmode(Sound.BackgroundMusic, ALLEGRO_PLAYMODE_LOOP)) {
        fprintf(stderr, "Failed to loop stream!\n");
    }
    if(Debug) { fprintf(stderr, "Stream is set to loop.\n"); }

    if(!al_set_audio_stream_playing(Sound.BackgroundMusic, true)) {
        fprintf(stderr, "Failed to play stream!\n");
    }
    if(Debug) { fprintf(stderr, "Stream playing.\n"); }


	return true;
}

void GameLogic::SetDebugMode(bool Verbose) {
	Debug = Verbose;
}
