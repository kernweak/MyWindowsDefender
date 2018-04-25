// DiaCPU.cpp : 实现文件
//

#include "stdafx.h"
#include "MFC_PE.h"
#include "DiaCPU.h"
#include "afxdialogex.h"
#include <Windows.h>
#include <WinBase.h>
#include <psapi.h>

// CDiaCPU 对话框

IMPLEMENT_DYNAMIC(CDiaCPU, CDialogEx)


double FIETIME2Double(const _FILETIME& fileTime) {
	return (double)(fileTime.dwHighDateTime*4.294967296E9) + (double)(fileTime.dwLowDateTime);
}

int GetCpuUsage() {
	_FILETIME idleTime, kernelTime, userTime;
	GetSystemTimes(&idleTime, &kernelTime, &userTime);
	HANDLE hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	WaitForSingleObject(hEvent, 1000);
	_FILETIME newIdleTime, newKernelTime, newUserTime;
	GetSystemTimes(&newIdleTime, &newKernelTime, &newUserTime);
	double dOldIdleTime = FIETIME2Double(idleTime);
	double dNewIdleTime = FIETIME2Double(newIdleTime);
	double dOldKernelTime = FIETIME2Double(kernelTime);
	double dNewKernelTime = FIETIME2Double(newKernelTime);
	double dOldUserTime = FIETIME2Double(userTime);
	double dNewUserTime = FIETIME2Double(newUserTime);
	return (int)(100.0 - (dNewIdleTime - dOldIdleTime) / (dNewKernelTime - dOldKernelTime + dNewUserTime - dOldUserTime)*100.0);
}

CDiaCPU::CDiaCPU(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIACPU, pParent)
	, m_CleanMem(_T(""))
{

}

CDiaCPU::~CDiaCPU()
{
}

void CDiaCPU::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST3, m_Cpu);
	DDX_Control(pDX, IDC_LIST2, m_Memory);
	DDX_Text(pDX, IDC_EDIT1, m_CleanMem);
}


DWORD WINAPI Proc2(
	LPVOID lpParameter
) {
	CDiaCPU* cpu = (CDiaCPU*)lpParameter;
	while (1) {
//		if (cpu->m_Flag == 1){
//			cpu->m_Flag = 0;
//			return 0;}
			
		cpu->showCPUandMem();
	}

	return 0;

}


BOOL CDiaCPU::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	DWORD dwOldStyle = m_Memory.GetExtendedStyle();
	m_Memory.SetExtendedStyle(
		dwOldStyle | LVS_EX_FULLROWSELECT |
		LVS_EX_GRIDLINES
	);
	CRect rc;
	m_Memory.GetClientRect(rc);
	int nWidth = rc.Width();
	m_Memory.InsertColumn(0, L"内存属性", 0, nWidth / 2);
	m_Memory.InsertColumn(1, L"数据", 0, nWidth / 2);
	m_Memory.InsertItem(0, L"实际物理内存");
	m_Memory.InsertItem(1, L"当前可用物理内存");
	m_Memory.InsertItem(2, L"当前内存占有率");
	m_Memory.InsertItem(3, L"已用物理内存大小");


	DWORD dwOldStyle1 = m_Cpu.GetExtendedStyle();
	m_Cpu.SetExtendedStyle(
		dwOldStyle1 | LVS_EX_FULLROWSELECT |
		LVS_EX_GRIDLINES
	);
	CRect rc1;
	m_Cpu.GetClientRect(rc1);
	int nWidth1 = rc1.Width();
	m_Cpu.InsertColumn(0, L"Cpu属性", 0, nWidth1 / 2);
	m_Cpu.InsertColumn(1, L"数据", 0, nWidth1 / 2);
	m_Cpu.InsertItem(0, L"CPU占有率");
	  

	HANDLE hTread = INVALID_HANDLE_VALUE;
	hTread = CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)Proc2, this, NULL, NULL);
	WaitForSingleObject(hTread, 1);
//	m_Flag = 0;
	UpdateData(FALSE);

	return 0;
}

void CDiaCPU::showCPUandMem()
{
	//显示内存
	MEMORYSTATUSEX stcMemStatusEx = { 0 };
	CString str1;
	LONGLONG i=0;
	//获取当前内存的状态
	stcMemStatusEx.dwLength = sizeof(stcMemStatusEx);
	GlobalMemoryStatusEx(&stcMemStatusEx);


	DWORDLONG preUsedMem = stcMemStatusEx.ullTotalPhys - stcMemStatusEx.ullAvailPhys;

	CString str;
	str.Format(_T("%ldMB"), stcMemStatusEx.ullTotalPhys / 1024 / 1024);
	m_Memory.SetItemText(0, 1, str);
	str = L"";
	str.Format(_T("%ldMB"), stcMemStatusEx.ullAvailPhys / 1024 / 1024);
	m_Memory.SetItemText(1, 1, str);

	str = L"";
	str.Format(_T("%%%ld"), (stcMemStatusEx.ullTotalPhys - stcMemStatusEx.ullAvailPhys)*100 / stcMemStatusEx.ullTotalPhys);
	m_Memory.SetItemText(2, 1, str);

	str = L"";
	str.Format(_T("%ldMB"), (stcMemStatusEx.ullTotalPhys-stcMemStatusEx.ullAvailPhys) / 1024 / 1024);
	m_Memory.SetItemText(3, 1, str);

	str1.Format(_T("%d"), GetCpuUsage());
	m_Cpu.SetItemText(0, 1, str1);

	SYSTEM_INFO i1;
	GetSystemInfo(&i1);
	//UpdateData(FALSE);
	
	m_Cpu.UpdateWindow();
	m_Memory.UpdateWindow();
}


BEGIN_MESSAGE_MAP(CDiaCPU, CDialogEx)
	ON_BN_CLICKED(IDOK, &CDiaCPU::OnBnClickedOk)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BUTTON1, &CDiaCPU::OnBnClickedCleanMem)
END_MESSAGE_MAP()


// CDiaCPU 消息处理程序


void CDiaCPU::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	//m_Flag = 1;
	//Sleep(3000);

	CDialogEx::OnOK();
	  ShowWindow(SW_HIDE);
}


void CDiaCPU::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CDialogEx::OnClose();

}


void CDiaCPU::OnBnClickedCleanMem()//清理内存
{
	// TODO: 在此添加控件通知处理程序代码
	MEMORYSTATUSEX stcMemStatusEx = { 0 };
	MEMORYSTATUSEX stcMemStatusEx1 = { 0 };
	stcMemStatusEx.dwLength = sizeof(stcMemStatusEx);
	stcMemStatusEx1.dwLength = sizeof(stcMemStatusEx);
	GlobalMemoryStatusEx(&stcMemStatusEx1);
	DWORD dwPIDList[1000] = { 0 };
	DWORD bufSize = sizeof(dwPIDList);
	DWORD dwNeedSize = 0;
	EnumProcesses(dwPIDList,bufSize,&dwNeedSize);
	for (DWORD i = 0;i < dwNeedSize / sizeof(DWORD);i++) {
		HANDLE hProcess = OpenProcess(PROCESS_SET_QUOTA, false, dwPIDList[i]);
		SetProcessWorkingSetSize(hProcess, -1, -1);
	}
	//获取清理后的内存状态
	GlobalMemoryStatusEx(&stcMemStatusEx);
	DWORDLONG afterCleanUsedMem = stcMemStatusEx.ullAvailPhys - stcMemStatusEx1.ullAvailPhys;
	m_CleanMem = L"";
	m_CleanMem.Format(_T("%ldMB"), afterCleanUsedMem / 1024 / 1024);
	UpdateData(false);

}
