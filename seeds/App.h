#define MODE_TITLE			0
#define MODE_GAME			1
#define MODE_PAUSE			2
#define MODE_SIGN			3
#define MODE_CUTSCENE		4
#define MODE_GAMEOVER		5
#define MODE_TIMEUP			6

class App
{
public:
	App();
	~App();
	bool LoadSpeechbubbleBitmaps();

	int		iAppMode; // the current state of the game.. ex: MODE_GAME for playing the game normally, MODE_PAUSE for paused, etc.
	
	Player * player; // the main player object
	Level  * current_level; // level object

	BitmapList	 * blBitmaps; // list of enemy bitmaps currently loaded (basically a depot for enemy images, etc.).  deleted at the end of a level.
	SoundList	 * slSounds; // list of sound files, mostly/totally used by enemies, currently loaded (basically a depot for enemy sounds, etc.).  deleted at the end of a level.
	
	// view of the map
	int		iViewX;
	int		iViewY;

	// player control/invisibility for cutscenes... set both to false for cutscenes and whatnot
	bool	bTickPlayer;
	bool	bDrawPlayer;
	bool	bAllowPlayerMovement;

	// variables regarding the Zone (Act) information image being displayed
	bool	bDisplayZone;
	int		iDisplayZoneX;
	int		iDisplayZoneY;
	int		iDisplayZoneTimer;

	// player's current level, exp (not stored in Player because Player object is deleted/recreated regularly and isn't persistent)
	int		iLives; // how many lives the player has
	int		iPlayerLevel;
	int		iExpCurrent;
	int		iExpNeeded;
	int		iPlayerAttack; // how much damage the player deals to enemies (starts at 5 and increases with level-ups)
	int		iPlayerHPCurrent;
	int		iPlayerHPMax;
	int		iPlayerSpecies; // which specie the player is currently at (0 = Orbulite, 1 = Orbudark, 2 = Orbumoon, 3 = Orbusun)
	int		iPlayerGold; // how much gold the player currently has
	unsigned long	iPlayerScore; // player's current score

	int		CalcExpNext(int level); // formula for calculating exp to next level
	void	AddExp(int amount); // adds exp to player

	void	AddGold(int amount); // adds gold, and awards a new life if gold reaches 50

	bool bLevelStoppedThisFrame; // whether the player died in this frame (mostly used in EnemyList::tick()

	// functions
	void	tick(); // tick the entire game
	void	draw(BITMAP * destination); // draw the entire game to "destination" (normally the buffer bitmap)

	void	playerDeath(); // called if the player dies.  this will remove a life and restart the level.

	void	drawInterface(BITMAP * destination); // draws the GUI

	void	ShowSign(char* line1, char* line2, char* line3, char* line4, char* line5, char* line6, char* line7, char* line8, char* line9, char* line10); // switches to sign-viewing mode with the given text

	// player sprites
	BITMAP * bmpOrbuliteSprites[6]; // frames of Orbulite (3 in each direction).  0, 1, 2 are left, and 3, 4, 5 are right.
	BITMAP * bmpOrbudarkSprites[6]; // frames of Orbudark (3 in each direction).  0, 1, 2 are left, and 3, 4, 5 are right.
	BITMAP * bmpOrbumoonSprites[6]; // frames of Orbumoon (3 in each direction).  0, 1, 2 are left, and 3, 4, 5 are right.
	BITMAP * bmpOrbusunSprites[6]; // frames of Orbusun (3 in each direction).  0, 1, 2 are left, and 3, 4, 5 are right.
	// note that the width and height of all frames must be equal, as code in Enemy.cpp checks the width/height of bmpOrbuliteSprites[0] when calculating player's bounding box
	// for collisions.

	// interface images
	BITMAP * bmpInterfaceLeft; // left-side interface (shows player details: portrait, level, HP, exp)
	BITMAP * bmpInterfaceRight; // right-side interface (shows enemy details: portrait, HP)
	BITMAP * bmpPortraitOrbulite; // Orbulite portrait
	BITMAP * bmpPortraitOrbudark; // Orbudark portrait
	BITMAP * bmpPortraitOrbumoon; // Orbumoon portrait
	BITMAP * bmpPortraitOrbusun; // Orbusun portrait
	BITMAP * bmpLivesOrbulite; // "Lives" image for Orbulite
	BITMAP * bmpLivesOrbudark; // "Lives" image for Orbudark
	BITMAP * bmpLivesOrbumoon; // "Lives" image for Orbumoon
	BITMAP * bmpLivesOrbusun; // "Lives" image for Orbusun
	BITMAP * bmpClock; // clock image (it is used to show the time remaining)
	BITMAP * bmpGold; // gold image (it used used to show the gold coins collected)
	BITMAP * bmpScore; // score image (says "Score:")
	BITMAP * bmpSign; // sign background
	BITMAP * bmpBarGreen; // green portion of HP bar
	BITMAP * bmpBarRed; // red portion of HP bar
	BITMAP * bmpGameover; // "GAME OVER" image
	BITMAP * bmpOutOfTime; // "You have run out of time..." image
	BITMAP * bmpLevelup1; // the basic "Level-Up!" image
	BITMAP * bmpLevelup2; // the normal "Level-Up & Mutation!" image, shown when player reaches level 8 and is currently Orbulite (not powered-up)
	BITMAP * bmpLevelup3; // the alternate "Level-Up & Mutation!" image, shown when player reaches level 8 and is currently powered-up

	// speech bubble bitmaps... these bitmaps are drawn to form a speech bubble image
	BITMAP * bmpSpeechC; // center..
	BITMAP * bmpSpeechL; // left..
	BITMAP * bmpSpeechR; // right..
	BITMAP * bmpSpeechT; // top..
	BITMAP * bmpSpeechB; // bottom..
	BITMAP * bmpSpeechTL; // top-left corner..
	BITMAP * bmpSpeechTR; //top-right corner..
	BITMAP * bmpSpeechBL; // bottom-left corner..
	BITMAP * bmpSpeechBR; // bottom-right corner.
	BITMAP * bmpSpeechPointRight; // pointy tile for speakers to the right
	BITMAP * bmpSpeechPointLeft; // pointy tile for speakers to the left

	int		iOutOfTimeY; // the Out of Time message scrolls up, so this is to measure its Y coordinate progress as it's scrolling up in MODE_TIMEUP
	int		iOutOfTimeTimer; // timer used in the pause that the "out of time" message does when it's in the center of the screen

	int		iLevelupMessage; // level-up message currently being shown (0 = none, 1 = normal Level-Up message (levelup1.bmp), 2 = levelup2.bmp, 3 = levelup3.bmp)
	int		iLevelupY; // the level-up message scrolls up, so this is to measure its Y coordinate progress as it's scrolling up
	int		iLevelupTimer; // timer used in the pause that the "level-up" message does when it's in the center of the screen

	FONT * fontArial26; // large text, primarily used in Lives counter

	// sounds and music used often
	SAMPLE * wavGameOver; // game over music (very short music)
	SAMPLE * wavJump; // jump sound
	SAMPLE * wavCoin; // coin-gained sound
	SAMPLE * wavNoice; // "NOICE!!" sound (played upon receiving powerups)
	SAMPLE * wavLevelUp; // "Level up!" sound (played upon leveling up)
	SAMPLE * wavOw; // "Ow!" sound (played upon dying)

	// sign strings
	std::string	strSign1; // line 1 of the current (or most recent) sign
	std::string	strSign2; // line 2 of the current (or most recent) sign
	std::string	strSign3; // line 3 of the current (or most recent) sign
	std::string	strSign4;
	std::string	strSign5;
	std::string	strSign6;
	std::string	strSign7;
	std::string	strSign8;
	std::string	strSign9;
	std::string	strSign10;

	// right-side interface (enemy details) variables...
	bool	bShowEnemyBox;  // whether to show right-side box
	int		iEnemyBoxTimer; // countdown until right-side enemy box stops being shown
	std::string strEnemyName; // name of the enemy shown in-right side box
	int		iEnemyHPCurrent; // current hp of last enemy attacked (shown in right-side box)
	int		iEnemyHPMax; // max hp of last enemy attacked (shown in right-side box)
	int		iEnemyHPPercent; // percentage of HP of last enemy attacked (shown in right-side box)
	BITMAP * bmpEnemyPortrait; //  // portrait bitmap of last enemy attacked (shown in right-side box)

	
	std::stringstream stream;
	std::string strLives;
	std::string strTime;
};

extern App * app;