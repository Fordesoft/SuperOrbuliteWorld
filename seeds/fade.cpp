/** fade.c - High Color Fading Routines Last Revision: 21 June, 2002 Author: Matthew Leverton Description: Functions to fade out the currently displayed screen to black, and to fade in a full screen bitmap to from a black screen. Usage: highcolor_fade_in(BITMAP *bmp_orig, int speed) bmp_orig: a full screen bitmap with the desired fade-in picture speed: a value from 1 to 255; the higher the value, the quicker the fade highcolor_fade_out(speed) speed: a value from 1 to 255; the higher the value, the quicker the fade Notes: The speed is best from a range of 8 to 64. These routines are rather slow and stop all action, so they are only useful as transitions. **/


#include <allegro.h>
#include "main.h"

void highcolor_fade_in(BITMAP *bmp_orig, int speed)
{
   BITMAP *bmp_buff;

    if ((bmp_buff = create_bitmap(SCREEN_W, SCREEN_H)))
    {
        int a;
        if (speed <= 0) speed = 16;
        
        for (a = 0; a < 256; a+=speed)
        {
            clear(bmp_buff);
            set_trans_blender(0,0,0,a);
            draw_trans_sprite(bmp_buff, bmp_orig, 0, 0);
            vsync();
			blit(bmp_buff, screen, 0,0, 0,0, SCREEN_W, SCREEN_H);
        }
        destroy_bitmap(bmp_buff);
    } 

	blit(bmp_orig, screen, 0,0, 0,0, SCREEN_W, SCREEN_H);
	ticks = old_ticks;
}

void highcolor_fade_out(int speed)
{
    BITMAP *bmp_orig, *bmp_buff;

    if ((bmp_orig = create_bitmap(SCREEN_W, SCREEN_H)))
    {
        if ((bmp_buff = create_bitmap(SCREEN_W, SCREEN_H)))
        {
            int a;
            blit(screen, bmp_orig, 0,0, 0,0, SCREEN_W, SCREEN_H);
            if (speed <= 0) speed = 16;
        
            for (a = 255-speed; a > 0; a-=speed)
            {
                clear(bmp_buff);
                set_trans_blender(0,0,0,a);
                draw_trans_sprite(bmp_buff, bmp_orig, 0, 0);
                vsync();
                blit(bmp_buff, screen, 0,0, 0,0, SCREEN_W, SCREEN_H);
            }
            destroy_bitmap(bmp_buff);
        }
        destroy_bitmap(bmp_orig);
    }

	rectfill(screen, 0,0, SCREEN_W,SCREEN_H, makecol(0,0,0));
	ticks = old_ticks;
}
