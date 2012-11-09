#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include "singleton.h"
using namespace std;

const float FPS = 60;
const int SCREEN_W = 640;
const int SCREEN_H = 480;
const int BALL_SIZE = 16;
const int DIFFICULTY = 4;
const int PADDLE_SIZE = BALL_SIZE * DIFFICULTY;
enum MYKEYS {
   KEY_SPACE, KEY_ESC
};

int main(int argc, char **argv)
{
    bool debug = false;
    if(argc > 1) {
        // assume switch is for debug mode
        debug = true;
    }

    ALLEGRO_DISPLAY *display = NULL;
    ALLEGRO_EVENT_QUEUE *event_queue = NULL;
    ALLEGRO_TIMER *timer = NULL;
    ALLEGRO_BITMAP *logo = NULL;
    ALLEGRO_BITMAP *title = NULL;
    ALLEGRO_FONT *smallfont = NULL;
    ALLEGRO_FONT *medfont = NULL;
    ALLEGRO_FONT *hugefont = NULL;
    ALLEGRO_VOICE *voice;
    ALLEGRO_MIXER *mixer;
    ALLEGRO_SAMPLE *wallhit = NULL;
    ALLEGRO_SAMPLE *paddlehit = NULL;
    ALLEGRO_AUDIO_STREAM *music = NULL;
    ALLEGRO_FILE *scoref = NULL;
    bool key[2] = { false, false };
    float alpha = 0;
    float colr = 0;
    float colg = 0;
    float colb = 0;
    int tick = 0;
    int menu = 1;
    int high_score = 0;
    bool restart = true;
    int score = 0, multi = 1, final_score = 1;
    bool game_over, redraw;
    char scoretxt[8] = "";
    char multitxt[4] = "";
    char final_scoretxt[16] = "";
    char hightxt[16] = "";

    // initialize allegro
    if(!al_init()) {
        fprintf(stderr, "Failed to initialize allegro!\n");
        return -1;
    }
    if(debug) { fprintf(stderr, "Allegro 5 initialized.\n"); }

    // install and confirm keyboard
    if(!al_install_keyboard()) {
      fprintf(stderr, "Failed to initialize the keyboard!\n");
      return -1;
    }
    if(debug) { fprintf(stderr, "Keyboard initialized.\n"); }

    // create and confirm mouse
    if(!al_install_mouse()) {
        fprintf(stderr, "Failed to initialize the mouse!\n");
        return -1;
    }
    if(debug) { fprintf(stderr, "Mouse initialized.\n"); }

    // create and confirm timer
    timer = al_create_timer(1.0 / FPS);
    if(!timer) {
        fprintf(stderr, "Failed to create timer!\n");
        return -1;
    }
    if(debug) { fprintf(stderr, "Timer initialized.\n"); }

    // initialize to load images (png)
    if(!al_init_image_addon()) {
        fprintf(stderr, "Failed to initialize images loading!\n");
        return -1;
    }
    if(debug) { fprintf(stderr, "Image loading initialized.\n"); }

    // initialize bitmap fonts
    al_init_font_addon();

    // initialize true type fonts
    if(!al_init_ttf_addon()) {
        fprintf(stderr, "Failed to initialize TrueType!\n");
        return -1;
    }
    if(debug) { fprintf(stderr, "TTF font loading initialized.\n"); }
    // actually load the font, I loaded the same font at different sizes.
    smallfont = al_load_font("assets/fonts/Geo-Regular.ttf", 24, 0);
    medfont = al_load_font("assets/fonts/Geo-Regular.ttf", 38, 0);
    hugefont = al_load_font("assets/fonts/Geo-Regular.ttf", 70, 0);
	if(!smallfont || !medfont || !hugefont) {
		fprintf(stderr, "Failed to load fonts!\n");
        al_destroy_display(display);
        al_destroy_timer(timer);
        return -1;
    }
    // initialize audio, just WAVs
    if(!al_install_audio()) {
        fprintf(stderr, "Failed to initialize audio!\n");
        return -1;
    }
    if(debug) { fprintf(stderr, "Audio initialized.\n"); }

    // initialize audio codecs like OGG and FLAC
    if(!al_init_acodec_addon()) {
        fprintf(stderr, "Failed to initialize audio codecs!\n");
        return -1;
    }
    if(debug) { fprintf(stderr, "Ogg/FLAC initialized.\n"); }

    // create and confirm allegro display
    display = al_create_display(SCREEN_W, SCREEN_H);
    if(!display) {
        fprintf(stderr, "Failed to create display!\n");
        al_destroy_timer(timer);
        return -1;
    }
    if(debug) { fprintf(stderr, "Display initialized.\n"); }

    // create and confirm title, ball, and paddle bitmaps
    logo = al_load_bitmap("assets/images/daklutz.png");
    title = al_load_bitmap("assets/images/title.png");
    if(!title || !logo) {
        fprintf(stderr, "Failed to load image files!\n");
        al_destroy_display(display);
        al_destroy_timer(timer);
        return -1;
    }
	if(debug) { fprintf(stderr, "Bitmaps loaded.\n"); }
    
	// Make sure we are on the backbuffer
    al_set_target_bitmap(al_get_backbuffer(display));
    
    voice = al_create_voice(44100, ALLEGRO_AUDIO_DEPTH_INT16, ALLEGRO_CHANNEL_CONF_2);
    if (!voice) {
        fprintf(stderr, "Could not create ALLEGRO_VOICE.\n");
        return 1;
    }
    if(debug) { fprintf(stderr, "Voice created.\n"); }

    mixer = al_create_mixer(44100, ALLEGRO_AUDIO_DEPTH_FLOAT32,ALLEGRO_CHANNEL_CONF_2);
    if (!mixer) {
        fprintf(stderr, "Could not create ALLEGRO_MIXER.\n");
        return 1;
    }
    if(debug) { fprintf(stderr, "Mixer created.\n"); }

    if (!al_attach_mixer_to_voice(mixer, voice)) {
        fprintf(stderr, "Attaching mixer to voice failed.\n");
        return 1;
    }
    if(debug) { fprintf(stderr, "Mixer attached to voice.\n"); }

    // create sfx
    if (!al_reserve_samples(2)) {
        fprintf(stderr, "Could not reserve samples!\n");
        return 1;
    }
    if(debug) { fprintf(stderr, "Samples reserved.\n"); }

    wallhit = al_load_sample("assets/sounds/wall_collision.ogg");
    paddlehit = al_load_sample("assets/sounds/paddle_collision.ogg");
    music = al_load_audio_stream("assets/sounds/music-tech.ogg", 4, 2048);
    if (!wallhit || !paddlehit || !music) {
        fprintf(stderr, "Failed to load sound files!\n");
        return 1;
    }
    if(debug) { fprintf(stderr, "Sound files loaded.\n"); }

    if(!al_attach_audio_stream_to_mixer(music, mixer) ) {
        fprintf(stderr, "Failed to attach stream to default mixer!\n");
        return 1;
    }
    if(debug) { fprintf(stderr, "Stream attached to mixer.\n"); }

    if(!al_set_mixer_playing(mixer, true)) {
        fprintf(stderr, "Failed to play mixer!\n");
    }
    if(debug) { fprintf(stderr, "Mixer playing.\n"); }

    if(!al_set_audio_stream_playmode(music, ALLEGRO_PLAYMODE_LOOP)) {
        fprintf(stderr, "Failed to loop stream!\n");
    }
    if(debug) { fprintf(stderr, "Stream is set to loop.\n"); }

    if(!al_set_audio_stream_playing(music, true)) {
        fprintf(stderr, "Failed to play stream!\n");
    }
    if(debug) { fprintf(stderr, "Stream playing.\n"); }

    // create and confirm event queue
    event_queue = al_create_event_queue();
    if(!event_queue) {
        fprintf(stderr, "Failed to create event_queue!\n");
        al_destroy_bitmap(title);
        al_destroy_bitmap(logo);
        al_destroy_display(display);
        al_destroy_timer(timer);
        return -1;
    }
    if(debug) { fprintf(stderr, "Event queue created successfully.\n"); }

    al_register_event_source(event_queue, al_get_display_event_source(display));
    al_register_event_source(event_queue, al_get_timer_event_source(timer));
    al_register_event_source(event_queue, al_get_mouse_event_source());
    al_register_event_source(event_queue, al_get_keyboard_event_source());

    // blank the display
    al_clear_to_color(al_map_rgb(0,0,0));
    al_flip_display();

    // actually start our timer
    al_start_timer(timer);

    // intro screen
    while (1) {
        ALLEGRO_EVENT ev;
        al_wait_for_event(event_queue, &ev);

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
            return 0;
        }
        else if(ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP) {
            break;
        }

        if(redraw && al_is_event_queue_empty(event_queue)) {
            redraw = false;
            al_clear_to_color(al_map_rgb_f(colr,colg,colb));
            al_draw_tinted_bitmap(logo, al_map_rgba_f(alpha, alpha, alpha, alpha), (SCREEN_W - al_get_bitmap_width(logo) ) / 2, (SCREEN_H - al_get_bitmap_height(logo) )/ 2, 0);
            al_flip_display();
        }
    }

    al_flip_display();
    al_clear_to_color(al_map_rgb(0,0,0));

	// load hiscore info
    scoref = al_fopen("assets/hiscore.txt", "r");
    if(scoref) {
        char *line = (char*) malloc (17);
        line = al_fgets(scoref, line, 17);
        high_score = atoi(line);
        free(line);
    }
    else {
        high_score = 0;
    }
	final_score = 0;

    sprintf(hightxt, "High: %d", high_score);
    al_fclose(scoref);

    // main menu loop
    while (1) {
        ALLEGRO_EVENT ev;
        al_wait_for_event(event_queue, &ev);

        if(ev.type == ALLEGRO_EVENT_TIMER) {
            // trigger redraw
            redraw = true;
        }
        else if(ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
			// exit if the display is closed
            return 0;
        }
        else if(ev.type == ALLEGRO_EVENT_MOUSE_AXES || ev.type == ALLEGRO_EVENT_MOUSE_ENTER_DISPLAY) {
            // track mouse movement			
			if(ev.mouse.y < 360) {
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
            else {
                return 0;
            }
        }

        if(redraw && al_is_event_queue_empty(event_queue)) {
			// process entire queue and only render if the timer has ticked
            redraw = false;
            al_draw_bitmap(title, (SCREEN_W - al_get_bitmap_width(title) ) / 2, 16, 0);
            al_draw_text(smallfont, al_map_rgb(128,128,0), SCREEN_W / 2, 250, ALLEGRO_ALIGN_CENTRE, "Version 0.1");
            al_flip_display();
        }
    }

    // seed rand function
    srand(al_get_time());

    // game wrapper lets us restart the game
    while(restart) {
		// hide the cursor
        al_hide_mouse_cursor(display);

		// assume we don't want to play again
        restart = false;

        // load the right map

			// set title
			// set base speed
			// create list of block entities
				
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
            al_wait_for_event(event_queue, &ev);

            if(ev.type == ALLEGRO_EVENT_TIMER) {
                // game logic loop
                if(!game_over) {
					// calculate each ball's new position by checking for...

						// paddle collision
						
						// block collision
							// reflect and damage block

						// decide if ball is lost
	
						// move that ball

					// make sure at least one ball is in play
						// increment score
					score += multi;
					// else game over
                    
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
						// horizontal clip
							/*if(ev.mouse.x < (PADDLE_SIZE / 2) ) {
								paddle_x = 0;
							}
							else if(ev.mouse.x > SCREEN_W - (PADDLE_SIZE / 2) ) {
								paddle_x = SCREEN_W - PADDLE_SIZE;
							}
							else {
								paddle_x = ev.mouse.x - (PADDLE_SIZE / 2);
							}*/
						// vertical clip
							/*if(ev.mouse.y < (PADDLE_SIZE / 2) ) {
								paddle_y = 0;
							}
							else if(ev.mouse.y > SCREEN_H - (PADDLE_SIZE / 2) ) {
								paddle_y = SCREEN_H - PADDLE_SIZE;
							}
							else {
								paddle_y = ev.mouse.y - (PADDLE_SIZE / 2);
							}*/
                }
                else {
                    // track mouse movement for retry menu logic
                }
            }
            else if(ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP) {
                if(game_over) {
                    // track mouse clicks for retry menu logic
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

            if(redraw && al_is_event_queue_empty(event_queue)) {
                //The render loop
                redraw = false;
                if(!game_over) {
					// game play
                    // blank
                    al_clear_to_color(al_map_rgb(128,128,128));
                    
					// render each ball

					// render each paddle

					// render each block

					// render HUD
					sprintf (scoretxt, "%d", score);
					sprintf (multitxt, "%d", multi);
                    al_draw_text(medfont, al_map_rgb(0,0,0), 8, 8, ALLEGRO_ALIGN_LEFT, "Score");
                    al_draw_text(medfont, al_map_rgb(255,255,255), 128, 8, ALLEGRO_ALIGN_LEFT, scoretxt);
                    al_draw_text(medfont, al_map_rgb(0,0,0), 300, 8, ALLEGRO_ALIGN_LEFT, "x");
                    al_draw_text(medfont, al_map_rgb(255,255,255), 316, 8, ALLEGRO_ALIGN_LEFT, multitxt);
                    al_draw_text(medfont, al_map_rgb(0,0,0), 424, 8, ALLEGRO_ALIGN_LEFT, hightxt);
                }
                else {
					// render retry menu
                    al_clear_to_color(al_map_rgb(128,128,128));
                }
                // always flip display
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
    scoref = al_fopen("assets/hiscore.txt", "w");
    if(scoref) {
        al_fputs(scoref, hightxt);
        al_fclose(scoref);
    }

	// proper cleanup is important!
    al_destroy_bitmap(title);
    al_destroy_bitmap(logo);
    al_destroy_timer(timer);
    al_destroy_display(display);
    al_destroy_event_queue(event_queue);
   return 0;
}
