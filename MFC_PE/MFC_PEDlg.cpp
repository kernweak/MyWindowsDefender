
// MFC_PEDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "MFC_PE.h"
#include "MFC_PEDlg.h"
#include "afxdialogex.h"
#include "DiaTask.h"
#include "DiaCPU.h"

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
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CMFC_PEDlg 对话框



CMFC_PEDlg::CMFC_PEDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_MFC_PE_DIALOG, pParent)
	, m_PEload(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMFC_PEDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_MFCEDITBROWSE1, m_PEload);
}

BEGIN_MESSAGE_MAP(CMFC_PEDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_DROPFILES()
	//ON_BN_CLICKED(IDC_BUTTON1, &CMFC_PEDlg::OnBnClickedConfirm)
	ON_BN_CLICKED(IDC_BUTTON2, &CMFC_PEDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON1, &CMFC_PEDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON3, &CMFC_PEDlg::OnBnClickedMCPU)
END_MESSAGE_MAP()


// CMFC_PEDlg 消息处理程序

BOOL CMFC_PEDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
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
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CMFC_PEDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CMFC_PEDlg::OnPaint()
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
HCURSOR CMFC_PEDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CMFC_PEDlg::OnDropFiles(HDROP hDropInfo)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	CDialogEx::OnDropFiles(hDropInfo);
	int DropCount = DragQueryFile(hDropInfo, -1, NULL, 0);//取得被拖动文件的数目
	for (int i = 0;i < DropCount;i++) {
		WCHAR wcStr[MAX_PATH];
		DragQueryFile(hDropInfo, i, wcStr, MAX_PATH);//获得拖拽的第i个文件的文件名
		//需要做的事情
		m_PEload = wcStr;
		CPEeditor EPeditor1(m_pNt);
		EPeditor1.m_PEload = m_PEload;
		EPeditor1.DoModal();
		DragFinish(hDropInfo); //拖拽结束后，释放内存
	}
}


//void CMFC_PEDlg::OnBnClickedConfirm()
//{
//	// TODO: 在此添加控件通知处理程序代码
//	UpdateData(TRUE);
//	CPEeditor EPeditor1;
//	EPeditor1.m_PEload = m_PEload;
//	EPeditor1.DoModal();
//}


void CMFC_PEDlg::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码

	TCHAR szDir[MAX_PATH] = { 0 };
	//打开文件
	OPENFILENAME ofn = { sizeof(OPENFILENAME) };
	TCHAR szFileName[MAX_PATH] = { 0 };
	ofn.Flags = OFN_HIDEREADONLY;
	ofn.hwndOwner = NULL;
	TCHAR szFilter[] = TEXT("EXE Files(*.exe)\0*.exe\0")TEXT("DLL Files(*.dll)\0*.dll\0")TEXT("All Files(*.*)\0*.*\0\0");
	ofn.lpstrFilter = szFilter;
	ofn.nMaxFile = MAX_PATH;
	ofn.nMaxFileTitle = MAX_PATH;
	//用来保存文件路径
	ofn.lpstrFile = szDir;
	ofn.lpstrFileTitle = szFileName;
	ofn.lpstrInitialDir = szDir;
	GetOpenFileName(&ofn);
	m_PEload = ofn.lpstrFile;
	m_pNt=getPNT(m_PEload);
	//点击确定跳出
	CPEeditor EPeditor1(m_pNt);
	EPeditor1.m_PEload = m_PEload;
	EPeditor1.DoModal();
}

PIMAGE_NT_HEADERS32 CMFC_PEDlg::getPNT(CString m_PEload)
{
	//获取文件句柄
	HANDLE hFile = CreateFile((m_PEload.GetBuffer()), GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	//获取文件大小
	DWORD dwFileSize = GetFileSize(hFile, NULL);
	//创建一个堆空间存放
	CHAR *pFileBuf = new CHAR[dwFileSize];

	DWORD ReadFileSize = 0;
	//将文件读入内存
	ReadFile(hFile, pFileBuf, dwFileSize, &ReadFileSize, NULL);
	//判断DOS头
	PIMAGE_DOS_HEADER pFile = (PIMAGE_DOS_HEADER)pFileBuf;
	if (pFile->e_magic != IMAGE_DOS_SIGNATURE) {//0x5A4D
		cout << "这不是一个PE文件" << endl;
		return 0;
	}
	//判断PE头部
	DWORD dwNewPos = (DWORD)pFileBuf + ((PIMAGE_DOS_HEADER)pFileBuf)->e_lfanew;
	PIMAGE_NT_HEADERS32 pNTHeader = (PIMAGE_NT_HEADERS32)(dwNewPos);
	CloseHandle(hFile);
	return pNTHeader;

}


void CMFC_PEDlg::OnBnClickedButton1()//按下任务管理器
{
	// TODO: 在此添加控件通知处理程序代码
	CDiaTask temp1;
	temp1.DoModal();
}


void CMFC_PEDlg::OnBnClickedMCPU()
{
	// TODO: 在此添加控件通知处理程序代码
	CDiaCPU tempCpu;
	//tempCpu.m_Flag = 0;
	//tempCpu.DoModal();
	CDiaCPU *pTD = new CDiaCPU();
	pTD->Create(IDD_DIACPU); //创建一个非模态对话框  
	pTD->ShowWindow(SW_SHOWNORMAL);
}
