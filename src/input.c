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
                player.angularVel -= (((MAX_HOOK_LENGTH - player.hookLength) >> 2) + INPUT_ANGULAR_ACC);
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
                player.angularVel += (((MAX_HOOK_LENGTH - player.hookLength) >> 2) + INPUT_ANGULAR_ACC);
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

    if (INPUT_KEYPRESS(J_A))
    {
        if (player.grounded || player.fallDelay > 0)
        {
            player.ySpd = -JUMP_ACCELERATION_IN_SUBPIXELS;
            player.fallDelay = 0;
            if (player.animIndex != AIR_IDLE_ANIM_INDEX)
            {
                player.animIndex = AIR_IDLE_ANIM_INDEX;
                player.numAnimFrames = AIR_IDLE_ANIM_FRAMES;
                player.animSpeed = AIR_IDLE_ANIM_SPEED;
                player.animFrame = 0;
            }
        }
    }
    else if (player.ySpd < 0 && !player.grounded && INPUT_KEYRELEASE(J_A))
    {
        player.ySpd -= player.ySpd >> 1;
    }

    if (INPUT_KEY(J_UP))
    {
        if (player.hookState == HS_ATTACHED && player.hookLength > MIN_HOOK_LENGTH && (player.hookAngle > (255 - MAX_ADJUST_ANGLE) || player.hookAngle < MAX_ADJUST_ANGLE))
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
        else if (player.grounded && player.xSpd == 0)
        {
            if (player.animIndex != LOOK_ANIM_INDEX)
            {
                player.animIndex = LOOK_ANIM_INDEX;
                player.numAnimFrames = LOOK_ANIM_FRAMES;
                player.animSpeed = LOOK_ANIM_SPEED;
                player.animFrame = 0;
                player.lookState = LS_UP;
            }
        }
    }
    else if (INPUT_KEY(J_DOWN))
    {
        if (player.hookState == HS_ATTACHED && player.hookLength < MAX_HOOK_LENGTH && (player.hookAngle > (255 - MAX_ADJUST_ANGLE) || player.hookAngle < MAX_ADJUST_ANGLE))
        {
            player.oldHookLength = player.hookLength;
            player.hookLength += 1;
            player.hookSegments = player.hookLength >> 3;
        }
        else if (player.grounded && player.xSpd == 0)
        {
            if (player.animIndex != CROUCH_ANIM_INDEX)
            {
                player.animIndex = CROUCH_ANIM_INDEX;
                player.numAnimFrames = CROUCH_ANIM_FRAMES;
                player.animSpeed = CROUCH_ANIM_SPEED;
                player.animFrame = 0;
                player.lookState = LS_DOWN;
            }
        }
    }
    else
    {
        player.lookState = LS_NONE;
    }

    if (INPUT_KEYRELEASE(J_UP))
    {
        if (player.animIndex == LOOK_ANIM_INDEX)
        {
            player.animIndex = GROUND_IDLE_ANIM_INDEX;
            player.numAnimFrames = GROUND_IDLE_ANIM_FRAMES;
            player.animSpeed = GROUND_IDLE_ANIM_SPEED;
            player.animFrame = 0;
            player.lookState = LS_NONE;
        }
    }

    if (INPUT_KEYRELEASE(J_DOWN))
    {
        if (player.animIndex == CROUCH_ANIM_INDEX)
        {
            player.animIndex = GROUND_IDLE_ANIM_INDEX;
            player.numAnimFrames = GROUND_IDLE_ANIM_FRAMES;
            player.animSpeed = GROUND_IDLE_ANIM_SPEED;
            player.animFrame = 0;
            player.lookState = LS_NONE;
        }
    }


    if (INPUT_KEYPRESS(J_B))
    {
        player.fallDelay = 0;
        if (player.hookState != HS_ATTACHED)
        {
            int16_t xCheck = PIXELS_TO_SUBPIXELS(4);
            int16_t yCheck = PIXELS_TO_SUBPIXELS(-4);
            uint16_t maxDist = MAX_HOOK_DISTANCE;
            player.hookAngle = ANGLE_315DEG;

            if (player.lookState == LS_UP)
            {
                xCheck = 0;
                yCheck = PIXELS_TO_SUBPIXELS(-8);
                maxDist = MAX_STRAIGHT_HOOK_DISTANCE;
                player.hookAngle = ANGLE_0DEG;
            }
            else if (player.facing)
            {
                xCheck = PIXELS_TO_SUBPIXELS(-4);
                player.hookAngle = ANGLE_45DEG;
            }

            uint8_t col_flags = 0;
            int16_t xTmp = 0;//PIXELS_TO_SUBPIXELS(4);
            int16_t yTmp = 0;
            while (!col_flags && ((abs16(xTmp) + abs16(yTmp)) < maxDist))
            {
                xTmp += xCheck;
                yTmp += yCheck;
                col_flags = check_tilemap_collision(player.x + xTmp, player.y + yTmp);
            }

            if ((abs16(xTmp) + abs16(yTmp)) >= maxDist || (abs16(xTmp) + abs16(yTmp)) <= MIN_HOOK_DISTANCE)
            {
                player.hookState = HS_LAUNCHED;
                player.hookLength = 64;
                player.oldHookLength = player.hookLength;
                player.hookSegments = 8;
            }
            else
            {
                player.hookX = player.x + xTmp;
                player.hookY = player.y + yTmp;
                uint16_t xSqr = (SUBPIXELS_TO_PIXELS(abs_sub(player.x, player.hookX)) * SUBPIXELS_TO_PIXELS(abs_sub(player.x, player.hookX)));
                uint16_t ySqr = (SUBPIXELS_TO_PIXELS(abs_sub(player.y, player.hookY)) * SUBPIXELS_TO_PIXELS(abs_sub(player.y, player.hookY)));
                player.hookState = HS_ATTACHED;
                player.hookLength = isqrt(xSqr + ySqr);
                player.oldHookLength = player.hookLength;
                player.hookSegments = player.hookLength >> 3;
                player.angularVel = player.xSpd >> 2;
                player.xSpd = 0;
            }

            if (player.animIndex != AIR_IDLE_ANIM_INDEX)
            {
                player.animIndex = AIR_IDLE_ANIM_INDEX;
                player.numAnimFrames = AIR_IDLE_ANIM_FRAMES;
                player.animSpeed = AIR_IDLE_ANIM_SPEED;
                player.animFrame = 0;
            }

            player.grounded = 0;
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
