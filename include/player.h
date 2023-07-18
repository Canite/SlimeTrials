#ifndef _PLAYER_H
#define _PLAYER_H

#include <gb/gb.h>
#include <gbdk/platform.h>
#include <gbdk/console.h>
#include <stdint.h>

#include "utils.h"

struct Player
{
    uint16_t x;
    uint16_t y;
    uint8_t w;
    uint8_t h;
    int16_t xSpd;
    int16_t ySpd;
    uint8_t colFlags;
    int16_t angularAcc;
    int16_t angularVel;
    uint8_t facing;
    uint8_t animIndex;
    uint8_t animFrame;
    uint8_t numAnimFrames;
    uint8_t animSpeed;
    uint8_t grounded;
    uint8_t hookState;
    uint16_t hookX;
    uint16_t hookY;
    uint8_t hookAngle;
    uint8_t hookLength;
    uint8_t oldHookLength;
    uint8_t hookSegments;
};

enum HookState
{
    HS_STOWED,
    HS_LAUNCHED,
    HS_ATTACHED
};

#define GROUND_IDLE_ANIM_INDEX  0
#define GROUND_IDLE_ANIM_FRAMES  1
#define GROUND_IDLE_ANIM_SPEED  1

#define AIR_IDLE_ANIM_INDEX  1
#define AIR_IDLE_ANIM_FRAMES  1
#define AIR_IDLE_ANIM_SPEED  1

#define GROUND_MOVE_ANIM_INDEX  2
#define GROUND_MOVE_ANIM_FRAMES  4
#define GROUND_MOVE_ANIM_SPEED  7

#define PLAYER_SPRITE_INDEX 0
#define PLAYER_SPRITE_TILE_INDEX 0
#define HOOK_SPRITE_INDEX 1
#define HOOK_SPRITE_TILE_INDEX 6

extern struct Player player;

void init_player();
void handle_input();

#endif
