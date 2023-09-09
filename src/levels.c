#include "../include/levels.h"

const level_t levels[] = {
    {
        .tiles = level1_tiles,
        .tiles_bank = BANK(level1_tiles),
        .collisions = level1_collisions,
        .collisions_bank = BANK(level1_collisions),
        .tile_width = level1_tile_width,
        .tile_height = level1_tile_height,
        .spawn_x = level1_spawn_x,
        .spawn_y = level1_spawn_y,
        .door_open = level1_door_open,
    },
    {
        .tiles = level2_tiles,
        .tiles_bank = BANK(level2_tiles),
        .collisions = level2_collisions,
        .collisions_bank = BANK(level2_collisions),
        .tile_width = level2_tile_width,
        .tile_height = level2_tile_height,
        .spawn_x = level2_spawn_x,
        .spawn_y = level2_spawn_y,
        .door_open = level2_door_open,
    },
    {
        .tiles = level3_tiles,
        .tiles_bank = BANK(level3_tiles),
        .collisions = level3_collisions,
        .collisions_bank = BANK(level3_collisions),
        .tile_width = level3_tile_width,
        .tile_height = level3_tile_height,
        .spawn_x = level3_spawn_x,
        .spawn_y = level3_spawn_y,
        .door_open = level3_door_open,
    },
    {
        .tiles = level4_tiles,
        .tiles_bank = BANK(level4_tiles),
        .collisions = level4_collisions,
        .collisions_bank = BANK(level4_collisions),
        .tile_width = level4_tile_width,
        .tile_height = level4_tile_height,
        .spawn_x = level4_spawn_x,
        .spawn_y = level4_spawn_y,
        .door_open = level4_door_open,
    },
    {
        .tiles = level5_tiles,
        .tiles_bank = BANK(level5_tiles),
        .collisions = level5_collisions,
        .collisions_bank = BANK(level5_collisions),
        .tile_width = level5_tile_width,
        .tile_height = level5_tile_height,
        .spawn_x = level5_spawn_x,
        .spawn_y = level5_spawn_y,
        .door_open = level5_door_open,
    },
    {
        .tiles = level6_tiles,
        .tiles_bank = BANK(level6_tiles),
        .collisions = level6_collisions,
        .collisions_bank = BANK(level6_collisions),
        .tile_width = level6_tile_width,
        .tile_height = level6_tile_height,
        .spawn_x = level6_spawn_x,
        .spawn_y = level6_spawn_y,
        .door_open = level6_door_open,
    },
    {
        .tiles = level7_tiles,
        .tiles_bank = BANK(level7_tiles),
        .collisions = level7_collisions,
        .collisions_bank = BANK(level7_collisions),
        .tile_width = level7_tile_width,
        .tile_height = level7_tile_height,
        .spawn_x = level7_spawn_x,
        .spawn_y = level7_spawn_y,
        .door_open = level7_door_open,
    },
};

void init_title(void)
{
    uint8_t currentBank = CURRENT_BANK;

    SWITCH_ROM(BANK(title));
    set_bkg_data(0, title_TILE_COUNT, title_tiles);
    SWITCH_ROM(BANK(title_screen_tiles));
    set_bkg_submap(0, 0, 20, 18, title_screen_tiles, title_screen_tile_width);

    SWITCH_ROM(currentBank);
}

void init_end_screen(void)
{
    uint8_t currentBank = CURRENT_BANK;

    SWITCH_ROM(BANK(end));
    set_bkg_data(0, end_TILE_COUNT, end_tiles);
    SWITCH_ROM(BANK(end_screen_tiles));
    set_bkg_submap(0, 0, 20, 18, end_screen_tiles, end_screen_tile_width);

    SWITCH_ROM(currentBank);
}

void init_level(uint8_t level) NONBANKED
{
    uint8_t initSprites = (gfx.sprites_inited == 0);
    if (level > 0 && level <= NUM_LEVELS)
    {
        game.level_data = levels[level - 1];
        game.flags = GF_NONE;
        game.levelFrame = 0;
        if (game.level_data.door_open == 1)
        {
            game.flags |= GF_DOOR_OPEN;
        }
        uint8_t currentBank = CURRENT_BANK;

        SWITCH_ROM(BANK(caverns));
        set_bkg_data(0, caverns_TILE_COUNT, caverns_tiles);

        SWITCH_ROM(BANK(font_bold));
        set_win_data(caverns_TILE_COUNT, font_bold_TILE_COUNT, font_bold_tiles);

        SWITCH_ROM(game.level_data.tiles_bank);
        set_bkg_submap(0, 0, 20, 18, game.level_data.tiles, game.level_data.tile_width);

        SWITCH_ROM(currentBank);

        init_player(initSprites, game.level_data.spawn_x, game.level_data.spawn_y);
        init_key(initSprites);
        init_atl(initSprites);
        gfx.sprites_inited = 1;
    }
}
