// SoundNode and SoundList classes
// ...
// Description:
//   These two classes are a solution so that specific images can be loaded and then used by multiple objects if need be.
//   That way, for example, if there are three Bootworms on a particular level, the Bootworm sounds will only be stored in memory once, and then
//   the three Bootworms will all point to here.

class SoundNode // a wrapper for sounds, with the purpose of providing a means to check whether a sound has already been loaded by recording its filename and putting it into a linked list
{
public:
	SoundNode(); // constructor
	~SoundNode(); // deconstructor (deletes sound data as well)

	SoundNode * next; // the next SoundNode in the linked list

	std::string strFilename; // full filename, including path, of the sound.  ex: "monsters/bootworm/1.bmp"
	SAMPLE * wavSound; // the sound itself
};

class SoundList
{
public:
	SoundList();
	~SoundList();

	SoundNode * head;
	SoundNode * tail;

	SAMPLE * LoadSound(std::string filename); // this is the main function used outside of the class.  this loads a sound and returns it (or returns a pointer to an existing one if the sound has already been loaded into memory)

	void AddNode(SoundNode * node); // insert a sound into the list.  generally not used outside of the class.
	void RemoveNode(std::string filename); // remove a node, based on the filename of the sound, and delete its sound data from memory.  can be used outside of the class.
};