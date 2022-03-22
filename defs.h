#pragma once
#include "afxdialogex.h"
#include "resource.h"

typedef unsigned char uchar;
using namespace std;

// 横向缩放因子
#define WF 8
// 纵向缩放因子
#define HF 8

// 画笔的粗细
#define LINEWIDTH 3
// 阈值化的红通道上限
#define R_MAX 30
// 检测矩形的最小横向边长
#define MIN_DX -1

// 腐蚀后最小的黄豆大小
#define BIG_RECT_LIMIT 4
// 距离变换阈值
#define MIN_DIST 2.0

// 测试图片路径
#define TEST_IMG_PATH "J:\\img\\_0005.bmp"
// 测试文件夹路径
#define TEST_IMG_DIR_PATH "J:\\img\\"

// #define SHOW_1C
#define SHOW_3C
// #define SHOW_ALL
// #define SHOW_BIG
#define SHOW_CIRCLE