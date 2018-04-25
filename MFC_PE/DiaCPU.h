#pragma once
#include "afxcmn.h"


// CDiaCPU �Ի���

class CDiaCPU : public CDialogEx
{
	DECLARE_DYNAMIC(CDiaCPU)

public:
	CDiaCPU(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDiaCPU();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIACPU };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_Cpu;
	CListCtrl m_Memory;
	BOOL OnInitDialog();
	void showCPUandMem();
};
