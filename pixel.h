#pragma once
#include "defs.h"

inline uchar get_pixel_3C(const uchar *imagePtr, const int &x, const int &y, const int channel, const int width)
{
    return *(imagePtr + (y * width + x) * 3 + channel);
}
inline uchar get_pixel_1C(const uchar *imagePtr, const int &x, const int &y, const int width)
{
    return *(imagePtr + y * width + x);
}
inline void set_pixel_1C(uchar *imagePtr, const int &x, const int &y, const uchar val, const int width)
{
    *(imagePtr + y * width + x) = val;
}
