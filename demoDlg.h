/**
 * @file demoDlg.h
 * @author inso(editor) (2101944271@qq.com)
 * @brief 
 * @version 0.1
 * @date 2022-03-26
 * 
 * @copyright Copyright (c) 2022
 * 
 */

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
	//定义图像处理对象(原图)
	MVImage m_image_3C_raw;
	//定义图像处理对象(预处理后的二值图)
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
	// 预处理图像
	void PreProcessImage(const uchar lower_r, const uchar lower_g, const uchar lower_b, const uchar upper_r, const uchar upper_g, const uchar upper_b);


	CdemoDlg(CWnd *pParent = nullptr); // 标准构造函数
	// 二值图的长宽
	int m_height;
	int m_width;
	// 原图的长宽
	int m_height_raw;
	int m_width_raw;
	// 原图的数据指针
	uchar *m_imagePtr_3C_raw;
	// 二值图的数据指针
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

// 相机回调函数
int __stdcall StreamCB(MV_IMAGE_INFO *pInfo, ULONG_PTR nUserVal);
