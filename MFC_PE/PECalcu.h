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
	BYTE* g_pFileImageBase = 0;
	PIMAGE_NT_HEADERS32 m_pNt = 0;
	CString m_PEload;
	BOOL OnInitDialog();
	afx_msg void OnBnClickedCalcu();
	DWORD VAtoFOA(DWORD VA);
	DWORD VAtoRVA(DWORD VA);
	DWORD RVAtoFOA(DWORD dwRVA);
	DWORD FOAtoRVA(DWORD dwRVA);
	DWORD RVAtoVA(DWORD dwRVA);
	DWORD FOAtoVA(DWORD dwFOA);
	//PIMAGE_NT_HEADERS32 m_pNt;
	int temp = -1;//������������ĸ�ֵ��
};

