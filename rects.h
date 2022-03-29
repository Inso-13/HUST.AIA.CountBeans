/**
 * @file rects.h
 * @author inso (2101944271@qq.com)
 * @brief 使用CRect结构体，连通域提取函数
 * @version 0.1
 * @date 2022-03-26
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#pragma once
#include "vector"
#include <atltypes.h>
#include "defs.h"
#include "pixel.h"

vector<CRect> GetRects(const int &width, const int &height, uchar *imagePtr_1C);
void UpdateRects(vector<pair<pair<int, int>, int>> *last_row, vector<pair<pair<int, int>, int>> *this_row, vector<CRect> &rects, const int height);
void ModifyRects(vector<CRect> &rects);
