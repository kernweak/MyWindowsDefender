#pragma once


// CPECalcu �Ի���

class CPECalcu : public CDialogEx
{
	DECLARE_DYNAMIC(CPECalcu)

public:
	CPECalcu(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CPECalcu();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIACALCU };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CString m_VA;
	CString m_RVA;
	CString m_FOA;
	afx_msg void OnBnClickedVA();
	afx_msg void OnBnClickedRVA();
	afx_msg void OnBnClickedFOA();
};
