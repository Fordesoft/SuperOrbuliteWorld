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

#ifdef DEBUG
// fstream is needed for some debugging output to file(s)
	#include <fstream>
#endif

App::App()
{
	iViewX = 0;
	iViewY = 40;
	blBitmaps = new BitmapList();
	slSounds = new SoundList();
	current_level = NULL;

	iPlayerLevel = 1;
	iPlayerAttack = 5;
	iPlayerHPCurrent = 20;
	iPlayerHPMax = 20;
	iExpCurrent = 0;
	iExpNeeded = 150;
	iPlayerSpecies = 0; // start out as an Orbulite (species 0)
	iPlayerGold = 0;
	iPlayerScore = 0;

	iLives = 3; // start out with 3 lives

	bTickPlayer = true;
	bDrawPlayer = true;
	bAllowPlayerMovement = true;

	bDisplayZone = false;
	iDisplayZoneX = 0;
	iDisplayZoneY = 0;
	iDisplayZoneTimer = 0;

	bShowEnemyBox = false;
	strEnemyName = "";
	iEnemyBoxTimer = 0; // countdown until right-side enemy box stops being shown
	iEnemyHPCurrent = 0; // current hp of last enemy attacked (shown in right-side box)
	iEnemyHPMax = 0; // max hp of last enemy attacked (shown in right-side box)
	bmpEnemyPortrait = NULL; //  // portrait bitmap of last enemy attacked (shown in right-side box)

	player = NULL;

	iAppMode = MODE_GAME; // start out in-game.. in the final release, this will be at MODE_TITLE instead.  so.. DEBUG in a sense.

	// load fonts
	fontArial26 = load_font("fonts/arial26.pcx", NULL, NULL);
	if (!fontArial26)
		allegro_message("Could not load fonts/arial26.pcx");

	// load GUI images

	bmpInterfaceLeft = load_bitmap("gui/interface-left.bmp", NULL);
	if (!bmpInterfaceLeft)
		allegro_message("Could not load gui/interface-left.bmp");

	bmpInterfaceRight = load_bitmap("gui/interface-right.bmp", NULL);
	if (!bmpInterfaceRight)
		allegro_message("Could not load gui/interface-right.bmp");

	bmpPortraitOrbulite = load_bitmap("gui/orbulite.bmp", NULL);
	if (!bmpPortraitOrbulite)
		allegro_message("Could not load gui/orbulite.bmp");

	bmpPortraitOrbudark = load_bitmap("gui/orbudark.bmp", NULL);
	if (!bmpPortraitOrbudark)
		allegro_message("Could not load gui/orbudark.bmp");

	bmpPortraitOrbumoon = load_bitmap("gui/orbumoon.bmp", NULL);
	if (!bmpPortraitOrbumoon)
		allegro_message("Could not load gui/orbumoon.bmp");

	bmpPortraitOrbusun = load_bitmap("gui/orbusun.bmp", NULL);
	if (!bmpPortraitOrbusun)
		allegro_message("Could not load gui/orbusun.bmp");

	bmpSign = load_bitmap("gui/sign_background.bmp", NULL);
	if (!bmpSign)
		allegro_message("Could not load gui/sign_background.bmp");

	bmpBarGreen = load_bitmap("gui/bar_green.bmp", NULL);
	if (!bmpBarGreen)
		allegro_message("Could not load gui/bar_green.bmp");
	
	bmpBarRed = load_bitmap("gui/bar_red.bmp", NULL);
	if (!bmpBarRed)
		allegro_message("Could not load gui/bar_red.bmp");

	bmpClock = load_bitmap("gui/clock.bmp", NULL);
	if (!bmpClock)
		allegro_message("Could not load gui/clock.bmp", NULL);

	bmpGold = load_bitmap("gui/gold.bmp", NULL);
	if (!bmpGold)
		allegro_message("Could not load gui/gold.bmp", NULL);

	bmpScore = load_bitmap("gui/score.bmp", NULL);
	if (!bmpScore)
		allegro_message("Could not load gui/score.bmp", NULL);

	// load Orbulite sprites
	bmpOrbuliteSprites[0] = load_bitmap("sprites/orbulite-l1.bmp", NULL);
	bmpOrbuliteSprites[1] = load_bitmap("sprites/orbulite-l2.bmp", NULL);
	bmpOrbuliteSprites[2] = load_bitmap("sprites/orbulite-l3.bmp", NULL);
	bmpOrbuliteSprites[3] = load_bitmap("sprites/orbulite-r1.bmp", NULL);
	bmpOrbuliteSprites[4] = load_bitmap("sprites/orbulite-r2.bmp", NULL);
	bmpOrbuliteSprites[5] = load_bitmap("sprites/orbulite-r3.bmp", NULL);

	// load Orbudark sprites
	bmpOrbudarkSprites[0] = load_bitmap("sprites/orbudark-l1.bmp", NULL);
	bmpOrbudarkSprites[1] = load_bitmap("sprites/orbudark-l2.bmp", NULL);
	bmpOrbudarkSprites[2] = load_bitmap("sprites/orbudark-l3.bmp", NULL);
	bmpOrbudarkSprites[3] = load_bitmap("sprites/orbudark-r1.bmp", NULL);
	bmpOrbudarkSprites[4] = load_bitmap("sprites/orbudark-r2.bmp", NULL);
	bmpOrbudarkSprites[5] = load_bitmap("sprites/orbudark-r3.bmp", NULL);

	// load Orbumoon sprites
	bmpOrbumoonSprites[0] = load_bitmap("sprites/orbumoon-l1.bmp", NULL);
	bmpOrbumoonSprites[1] = load_bitmap("sprites/orbumoon-l2.bmp", NULL);
	bmpOrbumoonSprites[2] = load_bitmap("sprites/orbumoon-l3.bmp", NULL);
	bmpOrbumoonSprites[3] = load_bitmap("sprites/orbumoon-r1.bmp", NULL);
	bmpOrbumoonSprites[4] = load_bitmap("sprites/orbumoon-r2.bmp", NULL);
	bmpOrbumoonSprites[5] = load_bitmap("sprites/orbumoon-r3.bmp", NULL);

	// load Orbusun sprites
	bmpOrbusunSprites[0] = load_bitmap("sprites/orbusun-l1.bmp", NULL);
	bmpOrbusunSprites[1] = load_bitmap("sprites/orbusun-l2.bmp", NULL);
	bmpOrbusunSprites[2] = load_bitmap("sprites/orbusun-l3.bmp", NULL);
	bmpOrbusunSprites[3] = load_bitmap("sprites/orbusun-r1.bmp", NULL);
	bmpOrbusunSprites[4] = load_bitmap("sprites/orbusun-r2.bmp", NULL);
	bmpOrbusunSprites[5] = load_bitmap("sprites/orbusun-r3.bmp", NULL);

	// make sure all Orbulite sprites loaded successfully
	if (!bmpOrbuliteSprites[0])
		allegro_message("Could not load sprites/orbulite-l1.bmp");
	if (!bmpOrbuliteSprites[1])
		allegro_message("Could not load sprites/orbulite-l2.bmp");
	if (!bmpOrbuliteSprites[2])
		allegro_message("Could not load sprites/orbulite-l3.bmp");
	if (!bmpOrbuliteSprites[3])
		allegro_message("Could not load sprites/orbulite-r1.bmp");
	if (!bmpOrbuliteSprites[4])
		allegro_message("Could not load sprites/orbulite-r2.bmp");
	if (!bmpOrbuliteSprites[5])
		allegro_message("Could not load sprites/orbulite-r3.bmp");
	

	// make sure all Orbudark sprites loaded successfully
	if (!bmpOrbudarkSprites[0])
		allegro_message("Could not load sprites/orbudark-l1.bmp");
	if (!bmpOrbudarkSprites[1])
		allegro_message("Could not load sprites/orbudark-l2.bmp");
	if (!bmpOrbudarkSprites[2])
		allegro_message("Could not load sprites/orbudark-l3.bmp");
	if (!bmpOrbudarkSprites[3])
		allegro_message("Could not load sprites/orbudark-r1.bmp");
	if (!bmpOrbudarkSprites[4])
		allegro_message("Could not load sprites/orbudark-r2.bmp");
	if (!bmpOrbudarkSprites[5])
		allegro_message("Could not load sprites/orbudark-r3.bmp");
	

	// make sure all Orbumoon sprites loaded successfully
	if (!bmpOrbumoonSprites[0])
		allegro_message("Could not load sprites/orbumoon-l1.bmp");
	if (!bmpOrbumoonSprites[1])
		allegro_message("Could not load sprites/orbumoon-l2.bmp");
	if (!bmpOrbumoonSprites[2])
		allegro_message("Could not load sprites/orbumoon-l3.bmp");
	if (!bmpOrbumoonSprites[3])
		allegro_message("Could not load sprites/orbumoon-r1.bmp");
	if (!bmpOrbumoonSprites[4])
		allegro_message("Could not load sprites/orbumoon-r2.bmp");
	if (!bmpOrbumoonSprites[5])
		allegro_message("Could not load sprites/orbumoon-r3.bmp");

	// make sure all Orbusun sprites loaded successfully
	if (!bmpOrbusunSprites[0])
		allegro_message("Could not load sprites/orbusun-l1.bmp");
	if (!bmpOrbusunSprites[1])
		allegro_message("Could not load sprites/orbusun-l2.bmp");
	if (!bmpOrbusunSprites[2])
		allegro_message("Could not load sprites/orbusun-l3.bmp");
	if (!bmpOrbusunSprites[3])
		allegro_message("Could not load sprites/orbusun-r1.bmp");
	if (!bmpOrbusunSprites[4])
		allegro_message("Could not load sprites/orbusun-r2.bmp");
	if (!bmpOrbusunSprites[5])
		allegro_message("Could not load sprites/orbusun-r3.bmp");
	
	bmpLivesOrbulite = load_bitmap("gui/orbulite_lives.bmp", NULL);
	bmpLivesOrbudark = load_bitmap("gui/orbudark_lives.bmp", NULL);
	bmpLivesOrbumoon = load_bitmap("gui/orbumoon_lives.bmp", NULL);
	bmpLivesOrbusun = load_bitmap("gui/orbusun_lives.bmp", NULL);

	if (!bmpLivesOrbulite)
		allegro_message("Could not load gui/orbulite_lives.bmp");
	if (!bmpLivesOrbudark)
		allegro_message("Could not load gui/orbudark_lives.bmp");
	if (!bmpLivesOrbumoon)
		allegro_message("Could not load gui/orbumoon_lives.bmp");
	if (!bmpLivesOrbusun)
		allegro_message("Could not load gui/orbusun_lives.bmp");

	bmpGameover = load_bitmap("gui/gameover.bmp", NULL);
	if (!bmpGameover)
		allegro_message("Could not load gui/gameover.bmp");

	bmpOutOfTime = load_bitmap("gui/out_of_time.bmp", NULL);
	if (!bmpOutOfTime)
		allegro_message("Could not load gui/out_of_time.bmp");
	iOutOfTimeY = 0;
	iOutOfTimeTimer = 0;

	bmpLevelup1 = load_bitmap("gui/levelup1.bmp", NULL);
	if (!bmpLevelup1)
		allegro_message("Could not load gui/levelup1.bmp");
	bmpLevelup2 = load_bitmap("gui/levelup2.bmp", NULL);
	if (!bmpLevelup2)
		allegro_message("Could not load gui/levelup2.bmp");
	bmpLevelup3 = load_bitmap("gui/levelup3.bmp", NULL);
	if (!bmpLevelup3)
		allegro_message("Could not load gui/levelup3.bmp");
	iLevelupY = 0;
	iLevelupTimer = 0;


	// load sounds/music
	wavGameOver = load_sample("music/game_over.wav");
	wavJump = load_sample("sounds/jump.wav");
	wavCoin = load_sample("sounds/coin.wav");
	wavNoice = load_sample("sounds/noice.wav");
	wavLevelUp = load_sample("sounds/level_up.wav");
	wavOw = load_sample("sounds/ow.wav");

	LoadSpeechbubbleBitmaps();

	// seed the random number generator
	srand(time(NULL));
}

int App::CalcExpNext(int level)
{
	int exp = (level * 150);
	return exp;
}

void App::AddExp(int amount)
{
	// add exp
	iExpCurrent += amount;

	// check for level-up(s)
	bool leveled_up = false;
	while (iExpCurrent >= iExpNeeded)
	{
		leveled_up = true; // make a note that a level-up has occured (for code below)
		iPlayerLevel++; // increase player's level
		iPlayerAttack += 1; // increase player's attack
		iPlayerHPCurrent += 5; // add 5 HP
		iPlayerHPMax += 5;
		iExpCurrent -= iExpNeeded; // subtract/consume exp
		iExpNeeded = CalcExpNext(iPlayerLevel); // calculate exp needed for next level

		// set Level-Up message to display on the screen, and transform into Orbusun if necessary
		if (iPlayerLevel != 8)
			iLevelupMessage = 1; // on any level-up except Level 8, show the normal level-up message
		else
		{
			// transforming into Orbusun, so add 3 bonus Attack and 15 bonus HP (totaling the +4 attack and +20 HP shown in the image displayed since +1 and +5 were added above)
			iPlayerAttack += 3;
			iPlayerHPCurrent += 15;
			iPlayerHPMax += 15;
			// also, set to Orbusun's movement and jump speeds
			app->player->fNormalMaxSpeed = ORBUSUN_NORMAL_MAX_SPEED;
			app->player->fAccelRate = ORBUSUN_ACCEL_RATE;
			app->player->fJumpSpeed = ORBUSUN_JUMP_SPEED;
			app->player->fFallRate = ORBUSUN_FALL_RATE;

			if (iPlayerSpecies == 0) // if level 8 and currently Orbulite
			{
				iLevelupMessage = 2; // on level 8, let the player know he/she has also mutated into Orbusun
				iPlayerSpecies = 3; // transform into Orbusun
			}
			else // if level 8 and currently Orbudark or Orbumoon
				iLevelupMessage = 3; // on level 8, let the player know he/she has also mutated into Orbusun, and let them know they won't see it until they are no longer Orbudark/Orbumoon
		}
		iLevelupTimer = 0; // set timer of level-up message's display time
		iLevelupY = 600; // set current Y position of level-up message on-screen
	}

	// if level-up(s) occured, play a sound effect
	if (leveled_up)
	{
		play_sample(app->wavLevelUp, 255, 128, 1000, 0); // play "Level up!!" sound effect
	}
}

App::~App()
{
	if (player != NULL)
		delete player;
	if (blBitmaps != NULL)
		delete blBitmaps;
	if (slSounds != NULL)
		delete slSounds;
	if (current_level != NULL)
		delete current_level;
}

void App::tick()
{

#ifdef DEBUG
// debug stuff: press the "A" key in-game to write a list of enemy instances to debug_output.txt
//				warning: do not press the A key in non-game modes (such as at the title screen)
	if (key_state[KEY_A] == 1)
	{
		Enemy * current_enemy = app->current_level->elEnemies->head;
		std::ofstream file;
		file.open("debug_output.txt");
		while (current_enemy != NULL)
		{
			file << current_enemy->strName.c_str() << " | " << current_enemy->bMiscObject;
			file << "\n";
			current_enemy = current_enemy->next;
		}
		file.close();
	}
#endif


	// tick the app.. whether it be the game world, the title screen, the sign screen, etc.
	switch (iAppMode)
	{
		// playing the game, so tick level, player, enemies, etc.
		case MODE_GAME:
		{
			
			// count down the time remaining for this level
			if (!current_level->bIsCutscene)
			{
				current_level->iTimeRemaining--;
				if (current_level->iTimeRemaining <= 0)
				{
					// set to TIMEUP mode to show the "You ran out of time..." message briefly before the player is killed
					iAppMode = MODE_TIMEUP;
					iOutOfTimeTimer = 0;
					iOutOfTimeY = 600;
					app->bDrawPlayer = false;
					app->bTickPlayer = false;
					app->iPlayerHPCurrent = 0;
				}
				else
				{
					if (bTickPlayer)
						player->tick(); // tick player (only if bTickPlayer is true.  bTickPlayer may be false during cutscenes)

					current_level->elEnemies->tick(); // tick enemies
				}
			

				// handle the Level-up display image
				if (iLevelupMessage != 0)
				{
					if (iLevelupY != 270)
						iLevelupY -= 5;
					else
					{
						iLevelupTimer++;
						if (     ( (iLevelupMessage == 1) && (iLevelupTimer >= 360) )  ||     ( (iLevelupTimer >= 1200) )       ) // 3 seconds for normal message, 10 for others
							iLevelupY = 265;
					}
					if (iLevelupY <= -200)
					{
						iLevelupMessage = 0; // stop displaying Level-Up message
					}
				}

				// handle the Zone (Act) display image
				if (bDisplayZone)
				{
					if (current_level->bmpZone != NULL)
					{
						if ( (iDisplayZoneTimer < 90) || (iDisplayZoneTimer > 530) )
							iDisplayZoneX -= 5;
						if (iDisplayZoneX < -current_level->bmpZone->w)
							bDisplayZone = false;
						iDisplayZoneTimer++;
					}
					else
						bDisplayZone = false;
				}
			}
			else // bIsCutscene == true
			{
				current_level->elEnemies->tick(); // tick enemies (includes cutscnee objects, which can do multiple things, including moving to the next level or cutscene)
			}

			break;
		}
		case MODE_GAMEOVER:
		{
			current_level->elEnemies->tick(); // tick enemies (enemies continue to move while the "GAME OVER" message is being displayed)
			break;
		}
		case MODE_TIMEUP:
		{
			current_level->elEnemies->tick(); // tick enemies (enemies continue to move while the "You ran out of time..." message is being displayed)
			if (iOutOfTimeY != 270)
				iOutOfTimeY -= 2;
			else
			{
				iOutOfTimeTimer++;
				if (iOutOfTimeTimer >= 360)
					iOutOfTimeY = 268;
			}
			if (iOutOfTimeY <= -200)
			{
				iAppMode = MODE_GAME;
				playerDeath();
			}
			break;
		}
		// paused.. so check for unpausing
		case MODE_PAUSE:
		{
			break;
		}
		// handle title screen interaction
		case MODE_TITLE:
		{
			break;
		}
		// a sign is being viewed, so check for Enter key to return back to the main game
		case MODE_SIGN:
		{
			if (key_state[KEY_ENTER] == 1)
			{
				iAppMode = MODE_GAME;
			}
			break;
		}
	}
}

void App::draw(BITMAP * destination)
{
	// handle drawing for each mode (game mode, title screen mode, game over screen mode, etc.)

	// draw the game level in MODE_GAME, MODE_SIGN, and MODE_GAMEOVER modes (i.e. when game is being played, and when a sign is being shown in the middle of gameplay, and during the game over screen)
	if ((iAppMode == MODE_GAME) || (iAppMode == MODE_SIGN) || (iAppMode == MODE_GAMEOVER) || (iAppMode == MODE_TIMEUP))
	{
			current_level->draw(destination);

			// draw player
			if (bDrawPlayer)
				player->draw(destination); // draw player (only if bDrawPlayer is true.  bDrawPlayer may be false during cutscenes)

			// draw enemies
			current_level->elEnemies->draw(destination);

			// draw interface for regular levels (not for cutscenes)
			if (!current_level->bIsCutscene)
				drawInterface(destination);

			// draw Zone (Act) image
			if (bDisplayZone)
			{
				if (current_level->bmpZone != NULL)
					draw_sprite(buffer, current_level->bmpZone, iDisplayZoneX, iDisplayZoneY);
			}

			// draw Level-Up image
			switch (iLevelupMessage)
			{
			case 0:
				// none, which is normally the case
				break;
			case 1:
				if (bmpLevelup1)
					draw_sprite(destination, bmpLevelup1, 150, iLevelupY); // normal level-up message (levelup1.bmp)
				break;
			case 2:
				if (bmpLevelup2)
					draw_sprite(destination, bmpLevelup2, 150, iLevelupY); // levelup2.bmp, shown at level 8 if player is not currently powered-up
				break;
			case 3:
				if (bmpLevelup3)
					draw_sprite(destination, bmpLevelup3, 150, iLevelupY); // levelup3.bmp, shown at level 8 if player IS currently powered-up
				break;
			}
	}

	// draw "You ran out of time..." message if MODE_TIMEUP
	if (iAppMode == MODE_TIMEUP)
	{
		draw_sprite(destination, bmpOutOfTime, 150, iOutOfTimeY);
	}

	// draw the sign if a sign is being shown (MODE_SIGN mode)
	if (iAppMode == MODE_SIGN)
	{
		blit(bmpSign, destination, 0, 0, 150, 150, bmpSign->w, bmpSign->h);
		textout_centre_ex(destination, font, strSign1.c_str(), (SCREEN_WIDTH / 2), 210, 0, -1);
		textout_centre_ex(destination, font, strSign2.c_str(), (SCREEN_WIDTH / 2), 230, 0, -1);
		textout_centre_ex(destination, font, strSign3.c_str(), (SCREEN_WIDTH / 2), 250, 0, -1);
		textout_centre_ex(destination, font, strSign4.c_str(), (SCREEN_WIDTH / 2), 270, 0, -1);
		textout_centre_ex(destination, font, strSign5.c_str(), (SCREEN_WIDTH / 2), 290, 0, -1);
		textout_centre_ex(destination, font, strSign6.c_str(), (SCREEN_WIDTH / 2), 310, 0, -1);
		textout_centre_ex(destination, font, strSign7.c_str(), (SCREEN_WIDTH / 2), 330, 0, -1);
		textout_centre_ex(destination, font, strSign8.c_str(), (SCREEN_WIDTH / 2), 350, 0, -1);
		textout_centre_ex(destination, font, strSign9.c_str(), (SCREEN_WIDTH / 2), 370, 0, -1);
		textout_centre_ex(destination, font, strSign10.c_str(), (SCREEN_WIDTH / 2), 390, 0, -1);
	}

	// draw "GAME OVER" text if mode is the game over mode
	if (iAppMode == MODE_GAMEOVER)
	{
		if (bmpGameover)
			draw_sprite(destination, bmpGameover, 151, 151);
	}
}

void App::playerDeath()
{
	// called whenever the player is killed (ex: by an enemy or by falling through a hole in the ground)

	// let the game know that the previous level has stopped in this frame (used mainly in EnemyList::tick())
	app->bLevelStoppedThisFrame = true;

	// stop the music
	current_level->stopMusic();

	// set back to Orbulite species, or Orbusun if level is 8 or higher.  stats are automatic (iJumpSpeed, etc.) because they are in Player class which is re-created upon player death
	if (iPlayerLevel < 8)
		iPlayerSpecies = 0; // set species back to Orbulite (in case it was Orbudark or Orbumoon
	else
		iPlayerSpecies = 3; // set species back to Orbusun (in case it was Orbudark or Orbumoon

	// delete the player and create a new one
	delete player;
	player = new Player();

	// subtract a life
	iLives--;

	// subtract 10% of the score
	if (iPlayerScore > 0)
	{
		unsigned long subtract_amount = ceil((float)iPlayerScore / 4.0f);
		if (subtract_amount > iPlayerScore)
			subtract_amount = iPlayerScore;
		iPlayerScore -= subtract_amount;
	}
	
	// check to see if player is out of lives
	if (iLives < 1)
	{
		// set mode to game over mode
		iAppMode = MODE_GAMEOVER;
		iPlayerHPCurrent = 0; // make sure current HP is 0 for display purposes
		bDrawPlayer = false;
		bTickPlayer = false;
		play_sample(wavGameOver, 255, 128, 1000, 0);
	}
	else
	{
		// player still has lives left...
		play_sample(app->wavOw, 255, 128, 1000, 0); // play "Ow!!" sound effect

		// fade out
		#ifndef DEBUG
			highcolor_fade_out(5);
		#endif

		// start off with full HP
		iPlayerHPCurrent = iPlayerHPMax;

		// restart the level (which refreshes the enemies, etc.)
		current_level->start();
	}
}

void App::AddGold(int amount)
{
	iPlayerGold += amount;
	while (iPlayerGold >= 50)
	{
		iPlayerGold -= 50;
		iLives++;
	}
}

void App::ShowSign(char* line1, char* line2, char* line3, char* line4, char* line5, char* line6, char* line7, char* line8, char* line9, char* line10)
{
	strSign1.assign(line1);
	strSign2.assign(line2);
	strSign3.assign(line3);
	strSign4.assign(line4);
	strSign5.assign(line5);
	strSign6.assign(line6);
	strSign7.assign(line7);
	strSign8.assign(line8);
	strSign9.assign(line9);
	strSign10.assign(line10);
	iAppMode = MODE_SIGN;
}

void App::drawInterface(BITMAP * destination)
{
	// draw left interface box
	if (bmpInterfaceLeft != NULL)
		draw_sprite(destination, bmpInterfaceLeft, 0, 0);

	stream.clear();
	stream << iLives;
	stream >> strLives;
	stream.clear();
	stream << ceil((float)current_level->iTimeRemaining / 120.0f);
	stream >> strTime;

	// draw player's species name and portrait
	if (iPlayerSpecies == 0) // Orbulite
	{
		// draw portrait and name in top-left box
		draw_sprite(destination, bmpPortraitOrbulite, 52, 37);
		textout_ex(destination, font, "Orbulite", 154, 56, 0, -1);

		// draw Lives
		draw_sprite(destination, bmpLivesOrbulite, 20, 560);
		textout_ex(destination, fontArial26, strLives.c_str(), 80, 562, 0, -1);
	}
	else if (iPlayerSpecies == 1) // Orbudark
	{
		// draw portrait and name in top-left box
		draw_sprite(destination, bmpPortraitOrbudark, 52, 37);
		textout_ex(destination, font, "Orbudark", 154, 56, 0, -1);

		// draw Lives
		draw_sprite(destination, bmpLivesOrbudark, 20, 560);
		textout_ex(destination, fontArial26, strLives.c_str(), 80, 562, 0, -1);
	}
	else if (iPlayerSpecies == 2) // Orbumoon
	{
		// draw portrait and name in top-left box
		draw_sprite(destination, bmpPortraitOrbumoon, 52, 37);
		textout_ex(destination, font, "Orbumoon", 154, 56, 0, -1);
		
		// draw Lives
		draw_sprite(destination, bmpLivesOrbumoon, 20, 560);
		textout_ex(destination, fontArial26, strLives.c_str(), 80, 562, 0, -1);
	}
	else if (iPlayerSpecies == 3) // Orbusun
	{
		// draw portrait and name in top-left box
		draw_sprite(destination, bmpPortraitOrbusun, 52, 37);
		textout_ex(destination, font, "Orbusun", 154, 56, 0, -1);
		
		// draw Lives
		draw_sprite(destination, bmpLivesOrbusun, 20, 560);
		textout_ex(destination, fontArial26, strLives.c_str(), 80, 562, 0, -1);
	}
	
	std::string strExp;
	std::string strLevel;
	std::string strHP;
	std::string strEnemyHP;
	std::string strGold;
	std::string strScore;
	stream.clear();
	stream << iExpCurrent << "/" << iExpNeeded;
	stream >> strExp;
	stream.clear();
	stream << iPlayerLevel;
	stream >> strLevel;
	stream.clear();
	stream << iPlayerHPCurrent << "/" << iPlayerHPMax;
	stream >> strHP;
	stream.clear();
	stream << iEnemyHPCurrent << "/" << iEnemyHPMax;
	stream >> strEnemyHP;
	stream.clear();
	stream << iPlayerGold;
	stream >> strGold;
	stream.clear();

	if (iPlayerScore < 10)
		stream << "00000" << iPlayerScore;
	else if (iPlayerScore < 100)
		stream << "0000" << iPlayerScore;
	else if (iPlayerScore < 1000)
		stream << "0000" << iPlayerScore;
	else if (iPlayerScore < 10000)
		stream << "000" << iPlayerScore;
	else if (iPlayerScore < 100000)
		stream << "00" << iPlayerScore;
	else
		stream << iPlayerScore;

	stream >> strScore;

	int iHPPercent = ceil( ((float)iPlayerHPCurrent / (float)iPlayerHPMax) * 100 );
	int iExpPercent = ceil( ((float)iExpCurrent / (float)iExpNeeded) * 100 );

	textout_ex(destination, font, "LVL", 254, 56, 0, -1);
	textout_ex(destination, font, strLevel.c_str(), 284, 56, 0, -1);

	// draw HP bar and HP text
	blit(bmpBarRed, destination, 0, 0, 218, 76, 100, 20);
	blit(bmpBarGreen, destination, 0, 0, 218, 76, iHPPercent, 20);
	if (iHPPercent < 100)
		line(destination, 218 + iHPPercent, 76, 218 + iHPPercent, 95, 0); // draw a line divider between the red and green parts of the HP bar
	textout_ex(destination, font, "HP:", 154, 78, 0, -1);
	textout_centre_ex(destination, font, strHP.c_str(), 270, 78, 0, -1);

	// draw EXP bar and EXP text
	blit(bmpBarRed, destination, 0, 0, 218, 98, 100, 20);
	blit(bmpBarGreen, destination, 0, 0, 218, 98, iExpPercent, 20);
	if (iExpPercent < 100)
		line(destination, 218 + iExpPercent, 98, 218 + iExpPercent, 117, 0); // draw a line divider between the red and green parts of the HP bar
	textout_ex(destination, font, "EXP:", 154, 100, 0, -1);
	textout_centre_ex(destination, font, strExp.c_str(), 270, 100, 0, -1);

	// draw time remaining
	draw_sprite(destination, bmpClock, 680, 560);
	textout_ex(destination, fontArial26, strTime.c_str(), 714, 562, 0, -1);

	// draw gold coin count
	draw_sprite(destination, bmpGold, 580, 560);
	textout_ex(destination, fontArial26, strGold.c_str(), 640, 562, 0, -1);

	// draw score counter
	draw_sprite(destination, bmpScore, (SCREEN_HEIGHT / 2), 565);
	textout_ex(destination, fontArial26, strScore.c_str(), 390, 567, 0, -1);

	// draw right-side interface (shows enemy details)
	if (bShowEnemyBox)
	{
		// draw interface box
		if (bmpInterfaceRight != NULL)
			draw_sprite(destination, bmpInterfaceRight, 460, 0);

		// draw enemy portrait
		if (bmpEnemyPortrait != NULL)
			draw_sprite(destination, bmpEnemyPortrait, 662, 22);

		// draw HP bar and HP text
		blit(bmpBarRed, destination, 0, 0, 548, 76, 100, 20);
		blit(bmpBarGreen, destination, 0, 0, 548, 76, iEnemyHPPercent, 20);
		textout_ex(destination, font, strEnemyName.c_str(), 490, 56, 0, -1); // display enemy's name
		if ((iEnemyHPPercent < 100) && (iEnemyHPPercent > 1))
			line(destination, 548 + iEnemyHPPercent, 76, 548 + iEnemyHPPercent, 95, 0); // draw a line divider between the red and green parts of the HP bar
		textout_ex(destination, font, "HP:", 490, 78, 0, -1);
		textout_centre_ex(destination, font, strEnemyHP.c_str(), 602, 76, 0, -1);

		// reduce timer of time to show this box
		iEnemyBoxTimer--; // reduce timer of showing this box
		if (iEnemyBoxTimer <= 0) // if time is up to stop showing this box
			bShowEnemyBox = false; // stop showing this box
	}
}


bool App::LoadSpeechbubbleBitmaps()
{
	std::string strTemp;

	strTemp = "gui/speech-c.bmp";
	bmpSpeechC = load_bitmap(strTemp.c_str(), NULL);
	if (!bmpSpeechC) // if the custom image doesn't exist..
	{
		// load the default one
		bmpSpeechC = load_bitmap("gui/speech-c.bmp", NULL);
		if (!bmpSpeechC)
		{
			allegro_message("Error: Couldn't find speech-c.bmp");
			return false;
		}
	}

	strTemp = "gui/speech-l.bmp";
	bmpSpeechL = load_bitmap(strTemp.c_str(), NULL);
	if (!bmpSpeechL) // if the custom image doesn't exist..
	{
		// load the default one
		bmpSpeechL = load_bitmap("gui/speech-l.bmp", NULL);
		if (!bmpSpeechL)
		{
			allegro_message("Error: Couldn't find speech-l.bmp");
			return false;
		}
	}

	strTemp = "gui/speech-tl.bmp";
	bmpSpeechTL = load_bitmap(strTemp.c_str(), NULL);
	if (!bmpSpeechTL) // if the custom image doesn't exist..
	{
		// load the default one
		bmpSpeechTL = load_bitmap("gui/speech-tl.bmp", NULL);
		if (!bmpSpeechTL)
		{
			allegro_message("Error: Couldn't find speech-tl.bmp");
			return false;
		}
	}

	strTemp = "gui/speech-t.bmp";
	bmpSpeechT = load_bitmap(strTemp.c_str(), NULL);
	if (!bmpSpeechT) // if the custom image doesn't exist..
	{
		// load the default one
		bmpSpeechT = load_bitmap("gui/speech-t.bmp", NULL);
		if (!bmpSpeechT)
		{
			allegro_message("Error: Couldn't find speech-t.bmp");
			return false;
		}
	}

	strTemp = "gui/speech-b.bmp";
	bmpSpeechB = load_bitmap(strTemp.c_str(), NULL);
	if (!bmpSpeechB) // if the custom image doesn't exist..
	{
		// load the default one
		bmpSpeechB = load_bitmap("gui/speech-b.bmp", NULL);
		if (!bmpSpeechB)
		{
			allegro_message("Error: Couldn't find speech-b.bmp");
			return false;
		}
	}

	strTemp = "gui/speech-bl.bmp";
	bmpSpeechBL = load_bitmap(strTemp.c_str(), NULL);
	if (!bmpSpeechBL) // if the custom image doesn't exist..
	{
		// load the default one
		bmpSpeechBL = load_bitmap("gui/speech-bl.bmp", NULL);
		if (!bmpSpeechBL)
		{
			allegro_message("Error: Couldn't find speech-bl.bmp");
			return false;
		}
	}

	strTemp = "gui/speech-br.bmp";
	bmpSpeechBR = load_bitmap(strTemp.c_str(), NULL);
	if (!bmpSpeechBR) // if the custom image doesn't exist..
	{
		// load the default one
		bmpSpeechBR = load_bitmap("gui/speech-br.bmp", NULL);
		if (!bmpSpeechBR)
		{
			allegro_message("Error: Couldn't find speech-br.bmp");
			return false;
		}
	}

	strTemp = "gui/speech-r.bmp";
	bmpSpeechR = load_bitmap(strTemp.c_str(), NULL);
	if (!bmpSpeechR) // if the custom image doesn't exist..
	{
		// load the default one
		bmpSpeechR = load_bitmap("gui/speech-r.bmp", NULL);
		if (!bmpSpeechR)
		{
			allegro_message("Error: Couldn't find speech-r.bmp");
			return false;
		}
	}

	strTemp = "gui/speech-tr.bmp";
	bmpSpeechTR = load_bitmap(strTemp.c_str(), NULL);
	if (!bmpSpeechTR) // if the custom image doesn't exist..
	{
		// load the default one
		bmpSpeechTR = load_bitmap("gui/speech-tr.bmp", NULL);
		if (!bmpSpeechTR)
		{
			allegro_message("Error: Couldn't find speech-tr.bmp");
			return false;
		}
	}

	strTemp = "gui/speech-pointright.bmp";
	bmpSpeechPointRight = load_bitmap(strTemp.c_str(), NULL);
	if (!bmpSpeechPointRight) // if the custom image doesn't exist..
	{
		// load the default one
		bmpSpeechPointRight = load_bitmap("gui/speech-pointright.bmp", NULL);
		if (!bmpSpeechPointRight)
		{
			allegro_message("Error: Couldn't find speech-pointright.bmp");
			return false;
		}
	}

	strTemp = "gui/speech-pointleft.bmp";
	bmpSpeechPointLeft = load_bitmap(strTemp.c_str(), NULL);
	if (!bmpSpeechPointLeft) // if the custom image doesn't exist..
	{
		// load the default one
		bmpSpeechPointLeft = load_bitmap("gui/speech-pointleft.bmp", NULL);
		if (!bmpSpeechPointLeft)
		{
			allegro_message("Error: Couldn't find speech-pointleft.bmp");
			return false;
		}
	}

	return true;
}