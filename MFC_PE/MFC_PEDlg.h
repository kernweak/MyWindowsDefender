
// MFC_PEDlg.h : ͷ�ļ�
//

#pragma once
#include"PEeditor.h"

// CMFC_PEDlg �Ի���
class CMFC_PEDlg : public CDialogEx
{
// ����
public:
	CMFC_PEDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MFC_PE_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnDropFiles(HDROP hDropInfo);
	CString m_PEload;//�ļ���·��
	//afx_msg void OnBnClickedConfirm();
	afx_msg void OnBnClickedButton2();
	PIMAGE_NT_HEADERS32 m_pNt;
	PIMAGE_NT_HEADERS32 getPNT(CString m_PEload);
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedMCPU();
	afx_msg void OnBnClickedFile();

};
