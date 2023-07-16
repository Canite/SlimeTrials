#include "../include/utils.h"

int16_t abs16(int16_t x)
{
    if (x < 0)
    {
        return -1 * x;
    }

    return x;
}

int8_t abs8(int8_t x)
{
    if (x < 0)
    {
        return -1 * x;
    }

    return x;
}

uint16_t abs_sub(uint16_t x, uint16_t y)
{
    if (x < y)
    {
        return y - x;
    }

    return x - y;
}

int16_t sign(int16_t x)
{
    if (x < 0) return -1;
    if (x >= 0) return 1;
}
