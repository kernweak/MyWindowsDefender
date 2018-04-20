// SectionSpace.cpp : 实现文件
//

#include "stdafx.h"
#include "MFC_PE.h"
#include "SectionSpace.h"
#include "afxdialogex.h"
#include<CommCtrl.h>

// CSectionSpace 对话框

IMPLEMENT_DYNAMIC(CSectionSpace, CDialogEx)

CSectionSpace::CSectionSpace(PIMAGE_NT_HEADERS32 pNt,CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG3, pParent),
	m_pNt(pNt)
{

}

CSectionSpace::~CSectionSpace()
{
}

void CSectionSpace::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST4, m_ListCtrl);
}


BEGIN_MESSAGE_MAP(CSectionSpace, CDialogEx)
END_MESSAGE_MAP()

BOOL CSectionSpace::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	CRect rectLocal;
	m_ListCtrl.GetClientRect(rectLocal);
	m_ListCtrl.SetColumnWidth(0, rectLocal.Width() / 6);         //设置列的宽度。
	m_ListCtrl.SetColumnWidth(1, rectLocal.Width() / 6);
	m_ListCtrl.SetColumnWidth(2, rectLocal.Width() / 6);
	m_ListCtrl.SetColumnWidth(4, rectLocal.Width() / 6);
	m_ListCtrl.SetColumnWidth(5, rectLocal.Width() / 6);
	m_ListCtrl.SetColumnWidth(5, rectLocal.Width() / 6);
	m_ListCtrl.InsertColumn(0, _T("名称"), LVCFMT_CENTER, 200, -1);
	m_ListCtrl.InsertColumn(1, _T("VOFFSET"), LVCFMT_CENTER, 200, -1);
	m_ListCtrl.InsertColumn(2, _T("VSIZE"), LVCFMT_CENTER, 200, -1);
	m_ListCtrl.InsertColumn(3, L"ROFFSET", LVCFMT_CENTER, 200, -1);
	m_ListCtrl.InsertColumn(4, _T("RSIZE"), LVCFMT_CENTER, 200, -1);
	m_ListCtrl.InsertColumn(5, _T("标志"), LVCFMT_CENTER, 200, -1);

	PIMAGE_SECTION_HEADER pSectionHeader = IMAGE_FIRST_SECTION(m_pNt);
	int i = 0;
	LV_ITEM li = {};
	char strName[9] = {};
	int nCountOfSection = m_pNt->FileHeader.NumberOfSections;
	for (int i = 0; i < nCountOfSection; i++)
	{
		memcpy(strName, pSectionHeader->Name, 8);
		CString str(strName);
		m_ListCtrl.InsertItem(i, str);
		str.Format(L"%08X", pSectionHeader->VirtualAddress);
		m_ListCtrl.SetItemText(i, 1, str);
		str.Format(L"%08X", pSectionHeader->Misc.VirtualSize);
		m_ListCtrl.SetItemText(i, 2, str);
		str.Format(L"%08X", pSectionHeader->PointerToRawData);
		m_ListCtrl.SetItemText(i, 3, str);
		str.Format(L"%08X", pSectionHeader->SizeOfRawData);
		m_ListCtrl.SetItemText(i, 4, str);
		str.Format(L"%08X", pSectionHeader->Characteristics );
		m_ListCtrl.SetItemText(i, 5, str);
		pSectionHeader++;
	}

	UpdateData(FALSE);
	return 0;
}
