#pragma once
#include<vector>
#include<fstream>
using namespace std;
#define BUFMSG 2048//当次消息最大字节数
#define BUFCHAT 1024//内容最大长度
#define BUFNAME 50 //名字最大字节数
#define BUFMSG 2048//一次接收消息的最大字节数


//头部：通讯目的，包括查询MD5，全盘扫描病毒，保存路径
enum CHATPURPOSE {
	SEARCHMD5 = 1,
	ALLSCAN,
	SAVEALLLOAD
};

typedef struct _SEARCHMD5 {
	CHAR szName[50];//要搜索的MD5
}RUNSEARCHMD5;


typedef struct _CHATSEND {
	CHATPURPOSE m_type;
	union {
		char buf[BUFMSG];
		RUNSEARCHMD5 seu;//搜索MD5
	}m_content;
}CHATSEND;

class CClientScoket
{
public:
	CClientScoket();
	virtual ~CClientScoket();
	//初始化客户端套接字，并尝试连接服务器
	bool ConnectServer(char* szIp, WORD port);
	//负责创建线程用来接收服务器的信息
	char* Recv();
	bool Send(CHATPURPOSE purpose, char*bufSend, DWORD dwLen, SOCKET client = NULL);
	bool Close();

public:
	//保存聊天记录使用
	HANDLE m_hEvent = 0;
	//客户端套接字
	SOCKET m_sClient = NULL;
	char m_szName[BUFNAME] = {};
	CHATSEND *m_pObjChatRecv;


private:
#define BUFNAME 50
	char m_bufRecv[BUFMSG] = {};
	fstream ReceiveFile;
	//recv function
	char* RecvMD5();
	//send function
	void SendMD5(char* bufSend, DWORD dwLen);
	CHATSEND m_ct;
};

