// PECalcu.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MFC_PE.h"
#include "PECalcu.h"
#include "afxdialogex.h"
#include<iostream>
using namespace std;


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
	ON_BN_CLICKED(IDC_BUTTON3, &CPECalcu::OnBnClickedCalcu)
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

BOOL CPECalcu::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	//��ȡ�ļ����


	return 0;
}


void CPECalcu::OnBnClickedCalcu()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	HANDLE hFile = CreateFile((m_PEload.GetBuffer()), GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	//��ȡ�ļ���С
	DWORD dwFileSize = GetFileSize(hFile, NULL);
	//����һ���ѿռ���
	CHAR *pFileBuf = new CHAR[dwFileSize];

	DWORD ReadFileSize = 0;
	//���ļ������ڴ�
	ReadFile(hFile, pFileBuf, dwFileSize, &ReadFileSize, NULL);
	//�ж�DOSͷ
	PIMAGE_DOS_HEADER pFile = (PIMAGE_DOS_HEADER)pFileBuf;
	if (pFile->e_magic != IMAGE_DOS_SIGNATURE) {//0x5A4D
		cout << "�ⲻ��һ��PE�ļ�" << endl;
		return;
	}
	//�ж�PEͷ��
	DWORD dwNewPos = (DWORD)pFileBuf + ((PIMAGE_DOS_HEADER)pFileBuf)->e_lfanew;
	PIMAGE_NT_HEADERS32 m_pNt = (PIMAGE_NT_HEADERS32)(dwNewPos);
	
	PIMAGE_FILE_HEADER pFileHeader = &(m_pNt->FileHeader);
	PIMAGE_OPTIONAL_HEADER32 pOptionalHeader = &(m_pNt->OptionalHeader);
	//PIMAGE_OPTIONAL_HEADER64

}

DWORD CPECalcu::RVAtoFOA(DWORD dwRVA)
{
	//��RVA�����ĸ�������
	//�ҵ��������κ�
	//��ȥ�������ε���ʼλ�ã��������ļ��е���ʼλ��
	int nCountOfSection = m_pNt->FileHeader.NumberOfSections;
	PIMAGE_SECTION_HEADER pSec = IMAGE_FIRST_SECTION(m_pNt);
	DWORD dwSectionAligment = m_pNt->OptionalHeader.SectionAlignment;
	for (int i = 0;i < nCountOfSection;i++) {
		//�����ڴ����ʵ��С
		DWORD dwRealVirSize = pSec->Misc.VirtualSize%dwSectionAligment ?
			pSec->Misc.VirtualSize%dwSectionAligment*dwSectionAligment + dwSectionAligment : pSec->Misc.VirtualSize;
		if (dwRVA >= pSec->VirtualAddress&&dwRVA < pSec->VirtualAddress + dwRealVirSize) {
			return dwRVA - pSec->VirtualAddress + pSec->PointerToRawData;
		}
	}
}


