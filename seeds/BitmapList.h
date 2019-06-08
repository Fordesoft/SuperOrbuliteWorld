// BitmapNode and BitmapList classes
// ...
// Description:
//   These two classes are a solution so that specific images can be loaded and then used by multiple objects if need be.
//   That way, for example, if there are three Bootworms on a particular level, the Bootworm bitmaps will only be stored in memory once, and then
//   the three Bootworms will all point to here.

class BitmapNode // a wrapper for Bitmaps, with the purpose of providing a means to check whether a bitmap has already been loaded by recording its filename and putting it into a linked list
{
public:
	BitmapNode(); // constructor
	~BitmapNode(); // deconstructor (deletes bitmap data as well)

	BitmapNode * next; // the next BitmapNode in the linked list

	std::string strFilename; // full filename, including path, of the bitmap.  ex: "monsters/bootworm/1.bmp"
	BITMAP * bmpImage; // the bitmap itself
};

class BitmapList
{
public:
	BitmapList();
	~BitmapList();

	BitmapNode * head;
	BitmapNode * tail;

	BITMAP * LoadBitmap(std::string filename); // this is the main function used outside of the class.  this loads a bitmap and returns it (or returns a pointer to an existing one if the bitmap has already been loaded into memory)

	void AddNode(BitmapNode * node); // insert a bitmap into the list.  generally not used outside of the class.
	void RemoveNode(std::string filename); // remove a node, based on the filename of the bitmap, and delete its bitmap data from memory.  can be used outside of the class.
};