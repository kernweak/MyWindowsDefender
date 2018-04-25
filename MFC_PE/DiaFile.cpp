// DiaFile.cpp : 实现文件
//

#include "stdafx.h"
#include "MFC_PE.h"
#include "DiaFile.h"
#include "afxdialogex.h"
#include <PowrProf.h>
#include "Md5.h"
// CDiaFile 对话框

IMPLEMENT_DYNAMIC(CDiaFile, CDialogEx)

CDiaFile::CDiaFile(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIAFILE, pParent)
	, m_foldload(_T(""))
	, m_wenJianXinXi(_T(""))
{

}

CDiaFile::~CDiaFile()
{
}

void CDiaFile::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MFCSHELLTREE2, m_SheelTree);
	DDX_Control(pDX, IDC_MFCSHELLLIST2, m_ShellList);
	DDX_Text(pDX, IDC_EDIT3, m_foldload);
	DDX_Text(pDX, IDC_EDIT4, m_wenJianXinXi);
}

BOOL CDiaFile::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	UpdateData(true);
	m_SheelTree.SetRelatedList(&m_ShellList);
	m_ShellList.EnableShellContextMenu(true);

	return 0;
}


BEGIN_MESSAGE_MAP(CDiaFile, CDialogEx)
	ON_NOTIFY(NM_CLICK, IDC_MFCSHELLTREE2, &CDiaFile::OnNMClickMfcshelltree2)
	ON_NOTIFY(NM_DBLCLK, IDC_MFCSHELLTREE2, &CDiaFile::OnNMDblclkMfcshelltree2)
	ON_NOTIFY(TVN_SELCHANGED, IDC_MFCSHELLTREE2, &CDiaFile::OnTvnSelchangedMfcshelltree2)
	ON_WM_DROPFILES()
	ON_NOTIFY(NM_CLICK, IDC_MFCSHELLLIST2, &CDiaFile::OnNMClickMfcshelllist2)
	ON_BN_CLICKED(IDC_BUTTON1, &CDiaFile::OnBnClickedFileInformation)
	ON_BN_CLICKED(IDC_BUTTON2, &CDiaFile::OnBnClickedVSClean)
END_MESSAGE_MAP()


// CDiaFile 消息处理程序


void CDiaFile::OnNMClickMfcshelltree2(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
	m_ShellList.GetCurrentFolder(m_foldload);
	UpdateData(false);
}


void CDiaFile::OnNMDblclkMfcshelltree2(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
	m_ShellList.GetCurrentFolder(m_foldload);
	UpdateData(false);
}


void CDiaFile::OnTvnSelchangedMfcshelltree2(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
	m_ShellList.GetCurrentFolder(m_foldload);
	UpdateData(false);
}


void CDiaFile::OnDropFiles(HDROP hDropInfo)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	CDialogEx::OnDropFiles(hDropInfo);
	int DropCount = DragQueryFile(hDropInfo, -1, NULL, 0);//取得被拖动文件的数目
	for (int i = 0;i < DropCount;i++) {
		WCHAR wcStr[MAX_PATH];
		DragQueryFile(hDropInfo, i, wcStr, MAX_PATH);//获得拖拽的第i个文件的文件名
													 //需要做的事情
		m_foldload = wcStr;
		UpdateData(false);
		DragFinish(hDropInfo); //拖拽结束后，释放内存
	}
}


void CDiaFile::OnNMClickMfcshelllist2(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
	m_ShellList.GetCurrentFolder(m_foldload);
	//m_ShellList.GetCurrentFolder(m_foldload);
	UpdateData(false);
	int temp = pNMItemActivate->iItem;
	wenJianMing = m_ShellList.GetItemText(temp, 0);
	m_foldload = m_foldload +L"\\"+wenJianMing;
	UpdateData(FALSE);
}


void CDiaFile::OnBnClickedFileInformation()
{
	//CString wenJianSum = m_foldload + L"\\" + wenJianMing;
	CString wenJianSum = m_foldload;
	WIN32_FILE_ATTRIBUTE_DATA wfad;
	// TODO: 在此添加控件通知处理程序代码WIN32_FILE_ATTRIBUTE_DATA wfad;
	GetFileAttributesEx(wenJianSum, GetFileExInfoStandard, &wfad);
	FILETIME ftLocal;
	FileTimeToLocalFileTime(&wfad.ftCreationTime, &ftLocal);
	SYSTEMTIME stcCreatTime;
	FileTimeToSystemTime(&ftLocal, &stcCreatTime);
	CString str;
	str.Format(_T("%d"), stcCreatTime.wYear);
	m_wenJianXinXi = wenJianMing;
	m_wenJianXinXi = wenJianMing + L" \r\n";
	m_wenJianXinXi = m_wenJianXinXi + L"创建日期：";
	m_wenJianXinXi = m_wenJianXinXi + str;
	m_wenJianXinXi = m_wenJianXinXi + L"Y/";
	str.Format(_T("%d"), stcCreatTime.wMonth);
	m_wenJianXinXi = m_wenJianXinXi + str;
	m_wenJianXinXi = m_wenJianXinXi + L"M/";
	str.Format(_T("%d"), stcCreatTime.wDay);
	m_wenJianXinXi = m_wenJianXinXi + str;
	m_wenJianXinXi = m_wenJianXinXi + L"D/";
	str.Format(_T("%d"), stcCreatTime.wHour);
	m_wenJianXinXi = m_wenJianXinXi + str;
	m_wenJianXinXi = m_wenJianXinXi + L"H:";
	str.Format(_T("%d"), stcCreatTime.wMinute);
	m_wenJianXinXi = m_wenJianXinXi + str;
	m_wenJianXinXi = m_wenJianXinXi + L"M:";
	str.Format(_T("%d"), stcCreatTime.wMinute);
	m_wenJianXinXi = m_wenJianXinXi + str;
	m_wenJianXinXi = m_wenJianXinXi + L"S\r\n";
	m_wenJianXinXi = m_wenJianXinXi + L"修改日期：";
	FileTimeToLocalFileTime(&wfad.ftLastWriteTime, &ftLocal);
	FileTimeToSystemTime(&ftLocal, &stcCreatTime);
	str.Format(_T("%d"), stcCreatTime.wYear);

	m_wenJianXinXi = m_wenJianXinXi + str;
	m_wenJianXinXi = m_wenJianXinXi + L"Y/";
	str.Format(_T("%d"), stcCreatTime.wMonth);
	m_wenJianXinXi = m_wenJianXinXi + str;
	m_wenJianXinXi = m_wenJianXinXi + L"M/";
	str.Format(_T("%d"), stcCreatTime.wDay);
	m_wenJianXinXi = m_wenJianXinXi + str;
	m_wenJianXinXi = m_wenJianXinXi + L"D/";
	str.Format(_T("%d"), stcCreatTime.wHour);
	m_wenJianXinXi = m_wenJianXinXi + str;
	m_wenJianXinXi = m_wenJianXinXi + L"H:";
	str.Format(_T("%d"), stcCreatTime.wMinute);
	m_wenJianXinXi = m_wenJianXinXi + str;
	m_wenJianXinXi = m_wenJianXinXi + L"M:";
	str.Format(_T("%d"), stcCreatTime.wMinute);
	m_wenJianXinXi = m_wenJianXinXi + str;
	m_wenJianXinXi = m_wenJianXinXi + L"S\r\n";

	DWORD64 qwFileSize = 0;
	qwFileSize = wfad.nFileSizeHigh;
	qwFileSize <<= sizeof(DWORD) * 8;
	qwFileSize += wfad.nFileSizeLow;

	DWORD64 G = 0, M = 0, K = 0, B = 0;
	B = qwFileSize % 1024;DWORD64 temp = 1;temp = qwFileSize;
	qwFileSize = qwFileSize - B;
	G = qwFileSize / 1024.0 / 1024 / 1024;
	//DWORD64 temp = 1024 * 1024 * 1024;
	//temp = temp * G;

	qwFileSize = qwFileSize - (1024 * 1024 * 1024 * G) - B;

	M = qwFileSize / 1024.0 / 1024;

	qwFileSize = qwFileSize - (1024 * 1024 * M);
	K = temp - (1024 * 1024 * M) - (1024 * 1024 * 1024 * G) - B;
	K = K / 1024;
	//	M= (qwFileSize - B-1024*K) % (1024 * 1024);
	//	G = (qwFileSize - B - 1024 * K - 1024 * 1024 * M) / 1024;
	//qwFileSize = qwFileSize / 1024.0;//kb
	//qwFileSize = qwFileSize / 1024;//mB
	m_wenJianXinXi = m_wenJianXinXi + L"文件大小：";
	str.Format(_T("%dG%dM%dK%dB"), G, M, K, B);
	m_wenJianXinXi = m_wenJianXinXi + str;
	//m_wenJianXinXi = wenJianSum;
	//L"创建时间：";
	m_wenJianXinXi+=L"\r\n";
	m_wenJianXinXi += L"MD5的值为:";
	USES_CONVERSION;
	char* tempmd5=W2A(m_foldload.GetBuffer());
	char* md5 = md5FileValue(tempmd5);
	CString strMD5(md5);
	m_wenJianXinXi = m_wenJianXinXi+ strMD5;
	UpdateData(false);
}

void CDiaFile::EnumLoad(TCHAR * load)
{
	BOOL Flag = FALSE;
	TCHAR szSearchPath[MAX_PATH] = {};
	_stprintf_s(szSearchPath, MAX_PATH, L"%s\\*.*", load);
	WIN32_FIND_DATA wsd = {};
	HANDLE hFile = FindFirstFile(szSearchPath, &wsd);
	if (hFile == INVALID_HANDLE_VALUE) {
		return;
	}
	do {
		if (wsd.cFileName[0] == '.') {
			continue;
		}
		if (wsd.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY) {
			TCHAR oppo[MAX_PATH];
			_stprintf_s(oppo, MAX_PATH, L"%s\\%s", load, wsd.cFileName);
			if (_tcscmp(L"Debug", wsd.cFileName) == 0 || _tcscmp(L"Release", wsd.cFileName) == 0
				|| _tcscmp(L"ipch", wsd.cFileName) == 0) {
				//_stprintf_s(oppo, MAX_PATH, L"%s%s", oppo, "\0");
				deleteFolder(oppo);
				cleanNumber1++;
				continue;
			}                                                           
			EnumLoad(oppo);
		}
		else {
			int Size = _tcslen(wsd.cFileName);
			if ((NULL != StrStr(wsd.cFileName, L".db")) ||
				(NULL != StrStr(wsd.cFileName, L".aps")) ||
				(NULL != StrStr(wsd.cFileName, L".ncb")) ||
				(NULL != StrStr(wsd.cFileName, L".idb")) ||
				(NULL != StrStr(wsd.cFileName, L".obj")) ||
				(NULL != StrStr(wsd.cFileName, L".pch")) ||
				(NULL != StrStr(wsd.cFileName, L".sbr")) ||
				(NULL != StrStr(wsd.cFileName, L".pch")) ||
				(NULL != StrStr(wsd.cFileName, L".tmp")) ||
				(NULL != StrStr(wsd.cFileName, L".pdb")) ||
				(NULL != StrStr(wsd.cFileName, L".bsc")) ||
				(NULL != StrStr(wsd.cFileName, L".ilk")) ||
				(NULL != StrStr(wsd.cFileName, L".bsc")) ||
				(NULL != StrStr(wsd.cFileName, L".res")) ||
				(NULL != StrStr(wsd.cFileName, L".sdf")) ||
				(NULL != StrStr(wsd.cFileName, L".dep")) ||
				(NULL != StrStr(wsd.cFileName, L".ipch")) ||
				(NULL != StrStr(wsd.cFileName, L".tlog")) ||
				(NULL != StrStr(wsd.cFileName, L".exp")) ||
				(NULL != StrStr(wsd.cFileName, L".hpj")) ||
				(NULL != StrStr(wsd.cFileName, L".opt")) ||
				(NULL != StrStr(wsd.cFileName, L".mdp")) ||
				(NULL != StrStr(wsd.cFileName, L".plg")) ||
				(NULL != StrStr(wsd.cFileName, L".clw"))
				) {
				TCHAR oppoStr[MAX_PATH];
				_stprintf_s(oppoStr, MAX_PATH, L"%s\\%s", load, wsd.cFileName);
				//deleteFolder(oppoStr);
				DeleteFile(oppoStr);
				cleanNumber++;
			}

		}
	} while (FindNextFile(hFile, &wsd));
}

void CDiaFile::deleteFolder(TCHAR * load)
{
	SHFILEOPSTRUCT FileOp = {};
	FileOp.fFlags = FOF_NOCONFIRMATION;
	CString temp = load;
	temp += '\0';
	FileOp.pFrom = temp;
	FileOp.wFunc = FO_DELETE;
	SHFileOperation(&FileOp);
}


void CDiaFile::OnBnClickedVSClean()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	TCHAR* p = (TCHAR*)m_foldload.GetBuffer();//L"D:\\project\\24_MFC_day007_3.2";// 

	EnumLoad(p);
	CString str;
	str.Format(L"清理结束，清理了%d个文件夹，%d个文件", cleanNumber, cleanNumber1);
	MessageBox(str, TEXT("通知"), 0);
	cleanNumber = 0;
	cleanNumber1 = 0;
}
