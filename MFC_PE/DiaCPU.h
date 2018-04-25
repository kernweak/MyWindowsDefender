#pragma once
#include "afxcmn.h"


// CDiaCPU 对话框

class CDiaCPU : public CDialogEx
{
	DECLARE_DYNAMIC(CDiaCPU)

public:
	CDiaCPU(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDiaCPU();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIACPU };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_Cpu;
	CListCtrl m_Memory;
	BOOL OnInitDialog();
	void showCPUandMem();
};
