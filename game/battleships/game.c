/*
 * ENCE260 GAME: BATTLESHIPS
 * @authors: Logan Beard
 *         & Jen Lowe
 * @date: 17/10/17
 * @brief: Two-Player battleships on UCFK4
 * 
*/

#include <stdbool.h>
#include <stdio.h>
#include "graphics.h" // <-- our module
#include "pacer.h"
#include "navswitch.h"
#include "led.h"
#include "tinygl.h"
#include "button.h"
#include "ir_uart.h"
#include "../fonts/font3x5_1.h"

#define PACER_RATE 300
#define SMALLEST_SHIP 2
#define LARGEST_SHIP 5
#define SHIP2 2
#define SHIP3 3
#define SHIP4 4
#define SHIP5 5
#define MAX_X_COORD 4
#define MAX_Y_COORD 6
#define NUM_ROWS 5
#define NUM_COLS 7
#define X_CENTER 2
#define Y_CENTER 3
#define MAX_HITS 14
#define TICK_CYCLE 100
#define LED_TICK_START (TICK_CYCLE - 5)
#define HALF_CYCLE (TICK_CYCLE / 2)
#define FIFTH_CYCLE (TICK_CYCLE / 5)
#define CHANGE_TURN_MSG 1
#define GAMEOVER_MSG 3


//my board (1=ship, 0=water)
int myboard[NUM_ROWS][NUM_COLS] = {
    {0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0}
};
//their board (1=ship, 0=water)
int theirboard[NUM_ROWS][NUM_COLS] = {
    {0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0}
};
//what I can see of their board (0=unknown, 1=miss, 2=hit)
int visible[NUM_ROWS][NUM_COLS] = {
    {0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0}
};

bool check_bounds(int x, int y)
{
    //check that a point of x and y is within the bounds of the array
    bool valid = false;
    if ((y >= 0 && y <= MAX_Y_COORD) && (x >= 0 && x <= MAX_X_COORD)) {
        valid = true;
    }
    return valid;
}

bool check_overlap(Ship ship, int sx2, int sy2, int ex2, int ey2)
{
    //check if two ships are overlapping when user tries to place a ship
    int ship_length = ship.length;
    int sx1 = ship.xstart;
    int sy1 = ship.ystart;
    int ex1 = ship.xend;
    int ey1 = ship.yend;
    bool overlap = false;
    char allignment1 = 0;
    char allignment2 = 0;


    if (sx1 == ex1) {
        //ship is horizontal
        allignment1 = 'h';
    } else {
        //ship is vertical
        allignment1 = 'v';
    }

    if (sx2 == ex2) {
        //ship is horizontal
        allignment2 = 'h';
    } else {
        //ship is vertical
        allignment2 = 'v';
    }


    if (allignment1 == 'v' && allignment2 == 'v') {
        if (sy2 == sy1) {
            if (ship_length == SHIP3) {
                if (ex1 < sx2) {
                    overlap = false;
                } else if (sx1 > ex2) {
                    overlap = false;
                } else {
                    overlap = true;
                }
            }
        }

    } else if (allignment1 == 'v' && allignment2 == 'h') {
        if (sx2 >= sx1 && sx2 <= ex1) {
            if (sy2 >= sy1 && ey2 <= sy1) {
                overlap = true;
            }
        }

    } else if (allignment1 == 'h' && allignment2 == 'v') {
        if (sy2 >= ey1 && sy2 <= sy1) {
            if (sx2 <= sx1 && ex2 >= sx1) {
                overlap = true;
            }
        }

    } else if (allignment1 == 'h' && allignment2 == 'h') {
        if (sx2 == sx1) {
            if (ship_length == SHIP5) {
                overlap = true;
            } else if (ey2 > sy1) {
                overlap = false;
            } else if (sy2 < ey1) {
                overlap = false;
            } else {
                overlap = true;
            }
        }

    }

    return overlap;
}

void placement(void)
{
    //main routine for ship placement, alters the myboard matrix
    Ship ship2 = {0, 0, 0, 0, 0};
    Ship ship3 = {0, 0, 0, 0, 0};
    Ship ship4 = {0, 0, 0, 0, 0};
    Ship ship5 = {0, 0, 0, 0, 0};
    
    int xstart = 0;
    int ystart = 0;

    int xend = 0;
    int yend = 0;

    int ship = 0;
    int tick = 0;

    bool placed = false;
    bool finished = false;

    led_set (LED1, 1);

    while (finished == false) {
        for (ship = SMALLEST_SHIP; ship <= LARGEST_SHIP; ship++) {
            //Place all four ships (of lengths 2, 3, 4, and 5)
            placed = false;
            xstart = 0;
            ystart = 0;
            yend = 0;
            xend = 0;
            xend = xend + ship - 1;
            tinygl_draw_line (tinygl_point (xstart, ystart), tinygl_point (xend, yend), 1);

            while (placed == false) {
                pacer_wait ();
                if (tick >= TICK_CYCLE) {
                    tick = 0;
                }
                tick++;
                navswitch_update ();
                display_placed(ship2, ship3, ship4, ship);
                tinygl_draw_line (tinygl_point (xstart, ystart), tinygl_point (xend, yend), tick < HALF_CYCLE);

                tinygl_update ();
                button_update ();

                if (navswitch_push_event_p (NAVSWITCH_NORTH))
                    //move the ship north
                {
                    if (ystart > 0 && yend <= MAX_Y_COORD) {
                        tinygl_draw_line (tinygl_point (xstart, ystart), tinygl_point (xend, yend), 0);
                        if ((check_bounds(xstart, ystart - 1) == true) && (check_bounds(xend, yend - 1) == true)) {
                            ystart--;
                            yend--;
                        }
                        tinygl_draw_line (tinygl_point (xstart, ystart), tinygl_point (xend, yend), 1);
                    }

                } else if (navswitch_push_event_p (NAVSWITCH_EAST))
                    //move the ship east
                {
                    if (xstart >= 0 && xend < MAX_X_COORD) {
                        tinygl_draw_line (tinygl_point (xstart, ystart), tinygl_point (xend, yend), 0);
                        if ((check_bounds(xstart + 1, ystart) == true) && (check_bounds(xend + 1, yend) == true)) {
                            xstart++;
                            xend++;
                        }
                        tinygl_draw_line (tinygl_point (xstart, ystart), tinygl_point (xend, yend), 1);
                    }

                } else if (navswitch_push_event_p (NAVSWITCH_SOUTH))
                    //move the ship south
                {
                    if (ystart >= 0 && yend < MAX_Y_COORD) {
                        tinygl_draw_line (tinygl_point (xstart, ystart), tinygl_point (xend, yend), 0);
                        if ((check_bounds(xstart, ystart + 1) == true) && (check_bounds(xend, yend + 1) == true)) {
                            ystart++;
                            yend++;
                        }
                        tinygl_draw_line (tinygl_point (xstart, ystart), tinygl_point (xend, yend), 1);
                    }

                } else if (navswitch_push_event_p (NAVSWITCH_WEST))
                    //move the ship west
                {
                    if (xstart > 0 && xend <= MAX_X_COORD) {
                        tinygl_draw_line (tinygl_point (xstart, ystart), tinygl_point (xend, yend), 0);
                        if ((check_bounds(xstart - 1, ystart) == true) && (check_bounds(xend - 1, yend) == true)) {
                            xstart--;
                            xend--;
                        }
                        tinygl_draw_line (tinygl_point (xstart, ystart), tinygl_point (xend, yend), 1);
                    }
                    
                } else if (navswitch_push_event_p (NAVSWITCH_PUSH))
                    //rotate the ship
                {
                    char allignment = 0;
                    int new_xstart = 0;
                    int new_ystart = 0;
                    int new_xend = 0;
                    int new_yend = 0;

                    tinygl_draw_line (tinygl_point (xstart, ystart), tinygl_point (xend, yend), 0);

                    if (xstart == xend) {
                        //ship is horizontal
                        allignment = 'h';
                    } else {
                        //ship is vertical
                        allignment = 'v';
                    }

                    if (allignment == 'v') {
                        //if ship is vertical rotate to horizontal
                        if (ship == SHIP2) {
                            new_xstart = xend;
                            new_ystart = ystart;
                            new_xend = xend;
                            new_yend = yend - 1;

                        } else if (ship == SHIP3) {
                            new_xstart = xstart + 1;
                            new_ystart = ystart + 1;
                            new_xend = xend - 1;
                            new_yend = yend - 1;

                        } else if (ship == SHIP4) {
                            new_xstart = xstart + 2;
                            new_ystart = ystart + 1;
                            new_xend = xend - 1;
                            new_yend = yend - 2;

                        } else if (ship == SHIP5) {
                            new_xstart = xstart + 2;
                            new_ystart = ystart + 2;
                            new_xend = xend - 2;
                            new_yend = yend - 2;
                        }

                    } else if (allignment == 'h') {
                        //if ship is horizontal rotate to vertical
                        if (ship == SHIP2) {
                            new_xstart = xstart - 1;
                            new_ystart = ystart;
                            new_xend = xend;
                            new_yend = ystart;

                        } else if (ship == SHIP3) {
                            new_xstart = xstart - 1;
                            new_ystart = ystart - 1;
                            new_xend = xend + 1;
                            new_yend = yend + 1;

                        } else if (ship == SHIP4) {
                            new_xstart = xstart - 2;
                            new_ystart = ystart - 1;
                            new_xend = xend + 1;
                            new_yend = yend + 2;

                        } else if (ship == SHIP5) {
                            new_xstart = xstart - 2;
                            new_ystart = ystart - 2;
                            new_xend = xend + 2;
                            new_yend = yend + 2;
                        }
                    }

                    if ((check_bounds(new_xstart, new_ystart) == true) && (check_bounds(new_xend, new_yend) == true)) {
                        //check if rotation is within LED array, if not, leave co-ords the same
                        xstart = new_xstart;
                        ystart = new_ystart;
                        xend = new_xend;
                        yend = new_yend;
                    }
                    //refresh board
                    tinygl_draw_line (tinygl_point (xstart, ystart), tinygl_point (xend, yend), 1);
                    tinygl_update ();
                
                } else if (button_push_event_p (0))
                    //confirm ship placement
                {
                    if (ship == SHIP2) {
                        ship2.length = ship;
                        ship2.xstart = xstart;
                        ship2.ystart = ystart;
                        ship2.xend = xend;
                        ship2.yend = yend;
                        placed = true;


                    } else if (ship == SHIP3) {
                        if (check_overlap(ship2, xstart, ystart, xend, yend) == false) {
                            ship3.length = ship;
                            ship3.xstart = xstart;
                            ship3.ystart = ystart;
                            ship3.xend = xend;
                            ship3.yend = yend;
                            placed = true;
                        }

                    } else if (ship == SHIP4) {
                        if ((check_overlap(ship2, xstart, ystart, xend, yend) == false) &&
                                (check_overlap(ship3, xstart, ystart, xend, yend) == false)) {
                            ship4.length = ship;
                            ship4.xstart = xstart;
                            ship4.ystart = ystart;
                            ship4.xend = xend;
                            ship4.yend = yend;
                            placed = true;
                        }

                    } else if (ship == SHIP5) {
                        if ((check_overlap(ship2, xstart, ystart, xend, yend) == false) &&
                                (check_overlap(ship3, xstart, ystart, xend, yend) == false) &&
                                (check_overlap(ship4, xstart, ystart, xend, yend) == false)) {
                            ship5.length = ship;
                            ship5.xstart = xstart;
                            ship5.ystart = ystart;
                            ship5.xend = xend;
                            ship5.yend = yend;
                            placed = true;
                            finished = true;
                        }

                    }

                }

            }

        }

    }
    //translate ship co-ords to the myboard matrix to be sent over IR

    int x = 0;
    int y = 0;

    if (ship2.xstart == ship2.xend) {
        x = ship2.xstart;
        for (y = ship2.ystart; y >= ship2.yend; y--) {
            myboard[x][y] = 1;
        }

    } else {
        y = ship2.ystart;
        for (x = ship2.xstart; x <= ship2.xend; x++) {
            myboard[x][y] = 1;
        }
    }

    if (ship3.xstart == ship3.xend) {
        x = ship3.xstart;
        for (y = ship3.ystart; y >= ship3.yend; y--) {
            myboard[x][y] = 1;
        }

    } else {
        y = ship3.ystart;
        for (x = ship3.xstart; x <= ship3.xend; x++) {
            myboard[x][y] = 1;
        }
    }
    
    if (ship4.xstart == ship4.xend) {
        x = ship4.xstart;
        for (y = ship4.ystart; y >= ship4.yend; y--) {
            myboard[x][y] = 1;
        }

    } else {
        y = ship4.ystart;
        for (x = ship4.xstart; x <= ship4.xend; x++) {
            myboard[x][y] = 1;
        }
    }

    if (ship5.xstart == ship5.xend) {
        x = ship5.xstart;
        for (y = ship5.ystart; y >= ship5.yend; y--) {
            myboard[x][y] = 1;
        }

    } else {
        y = ship5.ystart;
        for (x = ship5.xstart; x <= ship5.xend; x++) {
            myboard[x][y] = 1;
        }
    }

    tinygl_clear ();
    tinygl_update ();

}

void playing(void)
{
    //main routine for the playing of the game
    bool gameover = false;
    bool myturn = false;
    bool waiting = true;
    bool selected = false;

    int num_hits = 0; //win when = 14 (MAX_HITS)
    int xfire = 0;
    int yfire = 0;
    int x = X_CENTER;
    int y = Y_CENTER;
    int tick = 0;

    while (gameover == false) {
        tinygl_clear ();

        if (myturn == false) {
            //it is the other players turn
            waiting = true;

            while (waiting == true) {
                //other person is firing so wait
                pacer_wait ();
                led_set (LED1, 1);
                if (ir_uart_getc() == CHANGE_TURN_MSG) {
                    //recvd signal to change turn
                    waiting = false;
                    myturn = true;
                } else if (ir_uart_getc() == GAMEOVER_MSG) {
                    //recvd signal that other player has won
                    display_win_lose(0);
                }
            }

        } else if (myturn == true) {
            //it is my turn to fire
            led_set (LED1, 1);
            selected = false;
            x = X_CENTER;
            y = Y_CENTER;
            tick = 0;

            while (selected == false) {
                //selecting a position to fire on
                pacer_wait ();
                if (tick >= TICK_CYCLE) {
                    tick = 0;
                }
                tick++;
                led_set (LED1, tick > LED_TICK_START);
                draw_visible(visible, tick < FIFTH_CYCLE);
                navswitch_update ();
                tinygl_draw_point(tinygl_point (x, y), tick % 6 < 2); //flash when tick % 6 < 2
                tinygl_update ();

                if (navswitch_push_event_p (NAVSWITCH_NORTH))
                    //move the aimer reticle north
                {
                    if (check_bounds(x, y-1) == true) {
                        if (visible[x][y] != 2) {
                            tinygl_draw_point(tinygl_point (x, y), 0);
                        }
                        y--;
                    }
                } else if (navswitch_push_event_p (NAVSWITCH_EAST))
                    //move the aimer reticle east
                {
                    if (check_bounds(x+1, y) == true) {
                        if (visible[x][y] != 2) {
                            tinygl_draw_point(tinygl_point (x, y), 0);
                        }
                        x++;
                    }

                } else if (navswitch_push_event_p (NAVSWITCH_SOUTH))
                    //move the aimer reticle south
                {
                    if (check_bounds(x, y+1) == true) {
                        if (visible[x][y] != 2) {
                            tinygl_draw_point(tinygl_point (x, y), 0);
                        }
                        y++;
                    }
                } else if (navswitch_push_event_p (NAVSWITCH_WEST))
                    //move the aimer reticle west
                {
                    if (check_bounds(x-1, y) == true) {
                        if (visible[x][y] != 2) {
                            tinygl_draw_point(tinygl_point (x, y), 0);
                        }
                        x--;
                    }
                } else if (navswitch_push_event_p (NAVSWITCH_PUSH)) {
                    //confirm that position is to be fired on
                    xfire = x;
                    yfire = y;
                    selected = true;
                }

            }

            if (theirboard[xfire][yfire] == 1) {
                //you have hit a ship
                visible[xfire][yfire] = 2;
                num_hits++;
                display_hit_miss(1, xfire, yfire);
            } else {
                //you have missed a ship
                visible[xfire][yfire] = 1;
                display_hit_miss(0, xfire, yfire);
            }

            if (num_hits == MAX_HITS) {
                //you have won, broadcast message to other player
                gameover = true;
                tick = 0;
                while (tick < FIFTH_CYCLE) {
                    tick++;
                    ir_uart_putc(GAMEOVER_MSG);
                }
                display_win_lose(1);
            } else {
                //no-one has won, game still going, switch turns
                tick = 0;
                while (tick < FIFTH_CYCLE) {
                    tick++;
                    ir_uart_putc(CHANGE_TURN_MSG);
                }
                myturn = false;
            }

        }

    }

}

void receive_board(void)
{
    //receive your board from the other player
    int x = 0;
    int y = 0;
    led_set (LED1, 0);
    for (x = 0; x < MAX_X_COORD; x++) {
        for (y = 0; y < NUM_COLS; y++) {
            if (ir_uart_read_ready_p()) {
                theirboard[x][y]= ir_uart_getc();
            }
        }
    }
}

void send_board(void)
{
    //send your board to the other player
    int x = 0;
    int y = 0;
    led_set (LED1, 1);
    for (x = 0; x < MAX_X_COORD; x++) {
        for (y = 0; y < NUM_COLS; y++) {
            if (ir_uart_write_ready_p()) {
                ir_uart_putc(myboard[x][y]);
            }
        }
    }
}

void send_ready_signal(void)
{
    //send ready signal after ships have been placed
    bool ready = false;
    int received;
    while(ready == false) {
        ir_uart_putc (CHANGE_TURN_MSG);
        received = ir_uart_getc();
        if (received == CHANGE_TURN_MSG) {
            pacer_wait();
            ready = true;
        }
    }
}

int main (void)
{
    //initialise drivers/pacer
    system_init ();
    navswitch_init ();
    led_init ();
    button_init();
    pacer_init (PACER_RATE);
    tinygl_init (PACER_RATE);
    ir_uart_init();
    
    //execute game routine
    placement();
    send_ready_signal();
    send_board();
    receive_board();
    playing();
    
    return 0;
}


