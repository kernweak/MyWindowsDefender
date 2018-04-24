#pragma once


// CDiaDelay �Ի���

class CDiaDelay : public CDialogEx
{
	DECLARE_DYNAMIC(CDiaDelay)

public:
	CDiaDelay(PIMAGE_NT_HEADERS32 pnt, BYTE *FileBuf, CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDiaDelay();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG4 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	PIMAGE_NT_HEADERS32 m_pNt;
	BYTE *m_pFileBuf;
	BOOL OnInitDialog();
	void showDelay();
	CString m_DllNameRVA;
	CString m_ModuleHandleRVA;
	CString m_ImportAddressTableRVA;
	CString m_ImportNameTableRVA;
	CString m_BoundImportAddressTableRVA;
	CString m_UnLoadInformationTableRVA;
	CString m_TimeDateStamp;
	CString m_AllAttributes;
};
