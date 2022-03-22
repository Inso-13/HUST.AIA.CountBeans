#include "pch.h"
#include "rects.h"

/**
 * @brief 修正找到的矩形
 * 
 * @param rects 
 */
void ModifyRects(vector<CRect> &rects)
{
	int n_rects = (int)rects.size();
	int rect_size = 0;
	int x,y;
	int x1,x2,y1,y2,x3,y3;

	for(int i=0;i<n_rects;i++)
	{
		rect_size = (rects[i].right - rects[i].left) * (rects[i].bottom - rects[i].top + 1);
		if(rect_size > 36)
		{
			x1 = rects[i].left;
			x2 = rects[i].right;
			y1 = rects[i].top;
			y2 = rects[i].bottom;

			x3 = (x1+x2)/2;
			y3 = (y1+y2)/2;

			rects[i].top = 10000;
			rects.push_back(CRect(x1,y1,x3,y3));
			rects.push_back(CRect(x3,y3,x2,y2));
		}
		if(rect_size > 2 || rect_size < 0)
			continue;

		x = rects[i].left;
		y = rects[i].top;

		for(int j=0;j<n_rects;j++)
		{
			if(i==j)
				continue;
			x1 = rects[j].left;
			x2 = rects[j].right;
			y1 = rects[j].top;
			y2 = rects[j].bottom;
			if((x2-x1)*(y2-y1+1)>9)
				continue;
			if ((x2 - x1)*(y2-y1+1)<7 && (max(x1, x - 2) <= min(x2, x + 2)) && (max(y1, y - 3) <= min(y2, y + 3)))
			{
				rects[i].left = min(x1, x);
				rects[i].right = max(x2, x);
				rects[i].top = min(y1, y);
				rects[i].bottom = max(y2, y);
				rects[j].top = 10000;
				break;
			}
			else if((max(x1, x - 1) <= min(x2, x + 1)) && (max(y1, y - 2) <= min(y2, y + 2)))
			{
				rects[i].left = min(x1,x);
				rects[i].right = max(x2,x);
				rects[i].top = min(y1,y);
				rects[i].bottom = max(y2,y);
				rects[j].top = 10000;
				break;
			}
		}
	}
}

/**
 * @brief 更新已找到的矩形块，可能进行合并
 * 
 * @param last_row 
 * @param this_row 
 * @param rects 
 * @param height 
 */
void UpdateRects(vector<pair<pair<int, int>, int>> *last_row, vector<pair<pair<int, int>, int>> *this_row, vector<CRect> &rects, const int height)
{
    int n = (int)this_row->size();
    int m = (int)last_row->size();
    int x1, x2, lx1, lx2, n_rects, i_rect, j_rect;
    bool crossed = 0;

    for (int i = 0; i < n; i++)
    {
        x1 = (*this_row)[i].first.first;
        x2 = (*this_row)[i].first.second;
        crossed = 0;
        for (int j = 0; j < m; j++)
        {
            lx1 = (*last_row)[j].first.first;
            lx2 = (*last_row)[j].first.second;
            // if(x2 < lx1)
            // break;
            if (max(x1, lx1) <= min(x2, lx2))
            {
                i_rect = (*last_row)[j].second;
                (*this_row)[i].second = i_rect;
                rects[i_rect].bottom = height;
                rects[i_rect].left = min(rects[i_rect].left, (long)x1);
                rects[i_rect].right = max(rects[i_rect].right, (long)x2);
                crossed = 1;
                if (++j < m)
                {
                    lx1 = (*last_row)[j].first.first;
                    lx2 = (*last_row)[j].first.second;
                    if (max(x1, lx1 - 1) <= min(x2, lx2 + 1))
                    {
                        j_rect = (*last_row)[j].second;
                        rects[i_rect].top = min(rects[i_rect].top, rects[j_rect].top);
                        rects[i_rect].left = min(rects[i_rect].left, rects[j_rect].left);
                        rects[i_rect].right = max(rects[i_rect].right, rects[j_rect].right);
                        if (i_rect != j_rect)
                            rects[j_rect].top = 10000;
                    }
                }
                break;
            }
        }
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
 * @param width 
 * @param height 
 * @param imagePtr_1C 
 * @return vector<CRect> 
 */
vector<CRect> GetRects(const int &width, const int &height, uchar *imagePtr_1C)
{
    vector<CRect> rects;
    vector<pair<pair<int, int>, int>> row1;
    vector<pair<pair<int, int>, int>> row2;
    vector<pair<pair<int, int>, int>> *last_row = nullptr;
    vector<pair<pair<int, int>, int>> *this_row = nullptr;
    bool row_begined = 0, this_mask;
    int i = 0, line_first = 0, line_second = 0;

    for (int j = 0; j < height; j++)
    {
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
            while (i < width)
            {
                this_mask = (bool)get_pixel_1C(imagePtr_1C, i++, j, width);
                if ((this_mask && !row_begined) || (!this_mask && row_begined))
                    break;
            }
            if (!row_begined)
            {
                line_first = i - 1;
                row_begined = 1;
            }
            else
            {
                line_second = i - 1;
                row_begined = 0;
                if (line_second - line_first > MIN_DX)
                    this_row->push_back(make_pair(make_pair(line_first, line_second), -1));
            }
        }
        // if (row_begined && width - line_first > 1 + MIN_DX)
        // this_row->push_back(make_pair(make_pair(line_first, width - 1), -1));
        UpdateRects(last_row, this_row, rects, j);
        last_row->clear();
    }

    return rects;
}
