class Enemy
{
public:
	Enemy(int enemy_type_id, int x_pixel, int y_pixel, bool is_misc_object=false); // "is_misc_object" should only be true for powerups, objectives, etc. using this class

	Enemy * next; // next enemy in linked list

	int iInstanceId; // id of this particular instance (useful in linked list, and must be unique)
	int iTypeId; // the id of the type of monster.  must be greater than 0.
	
	BITMAP * bmpImage; // pointer to monster's current image frame

	BITMAP * bmpPortrait; // pointer to monster's portrait image (used on the right-side interface when monster has been attacked)

	BITMAP * bmpFrames[20]; // up to 20 frames of bitmappy goodness, pointing to an outside-existing bitmap in the BitmapList.  bmpImage will normally point to one of these.

	SAMPLE * wavSounds[10]; // up to 10 sounds/samples (wav files) that can be used, pointing to an outside-existing sample in the SoundList.

	float fX; // current x coordinate
	float fY; // current y coordinate
	int iX; // int value of x coodinate (for drawing, mainly, and possibly for collision detection)
	int iY; // int value of y coodinate (for drawing, mainly, and possibly for collision detection)
	float fVel_x, fVel_y; // current movement speed

	std::string strName; // name of this enemy (ex: Bootworm)
	int iHP; // how much HP the enemy has left (each jump takes 5+ away)
	int	iHPMax; // maximum amount of HP this enemy has/had
	int iAttack; // how much damage this enemy does to the player (5 by default, higher for later monsters)
	int iExpGiven; // how much exp the enemy gives to the player when killed
	bool bInvincible; // whether the enemy cannot be killed (often true for things like bullets shot from enemies)
	bool bVisible; // whether the enemy should be drawn
	bool bDoCustomDrawing; // whether customDrawing() should be called (default true and almost always true)

	int iStartX, iStartY; // coordinates of where the monster started when the level began

	bool bMiscObject; // false for regular enemies.  true for misc objects like powerups, objectives, etc. which all use the Enemy class.

	int  iVariables[10]; // a set of variables the enemy object can use.  usage differs from specific enemy type to specific enemy type.
	bool bMoveAfterAI; // true in most cases.  whether this object's tick() function should adjust X, Y coordinates according to velocity (should be set to false if the customAI() function is set to adjust X, Y in a specific enemy object's case)

	bool tick(); // called every frame; moves the monster, etc.  returns false normally, or true if this enemy needs to be deleted in this frame.
	void draw(BITMAP * destination); // called every frame; draws the monster on-screen
	bool customAI(); // hard-coded, custom AI per each monster type is in this code.  called from tick() function.  returns false normally, or true if this enemy needs to be deleted in this frame.
	void customDrawing(BITMAP * destination); // hard-coded, custom drawing code for particular enemies in addition to regular drawing (mainly just for cutscene objects)
	void DeathActions(); // hard-coded actions that is called upon a monster's defeat
	void DamageActions(); // hard-coded actions that is called upon a monster being hit (but not killed)

	bool collidingWithPlayer(); // whether this enemy is colliding with the player (called somewhere in tick() in most cases)
	bool collidingWithPlayersNextFrame(); // special function -- whether the enemy's current frame will be colliding with the player's next frame (used in blocks' CustomAI code to prevent a bug)

private:
	Enemy * enOtherParts[10]; // for multi-part enemies (the Realmkeeper), these pointers are used from the head object to point to the rest (body and blob)
	Speechbubble * sbSpeechBubbles[10]; // speech bubbles the object has created
};

class EnemyList
{
public:
	EnemyList(); // constructor -- initialize variable(s) and pointer(s) for an empty list
	~EnemyList(); // deconstructor -- delete all enemies in the list

	Enemy * head;
	Enemy * tail;

	int		iNextInstanceId; // uinique id for the next instance to create.  starts at 0 at the start of the level.

	void	AddEnemy(Enemy * new_enemy); // adds an enemy to the linked list and gives it a unique instance id
	void	RemoveEnemy(int instance_id); // remove an enemy from the list (pass the unique instance id of the enemy)

	void	tick(); // ticks all enemies in the list by calling their tick() functions
	void	draw(BITMAP * destination); // draws all enemies in the list by calling their draw() functions
};