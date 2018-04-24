// DiaResourceList.cpp : 实现文件
//

#include "stdafx.h"
#include "MFC_PE.h"
#include "DiaResourceList.h"
#include "afxdialogex.h"
#include"PECalcu.h"


// CDiaResourceList 对话框

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
	//找到资源表
	DWORD dwResRVA =
		m_pNt->OptionalHeader.DataDirectory[2].VirtualAddress;
	DWORD dwResFOA = (DWORD)(temp1.RVAtoFOA(dwResRVA) + m_FileBuf);
	PIMAGE_RESOURCE_DIRECTORY pRes = (PIMAGE_RESOURCE_DIRECTORY)dwResFOA;

	//第一层（种类）
	//种类个数
	DWORD dwCountOfResType =
		pRes->NumberOfIdEntries + pRes->NumberOfNamedEntries;
	PIMAGE_RESOURCE_DIRECTORY_ENTRY pResEntry =
		(PIMAGE_RESOURCE_DIRECTORY_ENTRY)(pRes + 1);//指向第一个Entry
	if(pResEntry->DataIsDirectory)
	for (int i = 0; i < dwCountOfResType;i++)
	{
		

		//判断这种资源是字符串还是ID
		if (pResEntry->NameIsString)
		{
			//如果是字符串，NameOffset保存的就是这个字符串的RVA
			//得到名字字符串的FOA
			DWORD dwName= (DWORD)(pResEntry->NameOffset+ (DWORD)pRes);
			//NameOffset所指向的结构体是IMAGE_RESOURCE_DIR_STRING_U类型
			//这里保存了字符串的长度和起始位置
			PIMAGE_RESOURCE_DIR_STRING_U pName = (PIMAGE_RESOURCE_DIR_STRING_U)dwName;
			//这里的字符串不是以0结尾的，所以需要拷贝出来加上‘\0’结尾后再打印
			WCHAR *pResName = new WCHAR[pName->Length + 1]{};
			memcpy(pResName, pName, (pName->Length) * sizeof(WCHAR));
			//因为是WCHAR，所以用wprintf
			//wprintf(L"%s\n", pResName);
			m_ResourceList += pResName;
			m_ResourceList += L"\n\r\n";

			//释放内存
			delete[] pResName;
		}
		else   //id
		{
			char* arryResType[] = { "", "鼠标指针（Cursor）", "位图（Bitmap）", "图标（Icon）", "菜单（Menu）"
				, "对话框（Dialog）", "字符串列表（String Table）", "字体目录（Font Directory）", "字体（Font）", "快捷键（Accelerators）"
				, "非格式化资源（Unformatted）", "消息列表（Message Table）", "鼠标指针组（Croup Cursor）", "", "图标组（Group Icon）", ""
				, "版本信息（Version Information）" };
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

			//判断是否有下一层
			if (pResEntry->DataIsDirectory)
			{
				DWORD dwResSecond = (DWORD)pRes + pResEntry->OffsetToDirectory;
				PIMAGE_RESOURCE_DIRECTORY pResSecond = (PIMAGE_RESOURCE_DIRECTORY)dwResSecond;
				//第二层个数
				DWORD dwCountOfSecond =
					pResSecond->NumberOfIdEntries + pResSecond->NumberOfNamedEntries;
				//遍历每一个资源
				PIMAGE_RESOURCE_DIRECTORY_ENTRY pResSecondEntry = (PIMAGE_RESOURCE_DIRECTORY_ENTRY)(pResSecond + 1);
				if (pResSecondEntry->DataIsDirectory)
				for (int iSecond = 0; iSecond < dwCountOfSecond;iSecond++)
				{
					

					//判断这种资源是字符串还是ID 
					if (pResEntry->NameIsString)
					{
						//如果是字符串，NameOffset保存的就是这个字符串的RVA
						//得到名字字符串的FOA
						DWORD dwNameFOA = (DWORD)(pResEntry->NameOffset + (DWORD)pRes);
						//NameOffset所指向的结构体是IMAGE_RESOURCE_DIR_STRING_U类型
						//这里保存了字符串的长度和起始位置
						PIMAGE_RESOURCE_DIR_STRING_U pName = (PIMAGE_RESOURCE_DIR_STRING_U)dwNameFOA;
						//这里的字符串不是以0结尾的，所以需要拷贝出来加上‘\0’结尾后再打印
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
					//判断有没有下一层
					//第三层
					if (pResSecondEntry->DataIsDirectory)
					{
						//第三层的起始位置
						DWORD dwResThrid =
							(DWORD)pRes + pResSecondEntry->OffsetToDirectory;
						PIMAGE_RESOURCE_DIRECTORY pResThrid = (PIMAGE_RESOURCE_DIRECTORY)dwResThrid;

						PIMAGE_RESOURCE_DIRECTORY_ENTRY pResThridEntry = (PIMAGE_RESOURCE_DIRECTORY_ENTRY)(pResThrid + 1);
						//第三层，已经是最后一层，使用PIMAGE_RESOURCE_DIRECTORY_ENTRY中的
						//OffsetToData成员，得到PIMAGE_RESOURCE_DATA_ENTRY结构的位置
						PIMAGE_RESOURCE_DATA_ENTRY pResData =
							(PIMAGE_RESOURCE_DATA_ENTRY)(pResThridEntry->OffsetToData + (DWORD)pRes);
						//资源的RVA和Size
						DWORD dwResDataRVA = pResData->OffsetToData;
						DWORD dwResDataSize = pResData->Size;
						//PIMAGE_RESOURCE_DATA_ENTRY中的OffsetToData是个RVA
						DWORD dwResDataFOA = (DWORD)(temp1.RVAtoFOA(dwResDataRVA) + (DWORD)pRes);
						//DWORD dwResDataFOA = (DWORD)(dwResDataRVA) + (DWORD)pRes;
						//资源的二进制数据
						//遍历打印资源的二进制数据
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
					//下一个资源
					pResSecondEntry++;
				}
			}
		}
		//下一种资源
		pResEntry++;
	}
}

BEGIN_MESSAGE_MAP(CDiaResourceList, CDialogEx)
END_MESSAGE_MAP()                          


// CDiaResourceList 消息处理程序          
