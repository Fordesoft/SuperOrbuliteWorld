#include <allegro.h>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <time.h>
#include <math.h>
#include <vector>
#include "mappyal.h"
#include "main.h"
#include "BitmapList.h"
#include "SoundList.h"
#include "Speechbubble.h"
#include "Enemy.h"
#include "Level.h"
#include "Player.h"
#include "App.h"

#include "fade.h" // functions for fading the screen in and out

// FPS limiting variables
volatile int	ticks = 0;
volatile int	old_ticks = 0;

// back buffer
BITMAP *buffer = NULL;

App * app = NULL;

int		key_state[KEY_MAX]; // similar to Allegro's key[] array, but this provides more functionality (0 = unpressed, 1 = tapped, 2 = held in)

void ticker()
{
	ticks++;
}
END_OF_FUNCTION(ticker);

bool tile_is_solid(int tile_x, int tile_y)
{
	BLKSTR *blockdata;

	// return false if the tile doesn't exist
	if (tile_x < 0)
		return false;
	if (tile_x > mapwidth - 1)
		return false;
	if (tile_y < 0)
		return false;
	if (tile_y > mapheight - 1)
		return false;

	// find the tile
	blockdata = MapGetBlock(tile_x, tile_y);

	// if user1 is set to 1 (done within the map editor Mappy), it is solid, otherwise it is not
	if (blockdata->user1 == 1)
		return true;
	else
		return false;
}

void make_tile_solid(int tile_x, int tile_y)
{
	BLKSTR *blockdata;

	// return false if the tile doesn't exist
	if (tile_x < 0)
		return;
	if (tile_x > mapwidth - 1)
		return;
	if (tile_y < 0)
		return;
	if (tile_y > mapheight - 1)
		return;

	// find the tile
	blockdata = MapGetBlock(tile_x, tile_y);

	MapChangeLayer(0);
	MapSetBlock(tile_x, tile_y, 22);
	MapChangeLayer(1);
}

void handle_logic()
{
	// update key_state[]
	for (int i = 0; i < KEY_MAX; i++)
	{
		if (key[i])
		{
			if (key_state[i] == 0) // if key wasn't pressed on the previous frame
				key_state[i] = 1; // key is now tapped once
			else
				key_state[i] = 2; // key is now held in
		}
		else
			key_state[i] = 0; // key is not pressed at all this frame
	}
	// end of updating key_state[]

	app->tick();
}

void handle_drawing()
{
	app->draw(buffer);
	blit(buffer, screen, 0,0,0,0,SCREEN_WIDTH,SCREEN_HEIGHT);
	clear_bitmap(buffer);
}

int main(int argc, char *argv[]) 
{
    if (allegro_init() != 0)
		exit(1);
    install_keyboard(); // initialize keyboard
	install_sound(DIGI_AUTODETECT, MIDI_AUTODETECT, NULL); // initialize sound
    set_color_depth(32);
    set_gfx_mode(GFX_AUTODETECT_WINDOWED, SCREEN_WIDTH,SCREEN_HEIGHT,0,0);
	
	buffer = create_bitmap(SCREEN_WIDTH, SCREEN_HEIGHT); // create an empty bitmap to use for the back buffer

	app = new App();
	
	font = load_font("fonts/arial16.pcx", NULL, NULL);

	// set up framerate stuff.  handle_frame() will be called 120 times per second
	install_timer();
	LOCK_VARIABLE(ticks);
	LOCK_FUNCTION(timer_handle);
	install_int_ex(ticker, BPS_TO_TIMER(120)); // call handle_frame() 120 times per second

#ifndef DEBUG
	josh_forde_logo();
	seeds_of_time_logo();
#endif

	//	app->current_level = new Level("maps/level1-1.FMP", "backgrounds/sky.bmp", "music/birth_island.wav", "zones/birthisland1.bmp", 200);
//	app->current_level = new Level("maps/level3-3.FMP", "backgrounds/stars.bmp", "music/atlantis_island.wav", "zones/atlantisislandboss.bmp", 200, -1.5f, -0.4f, false);
//	app->current_level = new Level("maps/ending1.FMP", "backgrounds/sky.bmp", "music/soft_melodies.wav"); // create the first scene of the ending

	app->current_level = new Level("maps/ending2.FMP", "backgrounds/sky.bmp", "music/soft_melodies.wav"); // create the ending2 scene
	app->current_level->load();
	app->current_level->start();


	while(!key[KEY_ESC]) //If the user hits escape, quit the program 
	{
		while(ticks == 0)
		{
			rest(100 / 120);
		}
		while(ticks > 0)
		{
			old_ticks = ticks;
			handle_logic();
			ticks--;
			if (old_ticks <= ticks) // logic is taking too long; abort and draw a frame
				break;
		}
		handle_drawing();
	}
	remove_timer(); // opposite of install_timer()
	delete app; // delete the app (contains player, level, loaded enemy images, etc.)
	destroy_bitmap(buffer); // delete the buffer


#ifndef DEBUG
	highcolor_fade_out(5);
#endif
	return 0; // Exit with no errors 
} 
END_OF_MAIN()



bool collision_detected(RECT& r1, RECT& r2)
{
    if (r1.bottom < r2.top) return(false);
    if (r1.top > r2.bottom) return(false);

    if (r1.right < r2.left) return(false);
    if (r1.left > r2.right) return(false);

    return(true);
};

int	random_number(int start, int end)
{
	int difference = end - start + 1;
	int value = (rand() % difference) + start;
	return value;

}

void write_to_file(char* filename, const char* text)
{
	std::ofstream file;
	file.open(filename);
	file << text;
	file.close();
}