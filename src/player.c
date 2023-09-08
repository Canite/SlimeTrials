#include "../include/player.h"

//INCBIN(slime, "res/slime.2bpp")
//INCBIN(hook, "res/hook.2bpp")

struct Player player = {0};

void init_player(uint8_t initSprites, uint16_t startX, uint16_t startY) NONBANKED
{
    if (initSprites)
    {
        set_sprite_data(PLAYER_SPRITE_TILE_INDEX, slime_TILE_COUNT, slime_tiles);
        set_sprite_data(HOOK_SPRITE_TILE_INDEX, hook_TILE_COUNT, hook_tiles);
        set_sprite_tile(PLAYER_SPRITE_INDEX, GROUND_IDLE_ANIM_INDEX);
    }

    hide_hook();

    player.x = player.oldX = PIXELS_TO_SUBPIXELS(startX);
    player.y = player.oldY = PIXELS_TO_SUBPIXELS(startY);
    player.xSpd = player.ySpd = 0;
    player.w = 8;
    player.h = 8;
    player.flags = 0;
    player.colFlags = 0;
    player.angularAcc = 0;
    player.angularVel = 0;
    player.facing = 0;
    player.lookState = LS_NONE;
    player.animIndex = GROUND_IDLE_ANIM_INDEX;
    player.animFrame = 0;
    player.numAnimFrames = GROUND_IDLE_ANIM_FRAMES;
    player.animSpeed = GROUND_IDLE_ANIM_SPEED;
    player.grounded = 0;
    player.fallDelay = 0;
    player.iFrames = 0;
    player.hookState = HS_STOWED;
    player.hookX = 0;
    player.hookY = 0;
    player.hookAngle = 0;
    player.hookLength = 0;
    player.oldHookLength = 0;
    player.hookSegments = 0;
}

void handle_input(void)
{

}
