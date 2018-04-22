#pragma once
#include "afxcmn.h"

// CImportList 对话框

class CImportList : public CDialogEx
{
	DECLARE_DYNAMIC(CImportList)

public:
	CImportList(PIMAGE_NT_HEADERS32 pnt, BYTE *FileBuf,CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CImportList();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DiaImportList };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_ListCtrl;// 
	BOOL OnInitDialog();
	BYTE *m_pFileBuf;
	PIMAGE_NT_HEADERS32 m_pNt;
	void showImport();
	CListCtrl m_CListCtrl2;
	afx_msg void OnNMClickList2(NMHDR *pNMHDR, LRESULT *pResult);
	CString temp2;
};
