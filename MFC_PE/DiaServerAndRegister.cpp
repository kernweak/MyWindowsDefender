// DiaServerAndRegister.cpp : 实现文件
//

#include "stdafx.h"
#include "MFC_PE.h"
#include "DiaServerAndRegister.h"
#include "afxdialogex.h"
#include <Windows.h>
#include<winsvc.h>

// CDiaServerAndRegister 对话框

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
	//初始化服务表
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
	函数建立了一个到服务控制管理器的连接，并打开指定的数据库。
	_Must_inspect_result_ WINADVAPI SC_HANDLE WINAPI OpenSCManagerW(
	_In_opt_ LPCWSTR    lpMachineName,  //指向零终止字符串，指定目标计算机的名称。如果该指针为NULL ，或者它指向一个空字符串，那么该函数连接到本地计算机上的服务控制管理器。
	_In_opt_ LPCWSTR    lpDatabaseName, //指向零终止字符串，指定将要打开的服务控制管理数据库的名称。此字符串应被置为 SERVICES_ACTIVE_DATABASE。如果该指针为NULL ，则打开默认的 SERVICES_ACTIVE_DATABASE数据库。
	_In_     DWORD      dwDesiredAccess //指定服务访问控制管理器的权限。在授予要求的权限前，系统会检查调用进程的权限令牌，该令牌针对与服务控制管理器相关的安全描述符的权限控制列表。此外，该函数的调用将隐式地指定SC_MANAGER_CONNECT 的访问权限。
	);
	如果函数成功，返回值是一个指定的服务控制管理器数据库的句柄。
	如果函数失败，返回值为NULL 。
	要获得更详细的错误信息，可以使用GetLastError 获得错误代码。

	dwDesiredAccess可以是下列值
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
		MessageBox(str, TEXT("错误"), 0);
		//printf("OpenSCManager fail(%ld)", GetLastError());
	}
	else {
		//printf("OpenSCManager -> scHandle=%p\n", scHandle);
		CString str1;
		str1.Format(L"服务控制管理句柄为%p\n", scHandle);
		MessageBox(str1, TEXT("scHandle"), 0);
		/*
		枚举当前系统服务
		_Must_inspect_result_
		WINADVAPI BOOL WINAPI EnumServicesStatusExW(
		_In_            SC_HANDLE               hSCManager,
		_In_            SC_ENUM_TYPE            InfoLevel,          //返回哪种服务属性。目前只定义了一种SC_ENUM_PROCESS_INFO，用来获取名字和服务状态信息
		_In_            DWORD                   dwServiceType,
		_In_            DWORD                   dwServiceState,
		_Out_writes_bytes_opt_(cbBufSize)
		LPBYTE                  lpServices,         //缓冲区指针，用于接收状态信息。数据的格式取决于第二个形参InfoLevel的值。
		数组的最大大小是256K字节。为了获取实际需要的大小，我们可以将此形参赋值为NULL，同时将下一个形参cbBufSize赋值为0，pcbBytesNeeded返回的就是实际的数组大小，同时函数调用失败并返回ERROR_MORE_DATA。XP和2003下，数组的最大大小是64K 字节
		_In_            DWORD                   cbBufSize,
		_Out_           LPDWORD                 pcbBytesNeeded,     //如果缓冲区太小，该指针指向的变量存储返回剩余服务信息所需要的字节大小。
		_Out_           LPDWORD                 lpServicesReturned, //指向实际返回的服务的个数的指针
		_Inout_opt_     LPDWORD                 lpResumeHandle,     //一个指向变量的指针。在输入上，指定枚举的开始点；在第一次调用EnumServicesStatusEx函数时，指针指向的变量必须设置为0。在输出上，如果函数成功，那么指向的变量的值为0。但是，如果函数返回0，且GetLastError函数得到了ERROR_MORE_DATA错误码，则这个值表示下一个服务的开始点，可以用作下次枚举lpResumeHandle的值。
		_In_opt_        LPCWSTR                 pszGroupName        //服务组名字。如果该形参是一个字符串，那么只枚举指定服务组的服务。如果形参为空字符串""，那么只枚举不属于任何服务组的服务。如果形参为NULL，那么枚举所有服务。
		);

		InfoLevel只能是如下该值
		typedef enum _SC_ENUM_TYPE {
		SC_ENUM_PROCESS_INFO        = 0
		} SC_ENUM_TYPE;

		dwServiceType可以取如下值
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

		dwServiceState可以取如下值
		#define SERVICE_ACTIVE                 0x00000001
		#define SERVICE_INACTIVE               0x00000002
		#define SERVICE_STATE_ALL              (SERVICE_ACTIVE | SERVICE_INACTIVE)

		成功返回非0，失败返回0
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
		//分两步走：先获取服务的个数，再获取具体服务的信息
		BOOL ret = EnumServicesStatusEx(scHandle, infoLevel, dwServiceType, dwServiceState, lpServices, cbBufSize, &pcbBytesNeeded, &servicesReturned, lpResumeHandle, pszGroupName);
		//由于此时空间不足，因此实际返回的服务数为0（即servicesReturned=0）
		printf("EnumServicesStatusEx scHandle=%p -> ret=%d, 需要的缓冲区大小=%ld, 缓冲区中服务的个数=%ld\n", scHandle, ret, pcbBytesNeeded, servicesReturned);
		CString str2;
		str2.Format(L"EnumServicesStatusEx scHandle=%p -> ret=%d, 需要的缓冲区大小=%ld, 缓冲区中服务的个数=%ld\n", scHandle, ret, pcbBytesNeeded, servicesReturned);
		MessageBox(str2, TEXT("通知"), 0);


		cbBufSize = pcbBytesNeeded;
		lpServices = new BYTE[cbBufSize];
		if (NULL == lpServices)
		{
			//printf("lpServices = new BYTE[%ld] -> fail(%ld)\n", cbBufSize, GetLastError());
			CString str3;
			str3.Format(L"缓冲区大小 = new BYTE[%ld] -> fail(%ld)\n", cbBufSize, GetLastError());
			MessageBox(str3, TEXT("通知"), 0);
		}
		else {
			ret = EnumServicesStatusEx(scHandle, infoLevel, dwServiceType, dwServiceState, lpServices, cbBufSize, &pcbBytesNeeded, &servicesReturned, lpResumeHandle, pszGroupName);
			//printf("EnumServicesStatusEx scHandle=%p, cbBufSize=%ld -> ret=%d, pcbBytesNeeded=%ld, servicesReturned=%ld\n", scHandle, cbBufSize, ret, pcbBytesNeeded, servicesReturned);
			CString str4;
			str4.Format(L"EnumServicesStatusEx scHandle=%p, 缓冲区大小=%ld -> ret=%d, 需要的缓冲区大小=%ld, servicesReturned=%ld\n", scHandle, cbBufSize, ret, pcbBytesNeeded, servicesReturned);
			MessageBox(str4, TEXT("通知"), 0);
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

			dwCurrentState可以是如下值
			#define SERVICE_STOPPED                        0x00000001
			#define SERVICE_START_PENDING                  0x00000002
			#define SERVICE_STOP_PENDING                   0x00000003
			#define SERVICE_RUNNING                        0x00000004
			#define SERVICE_CONTINUE_PENDING               0x00000005
			#define SERVICE_PAUSE_PENDING                  0x00000006
			#define SERVICE_PAUSED                         0x00000007

			dwServiceFlags可以是0或下列值
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


// CDiaServerAndRegister 消息处理程序


void CDiaServerAndRegister::OnNMClickList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
	int nCount = -1;
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	if (pNMListView->iItem != -1)
	{
		nCount = pNMListView->iItem;
		int temp = pNMItemActivate->iItem;//点了第几行
		 m_count = temp;
	}
}


void CDiaServerAndRegister::OnBnClickedStart()
{
	// TODO: 在此添加控件通知处理程序代码
	//获取服务信息
	LPCWSTR lpMachineName = NULL;
	LPCWSTR lpDatabaseName = NULL;
	DWORD dwSCManagerDesiredAccess = SC_MANAGER_ALL_ACCESS;
	SC_HANDLE scHandle = OpenSCManager(lpMachineName, lpDatabaseName, dwSCManagerDesiredAccess);
	CString temp= m_ListServer.GetItemText(m_count, 1);
	LPCTSTR lpServiceName=temp.GetBuffer();
	DWORD dwDesiredAccess = SERVICE_QUERY_STATUS | SERVICE_START | SERVICE_STOP;
	SC_HANDLE serviceHandle = OpenService(scHandle, lpServiceName, dwDesiredAccess);
	//判断服务是否打开
	if (serviceHandle == NULL) {
		CString str1;
		str1.Format(L"OpenService scHandle=%p, lpServiceName=%s -> ret=0 -> fail(%ld)\n", scHandle, lpServiceName, GetLastError());
		MessageBox(str1, TEXT("错误"), 0);
	}
	else {
		printf("OpenSCManager -> succ\n");
		CString str2;
		str2.Format(L"服务数据库打开成功");
		MessageBox(str2, TEXT("成功"), 0);
		BOOL ret = StartService(serviceHandle, NULL, NULL);
		if (!ret) {
			//〖1056〗-服务的实例已在运行中。
			CString str3;
			str3.Format(L"StartService serviceHandle=%p -> fail(%ld)\n", serviceHandle, GetLastError());
			MessageBox(str3, TEXT("错误"), 0);
		}
		else {
			CString str4;
			str4.Format(L"StartService serviceHandle=%p -> succ\n", serviceHandle);
			UpdateData(FALSE);
			MessageBox(str4, TEXT("成功"), 0);
		}
	}
	CloseServiceHandle(scHandle);
	DeleteService(serviceHandle);
}


void CDiaServerAndRegister::OnBnClickedClose()
{
	// TODO: 在此添加控件通知处理程序代码
	LPCWSTR lpMachineName = NULL;
	LPCWSTR lpDatabaseName = NULL;
	DWORD dwSCManagerDesiredAccess = SC_MANAGER_ALL_ACCESS;
	SC_HANDLE scHandle = OpenSCManager(lpMachineName, lpDatabaseName, dwSCManagerDesiredAccess);
	CString temp = m_ListServer.GetItemText(m_count, 1);
	LPCTSTR lpServiceName = temp.GetBuffer();
	DWORD dwDesiredAccess = SERVICE_QUERY_STATUS | SERVICE_START | SERVICE_STOP;
	SC_HANDLE serviceHandle = OpenService(scHandle, lpServiceName, dwDesiredAccess);
	//判断服务是否打开
	if (serviceHandle == NULL) {
		CString str1;
		str1.Format(L"OpenService scHandle=%p, lpServiceName=%s -> ret=0 -> fail(%ld)\n", scHandle, lpServiceName, GetLastError());
		MessageBox(str1, TEXT("错误"), 0);
	}
	else {
		printf("OpenSCManager -> succ\n");
		CString str2;
		str2.Format(L"服务数据库打开成功");
		MessageBox(str2, TEXT("成功"), 0);
		DWORD dwControl = SERVICE_CONTROL_STOP;
		SERVICE_STATUS serviceStatus = { 0 };
		//关闭服务
		bool ret = ControlService(serviceHandle, dwControl, &serviceStatus);
		if (!ret) {
			//〖1056〗-服务的实例已在运行中。
			CString str3;
			str3.Format(L"StartService serviceHandle=%p -> fail(%ld)\n", serviceHandle, GetLastError());
			MessageBox(str3, TEXT("错误"), 0);
		}
		else {
			CString str4;
			str4.Format(L"StartService serviceHandle=%p -> succ\n", serviceHandle);
			UpdateData(FALSE);
			MessageBox(str4, TEXT("成功"), 0);
		}
	}
}
