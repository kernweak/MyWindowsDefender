// DiaTask.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MFC_PE.h"
#include "DiaTask.h"
#include "afxdialogex.h"
#include <TlHelp32.h>

// CDiaTask �Ի���

IMPLEMENT_DYNAMIC(CDiaTask, CDialogEx)

CDiaTask::CDiaTask(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG5, pParent)
{

}

CDiaTask::~CDiaTask()
{
}

void CDiaTask::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST5, m_Process);
	DDX_Control(pDX, IDC_LIST2, m_Thread);
	DDX_Control(pDX, IDC_LIST6, m_Module);
	DDX_Control(pDX, IDC_LIST1, m_Windows);
}
int CDiaTask::m = 0;
// CDiaTask ��Ϣ�������

DWORD WINAPI Proc(
	LPVOID lpParameter
) {
	CDiaTask *temp1 = (CDiaTask*)lpParameter;
	temp1->showProcessE();
	temp1->UpdateData(FALSE);
	return 0;
}


void CDiaTask::showProcessE()
{
	m_Process.DeleteAllItems();
	HANDLE hTool32 = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
	if (INVALID_HANDLE_VALUE == hTool32)
	{
		printf("����error!\n");
		return;
	}
	// 2. ��ʼ��������
	PROCESSENTRY32 ti = { sizeof(PROCESSENTRY32) };
	BOOL bRet = Process32First(hTool32, &ti);

	this->m_Process.InsertItem(0, ti.szExeFile);

	if (!bRet)
	{

		printf("Thread32First error!\n");
		return;
	}
	int i = 0;
	do
	{
		//if (ti.th32OwnerProcessID == dwPID)
		{
			m_Process.UpdateWindow();
			m_Process.InsertItem(i, ti.szExeFile);
			CString str;
			str.Format(_T("%ld"), ti.th32ProcessID);
			m_Process.SetItemText(i, 1, str);
			str.Format(_T("%ld"), ti.cntThreads);
			m_Process.SetItemText(i, 2, str);
			str.Format(_T("%ld"), ti.th32ParentProcessID);
			m_Process.SetItemText(i, 3, str);
			i++;
		}
	} while (Process32Next(hTool32, &ti));
	UpdateData(FALSE);
}

void CDiaTask::showThreadE()
{
	m_Thread.DeleteAllItems();
	HANDLE hTool32 = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, NULL);
	if (INVALID_HANDLE_VALUE == hTool32)
	{
		printf("����error!\n");
		return;
	}
	// 2. ��ʼ��������
	THREADENTRY32 ti = { sizeof(THREADENTRY32) };
	BOOL bRet = Thread32First(hTool32, &ti);
	if (!bRet)
	{
		printf("Thread32First error!\n");
		return;
	}
	int i = 0;
	do
	{
		//if (ti.th32OwnerProcessID == dwPID)
		{
			m_Thread.UpdateWindow();
			CString str;
			str.Format(_T("%ld"), ti.th32ThreadID);
			m_Thread.InsertItem(i, str);
			str.Format(_T("%ld"), ti.th32OwnerProcessID);
			m_Thread.SetItemText(i, 1, str);
			str.Format(_T("%ld"), ti.tpBasePri);
			m_Thread.SetItemText(i, 2, str);
			i++;
		}
	} while (Thread32Next(hTool32, &ti));
	return;
}

void CDiaTask::showModuleE()
{
	m_Module.DeleteAllItems( );
	WORD n;
	n = _wtoi(m_pocessID);
	HANDLE hTool32 = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, n);
	if (INVALID_HANDLE_VALUE == hTool32)
	{
		MessageBoxW(L"����error!");
		//	printf("����error!\n");
		return ;
	}
	// 2. ��ʼ��������
	MODULEENTRY32 ti = { sizeof(MODULEENTRY32) };
	BOOL bRet = Module32First(hTool32, &ti);
	         
	if (!bRet)
	{
		printf("Thread32First error!\n");
		return ;
	}
	int i = 0;
	do
	{
		//if (ti.th32OwnerProcessID == dwPID)
		{
			m_Module.UpdateWindow();
			m_Module.InsertItem(i, ti.szModule);
			CString str;
			str.Format(_T("%ld"), ti.th32ProcessID);
			m_Module.SetItemText(i, 1, str);
			m_Module.SetItemText(i, 2, ti.szExePath);
			str.Format(_T("%ld"), ti.modBaseSize);
			m_Module.SetItemText(i, 3, str);
			i++;
		}
	} while (Module32Next(hTool32, &ti));





	return ;
}

BOOL CALLBACK EnumWndProc(HWND hwnd, LPARAM lParam)
{
	// �����Ƿ����
	//if (::IsWindow(hwnd) && ::IsWindowVisible(hwnd) && ((GetWindowLong(hwnd, GWL_EXSTYLE)&WS_EX_TOOLWINDOW) != WS_EX_TOOLWINDOW) &&
	//	(GetWindowLong(hwnd, GWL_HWNDPARENT) == 0)) 
	{
		CDiaTask *plist = (CDiaTask*)lParam;
		if (!::IsWindowVisible(hwnd))
		{
			return TRUE;
		}

		// �����Ƿ�ɼ���
		//if (!::IsWindowEnabled(hwnd))
		//{
		//	return TRUE;
		//}
		// Do something
		char szTitle[255];
		::GetWindowTextW(hwnd, (LPWSTR)szTitle, sizeof(szTitle));
		CString str;//(szTitle);
		str.Format(L"%s", szTitle);
		plist->m_Windows.InsertItem(CDiaTask::m, str);
		TCHAR buf[128] = {};
		GetClassName(hwnd, buf, sizeof(buf));
		TCHAR buf1[128] = {};
		GetWindowModuleFileName(hwnd, buf1, sizeof(buf1));
		if (str.GetLength() > 0) {
			plist->m_Windows.SetItemText(CDiaTask::m, 1, buf);
			plist->m_Windows.SetItemText(CDiaTask::m, 2, buf1);
			CDiaTask::m = CDiaTask::m + 1;
		}
	}
	return TRUE;
}




void CDiaTask::showWindowE()
{
	HWND hOldWnd = NULL;
	EnumWindows(EnumWndProc, (LPARAM)this);
}

BOOL CDiaTask::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	//��ʼ������
	DWORD dwOldStyle = m_Process.GetExtendedStyle();
	m_Process.SetExtendedStyle(
		dwOldStyle | LVS_EX_FULLROWSELECT |
		LVS_EX_GRIDLINES |
		LVS_EX_CHECKBOXES
	);
	CRect rc;
	m_Process.GetClientRect(rc);
	int nWidth = rc.Width();
	m_Process.InsertColumn(0, L"����", 0, nWidth / 4);
	m_Process.InsertColumn(1, L"PID", 0, nWidth / 4);
	m_Process.InsertColumn(2, L"�߳���", 0, nWidth / 4);
	m_Process.InsertColumn(3, L"������", 0, nWidth / 4);

	//��ʼ���߳�
	DWORD dwOldStyle1 = m_Thread.GetExtendedStyle();
	m_Thread.SetExtendedStyle(
		dwOldStyle1 | LVS_EX_FULLROWSELECT |
		LVS_EX_GRIDLINES |
		LVS_EX_CHECKBOXES
	);
	CRect rc1;
	m_Thread.GetClientRect(rc1);
	int nWidth1 = rc1.Width();
	m_Thread.InsertColumn(0, L"�߳�ID", 0, nWidth1 / 3);
	m_Thread.InsertColumn(1, L"��������", 0, nWidth1 / 3);
	m_Thread.InsertColumn(2, L"�߳����ں��з�������ȼ�", 0, nWidth1 / 3);
	//��ʼ��ģ��
	DWORD dwOldStyle2 = m_Module.GetExtendedStyle();
	m_Module.SetExtendedStyle(
		dwOldStyle2 | LVS_EX_FULLROWSELECT |
		LVS_EX_GRIDLINES |
		LVS_EX_CHECKBOXES
	);
	CRect rc2;
	m_Module.GetClientRect(rc2);
	int nWidth2 = rc2.Width();
	m_Module.InsertColumn(0, L"ģ����", 0, nWidth2 / 4);
	m_Module.InsertColumn(1, L"ģ���������̵�id", 0, nWidth2 / 4);
	m_Module.InsertColumn(2, L"ģ��·��", 0, nWidth2 / 4);
	m_Module.InsertColumn(3, L"ģ���ڴ��С", 0, nWidth2 / 4);
	//��ʼ������
	DWORD dwOldStyle3 = m_Windows.GetExtendedStyle();
	m_Windows.SetExtendedStyle(
		dwOldStyle3 | LVS_EX_FULLROWSELECT |
		LVS_EX_GRIDLINES |
		LVS_EX_CHECKBOXES
	);
	CRect rc3;
	m_Windows.GetClientRect(rc3);
	int nWidth3 = rc3.Width();
	m_Windows.InsertColumn(0, L"������", 0, nWidth3 / 3);
	m_Windows.InsertColumn(1, L"�����������������", 0, nWidth3 / 3);
	m_Windows.InsertColumn(2, L"���ھ��������ģ�������·�����ļ�����", 0, nWidth3 / 3);

	//HANDLE hTread = INVALID_HANDLE_VALUE;
	//hTread = CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)Proc, this, NULL, NULL);
	//WaitForSingleObject(hTread, -1);

	showProcessE();
	showThreadE();
	showWindowE();
	//SetTimer(WM_TIMER, 1000, NULL);
	UpdateData(FALSE);
	return 0;
}

void CDiaTask::showProcess()
{

}


BEGIN_MESSAGE_MAP(CDiaTask, CDialogEx)
	ON_WM_TIMER()
	ON_NOTIFY(NM_CLICK, IDC_LIST5, &CDiaTask::OnNMClickGetModule)
	ON_BN_CLICKED(IDC_BUTTON1, &CDiaTask::OnBnClickedButton1)
END_MESSAGE_MAP()





void CDiaTask::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	CDialogEx::OnTimer(nIDEvent);

	
	//showProcessE();
}


void CDiaTask::OnNMClickGetModule(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	*pResult = 0;
	int temp = pNMItemActivate->iItem;
	m_pocessID = m_Process.GetItemText(temp, 1);
	showModuleE();
}


void CDiaTask::OnBnClickedButton1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString str;
	str.Format(L"shutdown -s -t %d", 3600);
	system("str");
}
