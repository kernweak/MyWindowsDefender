#pragma once


// CDiaResourceList �Ի���

class CDiaResourceList : public CDialogEx
{
	DECLARE_DYNAMIC(CDiaResourceList)

public:
	CDiaResourceList(PIMAGE_NT_HEADERS32 pnt, BYTE *FileBuf, CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDiaResourceList();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIARESOURCELIST };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CString m_ResourceList;
	PIMAGE_NT_HEADERS32 m_pNt;
	BYTE *m_FileBuf;
	BOOL OnInitDialog();
	void showResource();
};
