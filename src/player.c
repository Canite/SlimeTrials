#include "../include/player.h"

struct Player player = {0};

void init_player()
{
    set_sprite_tile(0, player.animFrame);
    //set_sprite_tile(1, player.animFrame + 1);
    //set_sprite_tile(2, player.animFrame + 2);
    //set_sprite_tile(3, player.animFrame + 3);

    player.x = player.y = player.oldX = player.oldY = PIXELS_TO_SUBPIXELS(64);
    player.xSpd = player.ySpd = 0;
    player.w = 8;
    player.h = 8;
    player.colFlags = 0;
    player.angularAcc = 0;
    player.angularVel = 0;
    player.facing = 0;
    player.animIndex = 0;
    player.animFrame = 0;
    player.numAnimFrames = 1;
    player.animSpeed = 1;
    player.grounded = 0;
    player.hookState = HS_STOWED;
    player.hookX = 0;
    player.hookY = 0;
    player.hookAngle = 0;
    player.hookLength = 0;
    player.oldHookLength = 0;
    player.hookSegments = 0;
}

void handle_input()
{

}