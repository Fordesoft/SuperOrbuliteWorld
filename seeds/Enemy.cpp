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

Enemy::Enemy(int enemy_type_id, int x_pixel, int y_pixel, bool is_misc_object)
{
	iTypeId = enemy_type_id;

	fX = (float)x_pixel;
	fY = (float)y_pixel;
	fVel_x = 0;
	fVel_y = 0;
	iX = x_pixel;
	iY = y_pixel;
	iStartX = x_pixel;
	iStartY = y_pixel;
	iHP = 50;
	iHPMax = 5;
	iAttack = 5;
	strName = "";
	bMoveAfterAI = true; // handle movement normally by default
	bInvincible = false;
	bVisible = true;
	bDoCustomDrawing = true;

	iExpGiven = 100; // how much exp the enemy gives to the player when killed, default

	next = NULL;

	bmpImage = NULL;
	bmpPortrait = NULL;

	for (int i = 0; i < 20; i++)
		bmpFrames[i] = NULL;
	for (int i = 0; i < 10; i++)
		wavSounds[i] = NULL;
	for (int i = 0; i < 10; i++)
		iVariables[i] = 0;
	for (int i = 0; i < 10; i++)
		enOtherParts[i] = NULL;
	for (int i = 0; i < 10; i++)
		sbSpeechBubbles[i] = NULL;

	bMiscObject = is_misc_object;

	// hard-coded enemy data (artwork & HP)
	switch (enemy_type_id)
	{
		case 1: // Bootworm short-range to the left
		{
			strName = "Bootworm";
			bmpFrames[0] = app->blBitmaps->LoadBitmap("monsters/bootworm/left.bmp");
			bmpFrames[1] = app->blBitmaps->LoadBitmap("monsters/bootworm/right.bmp");
			bmpPortrait = app->blBitmaps->LoadBitmap("monsters/bootworm/portrait.bmp");
			bmpImage = bmpFrames[0];
			iExpGiven = 100;
			iHP = 20;
			iHPMax = iHP;
			iAttack = 5;
			wavSounds[0] = app->slSounds->LoadSound("sounds/bootworm_damage.wav");
			wavSounds[1] = app->slSounds->LoadSound("sounds/bootworm_death.wav");
			break;
		}
		case 2: // Bootworm long-range to the left
		{
			strName = "Bootworm";
			bmpFrames[0] = app->blBitmaps->LoadBitmap("monsters/bootworm/left.bmp");
			bmpFrames[1] = app->blBitmaps->LoadBitmap("monsters/bootworm/right.bmp");
			bmpPortrait = app->blBitmaps->LoadBitmap("monsters/bootworm/portrait.bmp");
			bmpImage = bmpFrames[0];
			iExpGiven = 100;
			iHP = 20;
			iHPMax = iHP;
			iAttack = 5;
			wavSounds[0] = app->slSounds->LoadSound("sounds/bootworm_damage.wav");
			wavSounds[1] = app->slSounds->LoadSound("sounds/bootworm_death.wav");
			break;
		}
		case 3: // Question Mark (?) Block 1 - Darkstone powerup
		{
			strName = "Darkstone block";
			iVariables[1] = 20; // set animation timer variable
			iVariables[2] = 1; // animation frame number
			bmpFrames[0] = app->blBitmaps->LoadBitmap("monsters/block/block_used.bmp");
			bmpFrames[1] = app->blBitmaps->LoadBitmap("monsters/block/block1.bmp");
			bmpFrames[2] = app->blBitmaps->LoadBitmap("monsters/block/block2.bmp");
			bmpFrames[3] = app->blBitmaps->LoadBitmap("monsters/block/block3.bmp");
			bmpImage = bmpFrames[1];
			break;
		}
		case 4: // Question Mark (?) Block 2 - coin
		{
			strName = "Coin block";
			iVariables[1] = 20; // set animation timer variable
			iVariables[2] = 1; // animation frame number
			bmpFrames[0] = app->blBitmaps->LoadBitmap("monsters/block/block_used.bmp");
			bmpFrames[1] = app->blBitmaps->LoadBitmap("monsters/block/block1.bmp");
			bmpFrames[2] = app->blBitmaps->LoadBitmap("monsters/block/block2.bmp");
			bmpFrames[3] = app->blBitmaps->LoadBitmap("monsters/block/block3.bmp");
			bmpImage = bmpFrames[1];
			break;
		}
		case 5: // Darkstone
		{
			strName = "Darkstone";
			bmpImage = app->blBitmaps->LoadBitmap("monsters/darkstone.bmp");
			bMoveAfterAI = false; // handle movement in AI instead
			break;
		}
		case 6: // Moonstone
		{
			strName = "Moonstone";
			bmpImage = app->blBitmaps->LoadBitmap("monsters/moonstone.bmp");
			bMoveAfterAI = false; // handle movement in AI instead
			break;
		}
		case 7: // Question Mark (?) Block 3 - Moonstone powerup
		{
			strName = "Moonstone block";
			iVariables[1] = 20; // set animation timer variable
			iVariables[2] = 1; // animation frame number
			bmpFrames[0] = app->blBitmaps->LoadBitmap("monsters/block/block_used.bmp");
			bmpFrames[1] = app->blBitmaps->LoadBitmap("monsters/block/block1.bmp");
			bmpFrames[2] = app->blBitmaps->LoadBitmap("monsters/block/block2.bmp");
			bmpFrames[3] = app->blBitmaps->LoadBitmap("monsters/block/block3.bmp");
			bmpImage = bmpFrames[1];
			break;
		}
		case 8: // Jaywalker
		{
			strName = "Jaywalker";
			bmpFrames[0] = app->blBitmaps->LoadBitmap("monsters/jaywalker/jaywalker-l1.bmp");
			bmpFrames[1] = app->blBitmaps->LoadBitmap("monsters/jaywalker/jaywalker-l2.bmp");
			bmpFrames[2] = app->blBitmaps->LoadBitmap("monsters/jaywalker/jaywalker-r1.bmp");
			bmpFrames[3] = app->blBitmaps->LoadBitmap("monsters/jaywalker/jaywalker-r2.bmp");
			bmpPortrait = app->blBitmaps->LoadBitmap("monsters/jaywalker/portrait.bmp");
			bmpImage = bmpFrames[0];
			iHP = 100;
			iHPMax = iHP;
			iExpGiven = 150;
			iAttack = 10;
			wavSounds[0] = app->slSounds->LoadSound("sounds/jaywalker_damage.wav");
			wavSounds[1] = app->slSounds->LoadSound("sounds/jaywalker_death.wav");
			bMoveAfterAI = false; // movement will be handled in CustomAI() instead of in tick()
			break;
		}
		case 9: // Janewalker
		{
			strName = "Janewalker";
			bmpFrames[0] = app->blBitmaps->LoadBitmap("monsters/janewalker/janewalker-l1.bmp");
			bmpFrames[1] = app->blBitmaps->LoadBitmap("monsters/janewalker/janewalker-l2.bmp");
			bmpFrames[2] = app->blBitmaps->LoadBitmap("monsters/janewalker/janewalker-r1.bmp");
			bmpFrames[3] = app->blBitmaps->LoadBitmap("monsters/janewalker/janewalker-r2.bmp");
			bmpPortrait = app->blBitmaps->LoadBitmap("monsters/janewalker/portrait.bmp");
			bmpImage = bmpFrames[0];
			iHP = 120;
			iHPMax = iHP;
			iExpGiven = 180;
			iAttack = 12;
			wavSounds[0] = app->slSounds->LoadSound("sounds/janewalker_damage.wav");
			wavSounds[1] = app->slSounds->LoadSound("sounds/janewalker_death.wav");
			bMoveAfterAI = false; // movement will be handled in CustomAI() instead of in tick()
			break;
		}
		case 10: // Bladeguin
		{
			strName = "Bladeguin";
			bmpFrames[0] = app->blBitmaps->LoadBitmap("monsters/bladeguin/l1.bmp");
			bmpFrames[1] = app->blBitmaps->LoadBitmap("monsters/bladeguin/l2.bmp");
			bmpFrames[2] = app->blBitmaps->LoadBitmap("monsters/bladeguin/l3.bmp");
			bmpFrames[3] = app->blBitmaps->LoadBitmap("monsters/bladeguin/l2.bmp");

			bmpFrames[4] = app->blBitmaps->LoadBitmap("monsters/bladeguin/r1.bmp");
			bmpFrames[5] = app->blBitmaps->LoadBitmap("monsters/bladeguin/r2.bmp");
			bmpFrames[6] = app->blBitmaps->LoadBitmap("monsters/bladeguin/r3.bmp");
			bmpFrames[7] = app->blBitmaps->LoadBitmap("monsters/bladeguin/r2.bmp");

			bmpFrames[8] = app->blBitmaps->LoadBitmap("monsters/bladeguin/lsword1.bmp");
			bmpFrames[9] = app->blBitmaps->LoadBitmap("monsters/bladeguin/lsword2.bmp");
			bmpFrames[10] = app->blBitmaps->LoadBitmap("monsters/bladeguin/lsword3.bmp");
			bmpFrames[11] = app->blBitmaps->LoadBitmap("monsters/bladeguin/lsword2.bmp");

			bmpFrames[12] = app->blBitmaps->LoadBitmap("monsters/bladeguin/rsword1.bmp");
			bmpFrames[13] = app->blBitmaps->LoadBitmap("monsters/bladeguin/rsword2.bmp");
			bmpFrames[14] = app->blBitmaps->LoadBitmap("monsters/bladeguin/rsword3.bmp");
			bmpFrames[15] = app->blBitmaps->LoadBitmap("monsters/bladeguin/rsword2.bmp");

			bmpPortrait = app->blBitmaps->LoadBitmap("monsters/bladeguin/portrait.bmp");
			bmpImage = bmpFrames[0];
			iHP = 50;
			iHPMax = iHP;
			iExpGiven = 150;
			iAttack = 7;
			bMoveAfterAI = false; // movement will be handled in AI code instead of in Enemy::tick()

			wavSounds[0] = app->slSounds->LoadSound("sounds/bladeguin_damage.wav");
			wavSounds[1] = app->slSounds->LoadSound("sounds/bladeguin_death.wav");
			break;
		}
		case 11: // Bootworm short-range to the right
		{
			strName = "Bootworm";
			bmpFrames[0] = app->blBitmaps->LoadBitmap("monsters/bootworm/left.bmp");
			bmpFrames[1] = app->blBitmaps->LoadBitmap("monsters/bootworm/right.bmp");
			bmpPortrait = app->blBitmaps->LoadBitmap("monsters/bootworm/portrait.bmp");
			bmpImage = bmpFrames[1];
			iVariables[2] = 1;
			iExpGiven = 100;
			iHP = 20;
			iHPMax = iHP;
			iAttack = 5;
			wavSounds[0] = app->slSounds->LoadSound("sounds/bootworm_damage.wav");
			wavSounds[1] = app->slSounds->LoadSound("sounds/bootworm_death.wav");
			break;
		}
		case 12: // Baby Sea Dragon
		{
			strName = "Baby Sea Dragon";
			bmpFrames[0] = app->blBitmaps->LoadBitmap("monsters/babyseadragon/babyseadragon.bmp");
			bmpPortrait = app->blBitmaps->LoadBitmap("monsters/babyseadragon/portrait.bmp");
			wavSounds[0] = app->slSounds->LoadSound("monsters/babyseadragon/large_energy_ball.wav");
			wavSounds[1] = app->slSounds->LoadSound("monsters/babyseadragon/stream.wav");
			bmpImage = bmpFrames[0];
			iExpGiven = 500;
			iHP = 50;
			iHPMax = iHP;
			iAttack = 5;
			bMoveAfterAI = false;
			break;
		}
		case 13: // Baby Sea Dragon boss's large bullet
		{
			strName = "Energy Ball of Water";
			bmpFrames[0] = app->blBitmaps->LoadBitmap("monsters/babyseadragon/bullet-large.bmp");
			bmpPortrait = app->blBitmaps->LoadBitmap("monsters/babyseadragon/portrait-energyball-large.bmp");
			bmpImage = bmpFrames[0];
			bInvincible = true;
			iExpGiven = 1;
			iHP = 1;
			iHPMax = iHP;
			iAttack = 10;
			bMoveAfterAI = true;
			break;
		}
		case 14: // Baby Sea Dragon boss's small bullet
		{
			strName = "Small Energy Ball";
			bmpFrames[0] = app->blBitmaps->LoadBitmap("monsters/babyseadragon/bullet-small.bmp");
			bmpPortrait = app->blBitmaps->LoadBitmap("monsters/babyseadragon/portrait-energyball-small.bmp");
			bmpImage = bmpFrames[0];
			bInvincible = true;
			iExpGiven = 1;
			iHP = 1;
			iHPMax = iHP;
			iAttack = 5;
			bMoveAfterAI = true;
			break;
		}
		case 15: // Lava spring (invincible, part of Red River Cave.  dangerous scenery, basically)
		{
			strName = "Lava spring";
			bmpFrames[0] = app->blBitmaps->LoadBitmap("monsters/lava_spring/frame1.bmp");
			bmpFrames[1] = app->blBitmaps->LoadBitmap("monsters/lava_spring/frame2.bmp");
			bmpFrames[2] = app->blBitmaps->LoadBitmap("monsters/lava_spring/frame3.bmp");
			bmpFrames[3] = app->blBitmaps->LoadBitmap("monsters/lava_spring/frame4.bmp");
			bmpPortrait = app->blBitmaps->LoadBitmap("monsters/lava_spring/portrait.bmp");
			bmpImage = bmpFrames[0];
			bInvincible = true;
			iExpGiven = 1;
			iHP = 1;
			iHPMax = iHP;
			iAttack = 10;
			iVariables[0] = 20; // set animation timer
			iVariables[2] = 3; // set state to begin at "stopped at bottom"
			iVariables[3] = 360; // set a 3-second pause at bottom
			fY += 32; // start at 32 pixels lower than appears in map editor
			iY += 32;
			bMoveAfterAI = true;
			break;
		}

		case 16: // Lava spring (fast version)
		{
			strName = "Lava spring";
			bmpFrames[0] = app->blBitmaps->LoadBitmap("monsters/lava_spring/frame1.bmp");
			bmpFrames[1] = app->blBitmaps->LoadBitmap("monsters/lava_spring/frame2.bmp");
			bmpFrames[2] = app->blBitmaps->LoadBitmap("monsters/lava_spring/frame3.bmp");
			bmpFrames[3] = app->blBitmaps->LoadBitmap("monsters/lava_spring/frame4.bmp");
			bmpPortrait = app->blBitmaps->LoadBitmap("monsters/lava_spring/portrait.bmp");
			bmpImage = bmpFrames[0];
			bInvincible = true;
			iExpGiven = 1;
			iHP = 1;
			iHPMax = iHP;
			iAttack = 10;
			iVariables[0] = 20; // set animation timer
			iVariables[2] = 3; // set state to begin at "stopped at bottom"
			iVariables[3] = 240; // set a 2-second pause at bottom
			fY += 32; // start at 32 pixels lower than appears in map editor
			iY += 32;
			bMoveAfterAI = true;
			break;
		}

		case 17: // Lava spring (delay 1)
		{
			iVariables[3] = 20; // set a pause at bottom

			strName = "Lava spring";
			bmpFrames[0] = app->blBitmaps->LoadBitmap("monsters/lava_spring/frame1.bmp");
			bmpFrames[1] = app->blBitmaps->LoadBitmap("monsters/lava_spring/frame2.bmp");
			bmpFrames[2] = app->blBitmaps->LoadBitmap("monsters/lava_spring/frame3.bmp");
			bmpFrames[3] = app->blBitmaps->LoadBitmap("monsters/lava_spring/frame4.bmp");
			bmpPortrait = app->blBitmaps->LoadBitmap("monsters/lava_spring/portrait.bmp");
			bmpImage = bmpFrames[0];
			bInvincible = true;
			iExpGiven = 1;
			iHP = 1;
			iHPMax = iHP;
			iAttack = 10;
			iVariables[0] = 20; // set animation timer
			iVariables[2] = 3; // set state to begin at "stopped at bottom"
			fY += 32; // start at 32 pixels lower than appears in map editor
			iY += 32;
			bMoveAfterAI = true;
			break;
		}
		case 18: // Lava spring (delay 2)
		{
			iVariables[3] = 40; // set a pause at bottom
			
			strName = "Lava spring";
			bmpFrames[0] = app->blBitmaps->LoadBitmap("monsters/lava_spring/frame1.bmp");
			bmpFrames[1] = app->blBitmaps->LoadBitmap("monsters/lava_spring/frame2.bmp");
			bmpFrames[2] = app->blBitmaps->LoadBitmap("monsters/lava_spring/frame3.bmp");
			bmpFrames[3] = app->blBitmaps->LoadBitmap("monsters/lava_spring/frame4.bmp");
			bmpPortrait = app->blBitmaps->LoadBitmap("monsters/lava_spring/portrait.bmp");
			bmpImage = bmpFrames[0];
			bInvincible = true;
			iExpGiven = 1;
			iHP = 1;
			iHPMax = iHP;
			iAttack = 10;
			iVariables[0] = 20; // set animation timer
			iVariables[2] = 3; // set state to begin at "stopped at bottom"
			fY += 32; // start at 32 pixels lower than appears in map editor
			iY += 32;
			bMoveAfterAI = true;
			break;
		}
		case 19: // Lava spring (delay 3)
		{
			iVariables[3] = 60; // set a pause at bottom

			strName = "Lava spring";
			bmpFrames[0] = app->blBitmaps->LoadBitmap("monsters/lava_spring/frame1.bmp");
			bmpFrames[1] = app->blBitmaps->LoadBitmap("monsters/lava_spring/frame2.bmp");
			bmpFrames[2] = app->blBitmaps->LoadBitmap("monsters/lava_spring/frame3.bmp");
			bmpFrames[3] = app->blBitmaps->LoadBitmap("monsters/lava_spring/frame4.bmp");
			bmpPortrait = app->blBitmaps->LoadBitmap("monsters/lava_spring/portrait.bmp");
			bmpImage = bmpFrames[0];
			bInvincible = true;
			iExpGiven = 1;
			iHP = 1;
			iHPMax = iHP;
			iAttack = 10;
			iVariables[0] = 20; // set animation timer
			iVariables[2] = 3; // set state to begin at "stopped at bottom"
			fY += 32; // start at 32 pixels lower than appears in map editor
			iY += 32;
			bMoveAfterAI = true;
			break;
		}
		case 20: // Lava spring (delay 4)
		{
			iVariables[3] = 80; // set a pause at bottom

			strName = "Lava spring";
			bmpFrames[0] = app->blBitmaps->LoadBitmap("monsters/lava_spring/frame1.bmp");
			bmpFrames[1] = app->blBitmaps->LoadBitmap("monsters/lava_spring/frame2.bmp");
			bmpFrames[2] = app->blBitmaps->LoadBitmap("monsters/lava_spring/frame3.bmp");
			bmpFrames[3] = app->blBitmaps->LoadBitmap("monsters/lava_spring/frame4.bmp");
			bmpPortrait = app->blBitmaps->LoadBitmap("monsters/lava_spring/portrait.bmp");
			bmpImage = bmpFrames[0];
			bInvincible = true;
			iExpGiven = 1;
			iHP = 1;
			iHPMax = iHP;
			iAttack = 10;
			iVariables[0] = 20; // set animation timer
			iVariables[2] = 3; // set state to begin at "stopped at bottom"
			fY += 32; // start at 32 pixels lower than appears in map editor
			iY += 32;
			bMoveAfterAI = true;
			break;
		}
		case 21: // Lava spring (delay 5)
		{
			iVariables[3] = 100; // set a pause at bottom
			
			strName = "Lava spring";
			bmpFrames[0] = app->blBitmaps->LoadBitmap("monsters/lava_spring/frame1.bmp");
			bmpFrames[1] = app->blBitmaps->LoadBitmap("monsters/lava_spring/frame2.bmp");
			bmpFrames[2] = app->blBitmaps->LoadBitmap("monsters/lava_spring/frame3.bmp");
			bmpFrames[3] = app->blBitmaps->LoadBitmap("monsters/lava_spring/frame4.bmp");
			bmpPortrait = app->blBitmaps->LoadBitmap("monsters/lava_spring/portrait.bmp");
			bmpImage = bmpFrames[0];
			bInvincible = true;
			iExpGiven = 1;
			iHP = 1;
			iHPMax = iHP;
			iAttack = 10;
			iVariables[0] = 20; // set animation timer
			iVariables[2] = 3; // set state to begin at "stopped at bottom"
			fY += 32; // start at 32 pixels lower than appears in map editor
			iY += 32;
			bMoveAfterAI = true;
			break;
		}
		case 22: // Lava spring (delay 6)
		{
			iVariables[3] = 120; // set a pause at bottom
			
			strName = "Lava spring";
			bmpFrames[0] = app->blBitmaps->LoadBitmap("monsters/lava_spring/frame1.bmp");
			bmpFrames[1] = app->blBitmaps->LoadBitmap("monsters/lava_spring/frame2.bmp");
			bmpFrames[2] = app->blBitmaps->LoadBitmap("monsters/lava_spring/frame3.bmp");
			bmpFrames[3] = app->blBitmaps->LoadBitmap("monsters/lava_spring/frame4.bmp");
			bmpPortrait = app->blBitmaps->LoadBitmap("monsters/lava_spring/portrait.bmp");
			bmpImage = bmpFrames[0];
			bInvincible = true;
			iExpGiven = 1;
			iHP = 1;
			iHPMax = iHP;
			iAttack = 10;
			iVariables[0] = 20; // set animation timer
			iVariables[2] = 3; // set state to begin at "stopped at bottom"
			fY += 32; // start at 32 pixels lower than appears in map editor
			iY += 32;
			bMoveAfterAI = true;
			break;
		}
		case 23: // Lava spring (delay 7)
		{
			iVariables[3] = 140; // set a pause at bottom
			
			strName = "Lava spring";
			bmpFrames[0] = app->blBitmaps->LoadBitmap("monsters/lava_spring/frame1.bmp");
			bmpFrames[1] = app->blBitmaps->LoadBitmap("monsters/lava_spring/frame2.bmp");
			bmpFrames[2] = app->blBitmaps->LoadBitmap("monsters/lava_spring/frame3.bmp");
			bmpFrames[3] = app->blBitmaps->LoadBitmap("monsters/lava_spring/frame4.bmp");
			bmpPortrait = app->blBitmaps->LoadBitmap("monsters/lava_spring/portrait.bmp");
			bmpImage = bmpFrames[0];
			bInvincible = true;
			iExpGiven = 1;
			iHP = 1;
			iHPMax = iHP;
			iAttack = 10;
			iVariables[0] = 20; // set animation timer
			iVariables[2] = 3; // set state to begin at "stopped at bottom"
			fY += 32; // start at 32 pixels lower than appears in map editor
			iY += 32;
			bMoveAfterAI = true;
			break;
		}
		case 24: // Lava spring (delay 8)
		{
			iVariables[3] = 160; // set a pause at bottom
			
			strName = "Lava spring";
			bmpFrames[0] = app->blBitmaps->LoadBitmap("monsters/lava_spring/frame1.bmp");
			bmpFrames[1] = app->blBitmaps->LoadBitmap("monsters/lava_spring/frame2.bmp");
			bmpFrames[2] = app->blBitmaps->LoadBitmap("monsters/lava_spring/frame3.bmp");
			bmpFrames[3] = app->blBitmaps->LoadBitmap("monsters/lava_spring/frame4.bmp");
			bmpPortrait = app->blBitmaps->LoadBitmap("monsters/lava_spring/portrait.bmp");
			bmpImage = bmpFrames[0];
			bInvincible = true;
			iExpGiven = 1;
			iHP = 1;
			iHPMax = iHP;
			iAttack = 10;
			iVariables[0] = 20; // set animation timer
			iVariables[2] = 3; // set state to begin at "stopped at bottom"
			fY += 32; // start at 32 pixels lower than appears in map editor
			iY += 32;
			bMoveAfterAI = true;
			break;
		}
		case 25: // Lava spring (delay 9)
		{
			iVariables[3] = 180; // set a pause at bottom
			
			strName = "Lava spring";
			bmpFrames[0] = app->blBitmaps->LoadBitmap("monsters/lava_spring/frame1.bmp");
			bmpFrames[1] = app->blBitmaps->LoadBitmap("monsters/lava_spring/frame2.bmp");
			bmpFrames[2] = app->blBitmaps->LoadBitmap("monsters/lava_spring/frame3.bmp");
			bmpFrames[3] = app->blBitmaps->LoadBitmap("monsters/lava_spring/frame4.bmp");
			bmpPortrait = app->blBitmaps->LoadBitmap("monsters/lava_spring/portrait.bmp");
			bmpImage = bmpFrames[0];
			bInvincible = true;
			iExpGiven = 1;
			iHP = 1;
			iHPMax = iHP;
			iAttack = 10;
			iVariables[0] = 20; // set animation timer
			iVariables[2] = 3; // set state to begin at "stopped at bottom"
			fY += 32; // start at 32 pixels lower than appears in map editor
			iY += 32;
			bMoveAfterAI = true;
			break;
		}
		case 26: // Lava spring (delay 10)
		{
			iVariables[3] = 200; // set a pause at bottom
			
			strName = "Lava spring";
			bmpFrames[0] = app->blBitmaps->LoadBitmap("monsters/lava_spring/frame1.bmp");
			bmpFrames[1] = app->blBitmaps->LoadBitmap("monsters/lava_spring/frame2.bmp");
			bmpFrames[2] = app->blBitmaps->LoadBitmap("monsters/lava_spring/frame3.bmp");
			bmpFrames[3] = app->blBitmaps->LoadBitmap("monsters/lava_spring/frame4.bmp");
			bmpPortrait = app->blBitmaps->LoadBitmap("monsters/lava_spring/portrait.bmp");
			bmpImage = bmpFrames[0];
			bInvincible = true;
			iExpGiven = 1;
			iHP = 1;
			iHPMax = iHP;
			iAttack = 10;
			iVariables[0] = 20; // set animation timer
			iVariables[2] = 3; // set state to begin at "stopped at bottom"
			fY += 32; // start at 32 pixels lower than appears in map editor
			iY += 32;
			bMoveAfterAI = true;
			break;
		}

		case 27: // Demon Bat (normal version, starts to the left)
		{
			strName = "Demon Bat";
			bmpFrames[0] = app->blBitmaps->LoadBitmap("monsters/demonbat/frame1.bmp");
			bmpFrames[1] = app->blBitmaps->LoadBitmap("monsters/demonbat/frame2.bmp");
			bmpFrames[2] = app->blBitmaps->LoadBitmap("monsters/demonbat/frame1.bmp");
			bmpFrames[3] = app->blBitmaps->LoadBitmap("monsters/demonbat/frame3.bmp");
			bmpPortrait = app->blBitmaps->LoadBitmap("monsters/demonbat/portrait.bmp");
			bmpImage = bmpFrames[0];
			iExpGiven = 20;
			iHP = 20;
			iHPMax = iHP;
			iAttack = 8;
			iVariables[0] = 10; // set animation timer
			iVariables[2] = 0; // start moving up-left
			bMoveAfterAI = true; // movement will be handled in tick() instead of in customAI()
			break;
		}

		case 28: // Demon Bat (normal version, starts to the right)
		{
			strName = "Demon Bat";
			bmpFrames[0] = app->blBitmaps->LoadBitmap("monsters/demonbat/frame1.bmp");
			bmpFrames[1] = app->blBitmaps->LoadBitmap("monsters/demonbat/frame2.bmp");
			bmpFrames[2] = app->blBitmaps->LoadBitmap("monsters/demonbat/frame1.bmp");
			bmpFrames[3] = app->blBitmaps->LoadBitmap("monsters/demonbat/frame3.bmp");
			bmpPortrait = app->blBitmaps->LoadBitmap("monsters/demonbat/portrait.bmp");
			bmpImage = bmpFrames[0];
			iExpGiven = 20;
			iHP = 20;
			iHPMax = iHP;
			iAttack = 8;
			iVariables[0] = 10; // set animation timer
			iVariables[2] = 3; // start moving up-right
			bMoveAfterAI = true; // movement will be handled in tick() instead of in customAI()
			break;
		}

		case 29: // Demon Bat (fast version, starts to the left)
		{
			strName = "Demon Bat";
			bmpFrames[0] = app->blBitmaps->LoadBitmap("monsters/demonbat/frame1.bmp");
			bmpFrames[1] = app->blBitmaps->LoadBitmap("monsters/demonbat/frame2.bmp");
			bmpFrames[2] = app->blBitmaps->LoadBitmap("monsters/demonbat/frame1.bmp");
			bmpFrames[3] = app->blBitmaps->LoadBitmap("monsters/demonbat/frame3.bmp");
			bmpPortrait = app->blBitmaps->LoadBitmap("monsters/demonbat/portrait.bmp");
			bmpImage = bmpFrames[0];
			iExpGiven = 30;
			iHP = 30;
			iHPMax = iHP;
			iAttack = 10;
			iVariables[0] = 8; // set animation timer
			iVariables[2] = 0; // start moving up-left
			bMoveAfterAI = true; // movement will be handled in tick() instead of in customAI()
			break;
		}

		case 30: // Demon Bat (fast version, starts to the left)
		{
			strName = "Demon Bat";
			bmpFrames[0] = app->blBitmaps->LoadBitmap("monsters/demonbat/frame1.bmp");
			bmpFrames[1] = app->blBitmaps->LoadBitmap("monsters/demonbat/frame2.bmp");
			bmpFrames[2] = app->blBitmaps->LoadBitmap("monsters/demonbat/frame1.bmp");
			bmpFrames[3] = app->blBitmaps->LoadBitmap("monsters/demonbat/frame3.bmp");
			bmpPortrait = app->blBitmaps->LoadBitmap("monsters/demonbat/portrait.bmp");
			bmpImage = bmpFrames[0];
			iExpGiven = 30;
			iHP = 30;
			iHPMax = iHP;
			iAttack = 10;
			iVariables[0] = 8; // set animation timer
			iVariables[2] = 3; // start moving up-right
			bMoveAfterAI = true; // movement will be handled in tick() instead of in customAI()
			break;
		}

		case 31: // Boulder (used in Red River Cave)
		{
			strName = "Boulder";
			bmpFrames[0] = app->blBitmaps->LoadBitmap("monsters/boulder/frame1.bmp");
			bmpFrames[1] = app->blBitmaps->LoadBitmap("monsters/boulder/frame2.bmp");
			bmpFrames[2] = app->blBitmaps->LoadBitmap("monsters/boulder/frame3.bmp");
			bmpPortrait = app->blBitmaps->LoadBitmap("monsters/boulder/portrait.bmp");
			bmpImage = bmpFrames[0];
			iExpGiven = 1;
			iHP = 1;
			iHPMax = iHP;
			iAttack = 15;
			bInvincible = true;
			iVariables[0] = 10; // set animation timer
			bMoveAfterAI = true; // movement will be handled in tick() instead of in customAI()
			break;
		}

		case 32: // Boulder generator misc object (invisible, used in Red River Cave, spawns Boulder enemies)
		{
			strName = "Boulder generator (32)";
			bmpImage = NULL; // invisible
			bInvincible = true;
			iVariables[0] = 0; // countdown timer of number of frames until time to drop the next boulder
			break;
		}

		case 33: // Boulder generator misc object on a delay (invisible, used in Red River Cave, spawns Boulder enemies)
		{
			strName = "Boulder generator (delayed, 33)";

			bmpImage = NULL; // invisible
			bInvincible = true;
			iVariables[0] = 180; // countdown timer of number of frames until time to drop the next boulder
			break;
		}

		case 34: // Teleporter misc object (invisible)
		{
			strName = "Teleporter";
			break;
		}

		case 35: // Spike enemy object
		{
			strName = "Spike";
			bmpFrames[0] = app->blBitmaps->LoadBitmap("monsters/spike/spike.bmp");
			bmpPortrait = app->blBitmaps->LoadBitmap("monsters/spike/portrait.bmp");
			bmpImage = bmpFrames[0];
			iExpGiven = 1;
			iHP = 1;
			iHPMax = iHP;
			iAttack = 10;
			bInvincible = true;
			// make the tile below this solid
			make_tile_solid(iX / 32, (iY / 32) + 1);
			break;
		}

		case 36: // Demon Minion boss enemy
		{
			strName = "Demon Minion";
			bmpFrames[0] = app->blBitmaps->LoadBitmap("monsters/demon_minion/frame1.bmp");
			bmpFrames[1] = app->blBitmaps->LoadBitmap("monsters/demon_minion/frame2.bmp");
			bmpFrames[2] = app->blBitmaps->LoadBitmap("monsters/demon_minion/frame3.bmp");
			bmpFrames[3] = app->blBitmaps->LoadBitmap("monsters/demon_minion/frame4.bmp");
			bmpPortrait = app->blBitmaps->LoadBitmap("monsters/demon_minion/portrait.bmp");
			bmpImage = bmpFrames[0];
			iExpGiven = 1000;
			iHP = 200;
			iHPMax = iHP;
			iAttack = 20;

			// move up 9 pixels
			fY -= 9;
			iY -= 9;
			iStartY -= 9;

			// set beginning variables
			iVariables[0] = 0; // normal state
			iVariables[1] = 360; // wait 3 seconds before sending out the first bats

			bMoveAfterAI = true; // movement will be handled in tick() instead of in customAI()

			wavSounds[0] = app->slSounds->LoadSound("sounds/demon_laugh.wav");
			wavSounds[1] = app->slSounds->LoadSound("sounds/demon_hurt.wav");
			wavSounds[2] = app->slSounds->LoadSound("sounds/demon_death.wav");
			wavSounds[3] = app->slSounds->LoadSound("sounds/demon_attack.wav");
			play_sample(wavSounds[0], 255, 128, 1000, 0);
			
			break;
		}


		case 37: // Demon Minion's Devil Bat projectile (version 1: moves left)
		{
			strName = "Devil Bat";
			bmpFrames[0] = app->blBitmaps->LoadBitmap("monsters/devilbat/frame1.bmp");
			bmpFrames[1] = app->blBitmaps->LoadBitmap("monsters/devilbat/frame2.bmp");
			bmpFrames[2] = app->blBitmaps->LoadBitmap("monsters/devilbat/frame1.bmp");
			bmpFrames[3] = app->blBitmaps->LoadBitmap("monsters/devilbat/frame3.bmp");
			bmpPortrait = app->blBitmaps->LoadBitmap("monsters/devilbat/portrait.bmp");
			bmpImage = bmpFrames[0];
			iExpGiven = 1;
			iHP = 200;
			iHPMax = iHP;
			bInvincible = true;
			iAttack = 10;
			iVariables[0] = 8; // set animation timer
			fVel_x = -3;
			bMoveAfterAI = true; // movement will be handled in tick() instead of in customAI()
			break;
		}


		case 38: // Demon Minion's Devil Bat projectile (version 2: moves up)
		{
			strName = "Devil Bat";
			bmpFrames[0] = app->blBitmaps->LoadBitmap("monsters/devilbat/frame1.bmp");
			bmpFrames[1] = app->blBitmaps->LoadBitmap("monsters/devilbat/frame2.bmp");
			bmpFrames[2] = app->blBitmaps->LoadBitmap("monsters/devilbat/frame1.bmp");
			bmpFrames[3] = app->blBitmaps->LoadBitmap("monsters/devilbat/frame3.bmp");
			bmpPortrait = app->blBitmaps->LoadBitmap("monsters/devilbat/portrait.bmp");
			bmpImage = bmpFrames[0];
			iExpGiven = 1;
			iHP = 200;
			iHPMax = iHP;
			bInvincible = true;
			iAttack = 20;
			iVariables[0] = 8; // set animation timer
			fVel_y = -3;
			bMoveAfterAI = true; // movement will be handled in tick() instead of in customAI()
			break;
		}

		case 39: // Baby Sea Dragon (non-boss version, in level 3-2)
			{
				strName = "Baby Sea Dragon";
				bmpFrames[0] = app->blBitmaps->LoadBitmap("monsters/babyseadragon/babyseadragon.bmp");
				bmpPortrait = app->blBitmaps->LoadBitmap("monsters/babyseadragon/portrait.bmp");
				wavSounds[0] = app->slSounds->LoadSound("monsters/babyseadragon/large_energy_ball.wav");
				wavSounds[1] = app->slSounds->LoadSound("monsters/babyseadragon/stream.wav");
				bmpImage = bmpFrames[0];
				iHP = 150;
				iHPMax = iHP;
				iExpGiven = 250;
				iAttack = 10;
				bMoveAfterAI = false;
				break;
			}

		case 40: // Baby Sea Dragon (non-boss version)'s large bullet
			{
				strName = "Energy Ball of Water";
				bmpFrames[0] = app->blBitmaps->LoadBitmap("monsters/babyseadragon/bullet-large.bmp");
				bmpPortrait = app->blBitmaps->LoadBitmap("monsters/babyseadragon/portrait-energyball-large.bmp");
				bmpImage = bmpFrames[0];
				bInvincible = true;
				iExpGiven = 1;
				iHP = 1;
				iHPMax = iHP;
				iAttack = 15;
				bMoveAfterAI = true;
				break;
			}
		case 41: // Baby Sea Dragon (non-boss version)'s small bullet
			{
				strName = "Small Energy Ball";
				bmpFrames[0] = app->blBitmaps->LoadBitmap("monsters/babyseadragon/bullet-small.bmp");
				bmpPortrait = app->blBitmaps->LoadBitmap("monsters/babyseadragon/portrait-energyball-small.bmp");
				bmpImage = bmpFrames[0];
				bInvincible = true;
				iExpGiven = 1;
				iHP = 1;
				iHPMax = iHP;
				iAttack = 10;
				bMoveAfterAI = true;
				break;
			}
		case 42: // fairy - an object, not an enemy.  flies to the right of the screen and disappears.
			{
				strName = "Fairy";
				bmpFrames[0] = app->blBitmaps->LoadBitmap("monsters/fairy/frame1.bmp");
				bmpFrames[1] = app->blBitmaps->LoadBitmap("monsters/fairy/frame2.bmp");
				bmpFrames[2] = app->blBitmaps->LoadBitmap("monsters/fairy/frame3.bmp");
				bmpPortrait = NULL;
				bmpImage = bmpFrames[0];
				bInvincible = true;
				bMoveAfterAI = false;
				bMiscObject = true;
				break;
			}
		case 43: // fairy spawner object
			{
				strName = "Fairy spawner";
				wavSounds[0] = app->slSounds->LoadSound("sounds/hey_listen.wav");
				iVariables[0] = 0;
				bMiscObject = true;
				break;
			}

		case 44: // Realmkeeper boss's head.  frame1: draw at (31, 0) from Realmkeeper's coordinates
			{
				iY += 8; // move down 8 pixels to position properly
				strName = "Realmkeeper";
				bmpFrames[0] = app->blBitmaps->LoadBitmap("monsters/realmkeeper/head1.bmp");
				bmpPortrait = app->blBitmaps->LoadBitmap("monsters/realmkeeper/portrait.bmp");
				bmpImage = bmpFrames[0];
				iHP = 500;
				iHPMax = iHP;
				iExpGiven = 250;
				iAttack = 10;
				bMoveAfterAI = false;
				iX -= 10;

				enOtherParts[0] = new Enemy(45, iX+3, iY+37, false);
				enOtherParts[1] = new Enemy(46, iX-31, iY+74, false);
				enOtherParts[2] = new Enemy(47, SCREEN_WIDTH-120, -30, false);
				app->current_level->elEnemies->AddEnemy(enOtherParts[0]);
				app->current_level->elEnemies->AddEnemy(enOtherParts[1]);
				app->current_level->elEnemies->AddEnemy(enOtherParts[2]);
				break;
			}

		case 45: // Realmkeeper boss's body.  frame1: draw at (34, 37) from Realmkeeper's coordinates
			{
				strName = "Realmkeeper's body";
				bmpFrames[0] = app->blBitmaps->LoadBitmap("monsters/realmkeeper/body1.bmp");
				bmpFrames[1] = app->blBitmaps->LoadBitmap("monsters/realmkeeper/body2.bmp");
				bmpFrames[2] = app->blBitmaps->LoadBitmap("monsters/realmkeeper/body3.bmp");
				bmpFrames[3] = app->blBitmaps->LoadBitmap("monsters/realmkeeper/body4.bmp");
				bmpFrames[4] = app->blBitmaps->LoadBitmap("monsters/realmkeeper/body5.bmp");
				bmpFrames[5] = app->blBitmaps->LoadBitmap("monsters/realmkeeper/body6.bmp");
				bmpPortrait = app->blBitmaps->LoadBitmap("monsters/realmkeeper/portrait_body.bmp");
				bmpImage = bmpFrames[0];
				iHP = 9999;
				iHPMax = iHP;
				iExpGiven = 0;
				bInvincible = true;
				iAttack = 20;
				bMoveAfterAI = false;
				break;
			}

		case 46: // Realmkeeper boss's blob.  frame1: draw at (0, 74) from Realmkeeper's coordinates
			{
				strName = "Realmkeeper's body";
				bmpFrames[0] = app->blBitmaps->LoadBitmap("monsters/realmkeeper/blob1.bmp");
				/*
				bmpFrames[1] = app->blBitmaps->LoadBitmap("monsters/realmkeeper/blob2.bmp");
				bmpFrames[2] = app->blBitmaps->LoadBitmap("monsters/realmkeeper/blob3.bmp");
				bmpFrames[3] = app->blBitmaps->LoadBitmap("monsters/realmkeeper/blob4.bmp");
				bmpFrames[4] = app->blBitmaps->LoadBitmap("monsters/realmkeeper/blob5.bmp");
				*/
				bmpPortrait = app->blBitmaps->LoadBitmap("monsters/realmkeeper/portrait_body.bmp");
				bmpImage = bmpFrames[0];
				iHP = 9999;
				iHPMax = iHP;
				iExpGiven = 0;
				bInvincible = true;
				iAttack = 20;
				bMoveAfterAI = false;
				break;
			}
		case 47: // the moon during the Realmkeeper boss battle.  created by the Realmkeeper's head at the start of the battle.
			{
				bMiscObject = true;
				bInvincible = true;
				strName = "Moon";
				bmpFrames[0] = app->blBitmaps->LoadBitmap("monsters/realmkeeper/moon1.bmp");
				bmpImage = bmpFrames[0];
				break;
			}
		case 48: // ice crystal (during Realmkeeper boss battle)
			{
				bmpFrames[0] = app->blBitmaps->LoadBitmap("monsters/realmkeeper/ice.bmp");
				bmpImage = bmpFrames[0];
				bmpPortrait = app->blBitmaps->LoadBitmap("monsters/realmkeeper/ice_portrait.bmp");
				strName = "Ice crystal";
				iAttack = 5;
				bMoveAfterAI = true;
				iHP = 9999;
				iHPMax = iHP;
				fVel_y = -3 -(random_number(0,3));
				bInvincible = true;
				break;
			}

		case 211: // Act 1-1 goal/finish
		case 212: // Act 1-2 goal/finish
		case 213: // Act 1-3 goal/finish
		case 221: // Act 2-1 goal/finish
		case 222: // Act 2-2 goal/finish
		case 223: // Act 2-3 goal/finish
		case 231: // Act 3-1 goal/finish
		case 232: // Act 3-2 goal/finish
		case 233: // Act 3-3 goal/finish
		{
			strName = "Goal";
			bmpFrames[0] = app->blBitmaps->LoadBitmap("monsters/finish.bmp");
			bmpImage = bmpFrames[0];
		}
		break;




		// cutscene objects:
		case 301:
		{
			strName = "Scene controller";
			app->iViewX = 0;
			app->iViewY = 1600 - SCREEN_HEIGHT; // bottom of the screen
		}
		break;

		case 302:
		{
			strName = "Scene controller";
			enOtherParts[0] = new Enemy(303, 14*32, 11*32, false);
			app->current_level->elEnemies->AddEnemy(enOtherParts[0]);
			sbSpeechBubbles[0] = new Speechbubble("Wow, I can talk via speechbubbles now!  And this room looks peculiar.. as if it were for a cutscene.  Could this mean that Josh is finally working on a storyline to accompany my adventure!?", enOtherParts[0], 250, 10, true);
		}
		break;

		case 303: // Orbulite object used in ending2.FMP
			{
				// iVariables[0] == animation counter (don't mess with this)
				// iVariables[1] == current frame index (set this when changing direction: 0 through 2 for left, or 3 through 5 for right)

				strName = "Orbulite";
				fY += 12;
				iY = fY;
				switch (app->iPlayerSpecies)
				{
				case 0:
					bmpFrames[0] = app->blBitmaps->LoadBitmap("sprites/orbulite-l1.bmp");
					bmpFrames[1] = app->blBitmaps->LoadBitmap("sprites/orbulite-l2.bmp");
					bmpFrames[2] = app->blBitmaps->LoadBitmap("sprites/orbulite-l3.bmp");
					bmpFrames[3] = app->blBitmaps->LoadBitmap("sprites/orbulite-r1.bmp");
					bmpFrames[4] = app->blBitmaps->LoadBitmap("sprites/orbulite-r2.bmp");
					bmpFrames[5] = app->blBitmaps->LoadBitmap("sprites/orbulite-r3.bmp");
					break;
				case 1:
					bmpFrames[0] = app->blBitmaps->LoadBitmap("sprites/orbudark-l1.bmp");
					bmpFrames[1] = app->blBitmaps->LoadBitmap("sprites/orbudark-l2.bmp");
					bmpFrames[2] = app->blBitmaps->LoadBitmap("sprites/orbudark-l3.bmp");
					bmpFrames[3] = app->blBitmaps->LoadBitmap("sprites/orbudark-r1.bmp");
					bmpFrames[4] = app->blBitmaps->LoadBitmap("sprites/orbudark-r2.bmp");
					bmpFrames[5] = app->blBitmaps->LoadBitmap("sprites/orbudark-r3.bmp");
					break;
				case 2:
					bmpFrames[0] = app->blBitmaps->LoadBitmap("sprites/orbumoon-l1.bmp");
					bmpFrames[1] = app->blBitmaps->LoadBitmap("sprites/orbumoon-l2.bmp");
					bmpFrames[2] = app->blBitmaps->LoadBitmap("sprites/orbumoon-l3.bmp");
					bmpFrames[3] = app->blBitmaps->LoadBitmap("sprites/orbumoon-r1.bmp");
					bmpFrames[4] = app->blBitmaps->LoadBitmap("sprites/orbumoon-r2.bmp");
					bmpFrames[5] = app->blBitmaps->LoadBitmap("sprites/orbumoon-r3.bmp");
					break;
				case 3:
					bmpFrames[0] = app->blBitmaps->LoadBitmap("sprites/orbusun-l1.bmp");
					bmpFrames[1] = app->blBitmaps->LoadBitmap("sprites/orbusun-l2.bmp");
					bmpFrames[2] = app->blBitmaps->LoadBitmap("sprites/orbusun-l3.bmp");
					bmpFrames[3] = app->blBitmaps->LoadBitmap("sprites/orbusun-r1.bmp");
					bmpFrames[4] = app->blBitmaps->LoadBitmap("sprites/orbusun-r2.bmp");
					bmpFrames[5] = app->blBitmaps->LoadBitmap("sprites/orbusun-r3.bmp");
					break;

				}
				bmpImage = bmpFrames[0];
			}

		case 304: // human object used in ending2.FMP
			{
				strName = "Human";
				// todo: load sprites here
			}
	}

}

bool Enemy::collidingWithPlayer()
{

	// create a bounding box for the enemy
	RECT	rBoxEnemy; // bounding box for this enemy
	rBoxEnemy.left = iX;
	rBoxEnemy.top = iY;
	rBoxEnemy.right = iX + bmpImage->w;
	rBoxEnemy.bottom = iY + bmpImage->h;

	// create a bounding box for the player
	RECT	rBoxPlayer;
	rBoxPlayer.left = app->player->iX;
	rBoxPlayer.top = app->player->iY;
	rBoxPlayer.right = app->player->iX + app->bmpOrbuliteSprites[0]->w;
	rBoxPlayer.bottom = app->player->iY + app->bmpOrbuliteSprites[0]->h;

	// if the player and this enemy are colliding..
	if (collision_detected(rBoxEnemy, rBoxPlayer))
		return true;
	else
		return false;
}

bool Enemy::collidingWithPlayersNextFrame()
{

	// create a bounding box for the enemy
	RECT	rBoxEnemy; // bounding box for this enemy
	rBoxEnemy.left = iX;
	rBoxEnemy.top = iY;
	rBoxEnemy.right = iX + bmpImage->w;
	rBoxEnemy.bottom = iY + bmpImage->h;

	// create a bounding box for the player
	RECT	rBoxPlayer;
	rBoxPlayer.left = floor(app->player->fX + app->player->fVel_x);
	rBoxPlayer.top = floor(app->player->iY + app->player->fVel_y);
	rBoxPlayer.right = floor(app->player->iX + app->player->fVel_x + app->bmpOrbuliteSprites[0]->w);
	rBoxPlayer.bottom = floor(app->player->iY + app->player->fVel_y + app->bmpOrbuliteSprites[0]->h);

	// if the player and this enemy are colliding..
	if (collision_detected(rBoxEnemy, rBoxPlayer))
		return true;
	else
		return false;
}

void Enemy::DeathActions()
{
	// called when the enemy is defeated by the player
	if (key_state[KEY_UP])
		play_sample(app->wavJump, 255, 128, 1000, 0);
	switch (iTypeId)
	{
		case 1: // Bootworm
		case 2: // Bootworm
		case 8: // Jaywalker
		case 9: // Janewalker
		case 10: // Bladeguin
		case 11: // Bootworm
			play_sample(wavSounds[1], 255, 128, 1000, 0);
				break;
		case 12: // Boss: Baby Sea Dragon
		{
			// create the level goal
			Enemy * goal = new Enemy(213, 608, 288, true);
			app->current_level->elEnemies->AddEnemy(goal);
			
			// play jump sound
			play_sample(app->wavJump, 255, 128, 1000, 0);
			break;
		}
		case 36: // Boss: Demon Minion
		{
			// create the level goal
			Enemy * goal = new Enemy(223, 384, 352, true);
			app->current_level->elEnemies->AddEnemy(goal);
			
			// play jump sound
			play_sample(app->wavJump, 255, 128, 1000, 0);
			play_sample(wavSounds[2], 255, 128, 1000, 0);
			break;
		}
		default:
		{
			break;
		}
	}
}

void Enemy::DamageActions()
{
	// called when the enemy is attacked by player, but not killed
	switch (iTypeId)
	{
		case 1: // Bootworm
		case 2: // Bootworm
		case 8: // Jaywalker
		case 9: // Janewalker
		case 10: // Bladeguin
		case 11: // Bootworm
			play_sample(wavSounds[0], 255, 128, 1000, 0);
			break;

		case 36: // Boss: Demon Minion
		{
			// play jump sound
			play_sample(app->wavJump, 255, 128, 1000, 0);
			// play hurt sound
			play_sample(wavSounds[1], 255, 128, 1000, 0);
			if (iVariables[0] == 0) // if normal state
			{
				iVariables[0] = 1; // set to "damaged" state (waits a few seconds before switching to state [2], which then sends bats upward)
				iVariables[2] = 3; // set to "hurt" frame of animation
				bmpImage = bmpFrames[3];
				iVariables[1] = 720; // wait 6 seconds before sending bats up
				
			}
			break;
		}
		case 44: // Boss: Realmkeeper (Realmkeeper's head)
		{
			// play jump sound
			play_sample(app->wavJump, 255, 128, 1000, 0);
			enOtherParts[0]->iVariables[2]++; // have the Realmkeeper's body count how many times the Realmkeeper has been hit since his last attack
			break;
		}
		

		default:
		{
			// play jump sound
			play_sample(app->wavJump, 255, 128, 1000, 0);
			break;
		}
	}
}

bool Enemy::tick()
{
	if (app->bLevelStoppedThisFrame)
		return false;

	// execute custom AI/code per monster type
	if (customAI())
		return true;

	if (bMoveAfterAI)
	{
		// adjust coordinates according to velocity
		fX += fVel_x;
		fY += fVel_y;
		// make int copies of coordinates
		iX = floor(fX);
		iY = floor(fY);
	}


	// if this is a regular enemy, check for (and handle) collisions with the player, and deal damage to player or enemy.
	// this is only done if mode is the GAME mode, because enemy is still ticked if mode is MODE_GAMEOVER (game over mode)
	if ((!bMiscObject) && (app->iAppMode == MODE_GAME) && (!app->current_level->bIsCutscene))
	{
		// create a bounding box for the enemy
		RECT	rBoxEnemy; // bounding box for this enemy
		rBoxEnemy.left = iX;
		rBoxEnemy.top = iY;
		rBoxEnemy.right = iX + bmpImage->w;
		rBoxEnemy.bottom = iY + bmpImage->h;

		// create a bounding box for the player
		RECT	rBoxPlayer;
		rBoxPlayer.left = app->player->iX;
		rBoxPlayer.top = app->player->iY;
		rBoxPlayer.right = app->player->iX + app->bmpOrbuliteSprites[0]->w;
		rBoxPlayer.bottom = app->player->iY + app->bmpOrbuliteSprites[0]->h;

		// if the player and this enemy are colliding..
		if (collision_detected(rBoxEnemy, rBoxPlayer))
		{
			// now, check to see whether the player has jumped on the enemy or otherwise

			// the player has jumped on the enemy if 1. player is falling and 2. on the last (previous) frame, the player's "bottom" (Y + height) was above the enemy's "top"
			if ( ((app->player->bJumping) || (app->player->bFalling)) && (app->player->iLastY + app->bmpOrbuliteSprites[0]->h <= rBoxEnemy.top) && (!bInvincible) )
			{
				// player has jumped on the enemy, so reduce this enemy's HP and remove the enemy if necessary
				iHP -= app->iPlayerAttack;


				// if HP is less than 0, enemy is removed from the game, and exp is given to the player
				if (iHP <= 0)
				{
					app->AddExp(iExpGiven); // add exp

					// set info to show enemy box with this monster's details
					app->strEnemyName = strName;
					app->bShowEnemyBox = true;
					app->iEnemyBoxTimer = 120; // show enemy box for two seconds (short time because enemy has been killed)
					app->iEnemyHPCurrent = 0;
					app->iEnemyHPMax = iHPMax;
					app->iEnemyHPPercent = ceil( ((float)iHP / (float)iHPMax) * 100 );
					app->bmpEnemyPortrait = bmpPortrait;
			
					app->iPlayerScore += 5; // add 5 points to score for defeating an enemy

					if (key_state[KEY_UP])
					{
						app->player->bJumping = true;
						app->player->fVel_y = -app->player->fJumpSpeed - 2; // jump/bounce a little higher than normal
					}
					DeathActions(); // call actions that the enemy performs upon getting killed
					return true; // return true to remove this enemy
				}
				else
				{
					// set info to show enemy box with this monster's details
					app->strEnemyName = strName;
					app->bShowEnemyBox = true;
					app->iEnemyBoxTimer = 300; // show enemy box for five seconds
					app->iEnemyHPCurrent = iHP;
					app->iEnemyHPMax = iHPMax;
					app->iEnemyHPPercent = ceil( ((float)iHP / (float)iHPMax) * 100 );
					app->bmpEnemyPortrait = bmpPortrait;

					app->iPlayerScore += 1; // add 1 point to score for jumping on an enemy without defeating it

					// enemy still has HP left, so the player just bounces off of the enemy	(the jump is higher if the Up arrow key is held at time of bounce)
					if (key_state[KEY_UP])
						app->player->fVel_y = -app->player->fJumpSpeed - 2; // jump/bounce a little higher than normal
					else
						app->player->fVel_y = -app->player->fJumpSpeed; // jump a short jump
					app->player->bJumping = true;

					DamageActions(); // execute custom code for when taking damage but not killed
				}
			}
			else if (!app->player->bInvincible)
			{
				// player has been hit.  reduce HP.  if player is still alive, do temporary (3 second) invincibility.  if not, call playerDeath()
				app->iPlayerHPCurrent -= iAttack;
				if (app->iPlayerHPCurrent < 0)
					app->iPlayerHPCurrent = 0;
				
				if (app->iPlayerHPCurrent == 0)
				{
					// player is killed
					app->playerDeath();
				}
				else
				{
					// set info to show enemy box with this monster's details
					app->strEnemyName = strName;
					app->bShowEnemyBox = true;
					app->iEnemyBoxTimer = 300; // show enemy box for five seconds
					app->iEnemyHPCurrent = iHP;
					app->iEnemyHPMax = iHPMax;
					app->iEnemyHPPercent = ceil( ((float)iHP / (float)iHPMax) * 100 );
					app->bmpEnemyPortrait = bmpPortrait;

					// player lost HP but is still alive, so give 3-second invincibility
					app->player->bInvincible = true;
					app->player->iInvincibilityTimer = 180;
				}
			}
		}
	}
	return false;
}

void Enemy::draw(BITMAP * destination)
{
	// draw sprite
	if (bmpImage != NULL)
		draw_sprite(destination, bmpImage, iX - app->iViewX, iY - app->iViewY);
	// draw speech bubble(s)
	for (int i = 0; i < 10; i++)
		if (sbSpeechBubbles[i] != NULL)
			sbSpeechBubbles[i]->draw(destination);
}

void EnemyList::tick()
{
	// ticks all enemies in the linked list by calling their tick() functions
	app->bLevelStoppedThisFrame = false;
	// if no enemies are in the list, return
	if (head == NULL)
		return;

	// traverse the list and tick each monster
	Enemy * current_enemy = head;
	Enemy * next_enemy = NULL;
	while (current_enemy != NULL)
	{
		if (app->bLevelStoppedThisFrame)
			return;
		next_enemy = current_enemy->next;
		if (current_enemy->tick())
			RemoveEnemy(current_enemy->iInstanceId);
		current_enemy = next_enemy;
	}
}

void EnemyList::draw(BITMAP * destination)
{
	// draws all enemies in the linked list by calling their draw() functions

	// if no enemies are in the list, return
	if (head == NULL)
		return;

	// traverse the list and draw each monster
	Enemy * current_enemy = head;
	while (current_enemy != NULL)
	{
		if (current_enemy->bVisible)
			current_enemy->draw(destination);
		if (current_enemy->bDoCustomDrawing)
			current_enemy->customDrawing(destination);
		current_enemy = current_enemy->next;
	}
}

EnemyList::EnemyList()
{
	iNextInstanceId = 0;
	head = NULL;
	tail = NULL;
}

EnemyList::~EnemyList()
{
	// traverse the linked list, deleting all enemies in the list
	Enemy * current_enemy = head;
	Enemy * next_enemy;
	while (current_enemy != NULL)
	{
		next_enemy = current_enemy->next;
//		delete current_enemy;
		current_enemy = next_enemy;
	}
}

void EnemyList::AddEnemy(Enemy * new_enemy)
{
	// adds an already-created enemy to the linked list of enemies

	new_enemy->iInstanceId = iNextInstanceId; // set the instance id
	iNextInstanceId++;

	if (head == NULL) // if list is empty
	{
		head = new_enemy;
		tail = new_enemy;
	}
	else
	{ // list has at least one enemy in it already
		
		// make this the new head of the list (it is at the front of the list instead of end of the list for drawing purposes, particularly with Boulders)
		new_enemy->next = head;
		head = new_enemy;
	}

}

void EnemyList::RemoveEnemy(int instance_id)
{
	// removes an enemy from the linked list (and deletes the enemy)

	// first, find the enemy with the matching instance id by traversing the linked list
	Enemy * current_enemy = head;
	Enemy * previous_enemy = NULL;
	while (current_enemy != NULL)
	{
		// if a match has been found on this current enemy
		if (current_enemy->iInstanceId == instance_id)
		{
			Enemy * next_enemy = current_enemy->next; // make a note of the next enemy so that the previous enemy can point to it
			delete current_enemy; // delete the enemy's data
			
			if (previous_enemy == NULL) // if the previous enemy is NULL (meaning this is the "head" enemy in the list)
			{
				if (head == tail) // if the head is the only enemy in the list
				{
					head = NULL; // set the head to NULL
					tail = NULL; // set the tail to NULL
				}
				else
					head = next_enemy; // the deleted monster's "next" monster is now the new head of the list
			}
			else
			{ // there was a real monster before this one (meaning the deleted monster was not "head")
				previous_enemy->next = next_enemy; // the deleted monster's "next" is now located directly after the deleted monster's "previous",
													// cutting the deleted monster out of the list

				// set the tail to the previous enemy if the deleted enemy was the old tail
				if (next_enemy == NULL)
					tail = previous_enemy;
			}

			break; // break out of this loop because all work is complete in the removal & deletion of the monster
		}
		
		// move to next enemy in the list
		previous_enemy = current_enemy;
		current_enemy = current_enemy->next;
	}

}

bool Enemy::customAI()
{
	switch (iTypeId)
	{
		// Bootworm (short-range)
		case 1:
		{
			// four variables are used here..
			//	var 0 - whether the Bootworm is stopped (0) or jumping (1)
			//  var 1 - a timer to time pauses inbetween jumps
			//  var 2 - direction of next jump (0 to 2 = left, 3 to 5 = right)
			//  var 3 - Y coordinate of stopped/ground/starting position

			bMoveAfterAI = false; // disable normal movement (X/Y coordinate adjustment), because this code will take care of it instead

			if (iVariables[0] == 0) // if Bootworm is stopped
			{
				iVariables[1]++; // increase timer
				if (iVariables[1] > 180) // after 3 seconds
				{
					// start jumping
					iVariables[0] = 1; // make a note that Bootworm is now jumping
					fVel_y = -3; // set jumping velocity upwards
					if (iVariables[2] < 1) // see which direction this jump is supposed to be in
					{
						fVel_x = -2; // jump left
						bmpImage = bmpFrames[0]; // set image to Left frame
					}
					else
					{
						fVel_x = 2; // jump right
						bmpImage = bmpFrames[1]; // set image to Right frame
					}
					// make a note of which direction the next jump should be in (0 = left, 1 = right)
					iVariables[2]++;
					if (iVariables[2] > 1)
						iVariables[2] = 0;
					// make a note of starting/ground Y coordinate (Bootworm will stop jumping upon reaching this Y coordinate)
					iVariables[3] = iY;
				}
			}
			else
			{
				fVel_y += 0.05;
				if (fVel_y > 5)
					fVel_y = 5;

				// adjust coordinates according to velocity
				fX += fVel_x;
				fY += fVel_y;

				// see if Bootworm has reached the ground again.  if so, stop the jump procedure and restart the delay to the next jump.
				if (fY >= iVariables[3])
				{
					fY = (float)iVariables[3];
					fVel_x = 0; // stop moving
					iVariables[1] = 0; // restart jump delay timer
					iVariables[0] = 0; // make a note that the Bootworm has stopped and is not jumping anymore at the moment
				}
			}

			// make int copies of coordinates
			iX = floor(fX);
			iY = floor(fY);
		}
		break;

		// Bootworm (long-range)
		case 2:
		{
			// four variables are used here..
			//	var 0 - whether the Bootworm is stopped (0) or jumping (1)
			//  var 1 - a timer to time pauses inbetween jumps
			//  var 2 - direction of next jump (0 to 2 = left, 3 to 5 = right)
			//  var 3 - Y coordinate of stopped/ground/starting position

			bMoveAfterAI = false; // disable normal movement (X/Y coordinate adjustment), because this code will take care of it instead

			if (iVariables[0] == 0) // if Bootworm is stopped
			{
				iVariables[1]++; // increase timer
				if (iVariables[1] > 180) // after 3 seconds
				{
					// start jumping
					iVariables[0] = 1; // make a note that Bootworm is now jumping
					fVel_y = -3; // set jumping velocity upwards
					if (iVariables[2] < 2) // see which direction this jump is supposed to be in
					{
						fVel_x = -2; // jump left
						bmpImage = bmpFrames[0]; // set image to Left frame
					}
					else
					{
						fVel_x = 2; // jump right
						bmpImage = bmpFrames[1]; // set image to Right frame
					}
					// make a note of which direction the next jump should be in (0 to 1 = left, 2 to 3 = right)
					iVariables[2]++;
					if (iVariables[2] > 3)
						iVariables[2] = 0;
					// make a note of starting/ground Y coordinate (Bootworm will stop jumping upon reaching this Y coordinate)
					iVariables[3] = iY;
				}
			}
			else
			{
				fVel_y += 0.05;
				if (fVel_y > 5)
					fVel_y = 5;

				// adjust coordinates according to velocity
				fX += fVel_x;
				fY += fVel_y;

				// see if Bootworm has reached the ground again.  if so, stop the jump procedure and restart the delay to the next jump.
				if (fY >= iVariables[3])
				{
					fY = (float)iVariables[3];
					fVel_x = 0; // stop moving
					iVariables[1] = 0; // restart jump delay timer
					iVariables[0] = 0; // make a note that the Bootworm has stopped and is not jumping anymore at the moment
				}
			}

			// make int copies of coordinates
			iX = floor(fX);
			iY = floor(fY);
		}
		break;

		
		// Question Mark (?) Block 1 - Darkstone powerup
		case 3:
		{
			if (iVariables[0] == 0) // if block is still active
			{
				iVariables[1]--; // decrement the animation timer
				if (iVariables[1] == 0)
				{
					iVariables[2]++; // increment animation frame number
					if (iVariables[2] > 3) // if animation is past the last frame
						iVariables[2] = 1; // reset animation
					int frame_number = iVariables[2];
					bmpImage = bmpFrames[frame_number];
					iVariables[1] = 20; // reset animation timer
				}
				// see if player is touching the block, but only if mode is GAME mode (MODE_GAME)
				if ((collidingWithPlayersNextFrame()) && (app->iAppMode == MODE_GAME))
				{
					// see if player is below the block (meaning hitting the block from below to destroy it)
					if ( (app->player->iY > iY) && (app->player->bJumping) )
					{
						// update the block
						iVariables[0] = 1; // set block to inactive
						bmpImage = bmpFrames[0]; // set to inactive block frame

						// play a sound here later, probably.. SOUND

						// create Darkstone
						// create the new misc object for Darkstone (misc objects use the Enemy class)
						Enemy * new_object = new Enemy(5, iX, iY, true);
						// add it to the linked list of enemies
						app->current_level->elEnemies->AddEnemy(new_object);

					}
				}
			}
		}
		break;
		
		// Question Mark (?) Block 2 - coin
		case 4:
		{
			if (iVariables[0] == 0) // if block is still active
			{
				iVariables[1]--; // decrement the animation timer
				if (iVariables[1] == 0)
				{
					iVariables[2]++; // increment animation frame number
					if (iVariables[2] > 3) // if animation is past the last frame
						iVariables[2] = 1; // reset animation
					int frame_number = iVariables[2];
					bmpImage = bmpFrames[frame_number];
					iVariables[1] = 20; // reset animation timer
				}
				// see if player is touching the block, but only if mode is GAME mode (MODE_GAME)
				if ((collidingWithPlayersNextFrame()) && (app->iAppMode == MODE_GAME))
				{
					// see if player is below the block (meaning hitting the block from below to destroy it)
					if ( (app->player->iY > iY) && (app->player->bJumping) )
					{
						iVariables[0] = 1; // set block to inactive
						bmpImage = bmpFrames[0]; // set to inactive block frame
						app->AddGold(1); // add 1 gold
						app->iPlayerScore += 1; // add 1 point to score
						// play the coin sound effect
						play_sample(app->wavCoin, 255, 128, 1000, 0);
					}
				}
			}
		}
		break;

		// Darkstone
		case 5:
		{
			// move up until exactly one block above starting point (starting point is the block that created this power-up)
			if (iY > iStartY - 32)
			{
				fY -= 0.4;
				if (fY < iStartY - 32)
				{
					fY = iStartY - 32;
				}
				iY = fY;
			}

			// check to see if player is picking up this power-up, but only if mode is GAME mode (MODE_GAME)
			if ((collidingWithPlayersNextFrame()) && (app->iAppMode == MODE_GAME))
			{
				// set player to be an Orbudark now..
				app->iPlayerSpecies = 1; // 1 = Orbudark
				app->player->fNormalMaxSpeed = ORBUDARK_NORMAL_MAX_SPEED;
				app->player->fAccelRate = ORBUDARK_ACCEL_RATE;
				app->player->fJumpSpeed = ORBUDARK_JUMP_SPEED;
				app->player->fFallRate = ORBUDARK_FALL_RATE;

				play_sample(app->wavNoice, 255, 128, 1000, 0); // play "NOICE!!" sound effect

				// remove this power-up from the game
				app->current_level->elEnemies->RemoveEnemy(iInstanceId);
			}

			break;
		}

		// Moonstone
		case 6:
		{
			// move up until exactly one block above starting point (starting point is the block that created this power-up)
			if (iY > iStartY - 32)
			{
				fY -= 0.4;
				if (fY < iStartY - 32)
				{
					fY = iStartY - 32;
				}
				iY = fY;
			}

			// check to see if player is picking up this power-up, but only if mode is GAME mode (MODE_GAME)
			if ((collidingWithPlayer()) && (app->iAppMode == MODE_GAME))
			{
				// set player to be an Orbudark now..
				app->iPlayerSpecies = 2; // 1 = Orbumoon
				app->player->fNormalMaxSpeed = ORBUMOON_NORMAL_MAX_SPEED;
				app->player->fAccelRate = ORBUMOON_ACCEL_RATE;
				app->player->fJumpSpeed = ORBUMOON_JUMP_SPEED;
				app->player->fFallRate = ORBUMOON_FALL_RATE;

				play_sample(app->wavNoice, 255, 128, 1000, 0); // play "NOICE!!" sound effect

				// remove this power-up from the game
				app->current_level->elEnemies->RemoveEnemy(iInstanceId);
			}

			break;
		}

		
		// Question Mark (?) Block 3 - Moonstone powerup
		case 7:
		{
			if (iVariables[0] == 0) // if block is still active
			{
				iVariables[1]--; // decrement the animation timer
				if (iVariables[1] == 0)
				{
					iVariables[2]++; // increment animation frame number
					if (iVariables[2] > 3) // if animation is past the last frame
						iVariables[2] = 1; // reset animation
					int frame_number = iVariables[2];
					bmpImage = bmpFrames[frame_number];
					iVariables[1] = 20; // reset animation timer
				}
				// see if player is touching the block, but only if mode is GAME mode (MODE_GAME)
				if ((collidingWithPlayersNextFrame()) && (app->iAppMode == MODE_GAME))
				{
					// see if player is below the block (meaning hitting the block from below to destroy it)
					if ( (app->player->iY > iY) && (app->player->bJumping) )
					{
						// update the block
						iVariables[0] = 1; // set block to inactive
						bmpImage = bmpFrames[0]; // set to inactive block frame

						// play a sound here later, probably.. SOUND

						// create Darkstone
						// create the new misc object for Moonstone (misc objects use the Enemy class)
						Enemy * new_object = new Enemy(6, iX, iY, true);
						// add it to the linked list of enemies
						app->current_level->elEnemies->AddEnemy(new_object);

					}
				}
			}
		}
		break;

		case 8: // Jaywalker
		{
			// variables (iVariables[])..
			// 0 = current movement direction (0 = stopped, 1 = left, 2 = right)
			// 1 = countdown timer to next direction/movement change
			// 2 = speed (0 = walking, 1 = running)
			// 3 = animation countdown timer (i.e. ticks until next frame change in animation)
			// 4 = current animation frame number

			// calculate the left and right boundaries
			int iMaxLeft = iStartX - 500;
			int iMaxRight = iStartX + 500;

			// handle movement change timer
			if (iVariables[1] <= 0)
			{
				// change direction and movement speed
				if (iVariables[0] != 0)
				{
					iVariables[0] = 0;
					// reset timer
					iVariables[1] = random_number(1, 4) * 60;
				}
				else
				{
					int new_direction = iVariables[0];
					new_direction = random_number(0, 2);
					if (new_direction > 2)
						new_direction = 0;
					iVariables[0] = new_direction;
					// reset timer
					iVariables[1] = random_number(1, 4) * 120;
				}
				
				// set animation frame according to new direction
				if (iVariables[0] == 0) // if Jaywalker is now stopped..
				{
					// make sure a non-walking frame is shown
					if (iVariables[4] < 2)
						iVariables[4] = 1;
					else
						iVariables[4] = 3;
				}
				else if (iVariables[0] == 1)
					iVariables[4] = 1;
				else if (iVariables[0] == 2)
					iVariables[4] = 3;

				// randomly set speed
				iVariables[2] = random_number(0, 1);
			}
			else
				iVariables[1]--;

			// handle animation timer
			if ( (iVariables[3] <= 0) && (iVariables[0] > 0) )
			{
				// switch animation frames based upon which frame is currently being shown
				if (iVariables[4] == 0)
					iVariables[4] = 1;
				else if (iVariables[4] == 1)
					iVariables[4] = 0;
				else if (iVariables[4] == 2)
					iVariables[4] = 3;
				else if (iVariables[4] == 3)
					iVariables[4] = 2;

				// reset animation timer
				if (iVariables[2] == 0)
					iVariables[3] = 60;
				else
					iVariables[3] = 10;
			}
			else
				iVariables[3]--;

			bmpImage = bmpFrames[ iVariables[4] ];

			// set velocity based on direction and speed variables calculated above
			if (iVariables[0] == 0)
				fVel_x = 0;
			else
			{
				float total_speed = 0.5;
				if (iVariables[2] == 1)
					total_speed = 3.0;

				if (iVariables[0] == 1)
					fVel_x = -total_speed;
				else if (iVariables[0] == 2)
					fVel_x = total_speed;
			}

			fX += fVel_x;

			if (fX <= iMaxLeft)
			{
				fX = iMaxLeft;
				iVariables[0] = 2;
				iVariables[4] = 2;
			}
			if (fX >= iMaxRight)
			{
				fX = iMaxRight;
				iVariables[0] = 1;
				iVariables[4] = 0;
			}

			iX = floor(fX);

		}
		break;

		case 9: // Janewalker
		{
			// similar to Jaywalker, except the movement speed & animation speed are doubled

			// variables (iVariables[])..
			// 0 = current movement direction (0 = stopped, 1 = left, 2 = right)
			// 1 = countdown timer to next direction/movement change
			// 2 = speed (0 = walking, 1 = running)
			// 3 = animation countdown timer (i.e. ticks until next frame change in animation)
			// 4 = current animation frame number

			// calculate the left and right boundaries
			int iMaxLeft = iStartX - 500;
			int iMaxRight = iStartX + 500;

			// handle movement change timer
			if (iVariables[1] <= 0)
			{
				// change direction and movement speed
				if (iVariables[0] != 0)
				{
					iVariables[0] = 0;
					// reset timer
					iVariables[1] = random_number(1, 4) * 60;
				}
				else
				{
					int new_direction = iVariables[0];
					new_direction = random_number(0, 2);
					if (new_direction > 2)
						new_direction = 0;
					iVariables[0] = new_direction;
					// reset timer
					iVariables[1] = random_number(1, 4) * 120;
				}
				
				// set animation frame according to new direction
				if (iVariables[0] == 0) // if Jaywalker is now stopped..
				{
					// make sure a non-walking frame is shown
					if (iVariables[4] < 2)
						iVariables[4] = 1;
					else
						iVariables[4] = 3;
				}
				else if (iVariables[0] == 1)
					iVariables[4] = 1;
				else if (iVariables[0] == 2)
					iVariables[4] = 3;

				// randomly set speed
				iVariables[2] = random_number(0, 1);
			}
			else
				iVariables[1]--;

			// handle animation timer
			if ( (iVariables[3] <= 0) && (iVariables[0] > 0) )
			{
				// switch animation frames based upon which frame is currently being shown
				if (iVariables[4] == 0)
					iVariables[4] = 1;
				else if (iVariables[4] == 1)
					iVariables[4] = 0;
				else if (iVariables[4] == 2)
					iVariables[4] = 3;
				else if (iVariables[4] == 3)
					iVariables[4] = 2;

				// reset animation timer
				if (iVariables[2] == 0)
					iVariables[3] = 30;
				else
					iVariables[3] = 5;
			}
			else
				iVariables[3]--;

			bmpImage = bmpFrames[ iVariables[4] ];

			// set velocity based on direction and speed variables calculated above
			if (iVariables[0] == 0)
				fVel_x = 0;
			else
			{
				float total_speed = 1.0;
				if (iVariables[2] == 1)
					total_speed = 6.0;

				if (iVariables[0] == 1)
					fVel_x = -total_speed;
				else if (iVariables[0] == 2)
					fVel_x = total_speed;
			}

			fX += fVel_x;

			if (fX <= iMaxLeft)
			{
				fX = iMaxLeft;
				iVariables[0] = 2;
				iVariables[4] = 2;
			}
			if (fX >= iMaxRight)
			{
				fX = iMaxRight;
				iVariables[0] = 1;
				iVariables[4] = 0;
			}

			iX = floor(fX);

		}
		break;

		// Bladeguin
		case 10:
		{
			// iVariables used...
			// iVariables[0] = animation frame countdown timer
			// iVariables[1] = current bitmap frame
			// iVariables[2] = direction (0 = left, 1 = right)
			// iVariables[4] = whether Bladeguin is stopped and swinging blades (0 = not stopped, 1 = yes stopped).. 0 means walking
			// iVariables[5] = countdown timer to start walking again

			// STEP 1 - change directions if necessary

			if (iVariables[4] == 0)
			{
				// if walking to the left, and has reached the left boundary
				if ((iVariables[2] == 0) && (iX <= iStartX - 300))
				{
					iVariables[4] = 1;
					iVariables[5] = 480;

					iVariables[1] = 8;
					iVariables[0] = 10; // set animation countdown timer

				}

				// if walking to the right, and has reached the right boundary
				if ((iVariables[2] == 1) && (iX >= iStartX + 300))
				{
					iVariables[4] = 1;
					iVariables[5] = 480;

					iVariables[1] = 12;
					iVariables[0] = 10; // set animation countdown timer
				}
			}

			// if stopped..
			if (iVariables[4] == 1)
			{
				// decrement the countdown timer of time until time to start walking again
				iVariables[5]--;

				// if the countdown says it's time to start walking again...
				if (iVariables[5] <= 0)
				{
					if (iVariables[2] == 0)
					{
						iVariables[2] = 1; // set direction to right
						iVariables[1] = 4; // set animation frame
						iVariables[0] = 10; // set animation countdown timer

						iVariables[4] = 0; // set to not stopped anymore
					}
					else
					{
						iVariables[2] = 0; // set direction to left
						iVariables[1] = 0; // set animation frame
						iVariables[0] = 10; // set animation countdown timer

						iVariables[4] = 0; // set to not stopped anymore
					}
				}
			}


			// STEP 2 - handle animation
			iVariables[0]--;
			if (iVariables[0] < 1)
			{
				iVariables[1]++;
				if (iVariables[4] == 0) // if not stopped
				{
					if (iVariables[2] == 0)
					{
						if (iVariables[1] > 3)
							iVariables[1] = 0;
					}
					else if (iVariables[2] == 1)
					{
						if (iVariables[1] > 7)
							iVariables[1] = 4;
					}
					iVariables[0] = 10;
				}
				else
				{
					// running through stopped animation..

					// if facing left
					if (iVariables[2] == 0)
					{
						if (iVariables[1] > 11)
							iVariables[1] = 8;
					}
					else // if facing right
					{
						if (iVariables[1] > 15)
							iVariables[1] = 12;
					}
					iVariables[0] = 10;
				}
			}



			// set bmp image
			int frame_number = iVariables[1];
			bmpImage = bmpFrames[frame_number];

			// set velocity to match the walking direction
			
			if (iVariables[4] == 0)
			{
				if (iVariables[2] == 0)
					fVel_x = -1.5;
				else
					fVel_x = 1.5;
			}
			else
				fVel_x = 0;


			fX += fVel_x;
			iX = floor(fX);

			
		}
		break;


		// Bootworm (short-range to the right)
		case 11:
		{
			// four variables are used here..
			//	var 0 - whether the Bootworm is stopped (0) or jumping (1)
			//  var 1 - a timer to time pauses inbetween jumps
			//  var 2 - direction of next jump (0 to 2 = left, 3 to 5 = right)
			//  var 3 - Y coordinate of stopped/ground/starting position

			bMoveAfterAI = false; // disable normal movement (X/Y coordinate adjustment), because this code will take care of it instead

			if (iVariables[0] == 0) // if Bootworm is stopped
			{
				iVariables[1]++; // increase timer
				if (iVariables[1] > 180) // after 3 seconds
				{
					// start jumping
					iVariables[0] = 1; // make a note that Bootworm is now jumping
					fVel_y = -3; // set jumping velocity upwards
					if (iVariables[2] < 1) // see which direction this jump is supposed to be in
					{
						fVel_x = -2; // jump left
						bmpImage = bmpFrames[0]; // set image to Left frame
					}
					else
					{
						fVel_x = 2; // jump right
						bmpImage = bmpFrames[1]; // set image to Right frame
					}
					// make a note of which direction the next jump should be in (0 = left, 1 = right)
					iVariables[2]++;
					if (iVariables[2] > 1)
						iVariables[2] = 0;
					// make a note of starting/ground Y coordinate (Bootworm will stop jumping upon reaching this Y coordinate)
					iVariables[3] = iY;
				}
			}
			else
			{
				fVel_y += 0.05;
				if (fVel_y > 5)
					fVel_y = 5;

				// adjust coordinates according to velocity
				fX += fVel_x;
				fY += fVel_y;

				// see if Bootworm has reached the ground again.  if so, stop the jump procedure and restart the delay to the next jump.
				if (fY >= iVariables[3])
				{
					fY = (float)iVariables[3];
					fVel_x = 0; // stop moving
					iVariables[1] = 0; // restart jump delay timer
					iVariables[0] = 0; // make a note that the Bootworm has stopped and is not jumping anymore at the moment
				}
			}

			// make int copies of coordinates
			iX = floor(fX);
			iY = floor(fY);
		}
		break;

		// Boss: Baby Sea Dragon
		case 12:
		{
			// iVariables[0] = bullet counter

			iVariables[0]++;

			if (fVel_y == 0)
				fVel_y = 1;
			else if (fY > iStartY + 250)
				fVel_y = -3;
			else
				fVel_y += 0.01;
			
			if (fVel_y > 3)
				fVel_y = 3;


			fY += fVel_y;;
			iY = fY;

			if (iVariables[0] >= 240)
			{
				if (iHP > 25)
				{
					// create a large energy ball
					play_sample(wavSounds[0], 255, 128, 1000, 0); // play the sound of a large energy ball being released
					app->current_level->elEnemies->AddEnemy(new Enemy(13, iX - 64, iY + 10, false));
					iVariables[0] = 0; // reset timer
				}
				else
				{
					// create a small energy ball
					play_sample(wavSounds[1], 255, 128, 1000, 0); // play the sound of the stream
					if (iVariables[0] % 10 == 0)
						app->current_level->elEnemies->AddEnemy(new Enemy(14, iX - 64, iY + 10, false));
					if (iVariables[0] >= 360)
						iVariables[0] = 0; // reset timer
				}
			}
		}
		break;

		// Boss: Baby Sea Dragon's large bullet
		case 13:
		{
			fVel_x = -3;

				if (iX < iStartX - 800)
					return true;
		}
		break;

		// Boss: Baby Sea Dragon's small bullet
		case 14:
		{
			fVel_x = -3;

			if (iX < iStartX - 800)
				return true;
		}
		break;


		// Lava spring (part of Red River Cave's dangerous scenery)
		case 15:
		{
			// iVariables[0] = animation timer (counts down, 0 being time to switch to next frame)
			// iVariables[1] = current frame number (0 through 3 for the four frames)
			// iVariables[2] = current movement state.  0 = moving up, 1 = stopped at top, 2 = moving down, 3 = stopped at bottom
			// iVariables[3] = timer used, when stopped, to countdown time until lava spring starts moving again

			// handle animation
			iVariables[0]--;
			if (iVariables[0] <= 0)
			{
				iVariables[1]++;
				if (iVariables[1] > 3)
					iVariables[1] = 0;
				bmpImage = bmpFrames[ (iVariables[1]) ];
				iVariables[0] = 20;
			}

			// if moving up and have reached the top
			if ( (iVariables[2] == 0) && (fY <= iStartY - 352) )
			{
				fVel_y = 0; // stop moving
				fY = iStartY - 352; // make sure Y position is perfectly accurate
				iVariables[2] = 1; // set movement state to "stopped at top"
				iVariables[3] = 360; // set a 3-second pause at top
			}
			else if (iVariables[2] == 1) // if stopped at top
			{
				// countdown time until time to start moving again
				iVariables[3]--;
				if (iVariables[3] <= 0)
				{
					// time to start moving again
					iVariables[2] = 2; // set to moving down
					fVel_y = 1.5; // set Y velocity to move down
				}
			}
			else if (iVariables[2] == 2) // if moving down
			{
				if (fY >= iStartY + 32) // if back to starting position
				{
					fY = iStartY + 32; // make sure Y position is perfectly accurate
					fVel_y = 0; // stop moving
					iVariables[2] = 3; // set state to "stopped at bottom"
					iVariables[3] = 360; // set a 3-second pause at bottom
				}
			}
			else if (iVariables[2] == 3) // if stopped at bottom
			{
				// countdown time until time to start moving again
				iVariables[3]--;
				if (iVariables[3] <= 0)
				{
					// time to start moving again
					iVariables[2] = 0; // set to moving up
					fVel_y = -1.5; // set Y velocity to move up
				}
			}


		}
		break;

		// Lava spring (fast version and delayed versions)
		case 16:
		case 17:
		case 18:
		case 19:
		case 20:
		case 21:
		case 22:
		case 23:
		case 24:
		case 25:
		case 26:
		{
			// iVariables[0] = animation timer (counts down, 0 being time to switch to next frame)
			// iVariables[1] = current frame number (0 through 3 for the four frames)
			// iVariables[2] = current movement state.  0 = moving up, 1 = stopped at top, 2 = moving down, 3 = stopped at bottom
			// iVariables[3] = timer used, when stopped, to countdown time until lava spring starts moving again

			// handle animation
			iVariables[0]--;
			if (iVariables[0] <= 0)
			{
				iVariables[1]++;
				if (iVariables[1] > 3)
					iVariables[1] = 0;
				bmpImage = bmpFrames[ (iVariables[1]) ];
				iVariables[0] = 20;
			}

			// if moving up and have reached the top
			if ( (iVariables[2] == 0) && (fY <= iStartY - 352) )
			{
				fVel_y = 0; // stop moving
				fY = iStartY - 352; // make sure Y position is perfectly accurate
				iVariables[2] = 1; // set movement state to "stopped at top"
				if (iTypeId == 16) // for the "fast" version..
					iVariables[3] = 240; // set a 2-second pause at top
				else // for the "delay" versions..
					iVariables[3] = 1; // set basically no delay
			}
			else if (iVariables[2] == 1) // if stopped at top
			{
				// countdown time until time to start moving again
				iVariables[3]--;
				if (iVariables[3] <= 0)
				{
					// time to start moving again
					iVariables[2] = 2; // set to moving down
					fVel_y = 3; // set Y velocity to move down
				}
			}
			else if (iVariables[2] == 2) // if moving down
			{
				if (fY >= iStartY + 32) // if back to starting position
				{
					fY = iStartY + 32; // make sure Y position is perfectly accurate
					fVel_y = 0; // stop moving
					iVariables[2] = 3; // set state to "stopped at bottom"
					if (iTypeId == 16) // for the regular "fast" version..
						iVariables[3] = 240; // set a 2-second pause at bottom
					else // for the "delay" versions..
						iVariables[3] = 1; // set basically no pause
				}
			}
			else if (iVariables[2] == 3) // if stopped at bottom
			{
				// countdown time until time to start moving again
				iVariables[3]--;
				if (iVariables[3] <= 0)
				{
					// time to start moving again
					iVariables[2] = 0; // set to moving up
					fVel_y = -3; // set Y velocity to move up
				}
			}


		}
		break;


		// Demon Bat (normal version to the left and right)
		case 27: // left
		case 28: // right
		{
			// iVariables[0] = animation countdown timer (switch to next frame upon hitting 0)
			// iVariables[1] = current animation frame number (0 through 3 for the four frames)
			// iVariables[2] = current movement state
			//		(0 = moving up and left, 1 = stopped at left top, 2 = moving down and right, 3 = moving up and right, 4 = stopped at top right, 5 = moving down and left)
			// iVariables[3] = countdown timer for time until time to start moving again

			// handle animation
			iVariables[0]--;
			if (iVariables[0] <= 0)
			{
				// set to next animation frame
				iVariables[1]++;
				if (iVariables[1] > 3)
					iVariables[1] = 0;
				bmpImage = bmpFrames[ (iVariables[1]) ];
				// reset animation timer
				iVariables[0] = 10;
			}

			// handle movement
			if (iVariables[2] == 0) // moving up and left
			{
				fVel_x -= 0.03;
				if (fVel_x < -3)
					fVel_x = -3;
				fVel_y -= 0.01;
				if (fVel_y < -1)
					fVel_y = -1;
				if ( (fVel_x == -3) && (fVel_y == -1) )
				{
					// movement has stopped at the top of the movement arc, make a note of it and start the timer of time until time to start moving again
					iVariables[2] = 1;
					iVariables[3] = 120;
					fVel_y = 0;
					fVel_x = 0;
				}
			}
			else if (iVariables[2] == 1) // stopped at top-left corner
			{
				iVariables[3]--;
				if (iVariables[3] <= 0)
				{
					iVariables[2] = 2; // set to down-and-right movement
				}
			}
			else if (iVariables[2] == 2) // moving down and right
			{
				fVel_x += 0.03;
				if (fVel_x > 3)
					fVel_x = 3;
				fVel_y += 0.01;
				if (fVel_y > 1)
					fVel_y = 1;
				if ( (fVel_x == 3) && (fVel_y == 1) )
				{
					// movement has stopped at the bottom of the movement arc, so shift into up-right movement
					iVariables[2] = 3;
					fVel_y = 0;
					fVel_x = 0;
				}
			}
			else if (iVariables[2] == 3) // moving up and right
			{
				fVel_x += 0.03;
				if (fVel_x > 3)
					fVel_x = 3;
				fVel_y -= 0.01;
				if (fVel_y < -1)
					fVel_y = -1;

				if ( (fVel_x == 3) && (fVel_y == -1) )
				{
					// movement has stopped at the top-right of the movement arc, so stop moving
					iVariables[2] = 4;
					iVariables[3] = 120; // set countdown timer of time until time to start moving again
					fVel_y = 0;
					fVel_x = 0;
				}

			}
			else if (iVariables[2] == 4) // stopped at top-right corner
			{
				iVariables[3]--;
				if (iVariables[3] <= 0)
				{
					iVariables[2] = 5; // set to down-and-left movement
				}
			}
			else if (iVariables[2] == 5) // moving down and left
			{
				fVel_x -= 0.03;
				if (fVel_x < -3)
					fVel_x = -3;
				fVel_y += 0.01;
				if (fVel_y > 1)
					fVel_y = 1;
				if ( (fVel_x == -3) && (fVel_y == 1) )
				{
					// movement has stopped at the bottom of the movement arc, so shift into up-left movement
					iVariables[2] = 0;
					fVel_y = 0;
					fVel_x = 0;
				}
			}

		}
		break;

		// Demon Bat (fast version to the left and right)
		case 29: // left
		case 30: // right
		{
			// iVariables[0] = animation countdown timer (switch to next frame upon hitting 0)
			// iVariables[1] = current animation frame number (0 through 3 for the four frames)
			// iVariables[2] = current movement state
			//		(0 = moving up and left, 1 = stopped at left top, 2 = moving down and right, 3 = moving up and right, 4 = stopped at top right, 5 = moving down and left)
			// iVariables[3] = countdown timer for time until time to start moving again

			// handle animation
			iVariables[0]--;
			if (iVariables[0] <= 0)
			{
				// set to next animation frame
				iVariables[1]++;
				if (iVariables[1] > 3)
					iVariables[1] = 0;
				bmpImage = bmpFrames[ (iVariables[1]) ];
				// reset animation timer
				iVariables[0] = 8;
			}

			// handle movement
			if (iVariables[2] == 0) // moving up and left
			{
				fVel_x -= 0.06;
				if (fVel_x < -6)
					fVel_x = -6;
				fVel_y -= 0.02;
				if (fVel_y < -2)
					fVel_y = -2;
				if ( (fVel_x == -6) && (fVel_y == -2) )
				{
					// movement has stopped at the top of the movement arc, make a note of it and start the timer of time until time to start moving again
					iVariables[2] = 1;
					iVariables[3] = 60;
					fVel_y = 0;
					fVel_x = 0;
				}
			}
			else if (iVariables[2] == 1) // stopped at top-left corner
			{
				iVariables[3]--;
				if (iVariables[3] <= 0)
				{
					iVariables[2] = 2; // set to down-and-right movement
				}
			}
			else if (iVariables[2] == 2) // moving down and right
			{
				fVel_x += 0.06;
				if (fVel_x > 6)
					fVel_x = 6;
				fVel_y += 0.02;
				if (fVel_y > 2)
					fVel_y = 2;
				if ( (fVel_x == 6) && (fVel_y == 2) )
				{
					// movement has stopped at the bottom of the movement arc, so shift into up-right movement
					iVariables[2] = 3;
					fVel_y = 0;
					fVel_x = 0;
				}
			}
			else if (iVariables[2] == 3) // moving up and right
			{
				fVel_x += 0.06;
				if (fVel_x > 6)
					fVel_x = 6;
				fVel_y -= 0.02;
				if (fVel_y < -2)
					fVel_y = -2;

				if ( (fVel_x == 6) && (fVel_y == -2) )
				{
					// movement has stopped at the top-right of the movement arc, so stop moving
					iVariables[2] = 4;
					iVariables[3] = 60; // set countdown timer of time until time to start moving again
					fVel_y = 0;
					fVel_x = 0;
				}

			}
			else if (iVariables[2] == 4) // stopped at top-right corner
			{
				iVariables[3]--;
				if (iVariables[3] <= 0)
				{
					iVariables[2] = 5; // set to down-and-left movement
				}
			}
			else if (iVariables[2] == 5) // moving down and left
			{
				fVel_x -= 0.06;
				if (fVel_x < -6)
					fVel_x = -6;
				fVel_y += 0.02;
				if (fVel_y > 2)
					fVel_y = 2;
				if ( (fVel_x == -6) && (fVel_y == 2) )
				{
					// movement has stopped at the bottom of the movement arc, so shift into up-left movement
					iVariables[2] = 0;
					fVel_y = 0;
					fVel_x = 0;
				}
			}

		}
		break;


		// Boulder
		case 31:
		{
			// iVariables[0] = animation countdown timer.  starts at 10.  upon reaching 0, animation switches to next frame, and timer resets to 10.
			// iVariables[1] = current animation frame index
			// iVariables[2] = current movement mode (0 = paused before moving at all, 1 = rolling left, 2 = rolling down)
			// iVariables[3] = timer of how long (in frames) the boulder has been paused before moving

			
			// if the object has reached the bottom of the map, return true (which deletes the object)
			if (fY > mapheight * 32)
				return true;

			// handle animation, only in movement modes 1 and 2 (because movement mode 0 is "not moving")
			if (iVariables[2] > 0)
			{
				if (iVariables[0] <= 0)
				{
					// set to next frame of animation
					iVariables[1]++;
					if (iVariables[1] > 2)
						iVariables[1] = 0;
					bmpImage = bmpFrames [ (iVariables[1]) ];

					// set countdown to next animation frame
					if (iVariables[2] == 1)
						iVariables[0] = 30; // 30 frame delay in animation (slow animation) in the slow left-moving mode
					else
						iVariables[0] = 10; // 10 frame delay in animation (fast animation) in the fast down-moving mode
				}
				else
					iVariables[0]--;
			}


			// handle movement
			if (iVariables[2] == 0)
			{
				iVariables[3]++;
				if (iVariables[3] >= 360) // 3-second pause before moving at all
					iVariables[2] = 1;
			}
			else if (iVariables[2] == 1)
			{
				fVel_x -= 0.005;
				if (fVel_x < -0.8)
					fVel_x = -0.8;
				if (fX < iStartX - 32)
					iVariables[2] = 2; // set movement mode to start moving down
			}
			else if (iVariables[2] == 2)
			{
				// start moving down
				fVel_y += 0.03;
				if (fVel_y > 5)
					fVel_y = 5;

				// gradually stop moving horizontally
				if (fVel_x < 0)
				{
					fVel_x += 0.005;
					if (fVel_x > 0)
						fVel_x = 0;
				}
			}

		}
		break;

		// Boulder generators (invisible misc object that spawns Boulders in Red River Cave)
		case 32:
		case 33: // same as #32, but on a delay
		{
			// see if it's time to create the next boulder yet
			if (iVariables[0] == 0)
			{
				// create a boulder
				Enemy * new_boulder = new Enemy(31, iX, iY, false);
				app->current_level->elEnemies->AddEnemy(new_boulder);

				// reset timer
				iVariables[0] = 360; // 3-second delay until next boulder generation (3 seconds is the same time boulders pause before falling)
			}
			else
				iVariables[0]--; // not time to create the next boulder yet, so just decrement the countdown timer
		}
		break;


		// Teleporter (teleports player to top of screen upon collision
		case 34:
		{
			if ( (app->player->iX >= iX) && (app->player->iX + app->bmpOrbuliteSprites[0]->w < iX + 160) )
			{
				if (app->player->iY > iY + 32)
					app->player->fY = -app->bmpOrbuliteSprites[0]->h;
			}
		}
		break;

		// Spike (doesn't do anything special)
		case 35:
		{
		}
		break;

		// Demon Minion
		case 36:
		{
			// iVariables[0] = current state (0 = normal, periodically sending Demon Bats to the left)
			// iVariables[1] = Demon Bat countdown timer (used in normal state)

			// iVariables[2] = current frame of animation
			// iVariables[3] = countdown timer to next frame

			// normal state (send Demon Bats periodically to the left)
			if (iVariables[0] == 0)
			{
				iVariables[1]--;
				if (iVariables[1] <= 0)
				{
					// send the third Demon Bat out to the left
					Enemy * new_bat = new Enemy(37, iX - 12, iY + 18, false);
					app->current_level->elEnemies->AddEnemy(new_bat);

					// reset timer
					iVariables[1] = 240;

				}
				else if (iVariables[1] == 20)
				{
					// send the second Demon Bat out to the left
					Enemy * new_bat = new Enemy(37, iX - 12, iY + 18, false);
					app->current_level->elEnemies->AddEnemy(new_bat);

					// set animation frame to third/final arm frame
					iVariables[2] = 2;
					bmpImage = bmpFrames[2];
				}
				else if (iVariables[1] == 40)
				{
					// send the first Demon Bat out to the left
					Enemy * new_bat = new Enemy(37, iX - 12, iY + 18, false);
					app->current_level->elEnemies->AddEnemy(new_bat);

					// set animation frame to second/middle arm frame
					iVariables[2] = 1;
					bmpImage = bmpFrames[1];
					play_sample(wavSounds[3], 255, 128, 1000, 0); // play the attack sound
				}
				else if (iVariables[1] == 100)
				{
					// set animation frame to the arm being down/normal
					iVariables[2] = 0;
					bmpImage = bmpFrames[0];
				}
			}
			// "hurt" state (waits a few seconds before switching to state [2], which then sends bats upward)
			else if (iVariables[0] == 1)
			{
				iVariables[1]--;
				if (iVariables[1] <= 0)
				{
					iVariables[0] = 2; // set to "send bats upward" state
					iVariables[2] = 0; // set to first frame of animation
					bmpImage = bmpFrames[0]; // set to first frame of animation
					iVariables[1] = 100;
				}
			}
			// "send bats upward" state (sends bats upward once before setting state to normal [0] state)
			else if (iVariables[0] == 2)
			{
				iVariables[1]--;
				if (iVariables[1] <= 0)
				{
					// reset timer
					iVariables[1] = 240;

					// set back to normal state
					iVariables[0] = 0;

				}
				else if (iVariables[1] == 20)
				{

					// set animation frame to third/final arm frame
					iVariables[2] = 2;
					bmpImage = bmpFrames[2];

				}
				else if (iVariables[1] == 40)
				{
					// send the first Demon Bat (up)
					Enemy * new_bat = new Enemy(38, iX - 72, iY + 64, false);
					new_bat->fVel_x = -0.5;
					app->current_level->elEnemies->AddEnemy(new_bat);

					// send the second Demon Bat (up)
					new_bat = new Enemy(38, iX - 52, iY + 64, false);
					new_bat->fVel_x = 0;
					app->current_level->elEnemies->AddEnemy(new_bat);

					// send the third Demon Bat (up)
					new_bat = new Enemy(38, iX - 32, iY + 64, false);
					new_bat->fVel_x = 0.5;
					app->current_level->elEnemies->AddEnemy(new_bat);

					// set animation frame to second/middle arm frame
					iVariables[2] = 1;
					bmpImage = bmpFrames[1];

					play_sample(wavSounds[3], 255, 128, 1000, 0); // play the attack sound
				}
				else if (iVariables[1] == 100)
				{
					// set animation frame to the arm being down/normal
					iVariables[2] = 0;
					bmpImage = bmpFrames[0];
				}
			}


		}
		break;


		// Demon Minion's Devil Bat projectile (version 1: flies to the left)
		case 37:
		{
			// iVariables[0] = animation countdown timer (switch to next frame upon hitting 0)
			// iVariables[1] = current animation frame number (0 through 3 for the four frames)
			// iVariables[2] = time (in frames) that this monster has been in the game (used in modifying Y velocity)


			// check to see if the monster/projectile has left the screen yet
			if (iX < 0 - bmpImage->w)
				return true; // remove this enemy

			// handle animation
			iVariables[0]--;
			if (iVariables[0] <= 0)
			{
				// set to next animation frame
				iVariables[1]++;
				if (iVariables[1] > 3)
					iVariables[1] = 0;
				bmpImage = bmpFrames[ (iVariables[1]) ];
				// reset animation timer
				iVariables[0] = 10;
			}

			// handle Y velocity
			iVariables[2]++;

			if (iVariables[2] < 30)
				fVel_y = 0;
			else if (iVariables[2] < 60)
				fVel_y -= 0.08;
			else if (iVariables[2] < 90)
				fVel_y = 0;
			else if (iVariables[2] < 120)
				fVel_y += 0.08;
			else if (iVariables[2] < 150)
				fVel_y = 0;
			else if (iVariables[2] < 180)
				fVel_y -= 0.08;
			else if (iVariables[2] < 210)
				fVel_y = 0;
			else if (iVariables[2] < 240)
				fVel_y += 0.08;
			else if (iVariables[2] < 270)
				fVel_y = 0;


		}
		break;



		// Demon Minion's Devil Bat projectile (version 2: flies up)
		case 38:
		{
			// iVariables[0] = animation countdown timer (switch to next frame upon hitting 0)
			// iVariables[1] = current animation frame number (0 through 3 for the four frames)
			// iVariables[2] = time (in frames) that this monster has been in the game (used in modifying Y velocity)


			// check to see if the monster/projectile has left the screen yet
			if (iY < 0 - bmpImage->h)
				return true; // remove this enemy

			// handle animation
			iVariables[0]--;
			if (iVariables[0] <= 0)
			{
				// set to next animation frame
				iVariables[1]++;
				if (iVariables[1] > 3)
					iVariables[1] = 0;
				bmpImage = bmpFrames[ (iVariables[1]) ];
				// reset animation timer
				iVariables[0] = 10;
			}

		}
		break;


		// Baby Sea Dragon (non-boss version, in level 3-2)
		case 39:
			{
				// iVariables[0] = bullet counter
				iVariables[0]++;

				if (fVel_y == 0)
					fVel_y = 1;
				else if (fY > iStartY + 250)
					fVel_y = -3;
				else
					fVel_y += 0.01;

				if (fVel_y > 3)
					fVel_y = 3;


				fY += fVel_y;
				iY = fY;

				if (iVariables[0] >= 240)
				{
					if (iHP > 75)
					{
						// create a large energy ball
						if (abs(fX - app->player->fX) < 600) // if player is within 600 pixels of this enemy, play the sound effect
							play_sample(wavSounds[0], 255, 128, 1000, 0); // play the sound of a large energy ball being released
						app->current_level->elEnemies->AddEnemy(new Enemy(40, iX - 64, iY + 10, false));
						iVariables[0] = 0; // reset timer
					}
					else
					{
						// create a small energy ball
						if (abs(fX - app->player->fX) < 600) // if player is within 600 pixels of this enemy, play the sound effect
							play_sample(wavSounds[1], 255, 128, 1000, 0); // play the sound of the stream
						if (iVariables[0] % 10 == 0)
							app->current_level->elEnemies->AddEnemy(new Enemy(41, iX - 64, iY + 10, false));
						if (iVariables[0] >= 360)
							iVariables[0] = 0; // reset timer
					}
				}
			}
			break;

			// non-boss version of Baby Sea Dragon's large bullet (appears in level 3-2)
		case 40:
			{
				fVel_x = -3;

				if (iX < iStartX - 300)
					return true;
			}
			break;

			// non-boss version of Baby Sea Dragon's small bullet (appears in level 3-2)
		case 41:
			{
				fVel_x = -3;

				if (iX < iStartX - 300)
					return true;
			}
			break;

		case 42: // fairy object (not an enemy, just flies to the right of the screen and disappears)
			{
				// handle animation
				iVariables[0]--;
				if (iVariables[0] <= 0)
				{
					// set to next animation frame
					iVariables[1]++;
					if (iVariables[1] > 2)
						iVariables[1] = 0;
					bmpImage = bmpFrames[ (iVariables[1]) ];
					// reset animation timer
					iVariables[0] = 10;
				}

				// move
				fX += 3;
				iX = fX;
				if (iX > iStartX + 800)
					return true;
			}
			break;


		case 43: // fairy object spawner
			{
				if (iVariables[0] == 0)
				{
					if ( (app->player->iX >= iX - 100) && (app->player->iX <= iX + 100) && (app->player->iY >= iY - 100) && (app->player->iY <= iY + 100) )
					{
						iVariables[0] = 1;
						// create a bunch of fairies
						app->current_level->elEnemies->AddEnemy(new Enemy(42, iX + 50, iY - 10, true));
						app->current_level->elEnemies->AddEnemy(new Enemy(42, iX + 80, iY - 30, true));
						app->current_level->elEnemies->AddEnemy(new Enemy(42, iX + 20, iY + 10, true));
						app->current_level->elEnemies->AddEnemy(new Enemy(42, iX + 100, iY + 20, true));
						app->current_level->elEnemies->AddEnemy(new Enemy(42, iX + 40, iY + 20, true));
						// play "hey! listen!" sound effect
						play_sample(wavSounds[0], 255, 128, 1000, 0);
					}
				}
			}
			break;

		case 45: // Realmkeeper boss's body.  frame1: draw at (34, 37) from Realmkeeper psd's coordinates
			{
				// iVariables[] variables:
				// [0] == animation counter
				// [1] == current state (0 == none and isn't really used, 1 == animating to bring arms out [aka attacking], 2 == animating to bring arms in)
				// [2] == a count of how many times he has BEEN attacked since the last time he last attacked

				// frame 1: 34, 37
				// frame 2: 35, 37
				// frame 3: 27, 37
				// frame 4: 12, 37
				// frame 5: 3, 23
				// frame 6: 1, 5


				if (iVariables[1] == 0)
				{
					iVariables[0]++;
					if (iVariables[0] == 240)
					{
						iVariables[0] = 0;
						iVariables[1] = 1;
					}
				}


				else if (iVariables[1] == 1) // if animating to bring arms out
				{
					iVariables[0]++;
					if (iVariables[0] == 100)
					{
						bmpImage = bmpFrames[1];
						iX++;
					}
					else if (iVariables[0] == 110)
					{
						bmpImage = bmpFrames[2];
						iX-=8;
					}
					else if (iVariables[0] == 120)
					{

						bmpImage = bmpFrames[3];
						iX-=15;

						// create ice crystals
						for (int i = 0; i < random_number(2 + (iVariables[2]/2), 4 + iVariables[2]); i++)
							app->current_level->elEnemies->AddEnemy(new Enemy(48, (random_number(0,7)*50), SCREEN_HEIGHT));

						for (int i = 0; i < random_number(2 + (iVariables[2]/2), 4 + iVariables[2]); i++)
							app->current_level->elEnemies->AddEnemy(new Enemy(48, (random_number(8,14)*50), SCREEN_HEIGHT));
						iVariables[2] = 0;
					}
					else if (iVariables[0] == 130)
					{

						bmpImage = bmpFrames[4];
						iX-=9;
						iY-=14;
					}
					else if (iVariables[0] == 140)
					{

						bmpImage = bmpFrames[5];
						iX-=2;
						iY-=18;

					}
					else if (iVariables[0] == 360)
					{
						iVariables[1] = 2; // de-animation
						iVariables[0] = 0;
					}
				}
				else if (iVariables[1] == 2) // if animating to bring arms back in
				{
					iVariables[0]++;
					if (iVariables[0] == 10)
					{
						bmpImage = bmpFrames[4];
						iX+=2;
						iY+=18;
					}
					else if (iVariables[0] == 20)
					{
						bmpImage = bmpFrames[3];
						iX+=9;
						iY+=14;
					}
					else if (iVariables[0] == 30)
					{
						bmpImage = bmpFrames[2];
						iX+=15;
					}
					else if (iVariables[0] == 40)
					{
						bmpImage = bmpFrames[1];
						iX+=8;
					}
					else if (iVariables[0] == 50)
					{
						bmpImage = bmpFrames[0];
						iX--;
					}
					else if (iVariables[0] == 300)
					{
						iVariables[0] = 0;
						iVariables[1] = 1;
					}
				}
			}
		break;

		// ice crystal that Realmkeeper uses
		case 48:
			{
				if (iY < -bmpImage->h)
					return true;
			}
		break;

		// Sign #1
		case 101:
		{
			if (app->iAppMode == MODE_GAME)
			{
				if (key_state[KEY_ENTER] == 1) // if Enter key is tapped
				{
					// if player is within the sign's range..
					if ( (app->player->iX >= iX - 100) && (app->player->iX <= iX + 100) && (app->player->iY >= iY - 100) && (app->player->iY <= iY + 100) )
					{
						app->ShowSign("===================", "Battling & Leveling Up", "===================", "You can defeat enemy monsters by jumping on them.", "Most monsters require several jumps to be defeated.", "Once a foe has been eliminated, you will gain experience points.", "Gain enough and you'll level up!  Once you level up, you can ", "deal more damage to your enemies, defeating them sooner.", "You'll also increase your maximum HP when you level up,", "allowing you to take more hits before losing a life."); // switch to sign-viewing mode
					}
				}
			}
		}
		break;

		// Sign #2
		case 102:
		{
			if (app->iAppMode == MODE_GAME)
			{
				if (key_state[KEY_ENTER] == 1) // if Enter key is tapped
				{
					// if player is within the sign's range..
					if ( (app->player->iX >= iX - 100) && (app->player->iX <= iX + 100) && (app->player->iY >= iY - 100) && (app->player->iY <= iY + 100) )
					{
						app->ShowSign("===================", "Don't fall into the water!", "===================", "", "", "You're still a baby Orbulite.", "You can't swim!", "", "", ""); // switch to sign-viewing mode
					}
				}
			}
		}
		break;

		// Sign #3
		case 103:
		{
			if (app->iAppMode == MODE_GAME)
			{
				if (key_state[KEY_ENTER] == 1) // if Enter key is tapped
				{
					// if player is within the sign's range..
					if ( (app->player->iX >= iX - 100) && (app->player->iX <= iX + 100) && (app->player->iY >= iY - 100) && (app->player->iY <= iY + 100) )
					{
						app->ShowSign("===================", "Dashing", "===================", "", "You will need to Dash to get across this gap.", "Dash by holding the Shift button as you move.", "", "", "", ""); // switch to sign-viewing mode
					}
				}
			}
		}
		break;

		// Sign #4
		case 104:
		{
			if (app->iAppMode == MODE_GAME)
			{
				if (key_state[KEY_ENTER] == 1) // if Enter key is tapped
				{
					// if player is within the sign's range..
					if ( (app->player->iX >= iX - 100) && (app->player->iX <= iX + 100) && (app->player->iY >= iY - 100) && (app->player->iY <= iY + 100) )
					{
						app->ShowSign("===================", "Believe it or not...", "===================", "", "Believe it or not, you can actually walk on these flashing clouds!", "The electricity in the flashing clouds will allow you to hover", "over them.  Go ahead, try it out!", "", "Just be careful that you don't miss a step!", ""); // switch to sign-viewing mode
					}
				}
			}
		}
		break;

		// Sign #5
		case 105:
		{
			if (app->iAppMode == MODE_GAME)
			{
				if (key_state[KEY_ENTER] == 1) // if Enter key is tapped
				{
					// if player is within the sign's range..
					if ( (app->player->iX >= iX - 100) && (app->player->iX <= iX + 100) && (app->player->iY >= iY - 100) && (app->player->iY <= iY + 100) )
					{
						app->ShowSign("===================", "Power-ups", "===================", "Each level has at least one power-up hidden somewhere.", "Here are the two types of power-ups in the game:", "Dark Stone - turns you into an Orbudark until you die.", "Moon Stone - turns you into the legendary Orbumoon until you die.", "Orbudark jumps high and runs fast.", "Orbumoon runs even faster (resulting in long jumps, too).", "Orbulites who reach Level 8 are also in for a surprise."); // switch to sign-viewing mode
					}
				}
			}
		}
		break;

		// Sign #6
		case 106:
		{
			if (app->iAppMode == MODE_GAME)
			{
				if (key_state[KEY_ENTER] == 1) // if Enter key is tapped
				{
					// if player is within the sign's range..
					if ( (app->player->iX >= iX - 100) && (app->player->iX <= iX + 100) && (app->player->iY >= iY - 100) && (app->player->iY <= iY + 100) )
					{
						app->ShowSign("===================", "Goal!", "===================", "", "Touch the flag ahead to complete the level!", "", "", "", "", ""); // switch to sign-viewing mode
					}
				}
			}
		}
		break;

		// Sign #7
		case 107:
		{
			if (app->iAppMode == MODE_GAME)
			{
				if (key_state[KEY_ENTER] == 1) // if Enter key is tapped
				{
					// if player is within the sign's range..
					if ( (app->player->iX >= iX - 100) && (app->player->iX <= iX + 100) && (app->player->iY >= iY - 100) && (app->player->iY <= iY + 100) )
					{
						app->ShowSign("", "", "", "Intruders-", "", "STAY OUT!!", "", "", "", "             -Demon Minion"); // switch to sign-viewing mode
					}
				}
			}
		}
		break;

		// Sign #8
		case 108:
		{
			if (app->iAppMode == MODE_GAME)
			{
				if (key_state[KEY_ENTER] == 1) // if Enter key is tapped
				{
					// if player is within the sign's range..
					if ( (app->player->iX >= iX - 100) && (app->player->iX <= iX + 100) && (app->player->iY >= iY - 100) && (app->player->iY <= iY + 100) )
					{
						app->ShowSign("===================", "Using enemies to your advantage", "===================", "", "By holding the jump button (up arrow key) as you land on", "an enemy, you can bounce off of the enemy for extra jump time.", "", "Try using this technique to make your way across this gap.", "", ""); // switch to sign-viewing mode
					}
				}
			}
		}
		break;

		// Sign #9
		case 109:
		{
			if (app->iAppMode == MODE_GAME)
			{
				if (key_state[KEY_ENTER] == 1) // if Enter key is tapped
				{
					// if player is within the sign's range..
					if ( (app->player->iX >= iX - 100) && (app->player->iX <= iX + 100) && (app->player->iY >= iY - 100) && (app->player->iY <= iY + 100) )
					{
						app->ShowSign("", "", "===================", "The Lava Dash", "===================", "", "This isn't going to be easy!", "Don't forget to Dash (hold the Shift key as you move).", "", ""); // switch to sign-viewing mode
					}
				}
			}
		}
		break;

		// Sign #10
		case 110:
		{
			if (app->iAppMode == MODE_GAME)
			{
				if (key_state[KEY_ENTER] == 1) // if Enter key is tapped
				{
					// if player is within the sign's range..
					if ( (app->player->iX >= iX - 100) && (app->player->iX <= iX + 100) && (app->player->iY >= iY - 100) && (app->player->iY <= iY + 100) )
					{
						app->ShowSign("", "", "===================", "Avoiding the spikes", "===================", "", "By holding the Dash button (Shift), you will be able to move faster", "to the left and right.", "You will need to do this to avoid the spikes.", "You may even want to hold the Dash button in during the entire level!"); // switch to sign-viewing mode
					}
				}
			}
		}
		break;

		// Act 1-1 Goal/finish
		case 211:
		{
			if (app->iAppMode == MODE_GAME)
			{
				// create a bounding box for the enemy
				RECT	rBoxEnemy; // bounding box for this enemy
				rBoxEnemy.left = iX;
				rBoxEnemy.top = iY;
				rBoxEnemy.right = iX + bmpImage->w;
				rBoxEnemy.bottom = iY + bmpImage->h;

				// create a bounding box for the player
				RECT	rBoxPlayer;
				rBoxPlayer.left = app->player->iX;
				rBoxPlayer.top = app->player->iY;
				rBoxPlayer.right = app->player->iX + app->bmpOrbuliteSprites[0]->w;
				rBoxPlayer.bottom = app->player->iY + app->bmpOrbuliteSprites[0]->h;

				// if the player and this goal/finish are colliding.. head to next level!
				if (collision_detected(rBoxEnemy, rBoxPlayer))
				{				
					// let the game know that the previous level has stopped in this frame (used mainly in EnemyList::tick())
					app->bLevelStoppedThisFrame = true;

					// delete the player and create a new one
					delete app->player;
					app->player = new Player();
					// start off with full HP
					app->iPlayerHPCurrent = app->iPlayerHPMax;

					// delete current level, and start the next level
					app->current_level->stopMusic();
					delete app->current_level;
					app->current_level = new Level("maps/level1-2.FMP", "backgrounds/sky.bmp", "music/birth_island.wav", "zones/birthisland2.bmp", 120);
					app->current_level->load();
					app->current_level->start();
				}
			}
		}
		break;

		// Act 1-2 Goal/finish
		case 212:
		{
			if (app->iAppMode == MODE_GAME)
			{
				// create a bounding box for the enemy
				RECT	rBoxEnemy; // bounding box for this enemy
				rBoxEnemy.left = iX;
				rBoxEnemy.top = iY;
				rBoxEnemy.right = iX + bmpImage->w;
				rBoxEnemy.bottom = iY + bmpImage->h;

				// create a bounding box for the player
				RECT	rBoxPlayer;
				rBoxPlayer.left = app->player->iX;
				rBoxPlayer.top = app->player->iY;
				rBoxPlayer.right = app->player->iX + app->bmpOrbuliteSprites[0]->w;
				rBoxPlayer.bottom = app->player->iY + app->bmpOrbuliteSprites[0]->h;

				// if the player and this goal/finish are colliding.. head to next level!
				if (collision_detected(rBoxEnemy, rBoxPlayer))
				{
					// let the game know that the previous level has stopped in this frame (used mainly in EnemyList::tick())
					app->bLevelStoppedThisFrame = true;

					// delete the player and create a new one
					delete app->player;
					app->player = new Player();
					// start off with full HP
					app->iPlayerHPCurrent = app->iPlayerHPMax;

					// delete current level, and start the next level
					app->current_level->stopMusic();
					delete app->current_level;
					app->current_level = new Level("maps/level1-3.FMP", "backgrounds/sky.bmp", "music/birth_island_boss.wav", "zones/birthislandboss.bmp", 120);
					app->current_level->load();
					app->current_level->start();
				}
			}
		}
		break;

		// Act 1-3 Goal/finish
		case 213:
		{
			if (app->iAppMode == MODE_GAME)
			{
				// create a bounding box for the enemy
				RECT	rBoxEnemy; // bounding box for this enemy
				rBoxEnemy.left = iX;
				rBoxEnemy.top = iY;
				rBoxEnemy.right = iX + bmpImage->w;
				rBoxEnemy.bottom = iY + bmpImage->h;

				// create a bounding box for the player
				RECT	rBoxPlayer;
				rBoxPlayer.left = app->player->iX;
				rBoxPlayer.top = app->player->iY;
				rBoxPlayer.right = app->player->iX + app->bmpOrbuliteSprites[0]->w;
				rBoxPlayer.bottom = app->player->iY + app->bmpOrbuliteSprites[0]->h;

				// if the player and this goal/finish are colliding.. head to next level!
				if (collision_detected(rBoxEnemy, rBoxPlayer))
				{
					// let the game know that the previous level has stopped in this frame (used mainly in EnemyList::tick())
					app->bLevelStoppedThisFrame = true;

					// delete the player and create a new one
					delete app->player;
					app->player = new Player();
					// start off with full HP
					app->iPlayerHPCurrent = app->iPlayerHPMax;

					// delete current level, and start the next level
					app->current_level->stopMusic();
					delete app->current_level;
					app->current_level = new Level("maps/level2-1.FMP", "backgrounds/cave.bmp", "music/red_river_cave.wav", "zones/redrivercave1.bmp", 180);
					app->current_level->load();
					app->current_level->start();
				}
			}
		}
		break;


		// Act 2-1 Goal/finish
		case 221:
		{
			if (app->iAppMode == MODE_GAME)
			{
				// create a bounding box for the enemy
				RECT	rBoxEnemy; // bounding box for this enemy
				rBoxEnemy.left = iX;
				rBoxEnemy.top = iY;
				rBoxEnemy.right = iX + bmpImage->w;
				rBoxEnemy.bottom = iY + bmpImage->h;

				// create a bounding box for the player
				RECT	rBoxPlayer;
				rBoxPlayer.left = app->player->iX;
				rBoxPlayer.top = app->player->iY;
				rBoxPlayer.right = app->player->iX + app->bmpOrbuliteSprites[0]->w;
				rBoxPlayer.bottom = app->player->iY + app->bmpOrbuliteSprites[0]->h;

				// if the player and this goal/finish are colliding.. head to next level!
				if (collision_detected(rBoxEnemy, rBoxPlayer))
				{
					// let the game know that the previous level has stopped in this frame (used mainly in EnemyList::tick())
					app->bLevelStoppedThisFrame = true;

					// delete the player and create a new one
					delete app->player;
					app->player = new Player();
					// start off with full HP
					app->iPlayerHPCurrent = app->iPlayerHPMax;

					// delete current level, and start the next level
					app->current_level->stopMusic();
					delete app->current_level;
					app->current_level = new Level("maps/level2-2.FMP", "backgrounds/cave.bmp", "music/red_river_cave.wav", "zones/redrivercave2.bmp", 120);
					app->current_level->load();
					app->current_level->start();
				}
			}
		}
		break;



		// Act 2-2 Goal/finish
		case 222:
		{
			if (app->iAppMode == MODE_GAME)
			{
				// create a bounding box for the enemy
				RECT	rBoxEnemy; // bounding box for this enemy
				rBoxEnemy.left = iX;
				rBoxEnemy.top = iY;
				rBoxEnemy.right = iX + bmpImage->w;
				rBoxEnemy.bottom = iY + bmpImage->h;

				// create a bounding box for the player
				RECT	rBoxPlayer;
				rBoxPlayer.left = app->player->iX;
				rBoxPlayer.top = app->player->iY;
				rBoxPlayer.right = app->player->iX + app->bmpOrbuliteSprites[0]->w;
				rBoxPlayer.bottom = app->player->iY + app->bmpOrbuliteSprites[0]->h;

				// if the player and this goal/finish are colliding.. head to next level!
				if (collision_detected(rBoxEnemy, rBoxPlayer))
				{
					// let the game know that the previous level has stopped in this frame (used mainly in EnemyList::tick())
					app->bLevelStoppedThisFrame = true;

					// delete the player and create a new one
					delete app->player;
					app->player = new Player();
					// start off with full HP
					app->iPlayerHPCurrent = app->iPlayerHPMax;

					// delete current level, and start the next level
					app->current_level->stopMusic();
					delete app->current_level;
					app->current_level = new Level("maps/level2-3.FMP", "backgrounds/cave.bmp", "music/red_river_cave_boss.wav", "zones/redrivercaveboss.bmp", 200);
					app->current_level->load();
					app->current_level->start();
				}
			}
		}
		break;


		// Act 2-3 Goal/finish
		case 223:
		{
			if (app->iAppMode == MODE_GAME)
			{
				// create a bounding box for the enemy
				RECT	rBoxEnemy; // bounding box for this enemy
				rBoxEnemy.left = iX;
				rBoxEnemy.top = iY;
				rBoxEnemy.right = iX + bmpImage->w;
				rBoxEnemy.bottom = iY + bmpImage->h;

				// create a bounding box for the player
				RECT	rBoxPlayer;
				rBoxPlayer.left = app->player->iX;
				rBoxPlayer.top = app->player->iY;
				rBoxPlayer.right = app->player->iX + app->bmpOrbuliteSprites[0]->w;
				rBoxPlayer.bottom = app->player->iY + app->bmpOrbuliteSprites[0]->h;

				// if the player and this goal/finish are colliding.. head to next level!
				if (collision_detected(rBoxEnemy, rBoxPlayer))
				{
					// let the game know that the previous level has stopped in this frame (used mainly in EnemyList::tick())
					app->bLevelStoppedThisFrame = true;

					// delete the player and create a new one
					delete app->player;
					app->player = new Player();
					// start off with full HP
					app->iPlayerHPCurrent = app->iPlayerHPMax;

					// delete current level, and start the next level
					app->current_level->stopMusic();
					delete app->current_level;
					app->current_level = new Level("maps/level3-1.FMP", "backgrounds/sky2.bmp", "music/atlantis_island.wav", "zones/atlantisisland1.bmp", 180);
					app->current_level->load();
					app->current_level->start();
				}
			}
		}
		break;

		// Act 3-1 Goal/finish
		case 231:
			{
				if (app->iAppMode == MODE_GAME)
				{
					// create a bounding box for the enemy
					RECT	rBoxEnemy; // bounding box for this enemy
					rBoxEnemy.left = iX;
					rBoxEnemy.top = iY;
					rBoxEnemy.right = iX + bmpImage->w;
					rBoxEnemy.bottom = iY + bmpImage->h;

					// create a bounding box for the player
					RECT	rBoxPlayer;
					rBoxPlayer.left = app->player->iX;
					rBoxPlayer.top = app->player->iY;
					rBoxPlayer.right = app->player->iX + app->bmpOrbuliteSprites[0]->w;
					rBoxPlayer.bottom = app->player->iY + app->bmpOrbuliteSprites[0]->h;

					// if the player and this goal/finish are colliding.. head to next level!
					if (collision_detected(rBoxEnemy, rBoxPlayer))
					{
						// let the game know that the previous level has stopped in this frame (used mainly in EnemyList::tick())
						app->bLevelStoppedThisFrame = true;

						// delete the player and create a new one
						delete app->player;
						app->player = new Player();
						// start off with full HP
						app->iPlayerHPCurrent = app->iPlayerHPMax;

						// delete current level, and start the next level
						app->current_level->stopMusic();
						delete app->current_level;
						app->current_level = new Level("maps/level3-2.FMP", "backgrounds/sky2.bmp", "music/atlantis_island.wav", "zones/atlantisisland2.bmp", 120);
						app->current_level->load();
						app->current_level->start();
					}
				}
			}
			break;

		// Act 3-2 Goal/finish
		case 232:
			{
				if (app->iAppMode == MODE_GAME)
				{
					// create a bounding box for the enemy
					RECT	rBoxEnemy; // bounding box for this enemy
					rBoxEnemy.left = iX;
					rBoxEnemy.top = iY;
					rBoxEnemy.right = iX + bmpImage->w;
					rBoxEnemy.bottom = iY + bmpImage->h;

					// create a bounding box for the player
					RECT	rBoxPlayer;
					rBoxPlayer.left = app->player->iX;
					rBoxPlayer.top = app->player->iY;
					rBoxPlayer.right = app->player->iX + app->bmpOrbuliteSprites[0]->w;
					rBoxPlayer.bottom = app->player->iY + app->bmpOrbuliteSprites[0]->h;

					// if the player and this goal/finish are colliding.. head to next level!
					if (collision_detected(rBoxEnemy, rBoxPlayer))
					{
						// let the game know that the previous level has stopped in this frame (used mainly in EnemyList::tick())
						app->bLevelStoppedThisFrame = true;

						// delete the player and create a new one
						delete app->player;
						app->player = new Player();
						// start off with full HP
						app->iPlayerHPCurrent = app->iPlayerHPMax;

						// delete current level, and start the next level
						app->current_level->stopMusic();
						delete app->current_level;
						app->current_level = new Level("maps/level3-3.FMP", "backgrounds/stars.bmp", "music/atlantis_island.wav", "zones/atlantisislandboss.bmp", 200, -1.5f, -0.4f, false);
						app->current_level->load();
						app->current_level->start();
					}
				}
			}
			break;


		case 301: // ending1 controller object
			{
				app->iViewX++; // scroll to the right
				iVariables[0]++; // temporary time counter

				if (iVariables[1] == 0)
				{
					if (iVariables[0] == 300) // delay before text starts scrolling
					{
						iVariables[0] = 0;
						iVariables[1] = 1;
						iVariables[2] = SCREEN_HEIGHT; // y coordinate of text to be drawn
					}
				}
				else if (iVariables[1] == 1)
				{
					if (iVariables[0] == 3) // scroll up on every 3rd frame
					{
						iVariables[2]--;
						iVariables[0] = 0;

					}
				}

				if (iVariables[2] < -250) // go to next scene once all text has scrolled off the screen
				{
					delete app->current_level;
					app->current_level = new Level("maps/ending2.FMP", "backgrounds/sky.bmp", "music/soft_melodies.wav"); // create the ending2 scene
					app->current_level->load();
					app->current_level->start();
				}
			}
			break;
		
			
			case 302: // ending2 controller object
			{
				iVariables[0]++; // temporary time counter
			}
			break;
			

			case 303: // Orbulite object used in cutscenes
			{
				if ( (fVel_x != 0) && (fVel_y == 0) )
				{
					// decrease the countdown timer (the timer counts down to the next frame switch)
					iVariables[0]--;

					// if it is time to switch frames..
					if (iVariables[0] <= 0)
					{
						// reset timer and switch to next frame
						iVariables[0] = PLAYER_FRAME_DELAY;

						// if frame is left direction..
						if (iVariables[1] < 3)
						{
							iVariables[1]++;
							if (iVariables[1] == 3)
								iVariables[1] = 0;
						}
						// if frame is right direction..
						else
						{
							iVariables[1]++;
							if (iVariables[1] > 5)
								iVariables[1] = 3;
						}

					}
				}
				else
				{
					// stop animating Orbulite because Orbulite is not moving..

					if (iVariables[1] < 3) // if current animation frame is in the left direction
						iVariables[1] = 0; // set to first frame of left direction
					else
						iVariables[1] = 3; // set to first frame of right direction
				}
				bmpImage = bmpFrames[(iVariables[1])];
			}
			break;

	}

	return false;
}

void Enemy::customDrawing(BITMAP * destination)
{
	switch (iTypeId)
	{
		case 301: // ending1 controller object
		{
			int lineCount;
			std::string linesToDraw[10];
			switch (iVariables[1])
			{
				case 0:
					lineCount = 0;
					break;
				case 1:
					lineCount = 7;
					linesToDraw[0] = "And so it came to be that a lowly Orbulite,";
					linesToDraw[1] = "weakest of all the monsters of the Zodiac Kingdom,";
					linesToDraw[2] = "fought the odds to become the hero of the day.";
					linesToDraw[3] = "";
					linesToDraw[4] = "Now, the grave threats of the rampaging Baby Sea Dragon,";
					linesToDraw[5] = "the unruly Demon Minion, and the power-hungry Realmkeeper";
					linesToDraw[6] = "were no more.";
					break;
			}

			for (int i = 0; i < lineCount; i++)
				textout_centre_ex(destination, font, linesToDraw[i].c_str(), (SCREEN_WIDTH / 2), iVariables[2] + (i * 30), 0, -1);

		}
		break;
	}
}