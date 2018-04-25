
// MFC_PEDlg.h : 头文件
//

#pragma once
#include"PEeditor.h"

// CMFC_PEDlg 对话框
class CMFC_PEDlg : public CDialogEx
{
// 构造
public:
	CMFC_PEDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MFC_PE_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnDropFiles(HDROP hDropInfo);
	CString m_PEload;//文件的路径
	//afx_msg void OnBnClickedConfirm();
	afx_msg void OnBnClickedButton2();
	PIMAGE_NT_HEADERS32 m_pNt;
	PIMAGE_NT_HEADERS32 getPNT(CString m_PEload);
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedMCPU();
	afx_msg void OnBnClickedFile();

};
