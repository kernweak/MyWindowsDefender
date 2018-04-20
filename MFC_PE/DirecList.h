#pragma once


// CDirecList 对话框

class CDirecList : public CDialogEx
{
	DECLARE_DYNAMIC(CDirecList)

public:
	CDirecList(PIMAGE_NT_HEADERS32 pnt,CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDirecList();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIADIRECLIST };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	PIMAGE_NT_HEADERS32 m_pNt;
	BOOL OnInitDialog();
	CString m_EXPORT_DIRECTORY_RVA;
	CString m_EXPORT_DIRECTORY_Size;
	CString m_IMPORT_DIREC_RVA;
	CString m_IMPORT_DIREC_Size;
	CString m_RESORCE_RVA;
	CString m_RESORCE_Size;
	CString m_RUNTIME_RVA;
	CString m_RUNTIME_Size;
	CString m_SECURITY_RVA;
	CString m_SECURITY_Size;
	CString m_BASERELOC_RVA;
	CString m_BASERELOC_Size;
	CString m_DEBUG_RVA;
	CString m_DEBUG_Size;
	CString m_ARCHITECTURE_RVA;
	CString m_ARCHITECTURE_Size;
	CString m_GLOBAL_RVA;
	CString m_GLOBAL_Size;
	CString m_TLS_RVA;
	CString m_TLS_Size;
	CString m_LOAD_RVA;
	CString m_LOAD_Size;
	CString m_LoadScan_RVA;
	CString m_LoadScan_Size;
	CString m_IAT_RVA;
	CString m_IAT_Size;
	CString m_COM_RVA;
	CString m_COM_Size;
	CString m_SAVE_RVA;
	CString m_SAVE_Size;
	void ShowDirecList();
};
