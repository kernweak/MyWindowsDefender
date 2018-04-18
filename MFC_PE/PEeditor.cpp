// PEeditor.cpp : 实现文件
//

#include "stdafx.h"
#include "MFC_PE.h"
#include "PEeditor.h"
#include "afxdialogex.h"


// CPEeditor 对话框

IMPLEMENT_DYNAMIC(CPEeditor, CDialogEx)

CPEeditor::CPEeditor(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOGPEeditor, pParent)
	, m_PEload(_T(""))
	, m_AddressOfEntryPoint(_T(""))
	, m_ImageBase(_T(""))
	, m_SizeOfImage(_T(""))
	, m_BaseOfCode(_T(""))
	, m_BaseOfData(_T(""))
	, m_SectionAlignment(_T(""))
	, m_FileAlignment(_T(""))
	, m_Magic(_T(""))
	, m_Subsystem(_T(""))
	, m_NumberOfSections(_T(""))
	, m_TimeDateStamp(_T(""))
	, m_SizeOfHeaders(_T(""))
	, m_DllCharacteristics(_T(""))
	, m_CheckSum(_T(""))
	, m_SizeOfOptionalHeader(_T(""))
	, m_NumberOfRvaAndSizes(_T(""))
{
}

CPEeditor::~CPEeditor()
{
}

void CPEeditor::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT17, m_PEload);
	DDX_Text(pDX, IDC_EDIT1, m_AddressOfEntryPoint);
	DDX_Text(pDX, IDC_EDIT2, m_ImageBase);
	DDX_Text(pDX, IDC_EDIT3, m_SizeOfImage);
	DDX_Text(pDX, IDC_EDIT4, m_BaseOfCode);
	DDX_Text(pDX, IDC_EDIT5, m_BaseOfData);
	DDX_Text(pDX, IDC_EDIT6, m_SectionAlignment);
	DDX_Text(pDX, IDC_EDIT7, m_FileAlignment);
	DDX_Text(pDX, IDC_EDIT8, m_Magic);
	DDX_Text(pDX, IDC_EDIT9, m_Subsystem);
	DDX_Text(pDX, IDC_EDIT10, m_NumberOfSections);
	DDX_Text(pDX, IDC_EDIT11, m_TimeDateStamp);
	DDX_Text(pDX, IDC_EDIT12, m_SizeOfHeaders);
	DDX_Text(pDX, IDC_EDIT13, m_DllCharacteristics);
	DDX_Text(pDX, IDC_EDIT14, m_CheckSum);
	DDX_Text(pDX, IDC_EDIT15, m_SizeOfOptionalHeader);
	DDX_Text(pDX, IDC_EDIT16, m_NumberOfRvaAndSizes);
}

BOOL CPEeditor::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	showPE();
		UpdateData(FALSE);
	return 0;
}

void CPEeditor::showPE()

{
	//获取文件句柄
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
		return ;
	}
	//判断PE头部
	DWORD dwNewPos = (DWORD)pFileBuf + ((PIMAGE_DOS_HEADER)pFileBuf)->e_lfanew;
	PIMAGE_NT_HEADERS32 pNTHeader = (PIMAGE_NT_HEADERS32)(dwNewPos);
	if (pNTHeader->Signature != IMAGE_NT_SIGNATURE) {
		cout << "这不是一个PE文件" << endl;
		return ;
	}
	//获取文件头，扩展头
	PIMAGE_FILE_HEADER pFileHeader = &(pNTHeader->FileHeader);
	PIMAGE_OPTIONAL_HEADER32 pOptionalHeader = &(pNTHeader->OptionalHeader);
	//PIMAGE_OPTIONAL_HEADER64
	m_AddressOfEntryPoint.Format(L"%x", pNTHeader->OptionalHeader.AddressOfEntryPoint);
	m_AddressOfEntryPoint.Format(L"%x", pNTHeader->OptionalHeader.AddressOfEntryPoint);
	m_ImageBase.Format(L"%x", pNTHeader->OptionalHeader.ImageBase);
	m_SizeOfImage.Format(L"%x", pNTHeader->OptionalHeader.SizeOfImage);
	m_BaseOfCode.Format(L"%x", pNTHeader->OptionalHeader.BaseOfCode);
	m_BaseOfData.Format(L"%x", pNTHeader->OptionalHeader.BaseOfData);
	m_SectionAlignment.Format(L"%x", pNTHeader->OptionalHeader.SectionAlignment);
	m_FileAlignment.Format(L"%x", pNTHeader->OptionalHeader.FileAlignment);
	m_Magic.Format(L"%x", pNTHeader->OptionalHeader.Magic);
	m_Subsystem.Format(L"%x", pNTHeader->OptionalHeader.Subsystem);
	m_NumberOfSections.Format(L"%x", pNTHeader->FileHeader.NumberOfSections);
	m_TimeDateStamp.Format(L"%x", pNTHeader->FileHeader.TimeDateStamp);
	m_SizeOfHeaders.Format(L"%x", pNTHeader->OptionalHeader.SizeOfHeaders);
	m_DllCharacteristics.Format(L"%x", pNTHeader->OptionalHeader.DllCharacteristics);
	m_CheckSum.Format(L"%x", pNTHeader->OptionalHeader.CheckSum);
	m_SizeOfOptionalHeader.Format(L"%x", pNTHeader->FileHeader.SizeOfOptionalHeader);
	m_NumberOfRvaAndSizes.Format(L"%x", pNTHeader->OptionalHeader.NumberOfRvaAndSizes);
	
}


BEGIN_MESSAGE_MAP(CPEeditor, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON4, &CPEeditor::OnBnClickedCalcu)
END_MESSAGE_MAP()


// CPEeditor 消息处理程序


void CPEeditor::OnBnClickedCalcu()
{
	// TODO: 在此添加控件通知处理程序代码
	CPECalcu Calcu;
	Calcu.DoModal();

}
