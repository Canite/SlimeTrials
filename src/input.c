#include "../include/input.h"

uint8_t joy = 0;
uint8_t old_joy = 0;

void process_title_input()
{
    if (INPUT_KEYPRESS(J_START))
    {
        game.gameState = GS_INGAME;
    }
}

void process_game_input()
{
    if (INPUT_KEYPRESS(J_START))
    {
        game.gameState = GS_PAUSE;
        return;
    }

    if (INPUT_KEY(J_LEFT))
    {
        player.facing = 1;
        if (player.grounded)
        {
            if (player.animIndex != GROUND_MOVE_ANIM_INDEX)
            {
                player.animIndex = GROUND_MOVE_ANIM_INDEX;
                player.numAnimFrames = GROUND_MOVE_ANIM_FRAMES;
                player.animSpeed = GROUND_MOVE_ANIM_SPEED;
                player.animFrame = 0;
            }
            player.xSpd -= X_GROUND_ACCELERATION_IN_SUBPIXELS;
            if (player.xSpd < -MAX_X_GROUND_SPEED_IN_SUBPIXELS) player.xSpd = -MAX_X_GROUND_SPEED_IN_SUBPIXELS;
        } 
        else if (player.hookState == HS_ATTACHED)
        {
            if (player.angularVel > -MAX_ANGULAR_VELOCITY)
            {
                player.angularVel -= (((MAX_HOOK_LENGTH - player.hookLength) >> 4) + INPUT_ANGULAR_ACC);
            }
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
            if (player.animIndex != GROUND_MOVE_ANIM_INDEX)
            {
                player.animIndex = GROUND_MOVE_ANIM_INDEX;
                player.numAnimFrames = GROUND_MOVE_ANIM_FRAMES;
                player.animSpeed = GROUND_MOVE_ANIM_SPEED;
                player.animFrame = 0;
            }
            player.xSpd += X_GROUND_ACCELERATION_IN_SUBPIXELS;
            if (player.xSpd > MAX_X_GROUND_SPEED_IN_SUBPIXELS) player.xSpd = MAX_X_GROUND_SPEED_IN_SUBPIXELS;
        } 
        else if (player.hookState == HS_ATTACHED)
        {
            if (player.angularVel < MAX_ANGULAR_VELOCITY)
            {
                player.angularVel += (((MAX_HOOK_LENGTH - player.hookLength) >> 4) + INPUT_ANGULAR_ACC);
            }
        }
        else
        {
            player.xSpd += X_ACCELERATION_IN_SUBPIXELS;
            if (player.xSpd > MAX_X_SPEED_IN_SUBPIXELS) player.xSpd = MAX_X_SPEED_IN_SUBPIXELS;
        }
    }
    else
    {
        // Idle
        if (player.grounded)
        {
            if (player.animIndex != GROUND_IDLE_ANIM_INDEX)
            {
                player.animIndex = GROUND_IDLE_ANIM_INDEX;
                player.numAnimFrames = GROUND_IDLE_ANIM_FRAMES;
                player.animSpeed = GROUND_IDLE_ANIM_SPEED;
                player.animFrame = 0;
            }
        }
    }

    // check button keypress
    if (player.grounded && INPUT_KEYPRESS(J_A))
    {
        player.ySpd = -JUMP_ACCELERATION_IN_SUBPIXELS;
        if (player.animIndex != AIR_IDLE_ANIM_INDEX)
        {
            player.animIndex = AIR_IDLE_ANIM_INDEX;
            player.numAnimFrames = AIR_IDLE_ANIM_FRAMES;
            player.animSpeed = AIR_IDLE_ANIM_SPEED;
            player.animFrame = 0;
        }
    }
    else if (player.ySpd < 0 && !player.grounded && INPUT_KEYRELEASE(J_A))
    {
        player.ySpd -= player.ySpd >> 1;
    }

    if (INPUT_KEY(J_UP))
    {
        if (player.hookState == HS_ATTACHED && player.hookLength > MIN_HOOK_LENGTH && (player.hookAngle > 243 || player.hookAngle < 12))
        {
            player.oldHookLength = player.hookLength;
            player.hookLength -= 1;
            uint8_t old_segments = player.hookSegments;
            player.hookSegments = player.hookLength >> 3;
            if (old_segments != player.hookSegments)
            {
                hide_sprite(HOOK_SPRITE_INDEX + player.hookSegments);
            }
        }
    }
    else if (INPUT_KEY(J_DOWN))
    {
        if (player.hookState == HS_ATTACHED && player.hookLength < MAX_HOOK_LENGTH && (player.hookAngle > 243 || player.hookAngle < 12))
        {
            player.oldHookLength = player.hookLength;
            player.hookLength += 1;
            player.hookSegments = player.hookLength >> 3;
        }
    }

    if (INPUT_KEYPRESS(J_B))
    {
        if (player.hookState != HS_ATTACHED)
        {
            //player.hookState = HS_LAUNCHED;
            if (player.facing)
            {
                player.hookX = player.x - PIXELS_TO_SUBPIXELS(50);
                player.hookAngle = ANGLE_45DEG;
            }
            else
            {
                player.hookX = player.x + PIXELS_TO_SUBPIXELS(50);
                player.hookAngle = ANGLE_315DEG;
            }
            player.hookY = player.y - PIXELS_TO_SUBPIXELS(50);
            uint16_t xSqr = (SUBPIXELS_TO_PIXELS(abs_sub(player.x, player.hookX)) * SUBPIXELS_TO_PIXELS(abs_sub(player.x, player.hookX)));
            uint16_t ySqr = (SUBPIXELS_TO_PIXELS(abs_sub(player.y, player.hookY)) * SUBPIXELS_TO_PIXELS(abs_sub(player.y, player.hookY)));

            if (xSqr + ySqr < MAX_HOOK_DISTANCE)
            {
                player.hookState = HS_ATTACHED;
                player.hookLength = isqrt(xSqr + ySqr);
                player.oldHookLength = player.hookLength;
                player.hookSegments = player.hookLength >> 3;
                player.angularVel = player.xSpd >> 2;
                player.xSpd = 0;
            }
            else
            {
                player.hookState = HS_LAUNCHED;
                player.hookLength = 64;
                player.oldHookLength = player.hookLength;
                player.hookSegments = 8;
            }

            if (player.animIndex != AIR_IDLE_ANIM_INDEX)
            {
                player.animIndex = AIR_IDLE_ANIM_INDEX;
                player.numAnimFrames = AIR_IDLE_ANIM_FRAMES;
                player.animSpeed = AIR_IDLE_ANIM_SPEED;
                player.animFrame = 0;
            }
        }
    }
    else if (INPUT_KEYRELEASE(J_B))
    {
        if (player.hookState == HS_ATTACHED)
        {
            player.hookState = HS_STOWED;
            player.xSpd = (((player.angularVel)) * COS(player.hookAngle)) >> 7;
            player.xSpd += sign(player.xSpd) << 1;
            player.ySpd = (((player.angularVel)) * -SIN(player.hookAngle)) >> 7;
            player.ySpd += sign(player.ySpd) << 1;
        }
        else if (player.hookState == HS_LAUNCHED)
        {
            player.hookState = HS_STOWED;
        }
    }
}

void process_pause_input()
{
    if (INPUT_KEYPRESS(J_START))
    {
        game.gameState = GS_INGAME;
    }
    else
    {
    }
}
