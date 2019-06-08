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



Player::Player()
{
	iX = 0;
	iY = 0;
	iLastX = 0;
	iLastY = 0;
	fX = 0;
	fY = 0;
	fVel_x = 0;
	fVel_y = 0;

	// set speed and jump speed
	if (app->iPlayerSpecies == 0) // Orbulite
	{
		fNormalMaxSpeed = ORBULITE_NORMAL_MAX_SPEED;
		fJumpSpeed = ORBULITE_JUMP_SPEED;
		fAccelRate = ORBULITE_ACCEL_RATE;
		fFallRate = ORBULITE_FALL_RATE;
	}
	else if (app->iPlayerSpecies == 1) // Orbudark
	{
		fNormalMaxSpeed = ORBUDARK_NORMAL_MAX_SPEED;
		fJumpSpeed = ORBUDARK_JUMP_SPEED;
		fAccelRate = ORBUDARK_ACCEL_RATE;
		fFallRate = ORBUDARK_FALL_RATE;
	}
	else if (app->iPlayerSpecies == 2) // Orbumoon
	{
		fNormalMaxSpeed = ORBUMOON_NORMAL_MAX_SPEED;
		fJumpSpeed = ORBUMOON_JUMP_SPEED;
		fAccelRate = ORBUMOON_ACCEL_RATE;
		fFallRate = ORBUMOON_FALL_RATE;
	}
	else // Orbusun (species 3)
	{
		fNormalMaxSpeed = ORBUSUN_NORMAL_MAX_SPEED;
		fJumpSpeed = ORBUSUN_JUMP_SPEED;
		fAccelRate = ORBUSUN_ACCEL_RATE;
		fFallRate = ORBUSUN_FALL_RATE;
	}

	bInvincible = false;
	iInvincibilityTimer = 0;


	bJumping = false; // not jumping in the beginning
	bFalling = true; // start falling right away in case the player begins above ground

	iFrameCurrent = 3; // start on right-direction animation frame 1
	iFrameTimer = PLAYER_FRAME_DELAY;
}

Player::~Player()
{
}

void Player::tick()
{
	// handle player movement
	if (app->bAllowPlayerMovement)
		handleMovement();

	// check for invincibility, and remove it when the timer is up
	if (bInvincible)
	{
		iInvincibilityTimer--;
		if (iInvincibilityTimer <= 0)
			bInvincible = false;
	}
}

void Player::handleMovement()
{
	if (key_state[KEY_LSHIFT])
		fMaxSpeed = fNormalMaxSpeed + 2;
	else
		fMaxSpeed = fNormalMaxSpeed;

	// handle left movement
	if (key_state[KEY_LEFT])
	{
		fVel_x -= fAccelRate;
		if (fVel_x < -fMaxSpeed)
			fVel_x = -fMaxSpeed;

		// switch to Left animation frames
		if (iFrameCurrent > 2)
		{
			iFrameCurrent = 0;
			iFrameTimer = PLAYER_FRAME_DELAY;
		}
	}
	else
	{
		if (fVel_x < 0)
		{
			fVel_x += fAccelRate;
			if (fVel_x > 0)
				fVel_x = 0;
		}
	}

	// handle right movement
	if (key_state[KEY_RIGHT])
	{
		fVel_x += fAccelRate;
		if (fVel_x > fMaxSpeed)
			fVel_x = fMaxSpeed;
		
		// switch to Right animation frames
		if (iFrameCurrent < 3)
		{
			iFrameCurrent = 3;
			iFrameTimer = PLAYER_FRAME_DELAY;
		}
	}
	else
	{
		if (fVel_x > 0)
		{
			fVel_x -= fAccelRate;
			if (fVel_x < 0)
				fVel_x = 0;
		}
	}


	// adjust velocity for jumping (this must be before KEY_UP event)
	if (bJumping)
	{
		if (fVel_y < 0)
		{
			if (key_state[KEY_UP])
				fVel_y += fFallRate; // all normal if up arrow key is held in still
			else
				fVel_y += fFallRate * 3; // fall faster (meaning a shorter jump) if Up arrow key was released
			if (fVel_y >= 0)
			{
				fVel_y = 0;
				bJumping = false;
				bFalling = true; // start falling
			}
		}
	}

	if (bFalling)
	{
		// increase downward velocity
		//if (fVel_y < fMaxSpeed)
			fVel_y += fFallRate;
		// cap out the speed if going above max speed
		if (fVel_y > fJumpSpeed)
			fVel_y = fJumpSpeed;
	}

	// handle jumping key
	if (key_state[KEY_UP])
	{
		// if not already jumping or falling
		if ( (!bJumping) && (!bFalling) )
		{
			// start jumping
			fVel_y = -fJumpSpeed;
			bJumping = true;

			// play jump sound
			play_sample(app->wavJump, 255, 128, 1000, 0);
		}
	}

	// animate player if moving horizontally but not jumping
	if ( (fVel_x != 0) && (fVel_y == 0) )
	{
		// decrease the countdown timer (the timer counts down to the next frame switch)
		iFrameTimer--;

		// if it is time to switch frames..
		if (iFrameTimer <= 0)
		{
			// reset timer and switch to next frame
			iFrameTimer = PLAYER_FRAME_DELAY;
			
			// if frame is left direction..
			if (iFrameCurrent < 3)
			{
				iFrameCurrent++;
				if (iFrameCurrent == 3)
					iFrameCurrent = 0;
			}
			// if frame is right direction..
			else
			{
				iFrameCurrent++;
				if (iFrameCurrent > 5)
					iFrameCurrent = 3;
			}
			
		}
	}
	else
	{
		// stop animating player because player is not moving..

		if (iFrameCurrent < 3) // if current animation frame is in the left direction
			iFrameCurrent = 0; // set to first frame of left direction
		else
			iFrameCurrent = 3; // set to first frame of right direction
	}
	

	// get tile coordinates of which tiles the edges of the player lies within (before and after movement)..

	RECT rCurrent;  // bounding box of current coordinates
	RECT rNext;  // bounding box of next coordinates

	rCurrent.bottom = floor( (fY + app->bmpOrbuliteSprites[0]->h - 1) / 32 ); // find Y tile coordinate of the very bottom of the character
	rCurrent.top = floor( (fY) / 32 ); // find the Y tile coordinate top
	rCurrent.left = floor( (fX) / 32 ); // find the X tile coordinate left
	rCurrent.right = floor( (fX + app->bmpOrbuliteSprites[0]->w - 1) / 32); // find the X tile coordiante right

	rNext.bottom = floor( (fY + fVel_y + app->bmpOrbuliteSprites[0]->h - 1) / 32 ); // find Y tile coordinate of the very bottom of the character
	rNext.top = floor( (fY + fVel_y) / 32 ); // find the Y tile coordinate top
	rNext.left = floor( (fX + fVel_x) / 32 ); // find the X tile coordinate left
	rNext.right = floor( (fX + fVel_x + app->bmpOrbuliteSprites[0]->w - 1) / 32); // find the X tile coordiante right
	rNext.middle_y = floor( (fY + fVel_y + app->bmpOrbuliteSprites[0]->h/2 - 1) / 32 );
	rNext.middle_x = floor( (fX + fVel_x + app->bmpOrbuliteSprites[0]->w/2 - 1) / 32 );


	// handle falling
	if ( (!bJumping) && (!bFalling) )
	{
		if (
			( (!tile_is_solid(rCurrent.left, rCurrent.bottom)) && (!tile_is_solid(rCurrent.left, rCurrent.bottom + 1)) )
			|| 
			( (tile_is_solid(rCurrent.right, rCurrent.bottom)) && (!tile_is_solid(rCurrent.right, rCurrent.bottom + 1)) )
			
			)
		{
			bFalling = true;
		}
	}



	// check for solid blocks below the player
	if (fVel_y > 0)
	{
		if (rCurrent.bottom != rNext.bottom) // if moving down into next tile's realm
		{
			if ( (tile_is_solid(rNext.left, rNext.bottom)) || (tile_is_solid(rNext.right, rNext.bottom)) || (tile_is_solid(rNext.middle_x, rNext.bottom)) )
			{
				fVel_y = 0; // stop moving down
				fY = (rNext.bottom * 32) - app->bmpOrbuliteSprites[0]->h; // adjust Y coordinate
				bJumping = false; // stop jumping if the player is jumping
				bFalling = false; // stop falling if applicable
			}
		}
	}

	// check for solid blocks above the player
	if (fVel_y < 0)
	{
		if (rCurrent.top != rNext.top) // if moving down into next tile's realm
		{
			if ( (tile_is_solid(rNext.left, rNext.top)) || (tile_is_solid(rNext.right, rNext.top)) || (tile_is_solid(rNext.middle_x, rNext.top)) )
			{
				fVel_y = 0;
				fY = (rNext.top * 32) + 32; // adjust Y coordinate
				bJumping = false; // stop jumping if the player is jumping
				bFalling = true; // start falling back down
			}
		}
	}

	// check for solid blocks to the right of the player
	if (fVel_x > 0)
	{
		if (rCurrent.right != rNext.right) // if moving right into next tile's realm
		{
			if ( (tile_is_solid(rNext.right, rNext.top)) || (tile_is_solid(rNext.right, rNext.bottom)) || (tile_is_solid(rNext.right, rNext.middle_y)) )
			{
				fVel_x = 0; // stop moving right
				fX = (rNext.right * 32) - app->bmpOrbuliteSprites[0]->w; // adjust X coordinate
			}
		}
	}


	// check for solid blocks to the left of the player
	if (fVel_x < 0)
	{
		if (rCurrent.left != rNext.left) // if moving left into next tile's realm
		{
			if ( (tile_is_solid(rNext.left, rNext.bottom)) || (tile_is_solid(rNext.left, rNext.top)) || (tile_is_solid(rNext.left, rNext.middle_y))  )
			{
				fVel_x = 0;
				fX = (rNext.left * 32) + 32; // adjust X coordinate
			}
		}
	}
	// adjust player position according to velocity
	fX += fVel_x;
	fY += fVel_y;

	// make sure player doesn't go past left/right boundaries of screen
	if (fX < 0)
	{
		fX = 0;
		if (fVel_x < 0)
			fVel_x = 0;
	}
	if (fX + app->bmpOrbuliteSprites[0]->w > mapwidth * 32)
	{
		fX = mapwidth * 32 - app->bmpOrbuliteSprites[0]->w;
		if (fVel_x > 0)
			fVel_x = 0;
	}

	// set the "previous x and y" coordinates to this frame's coodinates before updating them
	iLastX = iX;
	iLastY = iY;

	// set integer values of coordinates for drawing
	iX = fX;
	iY = fY;

	// scroll window to follow player
	if (app->current_level->bAllowScrolling)
	{
		app->iViewX = iX - (SCREEN_WIDTH / 2) + app->bmpOrbuliteSprites[0]->w;
		if (app->iViewX < 0)
			app->iViewX = 0;
		if (app->iViewX > mapwidth * 32 - 800)
			app->iViewX = mapwidth * 32 - 800;

		app->iViewY = iY - (SCREEN_HEIGHT / 2) + app->bmpOrbuliteSprites[0]->h;
		if (app->iViewY < 0)
			app->iViewY = 0;
		if (app->iViewY > mapheight * 32 - 600)
			app->iViewY = mapheight * 32 - 600;
	}

	// kill the player if the player is below the map (fallen through a hole)
	if (iY >= (mapheight + 1) * 32)
		app->playerDeath();

}

void Player::draw(BITMAP * destination)
{
	// draw player when not invincibile, or every other frame when invincibile
	if ( (!bInvincible) || (iInvincibilityTimer % 2 == 0) )
	{
		switch (app->iPlayerSpecies)
		{
			// Orbulite
			case 0:
				draw_sprite(destination, app->bmpOrbuliteSprites[iFrameCurrent], iX - app->iViewX, iY - app->iViewY);
				break;
			// Orbudark
			case 1:
				draw_sprite(destination, app->bmpOrbudarkSprites[iFrameCurrent], iX - app->iViewX, iY - app->iViewY);
				break;
			// Orbumoon
			case 2:
				draw_sprite(destination, app->bmpOrbumoonSprites[iFrameCurrent], iX - app->iViewX, iY - app->iViewY);
				break;
			// Orbusun
			case 3:
				draw_sprite(destination, app->bmpOrbusunSprites[iFrameCurrent], iX - app->iViewX, iY - app->iViewY);
				break;
		}
	}
}