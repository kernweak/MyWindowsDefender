// DiaCPU.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MFC_PE.h"
#include "DiaCPU.h"
#include "afxdialogex.h"
#include <Windows.h>
#include <WinBase.h>

// CDiaCPU �Ի���

IMPLEMENT_DYNAMIC(CDiaCPU, CDialogEx)


double FIETIME2Double(const _FILETIME& fileTime) {
	return double(fileTime.dwHighDateTime*4.294967296e9) + double(fileTime.dwLowDateTime);
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
	return int(100 - (dNewIdleTime - dOldIdleTime) / (dNewKernelTime - dOldKernelTime + dNewUserTime - dOldUserTime)*100.0);
}

CDiaCPU::CDiaCPU(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIACPU, pParent)
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
	m_Memory.InsertColumn(0, L"�ڴ�����", 0, nWidth / 2);
	m_Memory.InsertColumn(1, L"����", 0, nWidth / 2);
	m_Memory.InsertItem(0, L"��ǰ�ڴ�ռ����");
	m_Memory.InsertItem(1, L"���������ڴ��С");


	DWORD dwOldStyle1 = m_Cpu.GetExtendedStyle();
	m_Cpu.SetExtendedStyle(
		dwOldStyle1 | LVS_EX_FULLROWSELECT |
		LVS_EX_GRIDLINES
	);
	CRect rc1;
	m_Cpu.GetClientRect(rc1);
	int nWidth1 = rc1.Width();
	m_Cpu.InsertColumn(0, L"Cpu����", 0, nWidth1 / 2);
	m_Cpu.InsertColumn(1, L"����", 0, nWidth1 / 2);
	m_Cpu.InsertItem(0, L"CPUռ����");
	  

	
	UpdateData(FALSE);

	return 0;
}

void CDiaCPU::showCPUandMem()
{
	//��ʾ�ڴ�
	MEMORYSTATUS memStatus;
	GlobalMemoryStatus(&memStatus);
	CString str;
	str.Format(_T("%d"), memStatus.dwMemoryLoad);
	m_Memory.SetItemText(0, 1, str);
	str.Format(_T("%d"), memStatus.dwTotalPhys - memStatus.dwAvailPhys);
	m_Memory.SetItemText(1, 1, str);

	//��ʾCPU
	str.Format(_T("%d"), GetCpuUsage());
	m_Cpu.SetItemText(0, 1, str);
	//m_Memory.InsertItem(1, L"���������ڴ��С");



	SYSTEM_INFO i;
	GetSystemInfo(&i);
	UpdateData(FALSE);
}


BEGIN_MESSAGE_MAP(CDiaCPU, CDialogEx)
END_MESSAGE_MAP()


// CDiaCPU ��Ϣ�������
