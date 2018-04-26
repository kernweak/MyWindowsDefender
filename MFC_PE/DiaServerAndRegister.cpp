// DiaServerAndRegister.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MFC_PE.h"
#include "DiaServerAndRegister.h"
#include "afxdialogex.h"
#include <Windows.h>
#include<winsvc.h>

// CDiaServerAndRegister �Ի���

IMPLEMENT_DYNAMIC(CDiaServerAndRegister, CDialogEx)

CDiaServerAndRegister::CDiaServerAndRegister(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIASERVER, pParent)
{

}

CDiaServerAndRegister::~CDiaServerAndRegister()
{
}

void CDiaServerAndRegister::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_ListServer);
}

BOOL CDiaServerAndRegister::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	//��ʼ�������
	DWORD dwOldStyle = m_ListServer.GetExtendedStyle();
	m_ListServer.SetExtendedStyle(
		dwOldStyle | LVS_EX_FULLROWSELECT |
		LVS_EX_GRIDLINES |
		LVS_EX_CHECKBOXES
	);
	CRect rc;
	m_ListServer.GetClientRect(rc);
	int nWidth = rc.Width();
	m_ListServer.InsertColumn(0, L"lpDisplayName", 0, nWidth / 11);
	m_ListServer.InsertColumn(1, L"lpServiceName", 0, nWidth / 11);
	m_ListServer.InsertColumn(2, L"ServiceType", 0, nWidth / 11);
	m_ListServer.InsertColumn(3, L"CurrentState", 0, nWidth / 11);
	m_ListServer.InsertColumn(4, L"ControlsAccepted", 0, nWidth / 11);
	m_ListServer.InsertColumn(5, L"Win32ExitCode", 0, nWidth / 11);
	m_ListServer.InsertColumn(6, L"ServiceSpecificExitCode", 0, nWidth / 11);
	m_ListServer.InsertColumn(7, L"CheckPoint", 0, nWidth / 11);
	m_ListServer.InsertColumn(8, L"WaitHint", 0, nWidth / 11);
	m_ListServer.InsertColumn(9, L"ProcessId", 0, nWidth / 11);
	m_ListServer.InsertColumn(10, L"ServiceFlags", 0, nWidth / 11);
	showServer();
	UpdateData(FALSE);
	return 0;
}

void CDiaServerAndRegister::showServer()
{
	
	/*
	����������һ����������ƹ����������ӣ�����ָ�������ݿ⡣
	_Must_inspect_result_ WINADVAPI SC_HANDLE WINAPI OpenSCManagerW(
	_In_opt_ LPCWSTR    lpMachineName,  //ָ������ֹ�ַ�����ָ��Ŀ�����������ơ������ָ��ΪNULL ��������ָ��һ�����ַ�������ô�ú������ӵ����ؼ�����ϵķ�����ƹ�������
	_In_opt_ LPCWSTR    lpDatabaseName, //ָ������ֹ�ַ�����ָ����Ҫ�򿪵ķ�����ƹ������ݿ�����ơ����ַ���Ӧ����Ϊ SERVICES_ACTIVE_DATABASE�������ָ��ΪNULL �����Ĭ�ϵ� SERVICES_ACTIVE_DATABASE���ݿ⡣
	_In_     DWORD      dwDesiredAccess //ָ��������ʿ��ƹ�������Ȩ�ޡ�������Ҫ���Ȩ��ǰ��ϵͳ������ý��̵�Ȩ�����ƣ�����������������ƹ�������صİ�ȫ��������Ȩ�޿����б����⣬�ú����ĵ��ý���ʽ��ָ��SC_MANAGER_CONNECT �ķ���Ȩ�ޡ�
	);
	��������ɹ�������ֵ��һ��ָ���ķ�����ƹ��������ݿ�ľ����
	�������ʧ�ܣ�����ֵΪNULL ��
	Ҫ��ø���ϸ�Ĵ�����Ϣ������ʹ��GetLastError ��ô�����롣

	dwDesiredAccess����������ֵ
	// Service Control Manager object specific access types
	#define SC_MANAGER_CONNECT             0x0001
	#define SC_MANAGER_CREATE_SERVICE      0x0002
	#define SC_MANAGER_ENUMERATE_SERVICE   0x0004
	#define SC_MANAGER_LOCK                0x0008
	#define SC_MANAGER_QUERY_LOCK_STATUS   0x0010
	#define SC_MANAGER_MODIFY_BOOT_CONFIG  0x0020

	#define SC_MANAGER_ALL_ACCESS          (STANDARD_RIGHTS_REQUIRED      | \
	SC_MANAGER_CONNECT            | \
	SC_MANAGER_CREATE_SERVICE     | \
	SC_MANAGER_ENUMERATE_SERVICE  | \
	SC_MANAGER_LOCK               | \
	SC_MANAGER_QUERY_LOCK_STATUS  | \
	SC_MANAGER_MODIFY_BOOT_CONFIG)
	*/
	SC_HANDLE scHandle = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	if (scHandle == NULL) {
		CString str;
		str.Format(L"OpenSCManager fail(%ld)", GetLastError());
		MessageBox(str, TEXT("����"), 0);
		//printf("OpenSCManager fail(%ld)", GetLastError());
	}
	else {
		//printf("OpenSCManager -> scHandle=%p\n", scHandle);
		CString str1;
		str1.Format(L"������ƹ�����Ϊ%p\n", scHandle);
		MessageBox(str1, TEXT("scHandle"), 0);
		/*
		ö�ٵ�ǰϵͳ����
		_Must_inspect_result_
		WINADVAPI BOOL WINAPI EnumServicesStatusExW(
		_In_            SC_HANDLE               hSCManager,
		_In_            SC_ENUM_TYPE            InfoLevel,          //�������ַ������ԡ�Ŀǰֻ������һ��SC_ENUM_PROCESS_INFO��������ȡ���ֺͷ���״̬��Ϣ
		_In_            DWORD                   dwServiceType,
		_In_            DWORD                   dwServiceState,
		_Out_writes_bytes_opt_(cbBufSize)
		LPBYTE                  lpServices,         //������ָ�룬���ڽ���״̬��Ϣ�����ݵĸ�ʽȡ���ڵڶ����β�InfoLevel��ֵ��
		���������С��256K�ֽڡ�Ϊ�˻�ȡʵ����Ҫ�Ĵ�С�����ǿ��Խ����βθ�ֵΪNULL��ͬʱ����һ���β�cbBufSize��ֵΪ0��pcbBytesNeeded���صľ���ʵ�ʵ������С��ͬʱ��������ʧ�ܲ�����ERROR_MORE_DATA��XP��2003�£����������С��64K �ֽ�
		_In_            DWORD                   cbBufSize,
		_Out_           LPDWORD                 pcbBytesNeeded,     //���������̫С����ָ��ָ��ı����洢����ʣ�������Ϣ����Ҫ���ֽڴ�С��
		_Out_           LPDWORD                 lpServicesReturned, //ָ��ʵ�ʷ��صķ���ĸ�����ָ��
		_Inout_opt_     LPDWORD                 lpResumeHandle,     //һ��ָ�������ָ�롣�������ϣ�ָ��ö�ٵĿ�ʼ�㣻�ڵ�һ�ε���EnumServicesStatusEx����ʱ��ָ��ָ��ı�����������Ϊ0��������ϣ���������ɹ�����ôָ��ı�����ֵΪ0�����ǣ������������0����GetLastError�����õ���ERROR_MORE_DATA�����룬�����ֵ��ʾ��һ������Ŀ�ʼ�㣬���������´�ö��lpResumeHandle��ֵ��
		_In_opt_        LPCWSTR                 pszGroupName        //���������֡�������β���һ���ַ�������ôֻö��ָ��������ķ�������β�Ϊ���ַ���""����ôֻö�ٲ������κη�����ķ�������β�ΪNULL����ôö�����з���
		);

		InfoLevelֻ�������¸�ֵ
		typedef enum _SC_ENUM_TYPE {
		SC_ENUM_PROCESS_INFO        = 0
		} SC_ENUM_TYPE;

		dwServiceType����ȡ����ֵ
		#define SERVICE_KERNEL_DRIVER          0x0
		++
		0000001
		#define SERVICE_FILE_SYSTEM_DRIVER     0x00000002
		#define SERVICE_ADAPTER                0x00000004
		#define SERVICE_RECOGNIZER_DRIVER      0x00000008
		#define SERVICE_DRIVER                 (SERVICE_KERNEL_DRIVER       | \
		SERVICE_FILE_SYSTEM_DRIVER  | \
		SERVICE_RECOGNIZER_DRIVER)
		#define SERVICE_WIN32_OWN_PROCESS      0x00000010
		#define SERVICE_WIN32_SHARE_PROCESS    0x00000020
		#define SERVICE_WIN32                  (SERVICE_WIN32_OWN_PROCESS | \
		SERVICE_WIN32_SHARE_PROCESS)
		#define SERVICE_INTERACTIVE_PROCESS    0x00000100
		#define SERVICE_TYPE_ALL               (SERVICE_WIN32  | \
		SERVICE_ADAPTER | \
		SERVICE_DRIVER  | \
		SERVICE_INTERACTIVE_PROCESS)

		dwServiceState����ȡ����ֵ
		#define SERVICE_ACTIVE                 0x00000001
		#define SERVICE_INACTIVE               0x00000002
		#define SERVICE_STATE_ALL              (SERVICE_ACTIVE | SERVICE_INACTIVE)

		�ɹ����ط�0��ʧ�ܷ���0
		*/
		SC_ENUM_TYPE infoLevel = SC_ENUM_PROCESS_INFO;
		DWORD dwServiceType = SERVICE_WIN32;
		DWORD dwServiceState = SERVICE_STATE_ALL;
		LPBYTE lpServices = NULL;
		DWORD cbBufSize = 0;
		DWORD pcbBytesNeeded;
		DWORD servicesReturned;
		LPDWORD lpResumeHandle = NULL;
		LPCWSTR pszGroupName = NULL;
		//�������ߣ��Ȼ�ȡ����ĸ������ٻ�ȡ����������Ϣ
		BOOL ret = EnumServicesStatusEx(scHandle, infoLevel, dwServiceType, dwServiceState, lpServices, cbBufSize, &pcbBytesNeeded, &servicesReturned, lpResumeHandle, pszGroupName);
		//���ڴ�ʱ�ռ䲻�㣬���ʵ�ʷ��صķ�����Ϊ0����servicesReturned=0��
		printf("EnumServicesStatusEx scHandle=%p -> ret=%d, ��Ҫ�Ļ�������С=%ld, �������з���ĸ���=%ld\n", scHandle, ret, pcbBytesNeeded, servicesReturned);
		CString str2;
		str2.Format(L"EnumServicesStatusEx scHandle=%p -> ret=%d, ��Ҫ�Ļ�������С=%ld, �������з���ĸ���=%ld\n", scHandle, ret, pcbBytesNeeded, servicesReturned);
		MessageBox(str2, TEXT("֪ͨ"), 0);


		cbBufSize = pcbBytesNeeded;
		lpServices = new BYTE[cbBufSize];
		if (NULL == lpServices)
		{
			//printf("lpServices = new BYTE[%ld] -> fail(%ld)\n", cbBufSize, GetLastError());
			CString str3;
			str3.Format(L"��������С = new BYTE[%ld] -> fail(%ld)\n", cbBufSize, GetLastError());
			MessageBox(str3, TEXT("֪ͨ"), 0);
		}
		else {
			ret = EnumServicesStatusEx(scHandle, infoLevel, dwServiceType, dwServiceState, lpServices, cbBufSize, &pcbBytesNeeded, &servicesReturned, lpResumeHandle, pszGroupName);
			//printf("EnumServicesStatusEx scHandle=%p, cbBufSize=%ld -> ret=%d, pcbBytesNeeded=%ld, servicesReturned=%ld\n", scHandle, cbBufSize, ret, pcbBytesNeeded, servicesReturned);
			CString str4;
			str4.Format(L"EnumServicesStatusEx scHandle=%p, ��������С=%ld -> ret=%d, ��Ҫ�Ļ�������С=%ld, servicesReturned=%ld\n", scHandle, cbBufSize, ret, pcbBytesNeeded, servicesReturned);
			MessageBox(str4, TEXT("֪ͨ"), 0);
			/*
			typedef struct _ENUM_SERVICE_STATUS_PROCESSW {
			LPWSTR                    lpServiceName;
			LPWSTR                    lpDisplayName;
			SERVICE_STATUS_PROCESS    ServiceStatusProcess;
			} ENUM_SERVICE_STATUS_PROCESSW, *LPENUM_SERVICE_STATUS_PROCESSW;

			typedef struct _SERVICE_STATUS_PROCESS {
			DWORD   dwServiceType;
			DWORD   dwCurrentState;
			DWORD   dwControlsAccepted;
			DWORD   dwWin32ExitCode;
			DWORD   dwServiceSpecificExitCode;
			DWORD   dwCheckPoint;
			DWORD   dwWaitHint;
			DWORD   dwProcessId;
			DWORD   dwServiceFlags;
			} SERVICE_STATUS_PROCESS, *LPSERVICE_STATUS_PROCESS;

			dwCurrentState����������ֵ
			#define SERVICE_STOPPED                        0x00000001
			#define SERVICE_START_PENDING                  0x00000002
			#define SERVICE_STOP_PENDING                   0x00000003
			#define SERVICE_RUNNING                        0x00000004
			#define SERVICE_CONTINUE_PENDING               0x00000005
			#define SERVICE_PAUSE_PENDING                  0x00000006
			#define SERVICE_PAUSED                         0x00000007

			dwServiceFlags������0������ֵ
			#define SERVICE_RUNS_IN_SYSTEM_PROCESS  0x00000001
			*/
			CString strlpServiceName;

			LPENUM_SERVICE_STATUS_PROCESS lpServiceStatusProcess = (LPENUM_SERVICE_STATUS_PROCESS)lpServices;
			for (DWORD i = 0; i < servicesReturned; i++) {
				CString strlpServiceName(lpServiceStatusProcess[i].lpDisplayName);
				m_ListServer.InsertItem(i, strlpServiceName);
				CString strlpDisplayName(lpServiceStatusProcess[i].lpServiceName);
				m_ListServer.SetItemText(i, 1, strlpDisplayName);
				CString strdwServiceType;
				strdwServiceType.Format(_T("%ld"), lpServiceStatusProcess[i].ServiceStatusProcess.dwServiceType);
				m_ListServer.SetItemText(i, 2, strdwServiceType);

				CString strdwCurrentState;
				strdwCurrentState.Format(_T("%ld"), lpServiceStatusProcess[i].ServiceStatusProcess.dwCurrentState);
				m_ListServer.SetItemText(i, 3, strdwCurrentState);

				CString strdwControlsAccepted;
				strdwControlsAccepted.Format(_T("%ld"), lpServiceStatusProcess[i].ServiceStatusProcess.dwControlsAccepted);
				m_ListServer.SetItemText(i, 4, strdwControlsAccepted);

				CString strdwWin32ExitCode;
				strdwWin32ExitCode.Format(_T("%ld"), lpServiceStatusProcess[i].ServiceStatusProcess.dwWin32ExitCode);
				m_ListServer.SetItemText(i, 5, strdwWin32ExitCode);

				CString strdwServiceSpecificExitCode;
				strdwServiceSpecificExitCode.Format(_T("%ld"), lpServiceStatusProcess[i].ServiceStatusProcess.dwServiceSpecificExitCode);
				m_ListServer.SetItemText(i, 6, strdwServiceSpecificExitCode);

				CString strdwCheckPoint;
				strdwCheckPoint.Format(_T("%ld"), lpServiceStatusProcess[i].ServiceStatusProcess.dwCheckPoint);
				m_ListServer.SetItemText(i, 7, strdwCheckPoint);

				CString strdwWaitHint;
				strdwWaitHint.Format(_T("%ld"), lpServiceStatusProcess[i].ServiceStatusProcess.dwWaitHint);
				m_ListServer.SetItemText(i, 8, strdwWaitHint);

				CString strdwProcessId;
				strdwProcessId.Format(_T("%ld"), lpServiceStatusProcess[i].ServiceStatusProcess.dwProcessId);
				m_ListServer.SetItemText(i, 9, strdwProcessId);

				CString strdwServiceFlags;
				strdwServiceFlags.Format(_T("%ld"), lpServiceStatusProcess[i].ServiceStatusProcess.dwServiceFlags);
				m_ListServer.SetItemText(i, 10, strdwServiceFlags);
			}

			delete[] lpServices;
		}
		CloseServiceHandle(scHandle);
	}

}


BEGIN_MESSAGE_MAP(CDiaServerAndRegister, CDialogEx)
	ON_NOTIFY(NM_CLICK, IDC_LIST1, &CDiaServerAndRegister::OnNMClickList1)
	ON_BN_CLICKED(IDC_BUTTON1, &CDiaServerAndRegister::OnBnClickedStart)
	ON_BN_CLICKED(IDC_BUTTON2, &CDiaServerAndRegister::OnBnClickedClose)
END_MESSAGE_MAP()


// CDiaServerAndRegister ��Ϣ�������


void CDiaServerAndRegister::OnNMClickList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	*pResult = 0;
	int nCount = -1;
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	if (pNMListView->iItem != -1)
	{
		nCount = pNMListView->iItem;
		int temp = pNMItemActivate->iItem;//���˵ڼ���
		 m_count = temp;
	}
}


void CDiaServerAndRegister::OnBnClickedStart()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//��ȡ������Ϣ
	LPCWSTR lpMachineName = NULL;
	LPCWSTR lpDatabaseName = NULL;
	DWORD dwSCManagerDesiredAccess = SC_MANAGER_ALL_ACCESS;
	SC_HANDLE scHandle = OpenSCManager(lpMachineName, lpDatabaseName, dwSCManagerDesiredAccess);
	CString temp= m_ListServer.GetItemText(m_count, 1);
	LPCTSTR lpServiceName=temp.GetBuffer();
	DWORD dwDesiredAccess = SERVICE_QUERY_STATUS | SERVICE_START | SERVICE_STOP;
	SC_HANDLE serviceHandle = OpenService(scHandle, lpServiceName, dwDesiredAccess);
	//�жϷ����Ƿ��
	if (serviceHandle == NULL) {
		CString str1;
		str1.Format(L"OpenService scHandle=%p, lpServiceName=%s -> ret=0 -> fail(%ld)\n", scHandle, lpServiceName, GetLastError());
		MessageBox(str1, TEXT("����"), 0);
	}
	else {
		printf("OpenSCManager -> succ\n");
		CString str2;
		str2.Format(L"�������ݿ�򿪳ɹ�");
		MessageBox(str2, TEXT("�ɹ�"), 0);
		BOOL ret = StartService(serviceHandle, NULL, NULL);
		if (!ret) {
			//��1056��-�����ʵ�����������С�
			CString str3;
			str3.Format(L"StartService serviceHandle=%p -> fail(%ld)\n", serviceHandle, GetLastError());
			MessageBox(str3, TEXT("����"), 0);
		}
		else {
			CString str4;
			str4.Format(L"StartService serviceHandle=%p -> succ\n", serviceHandle);
			UpdateData(FALSE);
			MessageBox(str4, TEXT("�ɹ�"), 0);
		}
	}
	CloseServiceHandle(scHandle);
	DeleteService(serviceHandle);
}


void CDiaServerAndRegister::OnBnClickedClose()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	LPCWSTR lpMachineName = NULL;
	LPCWSTR lpDatabaseName = NULL;
	DWORD dwSCManagerDesiredAccess = SC_MANAGER_ALL_ACCESS;
	SC_HANDLE scHandle = OpenSCManager(lpMachineName, lpDatabaseName, dwSCManagerDesiredAccess);
	CString temp = m_ListServer.GetItemText(m_count, 1);
	LPCTSTR lpServiceName = temp.GetBuffer();
	DWORD dwDesiredAccess = SERVICE_QUERY_STATUS | SERVICE_START | SERVICE_STOP;
	SC_HANDLE serviceHandle = OpenService(scHandle, lpServiceName, dwDesiredAccess);
	//�жϷ����Ƿ��
	if (serviceHandle == NULL) {
		CString str1;
		str1.Format(L"OpenService scHandle=%p, lpServiceName=%s -> ret=0 -> fail(%ld)\n", scHandle, lpServiceName, GetLastError());
		MessageBox(str1, TEXT("����"), 0);
	}
	else {
		printf("OpenSCManager -> succ\n");
		CString str2;
		str2.Format(L"�������ݿ�򿪳ɹ�");
		MessageBox(str2, TEXT("�ɹ�"), 0);
		DWORD dwControl = SERVICE_CONTROL_STOP;
		SERVICE_STATUS serviceStatus = { 0 };
		//�رշ���
		bool ret = ControlService(serviceHandle, dwControl, &serviceStatus);
		if (!ret) {
			//��1056��-�����ʵ�����������С�
			CString str3;
			str3.Format(L"StartService serviceHandle=%p -> fail(%ld)\n", serviceHandle, GetLastError());
			MessageBox(str3, TEXT("����"), 0);
		}
		else {
			CString str4;
			str4.Format(L"StartService serviceHandle=%p -> succ\n", serviceHandle);
			UpdateData(FALSE);
			MessageBox(str4, TEXT("�ɹ�"), 0);
		}
	}
}
