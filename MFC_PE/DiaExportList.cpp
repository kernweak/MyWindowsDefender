// DiaExportList.cpp : 实现文件
//

#include "stdafx.h"
#include "MFC_PE.h"
#include "DiaExportList.h"
#include "afxdialogex.h"
#include"PECalcu.h"


// CDiaExportList 对话框

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
	//找到导出表
	m_ExportList=L"";
	CPECalcu temp1(m_pNt);
	DWORD dwExportRVA =
		m_pNt->OptionalHeader.DataDirectory[0].VirtualAddress;
	//获取在文件中的位置                      
	PIMAGE_EXPORT_DIRECTORY pExport =
		(PIMAGE_EXPORT_DIRECTORY)(temp1.RVAtoFOA(dwExportRVA) + m_FileBuf);

	//模块名字
	wchar_t tempK[3] = L"\r\n";
	char* pName = (char*)(temp1.RVAtoFOA(pExport->Name) + m_FileBuf);
	m_ExportList += pName;
	m_ExportList += tempK;
	//printf("%s\n", pName);
	//地址表中的个数
	DWORD dwCountOfFuntions = pExport->NumberOfFunctions;
	//名称表中的个数
	DWORD dwCountOfNames = pExport->NumberOfNames;

	//地址表地址
	PDWORD pAddrOfFuntion = (PDWORD)(temp1.RVAtoFOA(pExport->AddressOfFunctions) + m_FileBuf);
	//名称表地址
	PDWORD pAddrOfName = (PDWORD)(temp1.RVAtoFOA(pExport->AddressOfNames) + m_FileBuf);
	//序号表地址
	PWORD pAddrOfOrdial = (PWORD)(temp1.RVAtoFOA(pExport->AddressOfNameOrdinals) + m_FileBuf);
	//base值
	DWORD dwBase = pExport->Base;
	//遍历地址表中的元素

	for (int i = 0; i < dwCountOfFuntions; i++)
	{
		//地址表中可能存在无用的值（就是为0的值）
		if (pAddrOfFuntion[i] == 0)
		{
			continue;
		}
		//根据序号表中是否有值地址表的下标值，
		//来判断是否是名称导出
		bool bRet = false;
		for (int j = 0; j < dwCountOfNames; j++)
		{
			if (i == pAddrOfOrdial[j])
			{
				//取出名称表中的名称地址RVA
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
			//序号表中没有，说明是以序号导出的
			//printf("%04d  %08X\n", i + dwBase, pAddrOfFuntion[i]);
			wchar_t temp1[80];
			swprintf_s(temp1, 80, L"%04d  %08X\n", i + dwBase, pAddrOfFuntion[i]);
		}

	}
}


BEGIN_MESSAGE_MAP(CDiaExportList, CDialogEx)
END_MESSAGE_MAP()


// CDiaExportList 消息处理程序
