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

#define NUM_FLASHES 3
#define NUM_FLASHES_WIN 5
#define FLASH_TOTAL_DURATION 60
#define FLASH_ON_DURATION (FLASH_TOTAL_DURATION / 2)
#define HIT_MESSAGE_TICK_LENGTH 1030
#define MISS_MESSAGE_TICK_LENGH 1250
#define MAX_X_COORD 4
#define MAX_Y_COORD 6
#define NUM_ROWS 5
#define NUM_COLS 7
#define HIT_MISS_TEXT_SPEED 30
#define WIN_LOSE_TEXT_SPEED 20


typedef struct ship_struct
//struct for ship which containts the start and end x/y coordinates
{
    int length;
    int xstart;
    int ystart;
    int xend;
    int yend;
} Ship;


/*display known hits/misses from other persons board based on values
 * in 'visible' board matrix. 'on_or_off' parameter dictates whether
 * flashing light is on (1) or off (0) */
void draw_visible(int visible[][NUM_COLS], int on_or_off);

/*display ships that have already been placed */
void display_placed(Ship ship2, Ship ship3, Ship ship4, int ship);

/*display either hit or miss message based on 'hit' parameter
 * where a hit is represented by hit=1, miss is hit=0 */
void display_hit_miss(int hit, int x, int y);

/*display either win or lose message based on 'win' parameter
 * which is set to 1 for a win and 0 for a loss */
void display_win_lose(int win);
