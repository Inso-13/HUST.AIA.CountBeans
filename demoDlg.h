﻿
// demoDlg.h: 头文件
//

#pragma once

#include "util.h"
#include "defs.h"
#include "rects.h"
#include "distance.h"
#include "pixel.h"
#include "count.h"

// CdemoDlg 对话框
class CdemoDlg : public CDialogEx
{
	// 构造
public:
	//定义相机句柄对象
	HANDLE m_hCam;
	//定义图像处理对象
	MVImage m_image_3C_raw;
	MVImage m_image_1C;
	HDC m_hdc;
	//定义像素格式对象
	MV_PixelFormatEnums m_PixelFormat;
	//采集标识
	bool m_bRun;
	//通过回调函数获取图像数据信息
	int OnStreamCB(MV_IMAGE_INFO *pInfo);
	//画图
	void DrawImage();
	//关闭相机、释放资源
	void OnClose();
	//处理图像
	void ProcessImage();
	void PreProcessImage(const uchar lower_r, const uchar lower_g, const uchar lower_b, const uchar upper_r, const uchar upper_g, const uchar upper_b);


	CdemoDlg(CWnd *pParent = nullptr); // 标准构造函数
	int m_height;
	int m_width;
	int m_height_raw;
	int m_width_raw;
	uchar *m_imagePtr_3C_raw;
	uchar *m_imagePtr_1C;
// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum
	{
		IDD = IDD_DEMO_DIALOG
	};
#endif

protected:
	virtual void DoDataExchange(CDataExchange *pDX); // DDX/DDV 支持

	// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOpencam();
	afx_msg void OnBnClickedStartgrab();
	afx_msg void OnBnClickedClosecam();
	afx_msg void OnBnClickedTest();
};

int __stdcall StreamCB(MV_IMAGE_INFO *pInfo, ULONG_PTR nUserVal);
void ModifyRects(vector<CRect> &rects);
