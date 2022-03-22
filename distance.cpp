#include "pch.h"
#include "distance.h"

/**
 * @brief 从上、左两个方向更新计算距离
 *
 * @param i
 * @param j
 * @param rows
 * @param cols
 * @param f
 */
void D4AL(int i, int j, int rows, int cols, vector<vector<double>> &f)
{
    //上
    if (InArea(i - 1, j, rows, cols))
        f[i][j] = min(f[i][j], 1.0 + f[i - 1][j]);
    //左上
    if (InArea(i - 1, j - 1, rows, cols))
        f[i][j] = min(f[i][j], 1.4 + f[i - 1][j - 1]);
    //左
    if (InArea(i, j - 1, rows, cols))
        f[i][j] = min(f[i][j], 1.0 + f[i][j - 1]);
    //左下
    if (InArea(i + 1, j - 1, rows, cols))
        f[i][j] = min(f[i][j], 1.4 + f[i + 1][j - 1]);
}

/**
 * @brief 从下、右两个方向更新计算距离
 *
 * @param i
 * @param j
 * @param rows
 * @param cols
 * @param f
 */
void D4BR(int i, int j, int rows, int cols, vector<vector<double>> &f)
{
    //下
    if (InArea(i + 1, j, rows, cols))
        f[i][j] = min(f[i][j], 1.0 + f[i + 1][j]);
    //右下
    if (InArea(i + 1, j + 1, rows, cols))
        f[i][j] = min(f[i][j], 1.4 + f[i + 1][j + 1]);
    //右
    if (InArea(i, j + 1, rows, cols))
        f[i][j] = min(f[i][j], 1.0 + f[i][j + 1]);
    //右上
    if (InArea(i - 1, j + 1, rows, cols))
        f[i][j] = min(f[i][j], 1.4 + f[i - 1][j + 1]);
}

/**
 * @brief 判断是否在矩阵内,若越界返回false
 *
 * @param i
 * @param j
 * @param rows
 * @param cols
 * @return true
 * @return false
 */
bool InArea(int i, int j, int rows, int cols)
{
    if (i < 0 || i >= rows)
        return false;
    if (j < 0 || j >= cols)
        return false;
    return true;
}

/**
 * @brief 距离变换函数
 *
 * @param imagePtr
 * @param height
 * @param width
 * @return vector<vector<double>>
 */
vector<vector<double>> DistanceTransformD4(uchar *imagePtr, int height, int width)
{
    vector<double> temp(height, 0);
    vector<vector<double>> dists(width, temp);
    int rows = width;
    int cols = height;

    //初始化
    for (int i = 0; i < rows; ++i)
        for (int j = 0; j < cols; ++j)
            if (!get_pixel_1C(imagePtr, i, j, width))
                dists[i][j] = 0;
            else
                dists[i][j] = INT_MAX - 2; //简单的防止溢出
    //按行遍历图像，从上到下，从左到右
    for (int i = 0; i < rows; ++i)
        for (int j = 0; j < cols; ++j)
            D4AL(i, j, rows, cols, dists);

    //按行遍历图像，从下到上，从右到左
    for (int i = rows - 1; i >= 0; --i)
        for (int j = cols - 1; j >= 0; --j)
            D4BR(i, j, rows, cols, dists);

    return dists;
}