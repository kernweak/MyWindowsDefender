// DiaTLS.cpp : 实现文件
//

#include "stdafx.h"
#include "MFC_PE.h"
#include "DiaTLS.h"
#include "afxdialogex.h"
#include "PECalcu.h"


// CDiaTLS 对话框

IMPLEMENT_DYNAMIC(CDiaTLS, CDialogEx)

CDiaTLS::CDiaTLS(PIMAGE_NT_HEADERS32 pnt, BYTE *FileBuf,CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIATLS, pParent)
	, m_pNt(pnt)
	,m_pFileBuf(FileBuf)
	, m_StartAddressOfRawData(_T(""))
	, m_EndAddressOfRawData(_T(""))
	, m_AddressOfIndex(_T(""))
	, m_AddressOfCallBacks(_T(""))
	, m_SizeOfZeroFill(_T(""))
	, m_Characteristics(_T(""))
{

}

CDiaTLS::~CDiaTLS()
{
}

void CDiaTLS::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_StartAddressOfRawData);
	DDX_Text(pDX, IDC_EDIT2, m_EndAddressOfRawData);
	DDX_Text(pDX, IDC_EDIT3, m_AddressOfIndex);
	DDX_Text(pDX, IDC_EDIT4, m_AddressOfCallBacks);
	DDX_Text(pDX, IDC_EDIT6, m_SizeOfZeroFill);
	DDX_Text(pDX, IDC_EDIT5, m_Characteristics);
}

BOOL CDiaTLS::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	showTLS();
	UpdateData(FALSE);
	return 0;
}

void CDiaTLS::showTLS()
{
	CPECalcu temp1(m_pNt);
	//找到资源表
	DWORD dwResRVA =
		m_pNt->OptionalHeader.DataDirectory[9 ].VirtualAddress;
	DWORD dwResFOA = (DWORD)(temp1.RVAtoFOA(dwResRVA) + m_pFileBuf);
	PIMAGE_TLS_DIRECTORY32 pRes = (PIMAGE_TLS_DIRECTORY32)dwResFOA;
	m_StartAddressOfRawData.Format(L"%x", pRes->StartAddressOfRawData);
	m_EndAddressOfRawData.Format(L"%x", pRes->EndAddressOfRawData);
	m_AddressOfIndex.Format(L"%x", pRes->AddressOfIndex);
	m_AddressOfCallBacks.Format(L"%x", pRes->AddressOfCallBacks);
	m_SizeOfZeroFill.Format(L"%x", pRes->SizeOfZeroFill);
	m_Characteristics.Format(L"%x", pRes->Characteristics);

}


BEGIN_MESSAGE_MAP(CDiaTLS, CDialogEx)
END_MESSAGE_MAP()


// CDiaTLS 消息处理程序
