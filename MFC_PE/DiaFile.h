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
	BOOL OnInitDialog();
	afx_msg void OnNMClickMfcshelltree2(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMDblclkMfcshelltree2(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnTvnSelchangedMfcshelltree2(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDropFiles(HDROP hDropInfo);
	int m_flag = 0;
	CString wenJianMing;//�ļ���
	afx_msg void OnNMClickMfcshelllist2(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedFileInformation();
	void EnumLoad(TCHAR* load);
	void deleteFolder(TCHAR* load);
	afx_msg void OnBnClickedVSClean();
	int cleanNumber = 0;//������ļ�������
	int cleanNumber1 = 0;//�����ļ�����
};
