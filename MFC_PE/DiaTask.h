#pragma once
#include "afxcmn.h"


// CDiaTask 对话框

class CDiaTask : public CDialogEx
{
	DECLARE_DYNAMIC(CDiaTask)

public:
	CDiaTask(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDiaTask();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG5 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_Process;
	CListCtrl m_Thread;
	CListCtrl m_Module;
	CListCtrl m_Windows;
	CString m_pocessID;
	void showProcessE();
	void showThreadE();
	void showModuleE();
	void showWindowE();
	BOOL OnInitDialog();
	void showProcess();
	static int m;
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnNMClickGetModule(NMHDR *pNMHDR, LRESULT *pResult);
};
