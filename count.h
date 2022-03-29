/**
 * @file count.h
 * @author inso (2101944271@qq.com)
 * @brief 计算豆子个数的顶层api
 * @version 0.1
 * @date 2022-03-26
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#pragma once

#include <vector>
#include "defs.h"
#include "pixel.h"
#include "rects.h"
#include "MVGigE.h"
#include "MVImage.h"

int CountBeansBig(uchar *imagePtr, int height, int width, MVImage &image_3C);
int CountBeansAll(vector<vector<double>> &dists, int height, int width, MVImage &image_3C);