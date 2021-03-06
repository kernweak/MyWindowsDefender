// SoftMgr.cpp : 实现文件
//

#include "stdafx.h"
#include "MFC_PE.h"
#include "SoftMgr.h"
#include "afxdialogex.h"


// CSoftMgr 对话框

IMPLEMENT_DYNAMIC(CSoftMgr, CDialogEx)

CSoftMgr::CSoftMgr(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIASOFTWARE, pParent)
{
	m_SubKey1 = L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall";
	m_SubKey2 = L"SOFTWARE\\Wow6432Node\\Microsoft\\Windows\\CurrentVersion\\Uninstall";
	m_hRootKey = HKEY_LOCAL_MACHINE;

	m_KeyName = L"DisplayName";
	m_KeyNameVer = L"DisplayVersion";
	m_KeyNameSize = L"EstimatedSize";
	m_KeyNameDate = L"InstallDate";
	m_KeyNamePub = L"Publisher";
	m_KeyNameInsPath = L"InstallLocation";
	m_KeyNameUniPath = L"UninstallString";
}

CSoftMgr::~CSoftMgr()
{
}

void CSoftMgr::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_list_soft);
}

BOOL CSoftMgr::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	DWORD dwOldStyle = m_list_soft.GetExtendedStyle();
	
	m_list_soft.SetExtendedStyle(
		dwOldStyle | LVS_EX_FULLROWSELECT |
		LVS_EX_GRIDLINES |
		LVS_EX_CHECKBOXES
	);
	CRect rc;
	m_list_soft.GetClientRect(rc);
	int nWidth = rc.Width();

	m_list_soft.InsertColumn(0, L"名称", LVCFMT_CENTER, 250, -1);
	m_list_soft.InsertColumn(1, L"版本", LVCFMT_CENTER, 50, -1);
	m_list_soft.InsertColumn(2, L"安装日期", LVCFMT_CENTER, 90, -1);
	m_list_soft.InsertColumn(3, L"大小", LVCFMT_CENTER, 70, -1);
	m_list_soft.InsertColumn(4, L"安装路径", LVCFMT_CENTER, 250, -1);
	m_list_soft.InsertColumn(5, L"卸载路径", LVCFMT_CENTER, 250, -1);


	//获取软件信息
	UpdateSoftInfo();
	return 0;
}


BEGIN_MESSAGE_MAP(CSoftMgr, CDialogEx)
	ON_NOTIFY(NM_RCLICK, IDC_LIST1, &CSoftMgr::OnNMRClickListSoftware)
END_MESSAGE_MAP()


// CSoftMgr 消息处理程序


void CSoftMgr::GetSoftwareInfo(CString SubKey)
{
	//定义相关信息
	HKEY hKey = NULL;
	HKEY hSubKey = NULL;
	LONG lRet;
	CString strBuffer;
	CString strMidReg;

	DWORD dwIndex = 0;
	DWORD dwSoftSize = 0;
	WCHAR szKeyName[MAX_PATH] = { 0 };
	WCHAR szBuffer[MAX_PATH] = { 0 };
	DWORD dwKeyLen = MAXBYTE;
	DWORD dwNameLen = MAXBYTE;
	DWORD dwType = REG_DWORD | REG_EXPAND_SZ | REG_SZ;

	//打开子键
	lRet = RegOpenKeyEx(m_hRootKey, SubKey, 0, KEY_READ | KEY_WOW64_64KEY, &hKey);

	if (lRet == ERROR_SUCCESS)
	{
		//枚举软件目录子键
		while (ERROR_NO_MORE_ITEMS != RegEnumKeyEx(hKey, dwIndex, szKeyName, &dwKeyLen, 0, NULL, NULL, NULL))
		{

			strBuffer.Format(L"%s", szKeyName);
			if (!strBuffer.IsEmpty())
			{
				//格式化拼接子键
				strMidReg = SubKey + L"\\" + strBuffer;
				//打开单个软件子键
				if (ERROR_SUCCESS == RegOpenKeyEx(m_hRootKey, strMidReg, 0, KEY_READ | KEY_WOW64_64KEY, &hSubKey))
				{
					//获取信息
					SOFTINFO1 SoftInfo = { 0 };

					//名称
					RegQueryValueEx(hSubKey, m_KeyName, 0, &dwType, (LPBYTE)SoftInfo.szSoftName, &dwNameLen);
					dwNameLen = MAXBYTE;	//需要恢复此值

											//厂商
					RegQueryValueEx(hSubKey, m_KeyNamePub, 0, &dwType, (LPBYTE)SoftInfo.szSoftPub, &dwNameLen);
					dwNameLen = MAXBYTE;

					//日期
					RegQueryValueEx(hSubKey, m_KeyNameDate, 0, &dwType, (LPBYTE)SoftInfo.szSoftDate, &dwNameLen);
					dwNameLen = MAXBYTE;

					//版本
					RegQueryValueEx(hSubKey, m_KeyNameVer, 0, &dwType, (LPBYTE)SoftInfo.szSoftVer, &dwNameLen);
					dwNameLen = MAXBYTE;

					//安装路径
					RegQueryValueEx(hSubKey, m_KeyNameInsPath, 0, &dwType, (LPBYTE)SoftInfo.strSoftInsPath, &dwNameLen);
					dwNameLen = MAXBYTE;

					//卸载路径
					RegQueryValueEx(hSubKey, m_KeyNameUniPath, 0, &dwType, (LPBYTE)SoftInfo.strSoftUniPath, &dwNameLen);
					dwNameLen = MAXBYTE;

					//大小为DWORD值
					RegQueryValueEx(hSubKey, m_KeyNameSize, 0, &dwType, (LPBYTE)&dwSoftSize, &dwNameLen);
					dwNameLen = MAXBYTE;

					//转换文件大小
					CString strSize;
					if (dwSoftSize > 0 && dwSoftSize < 1024)
					{
						strSize.Format(L"%dKB", dwSoftSize);
					}
					else if (dwSoftSize > 1024 && dwSoftSize < 1024 * 1024)
					{
						strSize.Format(L"%dMB", dwSoftSize / 1024);
					}
					else if (dwSoftSize > 1024 * 1024)
					{
						strSize.Format(L"%dGB", dwSoftSize / 1024 / 1024);
					}
					else
					{
						strSize = L" ";
					}
					memcpy_s(SoftInfo.szSoftSize, (strSize.GetLength() + 1) * sizeof(WCHAR),
						strSize.GetBuffer(), (strSize.GetLength() + 1) * sizeof(WCHAR));

					CString strName = SoftInfo.szSoftName;
					if (!strName.IsEmpty())
					{
						m_vecSoftInfo.push_back(SoftInfo);
					}
				}
				dwIndex++;
				dwKeyLen = MAXBYTE;
				dwNameLen = MAXBYTE;
				dwSoftSize = 0;
				ZeroMemory(szKeyName, MAX_PATH);
			}

		}

	}
}

void CSoftMgr::UpdateSoftInfo()
{
	m_list_soft.DeleteAllItems();
	m_vecSoftInfo.clear();

	GetSoftwareInfo(m_SubKey1);

	GetSoftwareInfo(m_SubKey2);


	for (size_t i = 0; i < m_vecSoftInfo.size(); i++)
	{
		m_SoftName = m_vecSoftInfo[i].szSoftName;
		m_SoftPub = m_vecSoftInfo[i].szSoftPub;
		m_SoftDate = m_vecSoftInfo[i].szSoftDate;
		m_SoftSize = m_vecSoftInfo[i].szSoftSize;
		m_SoftVer = m_vecSoftInfo[i].szSoftVer;
		m_SoftInsPath = m_vecSoftInfo[i].strSoftInsPath;
		m_SoftUniPath = m_vecSoftInfo[i].strSoftUniPath;


		m_list_soft.InsertItem(i, m_SoftName);
		m_list_soft.SetItemText(i, 1, m_SoftVer);
		m_list_soft.SetItemText(i, 2, m_SoftDate);
		m_list_soft.SetItemText(i, 3, m_SoftSize);
		m_list_soft.SetItemText(i, 4, m_SoftInsPath);
		m_list_soft.SetItemText(i, 5, m_SoftUniPath);

	}
}

void CSoftMgr::OpenExploer()
{
	//获取路径
	if (m_SoftInsPath.IsEmpty())
	{
		MessageBox(L"打开路径失败！");
	}
	else
	{
		ShellExecute(NULL, _T("open"), _T("explorer.exe"), m_SoftInsPath, NULL, SW_SHOWNORMAL);
	}
	m_SoftInsPath = L"";
}

void CSoftMgr::UnistallSoft()
{
	//获取选中项

	if (m_SoftUniPath.IsEmpty())
	{
		MessageBox(L"卸载失败！");
		return;
	}
	else
	{
		ShellExecute(NULL, _T("open"), m_SoftUniPath, NULL, NULL, SW_SHOW);
	}
	m_SoftUniPath = L"";
}

void CSoftMgr::OnNMRClickListSoftware(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
	CMenu menu, *pmenu;
	//MessageBox(0, 0, 0);
	menu.LoadMenuW(IDR_MENU1);
	pmenu = menu.GetSubMenu(0);//指定包含菜单中的弹出菜单的位置。 位置值从0开始第一个菜单项。 
	ClientToScreen(&pNMItemActivate->ptAction);//在客户端坐标中指示发生事件的位置
	UINT Select = pmenu->TrackPopupMenu(TPM_RETURNCMD,
		pNMItemActivate->ptAction.x,
		pNMItemActivate->ptAction.y,
		this,
		NULL);



	if (pNMItemActivate->iItem != -1)
	{
		m_SoftInsPath = m_list_soft.GetItemText(pNMItemActivate->iItem, 5);//GetItemText:1.索引， 2.检索对象。返回一个CString的文本
		m_SoftUniPath = m_list_soft.GetItemText(pNMItemActivate->iItem, 6);//GetItemText:1.索引， 2.检索对象。返回一个CString的文本

	}


	//菜单项分支
	switch (Select)
	{
	case ID_32771:	//卸载
	{
		UnistallSoft();
	}
	break;
	case ID_32772:	//刷新
	{
		UpdateSoftInfo();
	}
	break;
	case ID_32773:	//打开文件夹
	{
		OpenExploer();
	}
	break;
	default:
		break;
	}
}
