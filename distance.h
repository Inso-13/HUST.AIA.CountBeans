/**
 * @file distance.h
 * @author inso (2101944271@qq.com)
 * @brief 距离变换相关函数
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

vector<vector<double>> DistanceTransformD4(uchar *imagePtr, int height, int width);
void D4AL(int i, int j, int rows, int cols, vector<vector<double>> &f);
void D4BR(int i, int j, int rows, int cols, vector<vector<double>> &f);
bool InArea(int i, int j, int rows, int cols);