#include "../include/input.h"

uint8_t joy = 0;
uint8_t old_joy = 0;

void process_title_input()
{
    if (INPUT_KEYPRESS(J_START))
    {
        nGameState = GS_INGAME;
        gotoxy(7u, 5u);
        printf("       ");
        gotoxy(5u, 8u);
        printf("         ");
    }
}

void process_game_input()
{
    if (INPUT_KEYPRESS(J_START))
    {
        nGameState = GS_PAUSE;
        return;
    }

    if (INPUT_KEY(J_LEFT))
    {
        player.facing = 1;
        if (player.grounded)
        {
            player.xSpd -= X_GROUND_ACCELERATION_IN_SUBPIXELS;
            if (player.xSpd < -MAX_X_GROUND_SPEED_IN_SUBPIXELS) player.xSpd = -MAX_X_GROUND_SPEED_IN_SUBPIXELS;
        } 
        else
        {
            player.xSpd -= X_ACCELERATION_IN_SUBPIXELS;
            if (player.xSpd < -MAX_X_SPEED_IN_SUBPIXELS) player.xSpd = -MAX_X_SPEED_IN_SUBPIXELS;
        }
    }
    else if (INPUT_KEY(J_RIGHT))
    {
        player.facing = 0;
        if (player.grounded)
        {
            player.xSpd += X_GROUND_ACCELERATION_IN_SUBPIXELS;
            if (player.xSpd > MAX_X_GROUND_SPEED_IN_SUBPIXELS) player.xSpd = MAX_X_GROUND_SPEED_IN_SUBPIXELS;
        } 
        else
        {
            player.xSpd += X_ACCELERATION_IN_SUBPIXELS;
            if (player.xSpd > MAX_X_SPEED_IN_SUBPIXELS) player.xSpd = MAX_X_SPEED_IN_SUBPIXELS;
        }
    }

    // check button keypress
    if (player.grounded && INPUT_KEYPRESS(J_A))
    {
        player.ySpd = -JUMP_ACCELERATION_IN_SUBPIXELS;
    }
    else if (player.ySpd < 0 && !player.grounded && INPUT_KEYRELEASE(J_A))
    {
        player.ySpd -= player.ySpd >> 1;
    }
}

void process_pause_input()
{
    if (INPUT_KEYPRESS(J_START))
    {
        nGameState = GS_INGAME;
        gotoxy(7u, 5u);
        printf("       ");
    }
    else
    {
        gotoxy(7u, 5u);
        printf("PAUSE");
    }
}