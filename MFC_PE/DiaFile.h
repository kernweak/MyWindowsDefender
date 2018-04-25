#pragma once
#include "afxshelltreectrl.h"
#include "afxshelllistctrl.h"


// CDiaFile 对话框

class CDiaFile : public CDialogEx
{
	DECLARE_DYNAMIC(CDiaFile)

public:
	CDiaFile(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDiaFile();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIAFILE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CMFCShellTreeCtrl m_SheelTree;
	CMFCShellListCtrl m_ShellList;
	CString m_foldload;
	CString m_wenJianXinXi;
};
