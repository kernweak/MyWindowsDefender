// DiaVirus.cpp : 实现文件
//


#include "stdafx.h"
#include "MFC_PE.h"
#include "DiaVirus.h"
#include "afxdialogex.h"
#include <TlHelp32.h>
#include<vector>
#include <windows.h>
#include <shlobj.h>
#include <iostream>
#include <fstream>
#include<string>
#include"Md5.h"
#include <PowrProf.h>

using namespace std;
// CDiaVirus 对话框
#define WM_MYSOCKET 0x410
IMPLEMENT_DYNAMIC(CDiaVirus, CDialogEx)

CDiaVirus::CDiaVirus(CClientScoket* pClient, CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG6, pParent), m_pClient(pClient)
	, m_FileInfor(_T(""))
	, m_Result1(_T(""))
	, m_AllScanLoad(_T(""))
	, m_ListPathName(_T(""))
{

}

CDiaVirus::~CDiaVirus()
{
}

CDiaVirus::CDiaVirus()
{
}

void CDiaVirus::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_FileInfor);
	DDX_Control(pDX, IDC_EDIT2, m_Result);
	DDX_Text(pDX, IDC_EDIT2, m_Result1);
	DDX_Text(pDX, IDC_EDIT3, m_AllScanLoad);
	DDX_LBString(pDX, IDC_LIST1, m_ListPathName);
	DDX_Control(pDX, IDC_LIST1, m_ListPathNameC);
}


BEGIN_MESSAGE_MAP(CDiaVirus, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CDiaVirus::OnBnClickedWhiteList)
	ON_WM_DROPFILES()
	ON_BN_CLICKED(IDC_BUTTON2, &CDiaVirus::OnBnClickedMD5)
	ON_MESSAGE(WM_MYSOCKET, &CDiaVirus::OnMysocket)
	ON_BN_CLICKED(IDC_BUTTON3, &CDiaVirus::OnBnClickedAllScan)
END_MESSAGE_MAP()


// CDiaVirus 消息处理程序


void CDiaVirus::OnBnClickedWhiteList()
{
	// TODO: 在此添加控件通知处理程序代码
	
	vector<wstring> tempPocess;
	//FILE *pfile = NULL;
	//if (!fopen_s(&pfile, "D:\\temp.txt", "r+"));
	//ifstream fin("D:\\temp.txt");
	//const   int  LINE_LENGTH = 100;
	//char  str[LINE_LENGTH];
	//while (fin.getline(str, LINE_LENGTH))
	//{
	//	tempPocess.push_back(str);
	//}

	wifstream infile;
	infile.open("D:\\temp.txt");
	const   int  LINE_LENGTH = 100;
	WCHAR  str[LINE_LENGTH];
	wstring value;
	while (infile.getline(str, LINE_LENGTH)) {
		//infile >> value;  //这就是读取到的汉字，用wstring存储
		tempPocess.push_back(str);
	}
	infile.close();

	string  s;

	HANDLE hTool32 = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
	if (INVALID_HANDLE_VALUE == hTool32)
	{
		printf("快照error!\n");
		return;
	}
	// 2. 开始遍历进程
	PROCESSENTRY32 ti = { sizeof(PROCESSENTRY32) };
	BOOL bRet = Process32First(hTool32, &ti);

	//this->m_Process.InsertItem(0, ti.szExeFile);

	if (!bRet)
	{

		printf("Thread32First error!\n");
		return;
	}
	int i = 0;
	int j = 0;
	do
	{
		wstring strtemp=ti.szExeFile;
		for (j = 0;j < tempPocess.size();j++) {
			if (tempPocess[j] == ti.szExeFile) {
				break;
			}
		}
		if (j == tempPocess.size()) {
			//不在白名单里，关闭进程
			WORD n= ti.th32ProcessID;
			HANDLE hwnd = OpenProcess(PROCESS_TERMINATE, FALSE, n);
			TerminateProcess(hwnd, 0);
			CString str;
			str.Format(L"根据白名单关闭了进程pid=%d,进程名=%s\n", n, ti.szExeFile);
			MessageBox(str, TEXT("通知"), 0);
		}
	} while (Process32Next(hTool32, &ti));
	UpdateData(FALSE);
}


void CDiaVirus::OnDropFiles(HDROP hDropInfo)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CDialogEx::OnDropFiles(hDropInfo);
	m_FileInfor = L"";
	m_FileInfor1 = L"";
	int DropCount = DragQueryFile(hDropInfo, -1, NULL, 0);//取得被拖动文件的数目
	for (int i = 0;i < DropCount;i++) {
		WCHAR wcStr[MAX_PATH];
		DragQueryFile(hDropInfo, i, wcStr, MAX_PATH);//获得拖拽的第i个文件的文件名
													 //需要做的事情
		m_FileInfor = wcStr;
		m_FileInfor1 = wcStr;
		USES_CONVERSION;
		char* tempmd5 = W2A(m_FileInfor.GetBuffer());
		m_FileInfor += L"\r\nMD5的值为:";
		
		char* md5 = md5FileValue(tempmd5);
		CString strMD5(md5);
		m_MD5 = strMD5;
		m_FileInfor = m_FileInfor + strMD5;


		UpdateData(FALSE);
		DragFinish(hDropInfo); //拖拽结束后，释放内存
		CDialogEx::OnDropFiles(hDropInfo);
	}
}

//
DWORD WINAPI Proc3(
	LPVOID lpParameter
) {
	CDiaVirus *Pthis = (CDiaVirus *)lpParameter;
	CHATSEND ct = {};
	while (1)
	{


		if (SOCKET_ERROR != recv(Pthis->m_pClient->m_sClient, (char*)&ct, sizeof(CHATSEND), NULL)) {
			char* tempCom = "不是病毒!";
			char* tempCom1 = "是病毒!";
			if ((0 != strcmp(tempCom, ct.m_content.buf)) && (0 != strcmp(tempCom1, ct.m_content.buf)))
				continue;
			//CString temp1(ct.m_content.buf);
			Pthis->m_Result1 = ct.m_content.buf;
			if (Pthis->m_flag != strcmp(tempCom, ct.m_content.buf)) {
				Pthis->m_flag = 1;//为1时说明比较不相等是个病毒
			}
			if (Pthis->m_flag == 1) {
				_tremove(Pthis->m_FileInfor1);
			}

			Pthis->m_flag = 0;
		}
	}
		return 0;
}
//

DWORD WINAPI Proc4 (
	LPVOID lpParameter
) {
	CDiaVirus *Pthis = (CDiaVirus *)lpParameter;
	CHATSEND ct = {};
	while (1)
	{


		if (SOCKET_ERROR != recv(Pthis->m_pClient->m_sClient, (char*)&ct, sizeof(CHATSEND), NULL)) {
			char* tempCom = "不是病毒!";
			char* tempCom1 = "是病毒!";
			if ((0 != strcmp(tempCom, ct.m_content.buf)) && (0 != strcmp(tempCom1, ct.m_content.buf)))
				continue;
			CString temp1(ct.m_content.buf);
			CString tempLoad = Pthis->m_Load;
		//	Pthis->m_Result1 = ct.m_content.buf;
		//	Pthis->m_Result1= Pthis->m_Result1+ Pthis->m_Load;
		//	Pthis->m_Result1= Pthis->m_Result1+ temp1;
		//	Pthis->m_Result1 += L"\r\n";
			if (Pthis->m_flag != strcmp(tempCom, ct.m_content.buf)) {
				Pthis->m_flag = 1;//为1时说明比较不相等是个病毒
			}
			if (Pthis->m_flag == 1) {
				
				_tremove(Pthis->m_Load);
			}

			//Pthis->m_flag = 0;
		}
	}
	return 0;
}


void CDiaVirus::OnBnClickedMD5()
{
	// TODO: 在此添加控件通知处理程序代码
	if(!m_MD5.IsEmpty()){
		CStringA strSearch = CW2A(m_MD5.GetBuffer(), CP_THREAD_ACP);
		m_pClient->Send(SEARCHMD5, strSearch.GetBuffer(), strSearch.GetLength() + 1);
	}
	HANDLE hTread = INVALID_HANDLE_VALUE;
	hTread = CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)Proc3, this, NULL, NULL);
	
	WaitForSingleObject(hTread, 20);
	//int tempsted=0;
	//if (m_flag == 1)
	//	_tremove(m_FileInfor1);
	UpdateData(false);
}


afx_msg LRESULT CDiaVirus::OnMysocket(WPARAM wParam, LPARAM lParam)
{

	return 0;
}



void CDiaVirus::OnBnClickedAllScan()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	TCHAR * load = m_AllScanLoad.GetBuffer();
	Enumload(load);
}

void CDiaVirus::Enumload(TCHAR * load)
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
			Enumload(oppo);
		}
		else {
			int Size = _tcslen(wsd.cFileName);
		 {
				TCHAR oppoStr[MAX_PATH];
				_stprintf_s(oppoStr, MAX_PATH, L"%s\\%s", load, wsd.cFileName);
				m_ListPathNameC.AddString(oppoStr);
				m_ListPathNameC.SetTopIndex(m_ListPathNameC.GetCount() - 1);

				//计算MD5值
				USES_CONVERSION;
				char* tempmd5 = W2A(oppoStr);
				
				m_Load = oppoStr;
				m_FileInfor += L"\r\nMD5的值为:";
				char* md5 = md5FileValue(tempmd5);
				CString strMD5(md5);
				if (!strMD5.IsEmpty()) {
					CStringA strSearch = CW2A(strMD5.GetBuffer(), CP_THREAD_ACP);
					m_pClient->Send(SEARCHMD5, strSearch.GetBuffer(), strSearch.GetLength() + 1);
					
				}


				UpdateWindow();
				HANDLE hTread1 = INVALID_HANDLE_VALUE;
				hTread1 = CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)Proc4, this, NULL, NULL);

				WaitForSingleObject(hTread1, 20);
				Sleep(300);
				if (m_flag == 1) {
					//m_Result1 = L"";
					m_Result1 = m_Result1 + m_Load;
					char* tempCom1 = "是病毒!";
					CString temp11(tempCom1);
					m_Result1 = m_Result1 + temp11;
					m_Result1 += L"\r\n";
					UpdateData(FALSE);
					m_flag = 0;
				}
				//DeleteFile(oppoStr);
				UpdateWindow();
			}

		}
	} while (FindNextFile(hFile, &wsd));

	FindClose(hFile);
}
