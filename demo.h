﻿/**
 * @file demo.h
 * @brief demo工程的主类
 * 
 */

#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含 'pch.h' 以生成 PCH"
#endif

#include "resource.h"		// 主符号


// CdemoApp:
// 有关此类的实现，请参阅 demo.cpp

class CdemoApp : public CWinApp
{
public:
	CdemoApp();

// 重写
public:
	virtual BOOL InitInstance();

// 实现

	DECLARE_MESSAGE_MAP()
};

extern CdemoApp theApp;
