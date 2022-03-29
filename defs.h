/**
 * @file defs.h
 * @author inso (2101944271@qq.com)
 * @brief 编译开关、常量定义
 * @version 0.1
 * @date 2022-03-26
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#pragma once

#include "afxdialogex.h"
#include "resource.h"

typedef unsigned char uchar;
using namespace std;

// 常量定义

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


// 编译开关

// 查看预处理后的单通道图
// #define SHOW_1C

// 查看原图
#define SHOW_3C

// 在原图上标画豆子的位置
#define SHOW_CIRCLE

// 查看距离变换、极大值提取后的二值图 @deprecated
// #define SHOW_ALL

// 查看腐蚀提取后的二值图 @deprecated
// #define SHOW_BIG