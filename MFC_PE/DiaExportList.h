#pragma once


// CDiaExportList �Ի���

class CDiaExportList : public CDialogEx
{
	DECLARE_DYNAMIC(CDiaExportList)

public:
	CDiaExportList(PIMAGE_NT_HEADERS32 pnt, BYTE *FileBuf,CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDiaExportList();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIAEXPORT };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CString m_ExportList;
	PIMAGE_NT_HEADERS32 m_pNt;
	BYTE *m_FileBuf;
	BOOL OnInitDialog();
	void showExport();
};
