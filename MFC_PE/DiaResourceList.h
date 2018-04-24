#pragma once


// CDiaResourceList 对话框

class CDiaResourceList : public CDialogEx
{
	DECLARE_DYNAMIC(CDiaResourceList)

public:
	CDiaResourceList(PIMAGE_NT_HEADERS32 pnt, BYTE *FileBuf, CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDiaResourceList();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIARESOURCELIST };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString m_ResourceList;
	PIMAGE_NT_HEADERS32 m_pNt;
	BYTE *m_FileBuf;
	BOOL OnInitDialog();
	void showResource();
};
