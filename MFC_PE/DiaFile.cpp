// DiaFile.cpp : 实现文件
//

#include "stdafx.h"
#include "MFC_PE.h"
#include "DiaFile.h"
#include "afxdialogex.h"


// CDiaFile 对话框

IMPLEMENT_DYNAMIC(CDiaFile, CDialogEx)

CDiaFile::CDiaFile(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIAFILE, pParent)
	, m_foldload(_T(""))
	, m_wenJianXinXi(_T(""))
{

}

CDiaFile::~CDiaFile()
{
}

void CDiaFile::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MFCSHELLTREE2, m_SheelTree);
	DDX_Control(pDX, IDC_MFCSHELLLIST2, m_ShellList);
	DDX_Text(pDX, IDC_EDIT3, m_foldload);
	DDX_Text(pDX, IDC_EDIT4, m_wenJianXinXi);
}


BEGIN_MESSAGE_MAP(CDiaFile, CDialogEx)
END_MESSAGE_MAP()


// CDiaFile 消息处理程序
