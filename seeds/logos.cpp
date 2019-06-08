
#include <allegro.h>
#include "main.h"
#include "fade.h"

void seeds_of_time_logo()
{
	BITMAP * tempScreen = create_bitmap(SCREEN_WIDTH, SCREEN_HEIGHT);

	BITMAP* bmpSeedsOfTimeLogo = load_bitmap("logos/seedsoftime.bmp", NULL);
	blit(bmpSeedsOfTimeLogo, tempScreen, 0, 0, 200, 160, 400, 440);
	highcolor_fade_in(tempScreen, 4);
	rest(3000);
	highcolor_fade_out(4);
}

void josh_forde_logo()
{
	BITMAP * tempScreen = create_bitmap(SCREEN_WIDTH, SCREEN_HEIGHT);

	BITMAP* bmpSeedsOfTimeLogo = load_bitmap("logos/joshforde.bmp", NULL);
	blit(bmpSeedsOfTimeLogo, tempScreen, 0, 0, 200, 80, 400, 440);
	highcolor_fade_in(tempScreen, 4);
	rest(3000);
	highcolor_fade_out(4);
}