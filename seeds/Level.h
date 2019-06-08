class Level
{
public:

	Level(char* map_filename, char* background_filename, char* music_filename, char* zone_image_filename, int time_limit, float background_vel_x = 0.0f, float background_vel_y = 0.0f, bool allow_scrolling=true);
	Level(char* map_filename, char* background_filename, char* music_filename, float background_vel_x = 0.0f, float background_vel_y = 0.0f);
	~Level();

	EnemyList	* elEnemies; // list of enemies currently in the level

	std::string strFilename; // filename of the map file
	std::string strBackgroundFilename; // filename of the background file (bmp)
	std::string strMusicFilename; // filename of the music file (wav)
	std::string strZoneFilename; // filename of the Zone (Act) image

	BITMAP * bmpBackground; // pointer to bitmap of background
	BITMAP * bmpZone; // pointer to bitmap of zone info (image saying "Birth Island Act 1", for example)
	SAMPLE * wavMusic; // pointer to digital sound of music (wav file, looped)

	float		fBackgroundVelX; // how fast the background scrolls
	float		fBackgroundVelY;
	float		fBackgroundOffsetX; // current scrolling offset of background
	float		fBackgroundOffsetY;

	int			iTimeLimit; // constant value of how many seconds (NOT ticks) the level has for a time limit
	int			iTimeRemaining; // ticks left until player runs out of time and loses a life as a result

	bool		bAllowScrolling;
	bool		bIsCutscene; // if true, this level is just a cutscene and not an actual, playable level

	void load();
	void start();
	void stopMusic(); // stops the music
	void draw(BITMAP * destination);

	void drawBackground(BITMAP * destination); // draws the background, and also scrolls the background too
};