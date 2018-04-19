// PECalcu.cpp : 实现文件
//

#include "stdafx.h"
#include "MFC_PE.h"
#include "PECalcu.h"
#include "afxdialogex.h"
#include<iostream>
using namespace std;


// CPECalcu 对话框

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


// CPECalcu 消息处理程序


void CPECalcu::OnBnClickedVA()
{
	// TODO: 在此添加控件通知处理程序代码
	GetDlgItem(IDC_EDIT1)->EnableWindow(TRUE);
	GetDlgItem(IDC_EDIT2)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT3)->EnableWindow(FALSE);
	temp = 1;
}


void CPECalcu::OnBnClickedRVA()
{
	// TODO: 在此添加控件通知处理程序代码
	GetDlgItem(IDC_EDIT1)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT2)->EnableWindow(TRUE);
	GetDlgItem(IDC_EDIT3)->EnableWindow(FALSE);
	temp = 2;
}


void CPECalcu::OnBnClickedFOA()
{
	GetDlgItem(IDC_EDIT1)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT2)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT3)->EnableWindow(TRUE);
	temp = 3;
	// TODO: 在此添加控件通知处理程序代码
}

BOOL CPECalcu::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	//获取文件句柄


	return 0;
}


void CPECalcu::OnBnClickedCalcu()
{
	// TODO: 在此添加控件通知处理程序代码
	HANDLE hFile = CreateFile((m_PEload.GetBuffer()), GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	//获取文件大小
	DWORD dwFileSize = GetFileSize(hFile, NULL);
	//创建一个堆空间存放
	CHAR *pFileBuf = new CHAR[dwFileSize];

	DWORD ReadFileSize = 0;
	//将文件读入内存
	ReadFile(hFile, pFileBuf, dwFileSize, &ReadFileSize, NULL);
	//判断DOS头
	PIMAGE_DOS_HEADER pFile = (PIMAGE_DOS_HEADER)pFileBuf;
	if (pFile->e_magic != IMAGE_DOS_SIGNATURE) {//0x5A4D
		cout << "这不是一个PE文件" << endl;
		return;
	}
	//判断PE头部
	DWORD dwNewPos = (DWORD)pFileBuf + ((PIMAGE_DOS_HEADER)pFileBuf)->e_lfanew;
	m_pNt = (PIMAGE_NT_HEADERS32)(dwNewPos);
	
	PIMAGE_FILE_HEADER pFileHeader = &(m_pNt->FileHeader);
	PIMAGE_OPTIONAL_HEADER32 pOptionalHeader = &(m_pNt->OptionalHeader);

	//PIMAGE_OPTIONAL_HEADER64
	CString temp1("0x");
	int temp2 = 0;
	DWORD temp3 = 0;
	UpdateData(TRUE);//获取变量值
	switch (temp) {
	case 1://输入的是VA
		temp1 += m_VA;
		StrToIntEx(temp1, STIF_SUPPORT_HEX, &temp2);
		temp3 = temp2;

		m_RVA.Format(L"%x",VAtoRVA(temp3));
		m_FOA.Format(L"%x", VAtoFOA(temp3));
		break;
	case 2://输入的是RVA
		temp1 += m_RVA;
		StrToIntEx(temp1, STIF_SUPPORT_HEX, &temp2);
		temp3 = temp2;
		m_VA.Format(L"%x",RVAtoVA(temp3));
		m_FOA.Format(L"%x", RVAtoFOA(temp3));
		break;
	case 3://输入的是FOA
		temp1 += m_FOA;
		StrToIntEx(temp1, STIF_SUPPORT_HEX, &temp2);
		temp3 = temp2;

		m_VA.Format(L"%x", FOAtoVA(temp3));
		m_RVA.Format(L"%x", FOAtoRVA(temp3));
		break;
	}
	UpdateData(FALSE);



	delete[] pFileBuf;
	CloseHandle(hFile);
}

DWORD CPECalcu::VAtoFOA(DWORD VA)
{
	return RVAtoFOA(VA - m_pNt->OptionalHeader.ImageBase);
}

DWORD CPECalcu::VAtoRVA(DWORD VA)
{
	return VA- m_pNt->OptionalHeader.ImageBase;
}

DWORD CPECalcu::RVAtoFOA(DWORD dwRVA)
{
	//此RVA落在哪个区段中
	//找到所在区段后，
	//减去所在区段的起始位置，加上在文件中的起始位置
	int nCountOfSection = m_pNt->FileHeader.NumberOfSections;
	PIMAGE_SECTION_HEADER pSec = IMAGE_FIRST_SECTION(m_pNt);
	DWORD dwSectionAligment = m_pNt->OptionalHeader.SectionAlignment;
	for (int i = 0;i < nCountOfSection;i++) {
		//求在内存的真实大小
		DWORD dwRealVirSize = pSec->Misc.VirtualSize%dwSectionAligment ?
			pSec->Misc.VirtualSize%dwSectionAligment*dwSectionAligment + dwSectionAligment : pSec->Misc.VirtualSize;
		if (dwRVA >= pSec->VirtualAddress&&dwRVA < pSec->VirtualAddress + dwRealVirSize) {
			return dwRVA - pSec->VirtualAddress + pSec->PointerToRawData;
		}
		pSec++;
	}
}

DWORD CPECalcu::FOAtoRVA(DWORD dwRVA)
{
	 
	//先判断在文件中哪个区段内
	//得到区段的起始相对虚拟地址
	int nCountOfSection = m_pNt->FileHeader.NumberOfSections;
	PIMAGE_SECTION_HEADER pSec = IMAGE_FIRST_SECTION(m_pNt);
	if (dwRVA < pSec->PointerToRawData)
		return dwRVA;
	DWORD dwSectionAligment = m_pNt->OptionalHeader.SectionAlignment;
	for (int i = 0;i < nCountOfSection;i++) {
		if (dwRVA >= pSec->PointerToRawData&&dwRVA < pSec->PointerToRawData + pSec->SizeOfRawData) {
			return dwRVA + pSec->VirtualAddress - pSec->PointerToRawData;
		}
		pSec++;
	}
	return 0;
}

DWORD CPECalcu::RVAtoVA(DWORD dwRVA)
{
	return dwRVA+m_pNt->OptionalHeader.ImageBase;
}

DWORD CPECalcu::FOAtoVA(DWORD dwFOA)
{
	return RVAtoVA(FOAtoRVA(dwFOA));
}




