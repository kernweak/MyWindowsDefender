#pragma once
#include "ClientScoket.h"
#include "afxwin.h"

// CDiaVirus �Ի���

class CDiaVirus : public CDialogEx
{
	DECLARE_DYNAMIC(CDiaVirus)

public:
	CDiaVirus(CClientScoket* pClient,CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDiaVirus();
	CDiaVirus();
// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIAVIRUS};
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedWhiteList();
	
	CString m_FileInfor;
	CString m_FileInfor1;//�����ļ�·��������ж��ǲ���Ҫ�������ɾ������
	CString m_MD5;
	afx_msg void OnDropFiles(HDROP hDropInfo);
	afx_msg void OnBnClickedMD5();
	CClientScoket *m_pClient;
protected:
	afx_msg LRESULT OnMysocket(WPARAM wParam, LPARAM lParam);
public:
	CEdit m_Result;
	CString m_Result1;
	int m_flag = 0;
	afx_msg void OnBnClickedAllScan();
	CString m_AllScanLoad;
	CString m_Load;//���浱ǰ�ļ�·��
	void Enumload(TCHAR * load);
	CString m_ListPathName;
	CListBox m_ListPathNameC;
};
