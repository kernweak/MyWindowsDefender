// DiaDelay.cpp : 实现文件
//

#include "stdafx.h"
#include "MFC_PE.h"
#include "DiaDelay.h"
#include "afxdialogex.h"
#include "PECalcu.h"


// CDiaDelay 对话框

IMPLEMENT_DYNAMIC(CDiaDelay, CDialogEx)

CDiaDelay::CDiaDelay(PIMAGE_NT_HEADERS32 pnt, BYTE *FileBuf, CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG4, pParent)
	, m_pNt(pnt)
	, m_pFileBuf(FileBuf)
	, m_DllNameRVA(_T(""))
	, m_ModuleHandleRVA(_T(""))
	, m_ImportAddressTableRVA(_T(""))
	, m_ImportNameTableRVA(_T(""))
	, m_BoundImportAddressTableRVA(_T(""))
	, m_UnLoadInformationTableRVA(_T(""))
	, m_TimeDateStamp(_T(""))
	, m_AllAttributes(_T(""))
{

}

CDiaDelay::~CDiaDelay()
{
}

void CDiaDelay::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_DllNameRVA);
	DDX_Text(pDX, IDC_EDIT2, m_ModuleHandleRVA);
	DDX_Text(pDX, IDC_EDIT3, m_ImportAddressTableRVA);
	DDX_Text(pDX, IDC_EDIT4, m_ImportNameTableRVA);
	DDX_Text(pDX, IDC_EDIT5, m_BoundImportAddressTableRVA);
	DDX_Text(pDX, IDC_EDIT6, m_UnLoadInformationTableRVA);
	DDX_Text(pDX, IDC_EDIT7, m_TimeDateStamp);
	DDX_Text(pDX, IDC_EDIT8, m_AllAttributes);
}

BOOL CDiaDelay::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	showDelay();
	UpdateData(FALSE);
	return 0;
}

void CDiaDelay::showDelay()
{
	CPECalcu temp1(m_pNt);
	//找到资源表
	DWORD dwResRVA =
		m_pNt->OptionalHeader.DataDirectory[13].VirtualAddress;
	DWORD dwResFOA = (DWORD)(temp1.RVAtoFOA(dwResRVA) + m_pFileBuf);
	PIMAGE_DELAYLOAD_DESCRIPTOR pRes = (PIMAGE_DELAYLOAD_DESCRIPTOR)dwResFOA;
	m_DllNameRVA.Format(L"%08X", pRes->DllNameRVA);
	m_ModuleHandleRVA.Format(L"%08X", pRes->ModuleHandleRVA);
	m_ImportAddressTableRVA.Format(L"%08X", pRes->ImportAddressTableRVA);
	m_ImportNameTableRVA.Format(L"%08X", pRes->ImportNameTableRVA);
	m_BoundImportAddressTableRVA.Format(L"%08X", pRes->BoundImportAddressTableRVA);
	m_UnLoadInformationTableRVA.Format(L"%08X", pRes->UnloadInformationTableRVA);
	m_TimeDateStamp.Format(L"%08X", pRes->TimeDateStamp);
	m_AllAttributes.Format(L"%08X", pRes->Attributes);
}


BEGIN_MESSAGE_MAP(CDiaDelay, CDialogEx)
END_MESSAGE_MAP()




// CDiaDelay 消息处理程序
