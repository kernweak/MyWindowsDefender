from socket import *
import threading
import struct
from enum import Enum
import time
from CDataBase import *
class EnumMessageType(Enum):
    SEARCHMD5=1
    ALLSCAN=2
    SAVEALLLOAD=3


class CServerSocket():
    # 看看是不是这里
    conn = CSqlForChat()

    def __init__(self, ip, port):
        ADDR = (ip, port)
        # 初始化socket
        print('正在启动服务器....')
        self.socketServer = socket(AF_INET, SOCK_STREAM)
        self.socketServer.bind(ADDR)
        self.socketServer.listen()
        print("服务器启动成功,等待客户端连接....")
        # 外部调用的accept接口

    def MyAccept(self):
        # 创建线程accept
        t = threading.Thread(target=self.__acceptProc__)
        t.start()

    def __acceptProc__(self):
        while True:
            # _accept返回的是个元组（套接字，客户端地址）
            socketClient, addrClient = self.socketServer.accept()
            # socketClient.send("连接成功！".encode('gb2312'))
            CServerSocket.dictClient[socketClient] = None
            # 创建单独线程等待客户端消息
            # 创建线程接收消息
            t = threading.Thread(target=self.__recvProc__, args=(socketClient,))
            t.start()

    # 接收消息的线程
    def __recvProc__(self, s):
        while True:
            try:
                message = s.recv(CServerSocket.BUFSIZE + 10)
                # 消息类型
                type, = struct.unpack("i", message[:4])
                CServerSocket.dictFun[type](s, message)
            except Exception as TheExp:
                print(str(TheExp))
                name = CServerSocket.dictClient.get(s)
                if name == None:
                    return
                s.close()
                name = CServerSocket.dictClient.pop(s)
                print("客户端退出：" + name)
                CServerSocket.UpdateUser(s, False, name)
                return


    def __SearchMD5__(s,msg):
        #查询MD5是否在数据库里
        MD5, = struct.unpack("50s", msg[4:54])
        MD5 = MD5.decode("gb2312").rstrip('\0')
        result = CServerSocket.conn.query("SELECT * from md5list WHERE MD5=%s", (MD5,))
        # 返回查询结果
        message_type = EnumMessageType.SEARCHMD5
        message_len = 50
        message = ""
        if result==None or result[1]==0:
            print("不是病毒：" + MD5)
            message="不是病毒!".encode('gb2312')
        else:
            print("是病毒：" + MD5)
            message = "是病毒!".encode('gb2312')
        message_send = struct.pack("l2048s", message_type.value, message)
        s.send(message_send)
        s.send(message_send)
        print('发送成功');
    def __ALLSCAN__(s, msg):
        # 查询MD5是否在数据库里
        MD5, = struct.unpack("50s", msg[4:54])
        MD5 = MD5.decode("gb2312").rstrip('\0')
        result = CServerSocket.conn.query("SELECT * from md5list WHERE MD5=%s", (MD5,))
        # 返回查询结果
        message_type = EnumMessageType.SEARCHMD5
        message_len = 50
        message = ""
        if result == None or result[1] == 0:
            print("不是病毒：" + MD5)
            message = "不是病毒!".encode('gb2312')
        else:
            print("是病毒：" + MD5)
            message = "是病毒!".encode('gb2312')
        message_send = struct.pack("l2048s", message_type.value, message)
        s.send(message_send)
        s.send(message_send)
        print('发送成功');


    dictFun={
        1: __SearchMD5__,
        2: __ALLSCAN__,

    }
    BUFSIZE=2048+4
    dictClient={
    }

