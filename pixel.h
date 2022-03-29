/**
 * @file pixel.h
 * @author inso (2101944271@qq.com)
 * @brief 像素读写函数接口
 * @version 0.1
 * @date 2022-03-26
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#pragma once

#include "defs.h"


/**
 * @brief 从三通道图中获得某一像素
 * 
 * @param imagePtr 图片数据指针
 * @param x 横坐标位置
 * @param y 纵坐标位置
 * @param channel 通道序号
 * @param width 图片宽度
 * @return uchar 给定位置、给定通道的像素值
 */
inline uchar get_pixel_3C(const uchar *imagePtr, const int &x, const int &y, const int channel, const int width)
{
    return *(imagePtr + (y * width + x) * 3 + channel);
}

/**
 * @brief 从单通道图中获得某一像素
 *
 * @param imagePtr 图片数据指针
 * @param x 横坐标位置
 * @param y 纵坐标位置
 * @param width 图片宽度
 * @return uchar 给定位置的像素值
 */
inline uchar get_pixel_1C(const uchar *imagePtr, const int &x, const int &y, const int width)
{
    return *(imagePtr + y * width + x);
}
/**
 * @brief 设置单通道图中的某一像素
 *
 * @param imagePtr 图片数据指针
 * @param x 横坐标位置
 * @param y 纵坐标位置
 * @param width 图片宽度
 */
inline void set_pixel_1C(uchar *imagePtr, const int &x, const int &y, const uchar val, const int width)
{
    *(imagePtr + y * width + x) = val;
}
