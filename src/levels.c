#include "../include/levels.h"

const level_t levels[] = {
    {
        .tiles = level1_tiles,
        .collisions = level1_collisions,
        .tile_width = level1_tile_width,
        .tile_height = level1_tile_height,
        .spawn_x = level1_spawn_x,
        .spawn_y = level1_spawn_y,
    },
    {
        .tiles = level2_tiles,
        .collisions = level2_collisions,
        .tile_width = level2_tile_width,
        .tile_height = level2_tile_height,
        .spawn_x = level2_spawn_x,
        .spawn_y = level2_spawn_y,
    },
    {
        .tiles = level3_tiles,
        .collisions = level3_collisions,
        .tile_width = level3_tile_width,
        .tile_height = level3_tile_height,
        .spawn_x = level3_spawn_x,
        .spawn_y = level3_spawn_y,
    },
};

void init_title(void)
{
    set_bkg_data(0, title_TILE_COUNT, title_tiles);
    set_bkg_submap(0, 0, 20, 18, title_screen_tiles, title_screen_tile_width);
}

void init_level(uint8_t level)
{
    uint8_t initPlayerSprites = (game.currentLevel == 0);
    if (level > 0 && level <= NUM_LEVELS)
    {
        game.level_data = levels[level - 1];
        set_bkg_data(0, caverns_TILE_COUNT, caverns_tiles);
        set_bkg_submap(0, 0, 20, 18, game.level_data.tiles, game.level_data.tile_width);
        init_player(initPlayerSprites, game.level_data.spawn_x, game.level_data.spawn_y);
    }
}
