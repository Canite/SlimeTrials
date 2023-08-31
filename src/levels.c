#include "../include/levels.h"

const level_t levels[] = {
    {
        .tiles = level1_tiles,
        .collisions = level1_collisions,
        .tile_width = level1_tile_width,
        .tile_height = level1_tile_height,
        .spawn_x = level1_spawn_x,
        .spawn_y = level1_spawn_y,
        .door_open = level1_door_open,
    },
    {
        .tiles = level2_tiles,
        .collisions = level2_collisions,
        .tile_width = level2_tile_width,
        .tile_height = level2_tile_height,
        .spawn_x = level2_spawn_x,
        .spawn_y = level2_spawn_y,
        .door_open = level2_door_open,
    },
    {
        .tiles = level3_tiles,
        .collisions = level3_collisions,
        .tile_width = level3_tile_width,
        .tile_height = level3_tile_height,
        .spawn_x = level3_spawn_x,
        .spawn_y = level3_spawn_y,
        .door_open = level3_door_open,
    },
    {
        .tiles = level4_tiles,
        .collisions = level4_collisions,
        .tile_width = level4_tile_width,
        .tile_height = level4_tile_height,
        .spawn_x = level4_spawn_x,
        .spawn_y = level4_spawn_y,
        .door_open = level4_door_open,
    },
};

void init_title(void)
{
    set_bkg_data(0, title_TILE_COUNT, title_tiles);
    set_bkg_submap(0, 0, 20, 18, title_screen_tiles, title_screen_tile_width);
}

void init_level(uint8_t level)
{
    uint8_t initSprites = (game.currentLevel == 0);
    if (level > 0 && level <= NUM_LEVELS)
    {
        game.level_data = levels[level - 1];
        game.flags = GF_NONE;
        if (game.level_data.door_open == 1)
        {
            game.flags |= GF_DOOR_OPEN;
        }

        set_bkg_data(0, caverns_TILE_COUNT, caverns_tiles);
        set_bkg_submap(0, 0, 20, 18, game.level_data.tiles, game.level_data.tile_width);
        init_player(initSprites, game.level_data.spawn_x, game.level_data.spawn_y);
        init_key(initSprites);
    }
}
