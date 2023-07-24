#include "../include/levels.h"

INCBIN(background_tiles, "res/background_tiles.2bpp")
INCBIN(title, "res/title.2bpp")

void init_title()
{
    set_bkg_data(0, 12, title);
    set_bkg_submap(0, 0, 20, 18, title_screen_tiles, title_screen_tile_width);
}

void init_level(uint8_t level)
{
    uint8_t initPlayerSprites = (game.currentLevel == 0);
    if (level == 1)
    {
        set_bkg_data(0, 5, background_tiles);
        set_bkg_submap(0, 0, 20, 18, level1_tiles, level1_tile_width);
        game.tileMap = level1_tiles;
        game.tileMapW = level1_tile_width;
        game.tileMapH = level1_tile_height;

        init_player(initPlayerSprites, level1_spawn_x, level1_spawn_y);
    }
}
