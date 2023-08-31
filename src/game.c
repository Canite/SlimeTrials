#include "../include/game.h"

struct Game game = {0};

void init_game(void)
{
    game.gameState = GS_TITLE_LOAD;
    game.gameFrame = 0;
    game.currentLevel = 0;
    game.level_data.tiles = 0;
    game.level_data.collisions = 0;
    game.level_data.tile_width = 0;
    game.level_data.tile_height = 0;
    game.mapPixelW = 0;
    game.mapPixelH = 0;
    game.mapX = 0;
    game.mapY = 0;
    game.oldMapX = 255;
    game.oldMapY = 255;
    game.flags = GF_NONE;
}

void game_loop(void)
{
    // Loop forever
    while(TRUE)
    {
        // Get new inputs
        old_joy = joy;
        joy = joypad();

        switch(game.gameState)
        {
            case GS_TITLE_LOAD:
                game.gameState = GS_TITLE;

            case GS_TITLE:
                process_title_input();

                break;
            case GS_INGAME:
                process_game_input();
                apply_physics();
                update_game_sprites();

                break;
            case GS_PAUSE:
                process_pause_input();

                break;
            default:
                break;
        }

        game.gameFrame += 1;

        // Done processing, yield CPU and wait for start of next frame
        vsync();

        // update camera after vblank
        update_camera();
    }
}

void start_level(uint8_t level)
{
    init_level(level);
    game.currentLevel = level;
    init_camera();
    camera.redraw = 1;
}
