#define DEBUG 1 // enable debug mode for faster testing (no fade-ins/fade-outs, for example)
#define SCREEN_WIDTH	800
#define SCREEN_HEIGHT	600

extern BITMAP *buffer;

extern volatile int	ticks;
extern volatile int	old_ticks;

typedef struct tagRECT {
   int left;
   int top;
   int right;
   int bottom;

   int middle_x; // extra value sometimes used - the "middle" of a horizontal line.  ex: "20" is the hmiddle of a block that is 40x100 pixels
   int middle_y;  // extra value sometimes used - the middle of a vertical line.  ex: "50" is the vmiddle of a block taht is 40x100 pixels
} RECT;

bool tile_is_solid(int tile_x, int tile_y);
void make_tile_solid(int tile_x, int tile_y);

void handle_frame();

bool collision_detected(RECT& r1, RECT& r2);

int	 random_number(int start, int end); // returns a random number from "start" to "end" (inclusive)

extern int		key_state[KEY_MAX]; // similar to the key[] array provided in Allegro, but this has 3 states (0 = unpressed, 1 = pressed once, 2 = held in)

void seeds_of_time_logo();
void josh_forde_logo();


void write_to_file(char* filename, const char* text); // for debugging purposes