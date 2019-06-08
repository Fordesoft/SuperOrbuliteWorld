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


Level::Level(char* map_filename, char* background_filename, char* music_filename, char* zone_image_filename, int time_limit, float background_vel_x, float background_vel_y, bool allow_scrolling)
{
	// set player to controllable and visible (in case a cutscene, before this level, disabled it)
	app->bTickPlayer = true;
	app->bDrawPlayer = true;

	bIsCutscene = false; // this is a regular, playable level, not a cutscene
	strFilename = map_filename;
	strBackgroundFilename = background_filename;
	strMusicFilename = music_filename;
	strZoneFilename = zone_image_filename;
	fBackgroundVelX = background_vel_x;
	fBackgroundVelY = background_vel_y;
	fBackgroundOffsetX = 0;
	fBackgroundOffsetY = 0;
	bmpBackground = NULL;
	bmpZone = NULL;
	elEnemies = NULL;
	iTimeRemaining = 0;
	iTimeLimit = time_limit;
	bAllowScrolling = allow_scrolling;
}

Level::Level(char* map_filename, char* background_filename, char* music_filename, float background_vel_x, float background_vel_y)
{
	// set player object to non-controllable and invisible
	app->bTickPlayer = false;
	app->bDrawPlayer = false;

	bIsCutscene = true; // this is a cutscene, not a playable level
	strFilename = map_filename;
	strBackgroundFilename = background_filename;
	strMusicFilename = music_filename;
	strZoneFilename = "";

	fBackgroundVelX = background_vel_x;
	fBackgroundVelY = background_vel_y;
	fBackgroundOffsetX = 0;
	fBackgroundOffsetY = 0;

	bmpBackground = NULL;
	bmpZone = NULL;
	elEnemies = NULL;
	iTimeRemaining = 0;
	iTimeLimit = 0;
	bAllowScrolling = true;
}

Level::~Level()
{
	if (bmpBackground)
		destroy_bitmap(bmpBackground);
	
	// delete list of enemies (and the enemies contained within)
	if (elEnemies != NULL)
	{
		delete elEnemies;
		elEnemies = NULL;
	}
}

void Level::load()
{
	// load and initialize the map
	std::string errorMsg = "problem in loading map: ";
	errorMsg += strFilename;
	if (MapLoad((char*)strFilename.c_str()))
		allegro_message(errorMsg.c_str());
	MapInitAnims();

	if (strBackgroundFilename.length() > 0)
		bmpBackground = load_bitmap(strBackgroundFilename.c_str(), NULL); // load background

	if (strMusicFilename.length() > 0)
		wavMusic = load_sample(strMusicFilename.c_str()); // load music (wav file)

	if (strZoneFilename.length() > 0)
		bmpZone = load_bitmap(strZoneFilename.c_str(), NULL); // load Zone info (Act info)

	ticks = old_ticks;
}

void Level::start()
{
	// initialize map data
	MapInitAnims();
	fBackgroundOffsetX = 0;
	fBackgroundOffsetY = 0;

	iTimeRemaining = (120 * iTimeLimit);

	// reset the scrolling view
	app->iViewX = 0;
	app->iViewY = 0;

	if (!app->player)
		app->player = new Player();

	// reset the player position etc.
	app->player->bFalling = true;
	app->player->bJumping = false;
	app->player->fX = 100;
	app->player->fY = 100;
	app->player->fVel_x = 0;
	app->player->fVel_y = 0;

	// don't show enemy box (right side of screen) at the start of the level
	app->bShowEnemyBox = false;
	app->iEnemyBoxTimer = 0;

	// (re-)create enemy list
	if (elEnemies != NULL)
		delete elEnemies;
	elEnemies = new EnemyList();

	// tell the app object to display the Zone (Act) image for a while
	app->bDisplayZone = true;
	app->iDisplayZoneX = 800;
	app->iDisplayZoneY = 30;
	app->iDisplayZoneTimer = 0;

	// create enemies by traversing the map and seeing if any of the tiles are Enemy tiles (user2 field > 0)
	// note: user2 field is the id of the type of monster and must be 1 or higher
	
	MapChangeLayer(1); // change to monster/player layer

	BLKSTR* blockdata;
	for (int x = 0; x < mapwidth; x++)
		for (int y = 0; y < mapheight; y++)
		{
			blockdata = MapGetBlock(x, y);
			if (blockdata->user2 > 0)
			{
				// create the new enemy
				Enemy * new_enemy = new Enemy(blockdata->user2, x * 32, y * 32, false);
				// add it to the linked list of enemies
				elEnemies->AddEnemy(new_enemy);
			}
			else if (blockdata->user3 > 0)
			{
				// create the new misc object (which uses the Enemy class)
				Enemy * new_object = new Enemy(blockdata->user3, x * 32, y * 32, true);
				// add it to the linked list of enemies
				elEnemies->AddEnemy(new_object);
			}
			else if (blockdata->user4 > 0)
			{
				// must be a tile designating player position, so set player x,y to here
				(app->player->fX = x * 32);
				app->player->fY = y * 32 + 12; // hard-coded to add 12 pixels so that player lines up with 32x32 tiles
			}
		}

	MapChangeLayer(0); // change back to main layer

	// start playing the music
	// TODO: uncomment this next line to unmute the music:
//	play_sample(wavMusic, 180, 128, 1000, 1);

	// fade in
#ifndef DEBUG
	BITMAP * bmpFadeTarget = create_bitmap(SCREEN_WIDTH, SCREEN_HEIGHT);
	app->tick();
	app->draw(bmpFadeTarget);
	highcolor_fade_in(bmpFadeTarget, 5);
#endif
}

void Level::stopMusic()
{
	// stops playing the level's music
	stop_sample(wavMusic);
}

void Level::draw(BITMAP * destination)
{
	drawBackground(destination); // draw the background

	// handle map drawing
	MapUpdateAnims();

	// draw layer 2, which is a sometimes-used "background" layer for tiles partially-hidden behind other tiles
	MapChangeLayer(2);
	MapDrawBGT(destination, app->iViewX, app->iViewY, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
	MapDrawFG(destination, app->iViewX, app->iViewY, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0);

	// draw layer 0, which is the main layer
	MapChangeLayer(0);
	MapDrawBGT(destination, app->iViewX, app->iViewY, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
	MapDrawFG(destination, app->iViewX, app->iViewY, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
}

void Level::drawBackground(BITMAP * destination)
{
	
	if (bmpBackground == NULL)
		return;

	// move the background
	fBackgroundOffsetX += fBackgroundVelX;
	fBackgroundOffsetY += fBackgroundVelY;

	// reset the background to a lower, equal number if possible (ex: if the background is 100x100 pixels, it makes more sense to have an X offset of 30 instead of 130)
	if (fBackgroundOffsetX > bmpBackground->w)
		fBackgroundOffsetX -= bmpBackground->w;
	else if (fBackgroundOffsetX < -bmpBackground->w)
		fBackgroundOffsetX += bmpBackground->w;
	if (fBackgroundOffsetY > bmpBackground->h)
		fBackgroundOffsetY -= bmpBackground->h;
	else if (fBackgroundOffsetY < -bmpBackground->h)
		fBackgroundOffsetY += bmpBackground->h;

	// draw the background, tiled
	int startx = 0, starty = 0;

	if (fBackgroundOffsetX < 0)
		startx = 0;
	if (fBackgroundOffsetY < 0)
		starty = 0;

	if (fBackgroundOffsetX > 0)
		startx = -bmpBackground->w;
	if (fBackgroundOffsetY > 0)
		starty = -bmpBackground->h;

	int max_width = 800;
	int max_height = 600;

	if (fBackgroundOffsetX < 0)
		max_width += bmpBackground->w;
	if (fBackgroundOffsetY < 0)
		max_height += bmpBackground->h;

	// draw the background
	for (int y = starty; y < max_height; y += bmpBackground->h)
	{
		for (int x = startx; x < max_width; x += bmpBackground->w)
			blit(bmpBackground, destination, 0, 0, x + fBackgroundOffsetX, y + fBackgroundOffsetY, bmpBackground->w, bmpBackground->h);
	}
}