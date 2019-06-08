#define SPEECHBUBBLE_PIXELS_BETWEEN_LINES 0
#define SPEECHBUBBLE_DEFAULT_PADDING 10
#define SPEECHBUBBLE_TILE_SIZE 32 // width/height of the square, tiled images that make up the speech bubble image

class Enemy;
class Speechbubble
{
//private:
public:
	int iUpperleftX, iUpperleftY; // the pixel coordinate of the top-left corner of the speechbubble
	int iPivotX, iPivotY;
	int	iPadding; // pixel padding between the edges and the text
	int iWidth, iHeight; // note: height does NOT include the little pointy tile
	std::vector<std::string> strText;
	BITMAP * bmpBackground;
	bool bSpeakerIsToTheRight;
	Enemy * enSpeaker; // object or enemy speaking this bubble.  the bubble will follow that object/enemy.
	void setLocationToFollowSpeaker(); // updates this speech bubble's location to follow the object/enemy speaking the bubble.

public:
	Speechbubble(std::string text, Enemy * speaker, int widthPixels, int padding, bool speakerIsToTheRight=false); // use this constructor usually
	Speechbubble(std::string text, int pivotX, int pivotY, int widthPixels, int padding, bool speakerIsToTheRight=false);
	void draw(BITMAP * destination);
	BITMAP * createBackground();

};