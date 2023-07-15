#include "../include/physics.h"

const int8_t sine_wave[256] = {
    0,3,6,9,12,16,19,22,25,28,31,34,37,40,43,46,49,51,54,57,60,
    63,65,68,71,73,76,78,81,83,85,88,90,92,94,96,98,100,102,104,
    106,107,109,111,112,113,115,116,117,118,120,121,122,122,123,
    124,125,125,126,126,126,127,127,127,127,127,127,127,126,126,
    126,125,125,124,123,122,122,121,120,118,117,116,115,113,112,
    111,109,107,106,104,102,100,98,96,94,92,90,88,85,83,81,78,76,
    73,71,68,65,63,60,57,54,51,49,46,43,40,37,34,31,28,25,22,19,
    16,12,9,6,3,0,-3,-6,-9,-12,-16,-19,-22,-25,-28,-31,-34,-37,
    -40,-43,-46,-49,-51,-54,-57,-60,-63,-65,-68,-71,-73,-76,-78,
    -81,-83,-85,-88,-90,-92,-94,-96,-98,-100,-102,-104,-106,-107,
    -109,-111,-112,-113,-115,-116,-117,-118,-120,-121,-122,-122,
    -123,-124,-125,-125,-126,-126,-126,-127,-127,-127,-127,-127,
    -127,-127,-126,-126,-126,-125,-125,-124,-123,-122,-122,-121,
    -120,-118,-117,-116,-115,-113,-112,-111,-109,-107,-106,-104,
    -102,-100,-98,-96,-94,-92,-90,-88,-85,-83,-81,-78,-76,-73,-71,
    -68,-65,-63,-60,-57,-54,-51,-49,-46,-43,-40,-37,-34,-31,-28,
    -25,-22,-19,-16,-12,-9,-6,-3
};

uint16_t isqrt(uint16_t x) NONBANKED
{
    uint16_t m, y, b;
    m = 0x4000;
    y = 0;
    while (m != 0) {
        b = y | m;
        y >>= 1;
        if (x >= b) {
            x -= b;
            y |= m;
        }
        m >>= 2;
    }
    return y;
}

void apply_physics()
{
    // Unhooked physics
    if (player.hookState != HS_ATTACHED)
    {
        player.x += player.xSpd;
        player.y += player.ySpd;

        // handle collision
        if (player.y > 2432) player.y = 2432;

        // decelerate Y and X
        if (player.y < 2432)
        {
            player.grounded = 0;
            player.ySpd += GRAVITY_CONST;
        }
        else
        {
            player.grounded = 1;
            player.ySpd = 0;
        }

        if (player.grounded)
        {
            if (player.xSpd < X_GROUND_DECELERATION_IN_SUBPIXELS && player.xSpd > -X_GROUND_DECELERATION_IN_SUBPIXELS)
            {
                player.xSpd = 0;
            }
            else if (player.xSpd < 0)
            {
                player.xSpd += X_GROUND_DECELERATION_IN_SUBPIXELS;
            }
            else if (player.xSpd)
            {
                player.xSpd -= X_GROUND_DECELERATION_IN_SUBPIXELS; 
            }
        }
        else
        {
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
    }
    // Hook physics
    else
    {
        player.angularAcc = SIN(player.hookAngle);
        // Division with negative numbers does not work properly in gbdk
        // So, change to positive before division (algorithm needs -sin() anyways)
        if (player.angularAcc < 0)
        {
            player.angularAcc = -1 * player.angularAcc;
            player.angularAcc /= (player.hookSegments << 1);
        }
        else
        {
            player.angularAcc /= (player.hookSegments << 1);
            player.angularAcc = -1 * player.angularAcc;
        }

        player.angularVel += player.angularAcc + (player.angularAcc >> 1) + 1;
        player.hookAngle += (player.angularVel) >> 4;

        player.x = ((player.hookLength * SIN(player.hookAngle)) >> 3) + player.hookX;
        player.y = ((player.hookLength * COS(player.hookAngle)) >> 3) + player.hookY;

        if (player.angularVel < 0)
        {
            player.angularVel += 1;
        }
        else if (player.angularVel)
        {
            player.angularVel -= 1;
        }
        // Taper off to angle 0 even if there is no more velocity
        // angularVel == 0
        else if (player.hookAngle > 0 && player.hookAngle <= 8)
        {
            player.hookAngle -= 1;
        }
        else if (player.hookAngle > 247 /* && player.hookAngle <= 255 //always true */)
        {
            player.hookAngle += 1;
        }
    }
}