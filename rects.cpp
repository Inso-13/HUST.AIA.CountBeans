#include "pch.h"
#include "rects.h"

/**
 * @brief 修正找到的矩形
 * 
 * @param rects 返回参数，修正后的矩形
 */
void ModifyRects(vector<CRect> &rects)
{
	int n_rects = (int)rects.size();  // 待修正的矩形的个数
	int rect_size = 0;  // 当前矩形的面积
	int x,y;    // 坐标
	int x1,x2,y1,y2,x3,y3;  // 坐标

    // 遍历所有矩形
	for(int i=0;i<n_rects;i++)
	{
        // 计算矩形的面积
		rect_size = (rects[i].right - rects[i].left) * (rects[i].bottom - rects[i].top + 1);

		// 分离面积太大的矩形、防止强粘连
        if(rect_size > 36)
		{
			x1 = rects[i].left;
			x2 = rects[i].right;
			y1 = rects[i].top;
			y2 = rects[i].bottom;

			x3 = (x1+x2)/2;
			y3 = (y1+y2)/2;

            // 删除矩形（面积为负数）
			rects[i].top = 10000;
			rects.push_back(CRect(x1,y1,x3,y3));
			rects.push_back(CRect(x3,y3,x2,y2));
		}

        // 对面积适中的矩形不做处理
		if(rect_size > 2 || rect_size < 0)
			continue;

        // 需要处理的矩形，可能会与其他矩形做合并
		x = rects[i].left;
		y = rects[i].top;

		for(int j=0;j<n_rects;j++)
		{
			// 排除自己
            if(i==j)
				continue;
			x1 = rects[j].left;
			x2 = rects[j].right;
			y1 = rects[j].top;
			y2 = rects[j].bottom;
			
            // 不能与面积太大的矩形做合并
            if((x2-x1)*(y2-y1+1)>9)
				continue;

            // 矩形合并
			if ((x2 - x1)*(y2-y1+1)<7 && (max(x1, x - 2) <= min(x2, x + 2)) && (max(y1, y - 3) <= min(y2, y + 3)))
			{
                // 合并矩形
				rects[i].left = min(x1, x);
				rects[i].right = max(x2, x);
				rects[i].top = min(y1, y);
				rects[i].bottom = max(y2, y);

                // 删除第二个矩形（面积为负数）
                rects[j].top = 10000;
				break;
			}
			else if((max(x1, x - 1) <= min(x2, x + 1)) && (max(y1, y - 2) <= min(y2, y + 2)))
			{
                // 合并矩形
                rects[i].left = min(x1, x);
                rects[i].right = max(x2,x);
				rects[i].top = min(y1,y);
				rects[i].bottom = max(y2,y);
                
                // 删除第二个矩形（面积为负数）
                rects[j].top = 10000;
                break;
			}
		}
	}
}

/**
 * @brief 更新已找到的矩形块，可能进行合并
 * 
 * @param last_row 上一行的连通域
 * @param this_row 这一行的连通域
 * @param rects 返回参数，矩形数组
 * @param height 当前行数
 */
void UpdateRects(vector<pair<pair<int, int>, int>> *last_row, vector<pair<pair<int, int>, int>> *this_row, vector<CRect> &rects, const int height)
{
    int n = (int)this_row->size();  //这一行的连通域个数
    int m = (int)last_row->size();  //上一行的连通域个数
    int x1, x2, lx1, lx2, n_rects, i_rect, j_rect;
    bool crossed = 0;   // 是否两行连通域之间有连通

    for (int i = 0; i < n; i++)
    {
        x1 = (*this_row)[i].first.first;
        x2 = (*this_row)[i].first.second;
        crossed = 0;
        for (int j = 0; j < m; j++)
        {
            lx1 = (*last_row)[j].first.first;
            lx2 = (*last_row)[j].first.second;

            // 上一行的第j个连通域与这一行的第i个连通域连通
            // 生长rects
            if (max(x1, lx1) <= min(x2, lx2))
            {
                i_rect = (*last_row)[j].second;
                (*this_row)[i].second = i_rect;
                // 更新矩形
                rects[i_rect].bottom = height;
                rects[i_rect].left = min(rects[i_rect].left, (long)x1);
                rects[i_rect].right = max(rects[i_rect].right, (long)x2);

                crossed = 1;
                // 继续往下搜索，防止孤立点的噪声
                if (++j < m)
                {
                    lx1 = (*last_row)[j].first.first;
                    lx2 = (*last_row)[j].first.second;
                    if (max(x1, lx1 - 1) <= min(x2, lx2 + 1))
                    {
                        // 更新矩形
                        j_rect = (*last_row)[j].second;
                        rects[i_rect].top = min(rects[i_rect].top, rects[j_rect].top);
                        rects[i_rect].left = min(rects[i_rect].left, rects[j_rect].left);
                        rects[i_rect].right = max(rects[i_rect].right, rects[j_rect].right);
                        if (i_rect != j_rect)
                            // 删除矩形（面积为负数）
                            rects[j_rect].top = 10000;
                    }
                }
                break;
            }
        }

        // 如果与上一行的所有连通域都不连通，就创建一个新的rect
        if (!crossed)
        {
            n_rects = (int)rects.size();
            (*this_row)[i].second = n_rects;
            rects.push_back(CRect(x1, height, x2, height));
        }
    }
}

/**
 * @brief 按行扫描，找到所有分离的矩形块位置
 * 
 * @param width 图片的宽度 
 * @param height 图片的高度
 * @param imagePtr_1C 单通道图片的数据指针
 * @return vector<CRect> 找到的连通域的rect
 */
vector<CRect> GetRects(const int &width, const int &height, uchar *imagePtr_1C)
{
    vector<CRect> rects;  // 返回值，储存所有的连通域
    vector<pair<pair<int, int>, int>> row1; // 某一行
    vector<pair<pair<int, int>, int>> row2; // 另一行
    // 分别指向row1,row2
    vector<pair<pair<int, int>, int>> *last_row = nullptr;
    vector<pair<pair<int, int>, int>> *this_row = nullptr;
    bool row_begined = 0;  // 连通域开始与否标志
    bool this_mask;  // 暂存当前扫描像素的类型
    int i = 0;  // 记录扫描位置
    int line_first = 0, line_second = 0;  // 连通域的开始与结束位置

    for (int j = 0; j < height; j++)
    {
        // 循环按顺序指向两行
        if (j % 2)
        {
            last_row = &row2;
            this_row = &row1;
        }
        else
        {
            last_row = &row1;
            this_row = &row2;
        }

        i = 0;
        row_begined = 0;
        while (i < width)
        {
            // 划分这一行的连通域
            while (i < width)
            {
                this_mask = (bool)get_pixel_1C(imagePtr_1C, i++, j, width);
                if ((this_mask && !row_begined) || (!this_mask && row_begined))
                    break;
            }

            // 如果连通域之前未开始
            if (!row_begined)
            {
                // 记录连通域开始的像素横坐标
                line_first = i - 1;
                row_begined = 1;
            }
            // 如果连通域之前已经开始
            else
            {
                // 记录连通域结束的像素横坐标
                line_second = i - 1;
                row_begined = 0;
                // 记录连通域
                if (line_second - line_first > MIN_DX)
                    this_row->push_back(make_pair(make_pair(line_first, line_second), -1));
            }
        }

        // 更新连通域
        UpdateRects(last_row, this_row, rects, j);
        // 清空上一行的连通域数据
        last_row->clear();
    }

    return rects;
}
