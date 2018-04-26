#pragma once
#include "afxcmn.h"


// CDiaServerAndRegister �Ի���

class CDiaServerAndRegister : public CDialogEx
{
	DECLARE_DYNAMIC(CDiaServerAndRegister)

public:
	CDiaServerAndRegister(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDiaServerAndRegister();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIASERVER };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	BOOL OnInitDialog();
	void showServer();
	CListCtrl m_ListServer;
	int m_count = -1;
	afx_msg void OnNMClickList1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedStart();
	afx_msg void OnBnClickedClose();
};
