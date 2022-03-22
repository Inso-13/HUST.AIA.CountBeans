#pragma once

#include <vector>
#include "defs.h"
#include "pixel.h"

vector<vector<double>> DistanceTransformD4(uchar *imagePtr, int height, int width);
void D4AL(int i, int j, int rows, int cols, vector<vector<double>> &f);
void D4BR(int i, int j, int rows, int cols, vector<vector<double>> &f);
bool InArea(int i, int j, int rows, int cols);