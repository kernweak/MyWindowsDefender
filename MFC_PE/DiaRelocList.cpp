// DiaRelocList.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MFC_PE.h"
#include "DiaRelocList.h"
#include "afxdialogex.h"
#include "PECalcu.h"


// CDiaRelocList �Ի���

IMPLEMENT_DYNAMIC(CDiaRelocList, CDialogEx)

CDiaRelocList::CDiaRelocList(PIMAGE_NT_HEADERS32 pnt, BYTE *FileBuf, CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIARELOCLIST, pParent),
	m_pNt(pnt),
	m_FileBuf(FileBuf),
	m_RelocList(_T(""))
{

}

CDiaRelocList::~CDiaRelocList()
{
}

void CDiaRelocList::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT2, m_RelocList);
}

BOOL CDiaRelocList::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	showReloc();
	UpdateData(FALSE);
	return 0;
}

void CDiaRelocList::showReloc()
{
	m_RelocList = L"";
	CPECalcu temp1(m_pNt);
	typedef struct _OFFSET_TYPE
	{
		WORD offset : 12; //��ҳ��ƫ����
		WORD type : 4;    //�ض�λ���ͣ�3��
	}OFFSET_TYPE, *POFFSET_TYPE;
	//�ض�λ��RVA
	DWORD dwRelocRVA = m_pNt->OptionalHeader.DataDirectory[5].VirtualAddress;
	//�Ƿ�Ϊ��
	if (!dwRelocRVA)
	{
		//printf("û���ض�λ��\n");
		CString tempstr;
		tempstr.Format(L"û���ض�λ��");
		m_RelocList += tempstr;
		m_RelocList += L"\r\n";
		return;
	}
	//�ض�λ�����ļ��еĵ�ַ
	PIMAGE_BASE_RELOCATION pReloc = (PIMAGE_BASE_RELOCATION)(temp1.RVAtoFOA(dwRelocRVA) + m_FileBuf);

	//ѭ���ض�λ��
	//���SizeOfBlockΪ0��˵��û����Ҫ�ض�λ��������
	while (pReloc->SizeOfBlock)
	{
		//��ǰ�ض�λҳRVA
		//printf("%08X\n\n", pReloc->VirtualAddress);
		CString tempstr1;
		tempstr1.Format(L"��ǰ�ض�λҳRVA:");
		m_RelocList += tempstr1;
		tempstr1 = L"";
		tempstr1.Format(L"%08X\r\n", pReloc->VirtualAddress);
		m_RelocList += tempstr1;
		m_RelocList += L"\r\n";
		//��һҳһ���ж��ٸ��ض�λ�飨�����ٸ���Ҫ�ض�λ�����ݣ�
		DWORD dwCount = (pReloc->SizeOfBlock - sizeof(IMAGE_BASE_RELOCATION)) / sizeof(WORD);
		//ָ���ض�λ���ַ
		POFFSET_TYPE pOffset = (POFFSET_TYPE)(pReloc + 1);
		//����ÿһ���ض�λ��
		for (int i = 0; i < dwCount;i++)
		{
			//����һҳ�е�λ�õ�ַRVA
			DWORD dwRelocDataRVA = pReloc->VirtualAddress + pOffset->offset;
			//ת��FOA
			DWORD dwRelocDataFOA = (DWORD)(temp1.RVAtoFOA(dwRelocDataRVA) + m_FileBuf);
			//ʵ����Ҫ�ض�λ�����ݵ�ַ�Ǹ�VA
			DWORD dwRealDataVA = *(DWORD*)dwRelocDataFOA;
			//ת��RVA���õ�FOA
			DWORD dwRealDataRVA = dwRealDataVA - m_pNt->OptionalHeader.ImageBase;
			DWORD dwRealDataFOA = (DWORD)(temp1.RVAtoFOA(dwRealDataRVA) + m_FileBuf);
			//��Ҫ�ض�λ�ľ������ݣ��ֽ�����ȷ����
			DWORD dwData = *(DWORD*)dwRealDataFOA;

		//printf("��Ҫ�ض�λ�ĵ�%d������ RVA��%08X  VA:%08X  DATA:%08X\n",
		//		i + 1, dwRelocDataRVA, dwRealDataVA, dwData);
			CString tempstr2;
			tempstr2.Format(L"��Ҫ�ض�λ�ĵ�%d������ RVA��%08X  VA:%08X  DATA:%08X", i + 1, dwRelocDataRVA, dwRealDataVA, dwData);
			m_RelocList += tempstr2;
			CString tempstr3 = L"\r\n";
			m_RelocList += tempstr3;

			//��һ���ض�λ����λ��
			pOffset++;
		}
		      
		//��һҳ
		pReloc =
			(PIMAGE_BASE_RELOCATION)(pReloc->SizeOfBlock + (DWORD)pReloc);
	}

}


BEGIN_MESSAGE_MAP(CDiaRelocList, CDialogEx)
END_MESSAGE_MAP()


// CDiaRelocList ��Ϣ�������
