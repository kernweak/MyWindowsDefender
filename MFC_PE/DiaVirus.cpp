// DiaVirus.cpp : ʵ���ļ�
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
// CDiaVirus �Ի���
#define WM_MYSOCKET 0x410
IMPLEMENT_DYNAMIC(CDiaVirus, CDialogEx)

CDiaVirus::CDiaVirus(CClientScoket* pClient, CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG6, pParent), m_pClient(pClient)
	, m_FileInfor(_T(""))
	, m_Result1(_T(""))
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
}


BEGIN_MESSAGE_MAP(CDiaVirus, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CDiaVirus::OnBnClickedWhiteList)
	ON_WM_DROPFILES()
	ON_BN_CLICKED(IDC_BUTTON2, &CDiaVirus::OnBnClickedMD5)
	ON_MESSAGE(WM_MYSOCKET, &CDiaVirus::OnMysocket)
END_MESSAGE_MAP()


// CDiaVirus ��Ϣ�������


void CDiaVirus::OnBnClickedWhiteList()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	
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
		//infile >> value;  //����Ƕ�ȡ���ĺ��֣���wstring�洢
		tempPocess.push_back(str);
	}
	infile.close();

	string  s;

	HANDLE hTool32 = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
	if (INVALID_HANDLE_VALUE == hTool32)
	{
		printf("����error!\n");
		return;
	}
	// 2. ��ʼ��������
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
			//���ڰ�������رս���
			WORD n= ti.th32ProcessID;
			HANDLE hwnd = OpenProcess(PROCESS_TERMINATE, FALSE, n);
			TerminateProcess(hwnd, 0);
			CString str;
			str.Format(L"���ݰ������ر��˽���pid=%d,������=%s\n", n, ti.szExeFile);
			MessageBox(str, TEXT("֪ͨ"), 0);
		}
	} while (Process32Next(hTool32, &ti));
	UpdateData(FALSE);
}


void CDiaVirus::OnDropFiles(HDROP hDropInfo)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	CDialogEx::OnDropFiles(hDropInfo);
	m_FileInfor = L"";
	int DropCount = DragQueryFile(hDropInfo, -1, NULL, 0);//ȡ�ñ��϶��ļ�����Ŀ
	for (int i = 0;i < DropCount;i++) {
		WCHAR wcStr[MAX_PATH];
		DragQueryFile(hDropInfo, i, wcStr, MAX_PATH);//�����ק�ĵ�i���ļ����ļ���
													 //��Ҫ��������
		m_FileInfor = wcStr;
		USES_CONVERSION;
		char* tempmd5 = W2A(m_FileInfor.GetBuffer());
		m_FileInfor += L"\r\nMD5��ֵΪ:";
		
		char* md5 = md5FileValue(tempmd5);
		CString strMD5(md5);
		m_MD5 = strMD5;
		m_FileInfor = m_FileInfor + strMD5;


		UpdateData(FALSE);
		DragFinish(hDropInfo); //��ק�������ͷ��ڴ�
		CDialogEx::OnDropFiles(hDropInfo);
	}
}

//
DWORD WINAPI Proc3(
	LPVOID lpParameter
) {
	CDiaVirus *Pthis = (CDiaVirus *)lpParameter;
	CHATSEND ct = {};
//	while (1)
//	{
		if (SOCKET_ERROR != recv(Pthis->m_pClient->m_sClient, (char*)&ct, sizeof(CHATSEND), NULL)) {

			CString temp1(ct.m_content.buf);
			Pthis->m_Result1 = temp1;

		}
//	}
		return 0;
}
//
void CDiaVirus::OnBnClickedMD5()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if(!m_MD5.IsEmpty()){
		CStringA strSearch = CW2A(m_MD5.GetBuffer(), CP_THREAD_ACP);
		m_pClient->Send(SEARCHMD5, strSearch.GetBuffer(), strSearch.GetLength() + 1);
	}
	HANDLE hTread = INVALID_HANDLE_VALUE;
	hTread = CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)Proc3, this, NULL, NULL);
	WaitForSingleObject(hTread, 1);
	UpdateData(false);
}


afx_msg LRESULT CDiaVirus::OnMysocket(WPARAM wParam, LPARAM lParam)
{

	return 0;
}
