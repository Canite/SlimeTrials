#include "../include/physics.h"

void apply_physics()
{
    player.x += player.xSpd;
    player.y += player.ySpd;

    // handle collision
    if (player.y > 2304) player.y = 2304;

    // decelerate Y and X
    if (player.y < 2304)
    {
        player.grounded = 0;
        player.ySpd += GRAVITY_CONST;
    }
    else
    {
        player.grounded = 1;
        player.ySpd = 0;
    }

    if (player.xSpd < X_DECELERATION_IN_SUBPIXELS && player.xSpd > -X_DECELERATION_IN_SUBPIXELS)
    {
        player.xSpd = 0;
    }
    else if (player.xSpd < 0)
    {
        player.xSpd += X_DECELERATION_IN_SUBPIXELS;
    }
    else if (player.xSpd)
    {
        player.xSpd -= X_DECELERATION_IN_SUBPIXELS; 
    }

}