/*
 * ENCE260 GAME: BATTLESHIPS
 * @authors: Logan Beard
 *         & Jen Lowe
 * @date: 17/10/17
 * @brief: Graphics module for battleships game
 * */

#include "tinygl.h"
#include "font.h"
#include "../fonts/font3x5_1.h"
#include "pacer.h"
#include "graphics.h"

void draw_visible(int visible[][7], int on_or_off)
{
/*display known hits/misses from other persons board based on values
 * in 'visible' board matrix. 'on_or_off' parameter dictates whether
 * flashing light is on (1) or off (0) */
    int x = 0;
    int y = 0;
    for (x = 0; x < MAX_X_COORD; x++) {
        for (y = 0; y < MAX_Y_COORD; y++) {
            int value = visible[x][y];
            if (value == 2) {
                tinygl_draw_point(tinygl_point (x, y), on_or_off);
            } else if (value == 1) {
                tinygl_draw_point(tinygl_point (x, y), 1);
            } else {
                tinygl_draw_point(tinygl_point (x, y), 0);
            }

        }
    }

}

void display_placed(Ship ship2, Ship ship3, Ship ship4, int ship)
{
/*display ships that have already been placed */
    if (ship == 3) {
        tinygl_draw_line (tinygl_point (ship2.xstart, ship2.ystart), tinygl_point (ship2.xend, ship2.yend), 1);

    } else if (ship == 4) {
        tinygl_draw_line (tinygl_point (ship2.xstart, ship2.ystart), tinygl_point (ship2.xend, ship2.yend), 1);
        tinygl_draw_line (tinygl_point (ship3.xstart, ship3.ystart), tinygl_point (ship3.xend, ship3.yend), 1);

    } else if (ship == 5) {
        tinygl_draw_line (tinygl_point (ship2.xstart, ship2.ystart), tinygl_point (ship2.xend, ship2.yend), 1);
        tinygl_draw_line (tinygl_point (ship3.xstart, ship3.ystart), tinygl_point (ship3.xend, ship3.yend), 1);
        tinygl_draw_line (tinygl_point (ship4.xstart, ship4.ystart), tinygl_point (ship4.xend, ship4.yend), 1);
    }
}


void display_hit_miss(int hit, int x, int y)
{
/*display either hit or miss message based on 'hit' parameter
 * where a hit is represented by hit=1, miss is hit=0 */
    int tick = 0;
    int duration = 0;
    int flashes = 0;

    if (hit == 1) {
        tinygl_text (" HIT");
        duration = HIT_MESSAGE_TICK_LENGTH;
        while (flashes < NUM_FLASHES) {
            flashes++;
            tick = 0;
            while (tick < FLASH_TOTAL_DURATION) {
                if (tick < FLASH_ON_DURATION) {
                    //flash screen by drawing boxes over all pixels
                    tinygl_draw_box (tinygl_point (0, 0), tinygl_point (4, 6), 1);
                    tinygl_draw_box (tinygl_point (1, 1), tinygl_point (3, 5), 1);
                    tinygl_draw_box (tinygl_point (2, 4), tinygl_point (2, 2), 1);
                    tinygl_draw_point(tinygl_point (x, y), 0);
                } else {
                    tinygl_clear ();
                    tinygl_draw_point(tinygl_point (x, y), 1);
                }
                pacer_wait ();
                tick++;
                tinygl_update ();
            }
        }
        tinygl_clear ();
        tinygl_update ();
        
        //display "HIT" message
        tinygl_font_set (&font3x5_1);
        tinygl_text_speed_set (HIT_MISS_TEXT_SPEED);
        tinygl_text_mode_set (TINYGL_TEXT_MODE_SCROLL);
        tinygl_text_dir_set (TINYGL_TEXT_DIR_ROTATE);
        tinygl_text (" HIT");
        tick = 0;
        while (tick < duration) {
            pacer_wait ();
            tick++;
            tinygl_update ();
        }
        tinygl_clear ();
        tinygl_update ();

    } else {
        //display "MISS" message
        duration = MISS_MESSAGE_TICK_LENGH;
        tinygl_font_set (&font3x5_1);
        tinygl_text_speed_set (HIT_MISS_TEXT_SPEED);
        tinygl_text_mode_set (TINYGL_TEXT_MODE_SCROLL);
        tinygl_text_dir_set (TINYGL_TEXT_DIR_ROTATE);
        tinygl_text (" MISS");
        tick = 0;
        while (tick < duration) {
            pacer_wait ();
            tick++;
            tinygl_update ();
        }
    }
    tinygl_clear ();
    tinygl_update ();
}

void display_win_lose(int win)
{
/*display either win or lose message based on 'win' parameter
 * which is set to 1 for a win and 0 for a loss */
    int tick = 0;
    int flashes = 0;
    if (win == 1) {
        while (flashes < NUM_FLASHES_WIN) {
            flashes++;
            tick = 0;
            while (tick < FLASH_TOTAL_DURATION) {
                if (tick < FLASH_ON_DURATION) {
                    //flash screen by drawing boxes over all pixels
                    tinygl_draw_box (tinygl_point (0, 0), tinygl_point (4, 6), 1);
                    tinygl_draw_box (tinygl_point (1, 1), tinygl_point (3, 5), 1);
                    tinygl_draw_box (tinygl_point (2, 4), tinygl_point (2, 2), 1);
                } else {
                    tinygl_clear ();
                }
                pacer_wait ();
                tick++;
                tinygl_update ();
            }
        }
        tinygl_clear ();
        tinygl_update ();
        
        //display "YOU WIN" message
        tinygl_font_set (&font3x5_1);
        tinygl_text_speed_set (WIN_LOSE_TEXT_SPEED);
        tinygl_text_mode_set (TINYGL_TEXT_MODE_SCROLL);
        tinygl_text_dir_set (TINYGL_TEXT_DIR_ROTATE);
        tinygl_text (" YOU WIN");
        while (1) {
            pacer_wait ();
            tick++;
            tinygl_update ();
        }

    } else {
        while (flashes < NUM_FLASHES) {
            flashes++;
            tick = 0;
            while (tick < FLASH_TOTAL_DURATION) {
                if (tick < FLASH_ON_DURATION) {
                    //flash screen by drawing boxes over all pixels
                    tinygl_draw_box (tinygl_point (0, 0), tinygl_point (4, 6), 1);
                    tinygl_draw_box (tinygl_point (1, 1), tinygl_point (3, 5), 1);
                    tinygl_draw_box (tinygl_point (2, 4), tinygl_point (2, 2), 1);
                } else {
                    tinygl_clear ();
                }
                pacer_wait ();
                tick++;
                tinygl_update ();
            }
        }
        tinygl_clear ();
        tinygl_update ();

        //display "YOU LOSE" message
        tinygl_font_set (&font3x5_1);
        tinygl_text_speed_set (WIN_LOSE_TEXT_SPEED);
        tinygl_text_mode_set (TINYGL_TEXT_MODE_SCROLL);
        tinygl_text_dir_set (TINYGL_TEXT_DIR_ROTATE);
        tinygl_text (" YOU LOSE");
        while (1) {
            pacer_wait ();
            tick++;
            tinygl_update ();
        }

    }

}
