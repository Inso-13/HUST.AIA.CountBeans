#pragma once

#include <vector>
#include "defs.h"
#include "rects.h"
#include "MVGigE.h"
#include "MVImage.h"

int CountBeansBig(uchar *imagePtr, int height, int width, MVImage &image_3C);
int CountBeansAll(vector<vector<double>> &dists, int height, int width, MVImage &image_3C);