#pragma once


// CDiaRelocList �Ի���

class CDiaRelocList : public CDialogEx
{
	DECLARE_DYNAMIC(CDiaRelocList)

public:
	CDiaRelocList(PIMAGE_NT_HEADERS32 pnt, BYTE *FileBuf,CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDiaRelocList();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIARELOCLIST };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CString m_RelocList;
	PIMAGE_NT_HEADERS32 m_pNt;
	BYTE *m_FileBuf;
	BOOL OnInitDialog();
	void showReloc();
};
