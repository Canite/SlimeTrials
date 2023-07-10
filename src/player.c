#include "../include/player.h"

struct Player player = {0};

void init_player()
{
    set_sprite_tile(0, player.animFrame);
    set_sprite_tile(1, player.animFrame + 1);
    set_sprite_tile(2, player.animFrame + 2);
    set_sprite_tile(3, player.animFrame + 3);

    player.x = player.y = PIXELS_TO_SUBPIXELS(64);
    player.xSpd = player.ySpd = PIXELS_TO_SUBPIXELS(0);
}

void handle_input()
{

}