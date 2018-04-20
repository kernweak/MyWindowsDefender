// PEeditor.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MFC_PE.h"
#include "PEeditor.h"
#include "afxdialogex.h"


// CPEeditor �Ի���

IMPLEMENT_DYNAMIC(CPEeditor, CDialogEx)

CPEeditor::CPEeditor(PIMAGE_NT_HEADERS32 pnt,CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOGPEeditor, pParent)
	, m_pNt(pnt)
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


CPEeditor::CPEeditor(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOGPEeditor, pParent)
	//, m_pNt(pnt)
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
	_TCHAR* temp = m_PEload.GetBuffer();
	//��ȡ�ļ����
	HANDLE hFile = CreateFile(temp, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
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
		return ;
	}
	//�ж�PEͷ��
	DWORD dwNewPos = (DWORD)pFileBuf + ((PIMAGE_DOS_HEADER)pFileBuf)->e_lfanew;
	PIMAGE_NT_HEADERS32 pNTHeader = (PIMAGE_NT_HEADERS32)(dwNewPos);
	if (pNTHeader->Signature != IMAGE_NT_SIGNATURE) {
		cout << "�ⲻ��һ��PE�ļ�" << endl;
		return ;
	}
	//��ȡ�ļ�ͷ����չͷ
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
	//�رվ��
	delete[] pFileBuf;
	CloseHandle(hFile);
	
}


BEGIN_MESSAGE_MAP(CPEeditor, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON4, &CPEeditor::OnBnClickedCalcu)
	ON_BN_CLICKED(IDC_BUTTON5, &CPEeditor::OnBnClickedGetTime)
	ON_BN_CLICKED(IDC_BUTTON3, &CPEeditor::OnBnClickedDirecList)
	ON_STN_CLICKED(IDC_STATIC8, &CPEeditor::OnStnClickedStatic8)
	ON_BN_CLICKED(IDC_BUTTON2, &CPEeditor::OnBnClickedSection)
END_MESSAGE_MAP()


// CPEeditor ��Ϣ�������


void CPEeditor::OnBnClickedCalcu()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CPECalcu Calcu;
	Calcu.m_PEload = m_PEload;
	Calcu.DoModal();
	
}


void CPEeditor::OnBnClickedGetTime()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	time_t dwTime = m_pNt->FileHeader.TimeDateStamp;
	struct tm Filetime;
	gmtime_s(&Filetime, (time_t*)&dwTime);
	CString strTime = {};
	strTime.Format(_T("%4d/%02d/%02d /%02d:/%02d:/%02d:"), Filetime.tm_year + 1900, Filetime.tm_mon + 1, Filetime.tm_mday, Filetime.tm_hour, Filetime.tm_min, Filetime.tm_sec);
	MessageBox(strTime, _T("ʱ��"), MB_OK);
}


void CPEeditor::OnBnClickedDirecList()
{
	CDirecList direcL(m_pNt);
	direcL.DoModal();
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}


void CPEeditor::OnStnClickedStatic8()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}


void CPEeditor::OnBnClickedSection()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
    CSectionSpace dwSect(m_pNt);
    dwSect.DoModal();
}
