// DiaResourceList.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MFC_PE.h"
#include "DiaResourceList.h"
#include "afxdialogex.h"
#include"PECalcu.h"


// CDiaResourceList �Ի���

IMPLEMENT_DYNAMIC(CDiaResourceList, CDialogEx)

CDiaResourceList::CDiaResourceList(PIMAGE_NT_HEADERS32 pnt, BYTE *FileBuf, CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIARESOURCELIST, pParent)
	, m_ResourceList(_T("")),
	m_pNt(pnt),
	m_FileBuf(FileBuf)
{

}

CDiaResourceList::~CDiaResourceList()
{
}

void CDiaResourceList::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_ResourceList);
}

BOOL CDiaResourceList::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	showResource();
	UpdateData(FALSE);
	return 0;
}

void CDiaResourceList::showResource()
{
	m_ResourceList = L"";
	CPECalcu temp1(m_pNt);
	//�ҵ���Դ��
	DWORD dwResRVA =
		m_pNt->OptionalHeader.DataDirectory[2].VirtualAddress;
	DWORD dwResFOA = (DWORD)(temp1.RVAtoFOA(dwResRVA) + m_FileBuf);
	PIMAGE_RESOURCE_DIRECTORY pRes = (PIMAGE_RESOURCE_DIRECTORY)dwResFOA;

	//��һ�㣨���ࣩ
	//�������
	DWORD dwCountOfResType =
		pRes->NumberOfIdEntries + pRes->NumberOfNamedEntries;
	PIMAGE_RESOURCE_DIRECTORY_ENTRY pResEntry =
		(PIMAGE_RESOURCE_DIRECTORY_ENTRY)(pRes + 1);//ָ���һ��Entry
	if(pResEntry->DataIsDirectory)
	for (int i = 0; i < dwCountOfResType;i++)
	{
		

		//�ж�������Դ���ַ�������ID
		if (pResEntry->NameIsString)
		{
			//������ַ�����NameOffset����ľ�������ַ�����RVA
			//�õ������ַ�����FOA
			DWORD dwName= (DWORD)(pResEntry->NameOffset+ (DWORD)pRes);
			//NameOffset��ָ��Ľṹ����IMAGE_RESOURCE_DIR_STRING_U����
			//���ﱣ�����ַ����ĳ��Ⱥ���ʼλ��
			PIMAGE_RESOURCE_DIR_STRING_U pName = (PIMAGE_RESOURCE_DIR_STRING_U)dwName;
			//������ַ���������0��β�ģ�������Ҫ�����������ϡ�\0����β���ٴ�ӡ
			WCHAR *pResName = new WCHAR[pName->Length + 1]{};
			memcpy(pResName, pName, (pName->Length) * sizeof(WCHAR));
			//��Ϊ��WCHAR��������wprintf
			//wprintf(L"%s\n", pResName);
			m_ResourceList += pResName;
			m_ResourceList += L"\n\r\n";

			//�ͷ��ڴ�
			delete[] pResName;
		}
		else   //id
		{
			char* arryResType[] = { "", "���ָ�루Cursor��", "λͼ��Bitmap��", "ͼ�꣨Icon��", "�˵���Menu��"
				, "�Ի���Dialog��", "�ַ����б�String Table��", "����Ŀ¼��Font Directory��", "���壨Font��", "��ݼ���Accelerators��"
				, "�Ǹ�ʽ����Դ��Unformatted��", "��Ϣ�б�Message Table��", "���ָ���飨Croup Cursor��", "", "ͼ���飨Group Icon��", ""
				, "�汾��Ϣ��Version Information��" };
			if (pResEntry->Id < 17)
			{
				//printf("%s\n", arryResType[pResEntry->Id]);
				CString temp2(arryResType[pResEntry->Id]);
				m_ResourceList += temp2;
				m_ResourceList += L"\r\n";
			}
			else
			{
				//printf("%04X\n", pResEntry->Id);
				CString temp2;
				temp2.Format(L"%04X", pResEntry->Id);
				m_ResourceList += temp2;
				m_ResourceList += L"\r\n";
			}

			//�ж��Ƿ�����һ��
			if (pResEntry->DataIsDirectory)
			{
				DWORD dwResSecond = (DWORD)pRes + pResEntry->OffsetToDirectory;
				PIMAGE_RESOURCE_DIRECTORY pResSecond = (PIMAGE_RESOURCE_DIRECTORY)dwResSecond;
				//�ڶ������
				DWORD dwCountOfSecond =
					pResSecond->NumberOfIdEntries + pResSecond->NumberOfNamedEntries;
				//����ÿһ����Դ
				PIMAGE_RESOURCE_DIRECTORY_ENTRY pResSecondEntry = (PIMAGE_RESOURCE_DIRECTORY_ENTRY)(pResSecond + 1);
				if (pResSecondEntry->DataIsDirectory)
				for (int iSecond = 0; iSecond < dwCountOfSecond;iSecond++)
				{
					

					//�ж�������Դ���ַ�������ID 
					if (pResEntry->NameIsString)
					{
						//������ַ�����NameOffset����ľ�������ַ�����RVA
						//�õ������ַ�����FOA
						DWORD dwNameFOA = (DWORD)(pResEntry->NameOffset + (DWORD)pRes);
						//NameOffset��ָ��Ľṹ����IMAGE_RESOURCE_DIR_STRING_U����
						//���ﱣ�����ַ����ĳ��Ⱥ���ʼλ��
						PIMAGE_RESOURCE_DIR_STRING_U pName = (PIMAGE_RESOURCE_DIR_STRING_U)dwNameFOA;
						//������ַ���������0��β�ģ�������Ҫ�����������ϡ�\0����β���ٴ�ӡ
						WCHAR *pResName = new WCHAR[pName->Length + 1]{};
						memcpy(pResName, pName, (pName->Length) * sizeof(WCHAR));
						//wprintf(L"%s\n", pResName);
						m_ResourceList += pResName;
						m_ResourceList += L"\r\n";
						delete[] pResName;
					}
					else   //id
					{
						//printf("%04X\n", pResEntry->Id);
						CString temp3;
						temp3.Format(L"%04X", pResEntry->Id);
						m_ResourceList += temp3;
						m_ResourceList += L"\r\n";
					}
					//�ж���û����һ��
					//������
					if (pResSecondEntry->DataIsDirectory)
					{
						//���������ʼλ��
						DWORD dwResThrid =
							(DWORD)pRes + pResSecondEntry->OffsetToDirectory;
						PIMAGE_RESOURCE_DIRECTORY pResThrid = (PIMAGE_RESOURCE_DIRECTORY)dwResThrid;

						PIMAGE_RESOURCE_DIRECTORY_ENTRY pResThridEntry = (PIMAGE_RESOURCE_DIRECTORY_ENTRY)(pResThrid + 1);
						//�����㣬�Ѿ������һ�㣬ʹ��PIMAGE_RESOURCE_DIRECTORY_ENTRY�е�
						//OffsetToData��Ա���õ�PIMAGE_RESOURCE_DATA_ENTRY�ṹ��λ��
						PIMAGE_RESOURCE_DATA_ENTRY pResData =
							(PIMAGE_RESOURCE_DATA_ENTRY)(pResThridEntry->OffsetToData + (DWORD)pRes);
						//��Դ��RVA��Size
						DWORD dwResDataRVA = pResData->OffsetToData;
						DWORD dwResDataSize = pResData->Size;
						//PIMAGE_RESOURCE_DATA_ENTRY�е�OffsetToData�Ǹ�RVA
						DWORD dwResDataFOA = (DWORD)(temp1.RVAtoFOA(dwResDataRVA) + (DWORD)pRes);
						//DWORD dwResDataFOA = (DWORD)(dwResDataRVA) + (DWORD)pRes;
						//��Դ�Ķ���������
						//������ӡ��Դ�Ķ���������
						PBYTE pData = (PBYTE)dwResDataFOA;
				//	for (int iData = 0; iData < dwResDataSize; iData++)
				//	{
				//		if (iData % 16 == 0 && iData != 0)
				//		{
				//			//printf("\n");
				//			m_ResourceList += "\r\n";
				//		}
				//		//printf("%02X ", pData[iData]);
				//		CString temp4;
				//		temp4.Format(L"%02X ", pData[iData]);
				//		m_ResourceList += temp4;
				//		m_ResourceList += L"\r\n";
				//	}
					}
					//��һ����Դ
					pResSecondEntry++;
				}
			}
		}
		//��һ����Դ
		pResEntry++;
	}
}

BEGIN_MESSAGE_MAP(CDiaResourceList, CDialogEx)
END_MESSAGE_MAP()                          


// CDiaResourceList ��Ϣ�������          
