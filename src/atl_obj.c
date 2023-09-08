#include "../include/atl_obj.h"

struct Atl atl = {0};

void init_atl(uint8_t initSprites)
{
    if (initSprites)
    {
        uint8_t currentBank = CURRENT_BANK;

        SWITCH_ROM(BANK(atl));
        set_sprite_data(ATL_SPRITE_TILE_INDEX, atl_TILE_COUNT, atl_tiles);
        set_sprite_tile(ATL_SPRITE_INDEX, ATL_SPRITE_TILE_INDEX);

        SWITCH_ROM(currentBank);
    }

    atl.x = 0;
    atl.y = 0;
    atl.w = 0;
    atl.h = 0;
    atl.xSpd = 0;
    atl.ySpd = 0;

    hide_sprite(ATL_SPRITE_INDEX);
}
