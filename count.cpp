/**
 * @file count.cpp
 * @author inso (2101944271@qq.com)
 * @brief 计算豆子个数的顶层api
 * @version 0.1
 * @date 2022-03-26
 *
 * @copyright Copyright (c) 2022
 *
 */

#include "pch.h"
#include "framework.h"
#include "demo.h"
#include "demoDlg.h"
#include "afxdialogex.h"
#include "count.h"

/**
 * @brief 找到黄豆的位置，并计数
 *
 * @return int 黄豆的个数
 */
int CountBeansBig(uchar *imagePtr, int height, int width, MVImage &image_3C)
{
    int temp = 255;     // 白色像素
    MVImage temp_image; // 腐蚀后的图像

    // 图像初始化，分配内存
    temp_image.CreateByPixelFormat(width - 8, height - 8, PixelFormat_Mono8);

    // 5*5腐蚀 为提高效率直接忽略边框
    // i,j从4开始
    for (int i = 4; i < width - 4; i++)
    {
        for (int j = 4; j < height - 4; j++)
        {
            temp = 255;
            for (int u = -2; u < 3; u++)
            {
                for (int v = -2; v < 3; v++)
                {
                    // 如果temp已经为0，直接跳出，提高效率
                    if (!temp)
                        break;

                    // 如果像素值为0，temp设为0
                    if (!get_pixel_1C(imagePtr, i + u, j + v, width))
                    {
                        temp = 0;
                        break;
                    }
                }
            }
            // 设置最终的像素值
            set_pixel_1C((uchar *)temp_image.GetBits(), i - 4, j - 4, temp, width - 8);
        }
    }
// 调试时使用，用于显示腐蚀结果
#ifdef SHOW_BIG
    CRect rct;
    GetDlgItem(pic)->GetClientRect(&rct);
    int dstW = rct.Width();
    int dstH = rct.Height();
    CDC *pDC = GetDlgItem(pic)->GetDC();
    {
        pDC->SetStretchBltMode(COLORONCOLOR);
        temp_image.Draw(pDC->GetSafeHdc(), 0, 0, dstW, dstH);
    }
    ReleaseDC(pDC);
#endif

// 设置画笔
#ifdef SHOW_CIRCLE
    HDC m_hdc = image_3C.GetDC();
    HPEN hpen = CreatePen(PS_SOLID, LINEWIDTH, RGB(255, 255, 0));
    SelectObject(m_hdc, hpen);
#endif

    // 从腐蚀后的图像里找到所有连通域的位置，每个连通域用CRect保存
    vector<CRect> rects = GetRects(width - 8, height - 8, (uchar *)temp_image.GetBits());

    int ret = 0;

    // 遍历所有找到的rect
    for (auto rect : rects)
    {
        // 排除被删除的rect，并排除太小的rect
        if (rect.top < 1000 && (rect.right - rect.left) * (rect.bottom - rect.top + 1) > BIG_RECT_LIMIT)
        {
            // 在原图上画圆形，标记黄豆位置
#ifdef SHOW_CIRCLE
            // 计算黄豆的中心点
            int centerX = ((rect.right + rect.left) / 2 + 4) * WF;
            int centerY = ((rect.bottom + rect.top) / 2 + 4) * HF;
            // 画圆形
            Ellipse(m_hdc, centerX - 6, centerY - 6, centerX + 6, centerY + 6);
#endif
            // 有效黄豆计数
            ret++;
        }
    }

    // 返回黄豆的个数
    temp_image.Destroy();
    return ret;
}

/**
 * @brief 找到所有豆子的位置，并计数
 *
 * @param dists 距离特征图
 * @return int 返回黄豆和绿豆的总数量
 */
int CountBeansAll(vector<vector<double>> &dists, int height, int width, MVImage &image_3C)
{
    bool flag = 1;      // 判断是否是极大值点
    int u, v;           // 暂存循环临时变量
    double this_dist;   // 暂存循环到当前位置的距离值
    MVImage temp_image; // 暂存距离变换、极大值变化后的临时图像
    // 分配空间
    temp_image.CreateByPixelFormat(width, height,  PixelFormat_Mono8);

    // 沿四个方向找到距离极大值点
    for (int i = 0; i < width; i++)
    {
        for (int j = 0; j < height; j++)
        {
            flag = 1;
            this_dist = dists[i][j];

            // 距离变换的最小阈值
            if (this_dist < MIN_DIST)
            {
                set_pixel_1C((uchar *)temp_image.GetBits(), i, j, 0, width);
                continue;
            }

            // 8个方向确认局部极大值
            // 正左
            u = i;
            while (flag && --u >= 0)
            {
                if (dists[u][j] > this_dist)
                {
                    flag = 0;
                    break;
                }
                else if (dists[u][j] < this_dist)
                    break;
            }
            //正右
            u = i;
            while (flag && ++u < width)
            {
                if (dists[u][j] > this_dist)
                {
                    flag = 0;
                    break;
                }
                else if (dists[u][j] < this_dist)
                    break;
            }
            // 正上
            v = j;
            while (flag && --v >= 0)
            {
                if (dists[i][v] > this_dist)
                {
                    flag = 0;
                    break;
                }
                else if (dists[i][v] < this_dist)
                    break;
            }
            // 正下
            v = j;
            while (flag && ++v < height)
            {
                if (dists[i][v] > this_dist)
                {
                    flag = 0;
                    break;
                }
                else if (dists[i][v] < this_dist)
                    break;
            }
            // 左上
            u = i;v = j;
            while (flag && --u >= 0&& --v >= 0)
            {
            	if (dists[u][v] > this_dist)
            	{
            		flag = 0;
            		break;
            	}
            	else if (dists[u][v] < this_dist)
            		break;
            }
            // 右下
            u = i;v = j;
            while (flag && ++u < width && ++v < height)
            {
            	if (dists[u][v] > this_dist)
            	{
            		flag = 0;
            		break;
            	}
            	else if (dists[u][v] < this_dist)
            		break;
            }
            // 右上
            v = j;u = i;
            while (flag && --v >= 0 && ++u < height)
            {
            	if (dists[u][v] > this_dist)
            	{
            		flag = 0;
            		break;
            	}
            	else if (dists[u][v] < this_dist)
            		break;
            }
            // 左下
            v = j;u = i;
            while (flag && ++v < height && --u >= 0)
            {
            	if (dists[u][v] > this_dist)
            	{
            		flag = 0;
            		break;
            	}
            	else if (dists[u][v] < this_dist)
            		break;
            }

            // 设置图像像素
            set_pixel_1C((uchar *)temp_image.GetBits(), i, j, flag ? 255 : 0, width);
        }
    }
    // 调试中使用，显示距离变换、取局部最大值后的二值图像
#ifdef SHOW_ALL
    CRect rct;
    GetDlgItem(pic)->GetClientRect(&rct);
    int dstW = rct.Width();
    int dstH = rct.Height();
    CDC *pDC = GetDlgItem(pic)->GetDC();
    {
        pDC->SetStretchBltMode(COLORONCOLOR);
        temp_image.Draw(pDC->GetSafeHdc(), 0, 0, dstW, dstH);
    }
    ReleaseDC(pDC);
#endif
    // 在原图上显示所有的豆子的位置
#ifdef SHOW_CIRCLE
    HDC m_hdc = image_3C.GetDC();
    HPEN hpen = CreatePen(PS_SOLID, LINEWIDTH, RGB(0, 255, 0));
    SelectObject(m_hdc, hpen);
#endif

    // 获得最大连通域，并用CRect储存
    vector<CRect> rects = GetRects(width, height, (uchar *)temp_image.GetBits());
    
    // 后处理，避免出现两个局部最大值过近的情况
    ModifyRects(rects);
    int ret = 0;
    for (auto rect : rects)
        // 除去被删除的rect
        if ((rect.right - rect.left + 1) * (rect.bottom - rect.top + 1) > 0)
        {
            // 在原图中标注豆子的位置
#ifdef SHOW_CIRCLE
            // 计算rect的中心
            int centerX = (rect.right + rect.left) / 2 * WF;
            int centerY = (rect.bottom + rect.top) / 2 * HF;
            // 画圆
            Ellipse(m_hdc, centerX - 3, centerY - 3, centerX + 3, centerY + 3);
#endif
            // 记录有效的豆子数量
            ret++;
        }

    temp_image.Destroy();
    return ret;
}