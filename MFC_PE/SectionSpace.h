#pragma once
#include "afxcmn.h"


// CSectionSpace 对话框

class CSectionSpace : public CDialogEx
{
	DECLARE_DYNAMIC(CSectionSpace)

public:
	CSectionSpace(PIMAGE_NT_HEADERS32 pNt,CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CSectionSpace();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG3 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	PIMAGE_NT_HEADERS32 m_pNt;
	CListCtrl m_ListCtrl;
	BOOL OnInitDialog();
};
