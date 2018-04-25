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
	BOOL OnInitDialog();
	afx_msg void OnNMClickMfcshelltree2(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMDblclkMfcshelltree2(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnTvnSelchangedMfcshelltree2(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDropFiles(HDROP hDropInfo);
	int m_flag = 0;
	CString wenJianMing;//文件名
	afx_msg void OnNMClickMfcshelllist2(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedFileInformation();
	void EnumLoad(TCHAR* load);
	void deleteFolder(TCHAR* load);
	afx_msg void OnBnClickedVSClean();
	int cleanNumber = 0;//清理的文件夹数量
	int cleanNumber1 = 0;//清理文件数量
};
