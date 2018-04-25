#pragma once
#include "afxshelltreectrl.h"
#include "afxshelllistctrl.h"


// CDiaFile �Ի���

class CDiaFile : public CDialogEx
{
	DECLARE_DYNAMIC(CDiaFile)

public:
	CDiaFile(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDiaFile();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIAFILE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CMFCShellTreeCtrl m_SheelTree;
	CMFCShellListCtrl m_ShellList;
	CString m_foldload;
	CString m_wenJianXinXi;
};
