import mysql.connector
class CSqlForChat():
    def __init__(self):
        #1.连接数据库
        config={'host':'192.168.43.81','user':'root','password':'root','port':'3306','database':'vssafe','charset':'utf8'}
        print("正在连接数据库...")
        self.conn=mysql.connector.connect(**config)
    def __del__(self):
        self.conn.close()
    def query(self,szSql,param=None):
        cursor=self.conn.cursor()
        try:
            cursor.execute(szSql,param)
            result=(cursor.fetchall(),cursor.rowcount)
            cursor.close()
            return result
        except:
            cursor.close()
            #发生错误时回滚
            self.conn.rollback()
            return None
    def insert(self,szSql,param=None):
        cursor=self.conn.cursor()
        try:
            cursor.execute(szSql, param)
            result=cursor.rowcount
            self.conn.commit()
            cursor.close()
            return result
        except:
            cursor.close()
            # 发生错误时回滚
            self.conn.rollback()
            return None
