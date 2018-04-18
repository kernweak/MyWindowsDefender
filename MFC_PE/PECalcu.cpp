// PECalcu.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MFC_PE.h"
#include "PECalcu.h"
#include "afxdialogex.h"


// CPECalcu �Ի���

IMPLEMENT_DYNAMIC(CPECalcu, CDialogEx)

CPECalcu::CPECalcu(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIACALCU, pParent)
	, m_VA(_T(""))
	, m_RVA(_T(""))
	, m_FOA(_T(""))
{

}

CPECalcu::~CPECalcu()
{
}

void CPECalcu::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_VA);
	DDX_Text(pDX, IDC_EDIT2, m_RVA);
	DDX_Text(pDX, IDC_EDIT3, m_FOA);
}


BEGIN_MESSAGE_MAP(CPECalcu, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CPECalcu::OnBnClickedVA)
	ON_BN_CLICKED(IDC_BUTTON2, &CPECalcu::OnBnClickedRVA)
	ON_BN_CLICKED(IDC_BUTTON7, &CPECalcu::OnBnClickedFOA)
END_MESSAGE_MAP()


// CPECalcu ��Ϣ�������


void CPECalcu::OnBnClickedVA()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	GetDlgItem(IDC_EDIT1)->EnableWindow(TRUE);
	GetDlgItem(IDC_EDIT2)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT3)->EnableWindow(FALSE);
}


void CPECalcu::OnBnClickedRVA()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	GetDlgItem(IDC_EDIT1)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT2)->EnableWindow(TRUE);
	GetDlgItem(IDC_EDIT3)->EnableWindow(FALSE);
}


void CPECalcu::OnBnClickedFOA()
{
	GetDlgItem(IDC_EDIT1)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT2)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT3)->EnableWindow(TRUE);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}
