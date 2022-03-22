
// demoDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "demo.h"
#include "demoDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum {
		IDD = IDD_ABOUTBOX};
#endif

protected:
	virtual void DoDataExchange(CDataExchange *pDX); // DDX/DDV 支持

	// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange *pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()

// CdemoDlg 对话框

CdemoDlg::CdemoDlg(CWnd *pParent /*=nullptr*/)
	: CDialogEx(IDD_DEMO_DIALOG, pParent), m_bRun(FALSE), m_hCam(NULL)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CdemoDlg::DoDataExchange(CDataExchange *pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CdemoDlg, CDialogEx)
ON_WM_SYSCOMMAND()
ON_WM_PAINT()
ON_WM_QUERYDRAGICON()
ON_BN_CLICKED(IDC_OpenCam, &CdemoDlg::OnBnClickedOpencam)
ON_BN_CLICKED(IDC_StartGrab, &CdemoDlg::OnBnClickedStartgrab)
ON_BN_CLICKED(IDC_CloseCam, &CdemoDlg::OnBnClickedClosecam)
ON_BN_CLICKED(IDC_Test, &CdemoDlg::OnBnClickedTest)
END_MESSAGE_MAP()

// CdemoDlg 消息处理程序

BOOL CdemoDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu *pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);	 // 设置大图标
	SetIcon(m_hIcon, FALSE); // 设置小图标

	// TODO: 在此添加额外的初始化代码
	MVSTATUS_CODES r;
	r = MVInitLib();
	if (r != MVST_SUCCESS)
	{
		MessageBox("函数库初始化失败！", "提示", MB_ICONWARNING);
		return TRUE;
	}
	r = MVUpdateCameraList();
	if (r != MVST_SUCCESS)
	{
		MessageBox("查找连接计算机的相机失败！", "提示", MB_ICONWARNING);
		return TRUE;
	}
	GetDlgItem(IDC_OpenCam)->EnableWindow(true);
	GetDlgItem(IDC_StartGrab)->EnableWindow(false);
	GetDlgItem(IDC_CloseCam)->EnableWindow(false);
	GetDlgItem(IDC_Test)->EnableWindow(true);

	return TRUE; // 除非将焦点设置到控件，否则返回 TRUE
}

void CdemoDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CdemoDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CdemoDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CdemoDlg::OnBnClickedOpencam()
{
	// TODO: 在此添加控件通知处理程序代码
	int nCams = 0;
	MVGetNumOfCameras(&nCams);
	if (nCams == 0)
	{
		MessageBox("没有找到相机 请确认连接和相机 IP 设置", "提示", MB_ICONWARNING);
		return;
	}
	MVSTATUS_CODES r = MVOpenCamByIndex(0, &m_hCam);
	if (m_hCam == NULL)
	{
		if (r == MVST_ACCESS_DENIED)
			MessageBox("无法打开相机,可能正被别的软件控制", "提示", MB_ICONWARNING);
		else
			MessageBox("无法打开相机", "提示", MB_ICONWARNING);
		return;
	}
	int w, h;
	MVGetWidth(m_hCam, &w);
	MVGetHeight(m_hCam, &h);
	MVGetPixelFormat(m_hCam, &m_PixelFormat);
	m_image_3C_raw.CreateByPixelFormat(w, h, m_PixelFormat);

	GetDlgItem(IDC_OpenCam)->EnableWindow(false);
	GetDlgItem(IDC_StartGrab)->EnableWindow(true);
	GetDlgItem(IDC_CloseCam)->EnableWindow(false);
	GetDlgItem(IDC_Test)->EnableWindow(false);
}

void CdemoDlg::OnBnClickedStartgrab()
{
	// TODO: 在此添加控件通知处理程序代码
	TriggerModeEnums enumMode;
	MVGetTriggerMode(m_hCam, &enumMode);
	if (enumMode != TriggerMode_Off)
	{
		MVSetTriggerMode(m_hCam, TriggerMode_Off);
		Sleep(100);
	}
	MVStartGrab(m_hCam, (MVStreamCB)StreamCB, (ULONG_PTR)this);
	m_bRun = true;
	GetDlgItem(IDC_OpenCam)->EnableWindow(false);
	GetDlgItem(IDC_StartGrab)->EnableWindow(false);
	GetDlgItem(IDC_CloseCam)->EnableWindow(true);
	GetDlgItem(IDC_Test)->EnableWindow(false);
}

void CdemoDlg::DrawImage()
{
	CRect rct;
	GetDlgItem(pic)->GetClientRect(&rct);
	int dstW = rct.Width();
	int dstH = rct.Height();
	CDC *pDC = GetDlgItem(pic)->GetDC();
	{
		pDC->SetStretchBltMode(COLORONCOLOR);
#ifdef SHOW_1C
		m_image_1C.Draw(pDC->GetSafeHdc(), 0, 0, dstW, dstH);
#else
#ifdef SHOW_3C
		m_image_3C_raw.Draw(pDC->GetSafeHdc(), 0, 0, dstW, dstH);
#endif
#endif
	}
	ReleaseDC(pDC);
}
int CdemoDlg::OnStreamCB(MV_IMAGE_INFO *pInfo)
{
	MVInfo2Image(m_hCam, pInfo, &m_image_3C_raw);
	PreProcessImage(0, 0, 0, 255, 255, R_MAX);
	ProcessImage();
	DrawImage();
	return 0;
}
int __stdcall StreamCB(MV_IMAGE_INFO *pInfo, ULONG_PTR nUserVal)
{
	CdemoDlg *pDlg = (CdemoDlg *)nUserVal;
	return (pDlg->OnStreamCB(pInfo));
}

void CdemoDlg::OnBnClickedClosecam()
{
	// TODO: 在此添加控件通知处理程序代码
	MVStopGrab(m_hCam);
	MVCloseCam(m_hCam);
	m_bRun = false;
	GetDlgItem(IDC_OpenCam)->EnableWindow(true);
	GetDlgItem(IDC_StartGrab)->EnableWindow(false);
	GetDlgItem(IDC_CloseCam)->EnableWindow(false);
	GetDlgItem(IDC_Test)->EnableWindow(true);
}
void CdemoDlg::OnClose()
{
	if (m_bRun != false)
		MVStopGrab(m_hCam);
	MVTerminateLib();
	CDialog::OnClose();
}

void CdemoDlg::OnBnClickedTest()
{
	// TODO: 在此添加控件通知处理程序代码
	static int first = 0;
	static vector<string> files;
	if (first == 0)
	{
		m_image_3C_raw.Load(TEST_IMG_PATH);
		GetFiles(TEST_IMG_DIR_PATH, files);
	}
	else
	{
		m_image_3C_raw.Load(files[min(first, (int)files.size()) - 1].c_str());
	}
	PreProcessImage(0, 0, 0, 255, 255, R_MAX);
	ProcessImage();
	DrawImage();

	first++;
	GetDlgItem(IDC_OpenCam)->EnableWindow(true);
	GetDlgItem(IDC_StartGrab)->EnableWindow(false);
	GetDlgItem(IDC_CloseCam)->EnableWindow(false);
	GetDlgItem(IDC_Test)->EnableWindow(true);
}

/**
 * @brief 图像预处理，包括二值化、尺度变换
 *
 * @param lower_r
 * @param lower_g
 * @param lower_b
 * @param upper_r
 * @param upper_g
 * @param upper_b
 */
void CdemoDlg::PreProcessImage(const uchar lower_r, const uchar lower_g, const uchar lower_b, const uchar upper_r, const uchar upper_g, const uchar upper_b)
{
	static int first = 1;
	m_imagePtr_3C_raw = (uchar *)m_image_3C_raw.GetBits();

	m_width_raw = m_image_3C_raw.GetWidth();
	m_height_raw = m_image_3C_raw.GetHeight();
	m_width = m_width_raw / WF;
	m_height = m_height_raw / HF;
	if (first)
	{
		m_image_1C.CreateByPixelFormat(m_width, m_height, PixelFormat_Mono8);
		first = 0;
	}
	m_imagePtr_1C = (uchar *)m_image_1C.GetBits();

	int r_s, g_s, b_s;
	uchar r, g, b;
	for (int i = 0; i < m_width; i++)
	{
		for (int j = 0; j < m_height; j++)
		{
			r_s = 0;
			g_s = 0;
			b_s = 0;
			for (int u = 0; u < WF; u++)
			{
				for (int v = 0; v < HF; v++)
				{
					r_s += get_pixel_3C(m_imagePtr_3C_raw, i * WF + u, j * HF + v, 0, m_width_raw);
					g_s += get_pixel_3C(m_imagePtr_3C_raw, i * WF + u, j * HF + v, 1, m_width_raw);
					b_s += get_pixel_3C(m_imagePtr_3C_raw, i * WF + u, j * HF + v, 2, m_width_raw);
				}
			}
			r = (uchar)(r_s / (WF * HF));
			g = (uchar)(g_s / (WF * HF));
			b = (uchar)(b_s / (WF * HF));

			if (r < lower_r || r > upper_r)
			{
				set_pixel_1C(m_imagePtr_1C, i, j, 0, m_width);
				continue;
			}
			if (g < lower_g || g > upper_g)
			{
				set_pixel_1C(m_imagePtr_1C, i, j, 0, m_width);
				continue;
			}
			if (b < lower_b || b > upper_b)
			{
				set_pixel_1C(m_imagePtr_1C, i, j, 0, m_width);
				continue;
			}
			set_pixel_1C(m_imagePtr_1C, i, j, 255, m_width);
		}
	}
}

/**
 * @brief 图像处理
 *
 */
void CdemoDlg::ProcessImage()
{
	int all = CountBeansAll(DistanceTransformD4(m_imagePtr_1C, m_height, m_width), m_height, m_width, m_image_3C_raw);
	int big = CountBeansBig(m_imagePtr_1C, m_height, m_width, m_image_3C_raw);
	GetDlgItem(IDC_BIG_N)->SetWindowTextA(to_string(big).c_str());
	GetDlgItem(IDC_Small_N)->SetWindowTextA(to_string(all - big).c_str());
	return;
}