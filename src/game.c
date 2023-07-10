#include "../include/game.h"

uint16_t nGameState = GS_TITLE_LOAD;

void game_loop()
{
    // Loop forever
    while(TRUE)
    {
        // Get new inputs
        old_joy = joy;
        joy = joypad();

        switch(nGameState)
        {
            case GS_TITLE_LOAD:
                gotoxy(7u, 5u);
                printf("UNKNOWN");
                gotoxy(5u, 8u);
                printf("PRESS START");
                nGameState = GS_TITLE;

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

        // debug
        printf("        ");
        gotoxy(1u, 1u);
        printf("%d %d", player.xSpd, player.ySpd);

		// Done processing, yield CPU and wait for start of next frame
        wait_vbl_done();
    }
}