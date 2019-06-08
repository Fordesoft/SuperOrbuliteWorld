
#define PLAYER_FRAME_DELAY 21


#define ORBULITE_NORMAL_MAX_SPEED	2
#define ORBULITE_ACCEL_RATE			0.05
#define ORBULITE_JUMP_SPEED			4.0
#define ORBULITE_FALL_RATE			0.05

#define ORBUDARK_NORMAL_MAX_SPEED	2.5
#define ORBUDARK_ACCEL_RATE			0.05
#define ORBUDARK_JUMP_SPEED			4.0
#define ORBUDARK_FALL_RATE			0.04

#define ORBUMOON_NORMAL_MAX_SPEED	3
#define ORBUMOON_ACCEL_RATE			0.10
#define ORBUMOON_JUMP_SPEED			4.0
#define ORBUMOON_FALL_RATE			0.045

#define ORBUSUN_NORMAL_MAX_SPEED	2.2
#define ORBUSUN_ACCEL_RATE			0.05
#define ORBUSUN_JUMP_SPEED			4.0
#define ORBUSUN_FALL_RATE			0.05


class Player
{
public:
	// constructor and deconstructor
	Player();
	~Player();

	// variables
	int iX, iY; // x and y coordinates
	float fX, fY; // x and y coordinates

	// movement and jumping variables.  can be increased by transforming.
	float fNormalMaxSpeed;
	float fAccelRate;
	float fJumpSpeed;
	float fFallRate; // how fast the player falls (i.e. how fast Y velocity changes when jumping/falling)
	float fMaxSpeed; // maximum speed of the monster (it is based off of NORMAL_MAX_SPEED).  higher if dash button (Shift) is held in.

	int iLastX, iLastY; // x and y coordinates of the previous frame (for collision detection purposes-- monsters need to know whether the player was above them when jumping on them)

	bool bJumping; // whether player is jumping
	bool bFalling; // whether player is falling
	bool bInvincible; // whether the player is (temporarily) invincible
	int  iInvincibilityTimer; // how many ticks left until the player becomes non-invincible (i.e. invincibility is set to false when this hits 0)

	float fVel_x, fVel_y; // movement speed/direction

	int	iFrameCurrent; // value of 0 through 5.  current frame to be drawn (0, 1, 2 are left; 3, 4, 5 are right)
	int iFrameTimer; // ticks left until frame is switched to next frame

	// functions
	void tick();
	void draw(BITMAP * destination);

	void handleMovement();
};