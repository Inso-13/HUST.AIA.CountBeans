#include "pch.h"
#include "framework.h"
#include "demo.h"
#include "demoDlg.h"
#include "afxdialogex.h"
#include "count.h"

/**
 * @brief 找到黄豆的位置，并计数
 *
 * @return int
 */
int CountBeansBig(uchar *imagePtr, int height, int width, MVImage &image_3C)
{
    int temp = 255;
    MVImage temp_image;
    temp_image.CreateByPixelFormat(width - 8, height - 8, PixelFormat_Mono8);

    // 5*5腐蚀
    for (int i = 4; i < width - 4; i++)
    {
        for (int j = 4; j < height - 4; j++)
        {
            temp = 255;
            for (int u = -2; u < 3; u++)
            {
                for (int v = -2; v < 3; v++)
                {
                    if (!temp)
                        break;
                    if (!get_pixel_1C(imagePtr, i + u, j + v, width))
                    {
                        temp = 0;
                        break;
                    }
                }
            }
            set_pixel_1C((uchar *)temp_image.GetBits(), i - 4, j - 4, temp, width - 8);
        }
    }
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
#ifdef SHOW_CIRCLE
    HDC m_hdc = image_3C.GetDC();
    HPEN hpen = CreatePen(PS_SOLID, LINEWIDTH, RGB(255, 255, 0));
    SelectObject(m_hdc, hpen);
#endif
    vector<CRect> rects = GetRects(width - 8, height - 8, (uchar *)temp_image.GetBits());
    int ret = 0;
    for (auto rect : rects)
    {
        if (rect.top < 1000 && (rect.right - rect.left) * (rect.bottom - rect.top + 1) > BIG_RECT_LIMIT)
        {
#ifdef SHOW_CIRCLE
            int centerX = ((rect.right + rect.left) / 2 + 4) * WF;
            int centerY = ((rect.bottom + rect.top) / 2 + 4) * HF;
            Ellipse(m_hdc, centerX - 6, centerY - 6, centerX + 6, centerY + 6);
#endif
            ret++;
        }
    }

    return ret;
}

/**
 * @brief 找到所有豆子的位置，并计数
 *
 * @param dists
 * @return int
 */
int CountBeansAll(vector<vector<double>> &dists, int height, int width, MVImage &image_3C)
{
    bool flag = 1;
    int u, v;
    double this_dist;
    MVImage temp_image;
    temp_image.CreateByPixelFormat(width, height, PixelFormat_Mono8);

    // 沿四个方向找到距离极大值点
    for (int i = 0; i < width; i++)
    {
        for (int j = 0; j < height; j++)
        {
            flag = 1;
            this_dist = dists[i][j];

            if (this_dist < MIN_DIST)
            {
                set_pixel_1C((uchar *)temp_image.GetBits(), i, j, 0, width);
                continue;
            }

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

            // u = i;v = j;
            // while (flag && --u >= 0&& --v >= 0)
            // {
            // 	if (dists[u][v] > this_dist)
            // 	{
            // 		flag = 0;
            // 		break;
            // 	}
            // 	else if (dists[u][v] < this_dist)
            // 		break;
            // }
            // u = i;v = j;
            // while (flag && ++u < width && ++v < height)
            // {
            // 	if (dists[u][v] > this_dist)
            // 	{
            // 		flag = 0;
            // 		break;
            // 	}
            // 	else if (dists[u][v] < this_dist)
            // 		break;
            // }
            // v = j;u = i;
            // while (flag && --v >= 0 && ++u < height)
            // {
            // 	if (dists[u][v] > this_dist)
            // 	{
            // 		flag = 0;
            // 		break;
            // 	}
            // 	else if (dists[u][v] < this_dist)
            // 		break;
            // }
            // v = j;u = i;
            // while (flag && ++v < height && --u >= 0)
            // {
            // 	if (dists[u][v] > this_dist)
            // 	{
            // 		flag = 0;
            // 		break;
            // 	}
            // 	else if (dists[u][v] < this_dist)
            // 		break;
            // }

            set_pixel_1C((uchar *)temp_image.GetBits(), i, j, flag ? 255 : 0, width);
        }
    }
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
#ifdef SHOW_CIRCLE
    HDC m_hdc = image_3C.GetDC();
    HPEN hpen = CreatePen(PS_SOLID, LINEWIDTH, RGB(0, 255, 0));
    SelectObject(m_hdc, hpen);
#endif
    vector<CRect> rects = GetRects(width, height, (uchar *)temp_image.GetBits());
    ModifyRects(rects);
    int ret = 0;
    for (auto rect : rects)
        if ((rect.right - rect.left + 1) * (rect.bottom - rect.top + 1) > 0)
        {
#ifdef SHOW_CIRCLE
            int centerX = (rect.right + rect.left) / 2 * WF;
            int centerY = (rect.bottom + rect.top) / 2 * HF;
            Ellipse(m_hdc, centerX - 3, centerY - 3, centerX + 3, centerY + 3);
#endif
            ret++;
        }
    return ret;
}