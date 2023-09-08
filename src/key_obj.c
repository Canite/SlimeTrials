#include "../include/key_obj.h"

struct Key key = {0};

void init_key(uint8_t initSprites)
{
    if (initSprites)
    {
        uint8_t currentBank = CURRENT_BANK;

        SWITCH_ROM(BANK(key));
        set_sprite_data(KEY_SPRITE_TILE_INDEX, key_TILE_COUNT, key_tiles);
        set_sprite_tile(KEY_SPRITE_INDEX, KEY_SPRITE_TILE_INDEX);

        SWITCH_ROM(currentBank);
    }

    key.x = 0;
    key.y = 0;
    key.w = 0;
    key.h = 0;
    key.xSpd = 0;
    key.ySpd = 0;

    hide_sprite(KEY_SPRITE_INDEX);
}
