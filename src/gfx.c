#include "../include/gfx.h"

void update_game_sprites()
{
    set_sprite_tile(0, player.animFrame);
    set_sprite_tile(1, player.animFrame + 1);
    set_sprite_tile(2, player.animFrame + 2);
    set_sprite_tile(3, player.animFrame + 3);

    if (player.facing)
    {
        set_sprite_prop(0, 1 << 5);
        set_sprite_prop(1, 1 << 5);
        set_sprite_prop(2, 1 << 5);
        set_sprite_prop(3, 1 << 5);
        move_sprite(1, SUBPIXELS_TO_PIXELS(player.x), SUBPIXELS_TO_PIXELS(player.y));
        move_sprite(0, SUBPIXELS_TO_PIXELS(player.x) + 8, SUBPIXELS_TO_PIXELS(player.y));
        move_sprite(3, SUBPIXELS_TO_PIXELS(player.x), SUBPIXELS_TO_PIXELS(player.y) + 8);
        move_sprite(2, SUBPIXELS_TO_PIXELS(player.x) + 8, SUBPIXELS_TO_PIXELS(player.y) + 8);
    }
    else
    {
        set_sprite_prop(0, 0);
        set_sprite_prop(1, 0);
        set_sprite_prop(2, 0);
        set_sprite_prop(3, 0);
        move_sprite(0, SUBPIXELS_TO_PIXELS(player.x), SUBPIXELS_TO_PIXELS(player.y));
        move_sprite(1, SUBPIXELS_TO_PIXELS(player.x) + 8, SUBPIXELS_TO_PIXELS(player.y));
        move_sprite(2, SUBPIXELS_TO_PIXELS(player.x), SUBPIXELS_TO_PIXELS(player.y) + 8);
        move_sprite(3, SUBPIXELS_TO_PIXELS(player.x) + 8, SUBPIXELS_TO_PIXELS(player.y) + 8);
    }
}