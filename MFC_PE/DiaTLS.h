#pragma once


// CDiaTLS 对话框

class CDiaTLS : public CDialogEx
{
	DECLARE_DYNAMIC(CDiaTLS)

public:
	CDiaTLS(PIMAGE_NT_HEADERS32 pnt, BYTE *FileBuf,CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDiaTLS();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIATLS };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString m_StartAddressOfRawData;
	CString m_EndAddressOfRawData;
	CString m_AddressOfIndex;
	CString m_AddressOfCallBacks;
	CString m_SizeOfZeroFill;
	CString m_Characteristics;
	PIMAGE_NT_HEADERS32 m_pNt;
	BYTE *m_pFileBuf;
	BOOL OnInitDialog();
	void showTLS();
};
