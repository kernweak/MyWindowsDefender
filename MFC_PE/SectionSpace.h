#pragma once
#include "afxcmn.h"


// CSectionSpace �Ի���

class CSectionSpace : public CDialogEx
{
	DECLARE_DYNAMIC(CSectionSpace)

public:
	CSectionSpace(PIMAGE_NT_HEADERS32 pNt,CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CSectionSpace();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG3 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	PIMAGE_NT_HEADERS32 m_pNt;
	CListCtrl m_ListCtrl;
	BOOL OnInitDialog();
};
