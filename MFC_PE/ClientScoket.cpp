#include "stdafx.h"
#include "ClientScoket.h"
#include <WS2tcpip.h>
#include "windows.h"
#include"DiaVirus.h"
CClientScoket::CClientScoket()
{
}


CClientScoket::~CClientScoket()
{
}
DWORD WINAPI Proc5(
	LPVOID lpParameter
) {
	CClientScoket *Pthis = (CClientScoket *)lpParameter;
	CHATSEND ct = {};
	CDiaVirus temp;
	while (1)
	{
		if (SOCKET_ERROR != recv(Pthis->m_sClient, (char*)&ct, sizeof(CHATSEND), NULL)) {
		
			//temp.m_Result
			//	CString aa(ct.m_content.seu);
			//aa.Format(L"%S", ct.m_content.seu);
			//temp.m_Result.SetWindowTextW(aa);

		}
	}
}

bool CClientScoket::ConnectServer(char * szIp, WORD port)
{
	//1.初始化套接字动态库
	WSADATA wsd;
	if (WSAStartup(MAKEWORD(2, 2), &wsd) != 0) {
		return false;
	}
	//检查返回的库是否是2.2
	if (LOBYTE(wsd.wVersion) != 2 || HIBYTE(wsd.wHighVersion) != 2) {
		WSACleanup();
		return false;
	}
	//2.创建套接字
	m_sClient = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (m_sClient == INVALID_SOCKET) {
		WSACleanup();
		return false;
	}
	//3.设置服务器地址
	SOCKADDR_IN servAddr = {};//服务器地址
	servAddr.sin_family = AF_INET;
	//转ip
	inet_pton(AF_INET, szIp, &servAddr.sin_addr);
	//获取端口
	servAddr.sin_port = htons((short)port);
	//6.连接服务器
	//SOCKERT sService;//跟服务端相连的套接字
	int nRetValue = connect(m_sClient, (sockaddr*)&servAddr, sizeof(SOCKADDR_IN));

	//HANDLE hTread = INVALID_HANDLE_VALUE;
	//hTread = CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)Proc3, this, NULL, NULL);
	//WaitForSingleObject(hTread,20);

	return true;
}

char * CClientScoket::Recv()
{
	CHATSEND ct = {};

	if (SOCKET_ERROR == recv(m_sClient, (char*)&ct, sizeof(CHATSEND), NULL)) {

		int temp = 0;
		printf("recv error!\n");
		
		return nullptr;
	}
	m_pObjChatRecv = &ct;
	switch (ct.m_type) {
	case SEARCHMD5:
		return RecvMD5();
	default:
		break;
	}
	return nullptr;
}

bool CClientScoket::Send(CHATPURPOSE purpose, char * bufSend, DWORD dwLen, SOCKET client)
{
	switch (purpose)
	{
	case SEARCHMD5:
		SendMD5(bufSend, dwLen);
		break;
	default:
		break;
	}
	return false;
}

bool CClientScoket::Close()
{
	WSACleanup();
	closesocket(m_sClient);
	return true;
}

char * CClientScoket::RecvMD5()
{
	MessageBoxA(NULL, m_pObjChatRecv->m_content.seu.szName, "搜索MD5", MB_OK);
	return nullptr;
}

void CClientScoket::SendMD5(char * bufSend, DWORD dwLen)
{
	CHATSEND ct = { SEARCHMD5 };
	memcpy_s(ct.m_content.seu.szName, dwLen, bufSend, dwLen);
	send(m_sClient, (char*)&ct, sizeof(ct), NULL);
}
