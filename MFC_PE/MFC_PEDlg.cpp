
// MFC_PEDlg.cpp : ʵ���ļ�
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


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
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


// CMFC_PEDlg �Ի���



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


// CMFC_PEDlg ��Ϣ�������

BOOL CMFC_PEDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CMFC_PEDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CMFC_PEDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CMFC_PEDlg::OnDropFiles(HDROP hDropInfo)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	CDialogEx::OnDropFiles(hDropInfo);
	int DropCount = DragQueryFile(hDropInfo, -1, NULL, 0);//ȡ�ñ��϶��ļ�����Ŀ
	for (int i = 0;i < DropCount;i++) {
		WCHAR wcStr[MAX_PATH];
		DragQueryFile(hDropInfo, i, wcStr, MAX_PATH);//�����ק�ĵ�i���ļ����ļ���
		//��Ҫ��������
		m_PEload = wcStr;
		CPEeditor EPeditor1(m_pNt);
		EPeditor1.m_PEload = m_PEload;
		EPeditor1.DoModal();
		DragFinish(hDropInfo); //��ק�������ͷ��ڴ�
	}
}


//void CMFC_PEDlg::OnBnClickedConfirm()
//{
//	// TODO: �ڴ���ӿؼ�֪ͨ����������
//	UpdateData(TRUE);
//	CPEeditor EPeditor1;
//	EPeditor1.m_PEload = m_PEload;
//	EPeditor1.DoModal();
//}


void CMFC_PEDlg::OnBnClickedButton2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	TCHAR szDir[MAX_PATH] = { 0 };
	//���ļ�
	OPENFILENAME ofn = { sizeof(OPENFILENAME) };
	TCHAR szFileName[MAX_PATH] = { 0 };
	ofn.Flags = OFN_HIDEREADONLY;
	ofn.hwndOwner = NULL;
	TCHAR szFilter[] = TEXT("EXE Files(*.exe)\0*.exe\0")TEXT("DLL Files(*.dll)\0*.dll\0")TEXT("All Files(*.*)\0*.*\0\0");
	ofn.lpstrFilter = szFilter;
	ofn.nMaxFile = MAX_PATH;
	ofn.nMaxFileTitle = MAX_PATH;
	//���������ļ�·��
	ofn.lpstrFile = szDir;
	ofn.lpstrFileTitle = szFileName;
	ofn.lpstrInitialDir = szDir;
	GetOpenFileName(&ofn);
	m_PEload = ofn.lpstrFile;
	m_pNt=getPNT(m_PEload);
	//���ȷ������
	CPEeditor EPeditor1(m_pNt);
	EPeditor1.m_PEload = m_PEload;
	EPeditor1.DoModal();
}

PIMAGE_NT_HEADERS32 CMFC_PEDlg::getPNT(CString m_PEload)
{
	//��ȡ�ļ����
	HANDLE hFile = CreateFile((m_PEload.GetBuffer()), GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	//��ȡ�ļ���С
	DWORD dwFileSize = GetFileSize(hFile, NULL);
	//����һ���ѿռ���
	CHAR *pFileBuf = new CHAR[dwFileSize];

	DWORD ReadFileSize = 0;
	//���ļ������ڴ�
	ReadFile(hFile, pFileBuf, dwFileSize, &ReadFileSize, NULL);
	//�ж�DOSͷ
	PIMAGE_DOS_HEADER pFile = (PIMAGE_DOS_HEADER)pFileBuf;
	if (pFile->e_magic != IMAGE_DOS_SIGNATURE) {//0x5A4D
		cout << "�ⲻ��һ��PE�ļ�" << endl;
		return 0;
	}
	//�ж�PEͷ��
	DWORD dwNewPos = (DWORD)pFileBuf + ((PIMAGE_DOS_HEADER)pFileBuf)->e_lfanew;
	PIMAGE_NT_HEADERS32 pNTHeader = (PIMAGE_NT_HEADERS32)(dwNewPos);
	CloseHandle(hFile);
	return pNTHeader;

}


void CMFC_PEDlg::OnBnClickedButton1()//�������������
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CDiaTask temp1;
	temp1.DoModal();
}


void CMFC_PEDlg::OnBnClickedMCPU()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CDiaCPU tempCpu;
	//tempCpu.m_Flag = 0;
	//tempCpu.DoModal();
	CDiaCPU *pTD = new CDiaCPU();
	pTD->Create(IDD_DIACPU); //����һ����ģ̬�Ի���  
	pTD->ShowWindow(SW_SHOWNORMAL);
}
