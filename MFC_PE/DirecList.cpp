// DirecList.cpp : 实现文件
//

#include "stdafx.h"
#include "MFC_PE.h"
#include "DirecList.h"
#include "afxdialogex.h"


// CDirecList 对话框

IMPLEMENT_DYNAMIC(CDirecList, CDialogEx)

CDirecList::CDirecList(PIMAGE_NT_HEADERS32 pnt,CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIADIRECLIST, pParent),
	m_pNt(pnt)
	, m_EXPORT_DIRECTORY_RVA(_T(""))
	, m_EXPORT_DIRECTORY_Size(_T(""))
	, m_IMPORT_DIREC_RVA(_T(""))
	, m_IMPORT_DIREC_Size(_T(""))
	, m_RESORCE_RVA(_T(""))
	, m_RESORCE_Size(_T(""))
	, m_RUNTIME_RVA(_T(""))
	, m_RUNTIME_Size(_T(""))
	, m_SECURITY_RVA(_T(""))
	, m_SECURITY_Size(_T(""))
	, m_BASERELOC_RVA(_T(""))
	, m_BASERELOC_Size(_T(""))
	, m_DEBUG_RVA(_T(""))
	, m_DEBUG_Size(_T(""))
	, m_ARCHITECTURE_RVA(_T(""))
	, m_ARCHITECTURE_Size(_T(""))
	, m_GLOBAL_RVA(_T(""))
	, m_GLOBAL_Size(_T(""))
	, m_TLS_RVA(_T(""))
	, m_TLS_Size(_T(""))
	, m_LOAD_RVA(_T(""))
	, m_LOAD_Size(_T(""))
	, m_LoadScan_RVA(_T(""))
	, m_LoadScan_Size(_T(""))
	, m_IAT_RVA(_T(""))
	, m_IAT_Size(_T(""))
	, m_COM_RVA(_T(""))
	, m_COM_Size(_T(""))
	, m_SAVE_RVA(_T(""))
	, m_SAVE_Size(_T(""))
{

}

CDirecList::~CDirecList()
{
}

void CDirecList::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_EXPORT_DIRECTORY_RVA);
	DDX_Text(pDX, IDC_EDIT2, m_EXPORT_DIRECTORY_Size);
	DDX_Text(pDX, IDC_EDIT3, m_IMPORT_DIREC_RVA);
	DDX_Text(pDX, IDC_EDIT4, m_IMPORT_DIREC_Size);
	DDX_Text(pDX, IDC_EDIT5, m_RESORCE_RVA);
	DDX_Text(pDX, IDC_EDIT6, m_RESORCE_Size);
	DDX_Text(pDX, IDC_EDIT7, m_RUNTIME_RVA);
	DDX_Text(pDX, IDC_EDIT8, m_RUNTIME_Size);
	DDX_Text(pDX, IDC_EDIT9, m_SECURITY_RVA);
	DDX_Text(pDX, IDC_EDIT10, m_SECURITY_Size);
	DDX_Text(pDX, IDC_EDIT11, m_BASERELOC_RVA);
	DDX_Text(pDX, IDC_EDIT12, m_BASERELOC_Size);
	DDX_Text(pDX, IDC_EDIT14, m_DEBUG_RVA);
	DDX_Text(pDX, IDC_EDIT15, m_DEBUG_Size);
	DDX_Text(pDX, IDC_EDIT17, m_ARCHITECTURE_RVA);
	DDX_Text(pDX, IDC_EDIT18, m_ARCHITECTURE_Size);
	DDX_Text(pDX, IDC_EDIT19, m_GLOBAL_RVA);
	DDX_Text(pDX, IDC_EDIT20, m_GLOBAL_Size);
	DDX_Text(pDX, IDC_EDIT21, m_TLS_RVA);
	DDX_Text(pDX, IDC_EDIT22, m_TLS_Size);
	DDX_Text(pDX, IDC_EDIT23, m_LOAD_RVA);
	DDX_Text(pDX, IDC_EDIT24, m_LOAD_Size);
	DDX_Text(pDX, IDC_EDIT25, m_LoadScan_RVA);
	DDX_Text(pDX, IDC_EDIT26, m_LoadScan_Size);
	DDX_Text(pDX, IDC_EDIT27, m_IAT_RVA);
	DDX_Text(pDX, IDC_EDIT28, m_IAT_Size);
	DDX_Text(pDX, IDC_EDIT29, m_COM_RVA);
	DDX_Text(pDX, IDC_EDIT30, m_COM_Size);
	DDX_Text(pDX, IDC_EDIT31, m_SAVE_RVA);
	DDX_Text(pDX, IDC_EDIT32, m_SAVE_Size);
}

BOOL CDirecList::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	ShowDirecList();
	UpdateData(FALSE);
	return 0;
}

void CDirecList::ShowDirecList()
{
	PIMAGE_FILE_HEADER pFileHeader = &(m_pNt->FileHeader);
	PIMAGE_OPTIONAL_HEADER32 pOptionalHeader = &(m_pNt->OptionalHeader);
	//获取目录表
	PIMAGE_DATA_DIRECTORY pDataDirectory = pOptionalHeader->DataDirectory;
	m_EXPORT_DIRECTORY_RVA.Format(L"%08x", pDataDirectory[0].VirtualAddress);
	m_EXPORT_DIRECTORY_Size.Format(L"%08x", pDataDirectory[0].Size);m_EXPORT_DIRECTORY_RVA.Format(L"%08x", pDataDirectory[0].VirtualAddress);
	m_IMPORT_DIREC_RVA.Format(L"%08x", pDataDirectory[1].Size);
	m_IMPORT_DIREC_Size.Format(L"%08x", pDataDirectory[1].VirtualAddress);
	m_RESORCE_RVA.Format(L"%08x", pDataDirectory[2].Size);
	m_RESORCE_Size.Format(L"%08x", pDataDirectory[2].VirtualAddress);
	m_RUNTIME_RVA.Format(L"%08x", pDataDirectory[3].Size);
	m_RUNTIME_Size.Format(L"%08x", pDataDirectory[3].VirtualAddress);
	m_SECURITY_RVA.Format(L"%08x", pDataDirectory[4].Size);
	m_SECURITY_Size.Format(L"%08x", pDataDirectory[4].VirtualAddress);
	m_BASERELOC_RVA.Format(L"%08x", pDataDirectory[5].Size);
	m_BASERELOC_Size.Format(L"%08x", pDataDirectory[5].VirtualAddress);
	m_DEBUG_RVA.Format(L"%08x", pDataDirectory[6].Size);
	m_DEBUG_Size.Format(L"%08x", pDataDirectory[6].VirtualAddress);
	m_ARCHITECTURE_RVA.Format(L"%08x", pDataDirectory[7].Size);
	m_ARCHITECTURE_Size.Format(L"%08x", pDataDirectory[7].VirtualAddress);
	m_GLOBAL_RVA.Format(L"%08x", pDataDirectory[8].Size);
	m_GLOBAL_Size.Format(L"%08x", pDataDirectory[8].VirtualAddress);
	m_TLS_RVA.Format(L"%08x", pDataDirectory[9].Size);
	m_TLS_Size.Format(L"%08x", pDataDirectory[9].VirtualAddress);
	m_LOAD_RVA.Format(L"%08x", pDataDirectory[10].Size);
	m_LOAD_Size.Format(L"%08x", pDataDirectory[10].VirtualAddress);
	m_LoadScan_RVA.Format(L"%08x", pDataDirectory[11].Size);
	m_LoadScan_Size.Format(L"%08x", pDataDirectory[11].VirtualAddress);
	m_IAT_RVA.Format(L"%08x", pDataDirectory[12].Size);
	m_IAT_Size.Format(L"%08x", pDataDirectory[12].VirtualAddress);
	m_COM_RVA.Format(L"%08x", pDataDirectory[14].Size);
	m_COM_Size.Format(L"%08x", pDataDirectory[14].VirtualAddress);
	m_SAVE_RVA.Format(L"%08x", pDataDirectory[13].Size);
	m_SAVE_Size.Format(L"%08x", pDataDirectory[13].VirtualAddress);

}



BEGIN_MESSAGE_MAP(CDirecList, CDialogEx)
END_MESSAGE_MAP()


// CDirecList 消息处理程序
