// DiaExportList.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MFC_PE.h"
#include "DiaExportList.h"
#include "afxdialogex.h"
#include"PECalcu.h"


// CDiaExportList �Ի���

IMPLEMENT_DYNAMIC(CDiaExportList, CDialogEx)

CDiaExportList::CDiaExportList(PIMAGE_NT_HEADERS32 pnt, BYTE *FileBuf,CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIAEXPORT, pParent),
	m_pNt(pnt),
	m_FileBuf(FileBuf)
	, m_ExportList(_T(""))
{

}

CDiaExportList::~CDiaExportList()
{
}

void CDiaExportList::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT2, m_ExportList);
}

BOOL CDiaExportList::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	showExport();
	UpdateData(FALSE);
	return 0;
	return 0;
}

void CDiaExportList::showExport()
{
	//�ҵ�������
	m_ExportList=L"";
	CPECalcu temp1(m_pNt);
	DWORD dwExportRVA =
		m_pNt->OptionalHeader.DataDirectory[0].VirtualAddress;
	//��ȡ���ļ��е�λ��                      
	PIMAGE_EXPORT_DIRECTORY pExport =
		(PIMAGE_EXPORT_DIRECTORY)(temp1.RVAtoFOA(dwExportRVA) + m_FileBuf);

	//ģ������
	wchar_t tempK[3] = L"\r\n";
	char* pName = (char*)(temp1.RVAtoFOA(pExport->Name) + m_FileBuf);
	m_ExportList += pName;
	m_ExportList += tempK;
	//printf("%s\n", pName);
	//��ַ���еĸ���
	DWORD dwCountOfFuntions = pExport->NumberOfFunctions;
	//���Ʊ��еĸ���
	DWORD dwCountOfNames = pExport->NumberOfNames;

	//��ַ���ַ
	PDWORD pAddrOfFuntion = (PDWORD)(temp1.RVAtoFOA(pExport->AddressOfFunctions) + m_FileBuf);
	//���Ʊ��ַ
	PDWORD pAddrOfName = (PDWORD)(temp1.RVAtoFOA(pExport->AddressOfNames) + m_FileBuf);
	//��ű��ַ
	PWORD pAddrOfOrdial = (PWORD)(temp1.RVAtoFOA(pExport->AddressOfNameOrdinals) + m_FileBuf);
	//baseֵ
	DWORD dwBase = pExport->Base;
	//������ַ���е�Ԫ��

	for (int i = 0; i < dwCountOfFuntions; i++)
	{
		//��ַ���п��ܴ������õ�ֵ������Ϊ0��ֵ��
		if (pAddrOfFuntion[i] == 0)
		{
			continue;
		}
		//������ű����Ƿ���ֵ��ַ����±�ֵ��
		//���ж��Ƿ������Ƶ���
		bool bRet = false;
		for (int j = 0; j < dwCountOfNames; j++)
		{
			if (i == pAddrOfOrdial[j])
			{
				//ȡ�����Ʊ��е����Ƶ�ַRVA
				DWORD dwNameRVA = pAddrOfName[j];
				char* pFunName = (char*)(temp1.RVAtoFOA(dwNameRVA) + m_FileBuf);
				wchar_t temp[80];
				swprintf_s(temp, 80, L"%04d ", i + dwBase);
				m_ExportList += temp;
				CString temp2(pFunName);
				m_ExportList += temp2;
				wchar_t temp3[80];
				swprintf_s(temp3, 80, L"  0x%08x\r\n", pAddrOfFuntion[i]);
				CString temp4(temp3);
				m_ExportList += temp4;
				bRet = true;
				break;
			}
		}
		if (!bRet)
		{
			//��ű���û�У�˵��������ŵ�����
			//printf("%04d  %08X\n", i + dwBase, pAddrOfFuntion[i]);
			wchar_t temp1[80];
			swprintf_s(temp1, 80, L"%04d  %08X\n", i + dwBase, pAddrOfFuntion[i]);
		}

	}
}


BEGIN_MESSAGE_MAP(CDiaExportList, CDialogEx)
END_MESSAGE_MAP()


// CDiaExportList ��Ϣ�������
