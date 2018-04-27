#pragma once
#include<vector>
#include<fstream>
using namespace std;
#define BUFMSG 2048//������Ϣ����ֽ���
#define BUFCHAT 1024//������󳤶�
#define BUFNAME 50 //��������ֽ���
#define BUFMSG 2048//һ�ν�����Ϣ������ֽ���


//ͷ����ͨѶĿ�ģ�������ѯMD5��ȫ��ɨ�財��������·��
enum CHATPURPOSE {
	SEARCHMD5 = 1,
	ALLSCAN,
	SAVEALLLOAD
};

typedef struct _SEARCHMD5 {
	CHAR szName[50];//Ҫ������MD5
}RUNSEARCHMD5;


typedef struct _CHATSEND {
	CHATPURPOSE m_type;
	union {
		char buf[BUFMSG];
		RUNSEARCHMD5 seu;//����MD5
	}m_content;
}CHATSEND;

class CClientScoket
{
public:
	CClientScoket();
	virtual ~CClientScoket();
	//��ʼ���ͻ����׽��֣����������ӷ�����
	bool ConnectServer(char* szIp, WORD port);
	//���𴴽��߳��������շ���������Ϣ
	char* Recv();
	bool Send(CHATPURPOSE purpose, char*bufSend, DWORD dwLen, SOCKET client = NULL);
	bool Close();

public:
	//���������¼ʹ��
	HANDLE m_hEvent = 0;
	//�ͻ����׽���
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

