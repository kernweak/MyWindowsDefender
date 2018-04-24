#pragma once


// CDiaRelocList 对话框

class CDiaRelocList : public CDialogEx
{
	DECLARE_DYNAMIC(CDiaRelocList)

public:
	CDiaRelocList(PIMAGE_NT_HEADERS32 pnt, BYTE *FileBuf,CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDiaRelocList();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIARELOCLIST };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString m_RelocList;
	PIMAGE_NT_HEADERS32 m_pNt;
	BYTE *m_FileBuf;
	BOOL OnInitDialog();
	void showReloc();
};
