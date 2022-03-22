#pragma once
#include "vector"
#include <atltypes.h>
#include "defs.h"
#include "pixel.h"

vector<CRect> GetRects(const int &width, const int &height, uchar *imagePtr_1C);
void UpdateRects(vector<pair<pair<int, int>, int>> *last_row, vector<pair<pair<int, int>, int>> *this_row, vector<CRect> &rects, const int height);
void ModifyRects(vector<CRect> &rects);
