// DiaRelocList.cpp : 实现文件
//

#include "stdafx.h"
#include "MFC_PE.h"
#include "DiaRelocList.h"
#include "afxdialogex.h"
#include "PECalcu.h"


// CDiaRelocList 对话框

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
		WORD offset : 12; //本页的偏移量
		WORD type : 4;    //重定位类型（3）
	}OFFSET_TYPE, *POFFSET_TYPE;
	//重定位表RVA
	DWORD dwRelocRVA = m_pNt->OptionalHeader.DataDirectory[5].VirtualAddress;
	//是否为空
	if (!dwRelocRVA)
	{
		//printf("没有重定位表\n");
		CString tempstr;
		tempstr.Format(L"没有重定位表");
		m_RelocList += tempstr;
		m_RelocList += L"\r\n";
		return;
	}
	//重定位表在文件中的地址
	PIMAGE_BASE_RELOCATION pReloc = (PIMAGE_BASE_RELOCATION)(temp1.RVAtoFOA(dwRelocRVA) + m_FileBuf);

	//循环重定位表
	//如果SizeOfBlock为0，说明没有需要重定位的数据了
	while (pReloc->SizeOfBlock)
	{
		//当前重定位页RVA
		//printf("%08X\n\n", pReloc->VirtualAddress);
		CString tempstr1;
		tempstr1.Format(L"当前重定位页RVA:");
		m_RelocList += tempstr1;
		tempstr1 = L"";
		tempstr1.Format(L"%08X\r\n", pReloc->VirtualAddress);
		m_RelocList += tempstr1;
		m_RelocList += L"\r\n";
		//这一页一共有多少个重定位块（即多少个需要重定位的数据）
		DWORD dwCount = (pReloc->SizeOfBlock - sizeof(IMAGE_BASE_RELOCATION)) / sizeof(WORD);
		//指向重定位块地址
		POFFSET_TYPE pOffset = (POFFSET_TYPE)(pReloc + 1);
		//遍历每一个重定位块
		for (int i = 0; i < dwCount;i++)
		{
			//在这一页中的位置地址RVA
			DWORD dwRelocDataRVA = pReloc->VirtualAddress + pOffset->offset;
			//转成FOA
			DWORD dwRelocDataFOA = (DWORD)(temp1.RVAtoFOA(dwRelocDataRVA) + m_FileBuf);
			//实际需要重定位的数据地址是个VA
			DWORD dwRealDataVA = *(DWORD*)dwRelocDataFOA;
			//转成RVA，得到FOA
			DWORD dwRealDataRVA = dwRealDataVA - m_pNt->OptionalHeader.ImageBase;
			DWORD dwRealDataFOA = (DWORD)(temp1.RVAtoFOA(dwRealDataRVA) + m_FileBuf);
			//需要重定位的具体数据（字节数不确定）
			DWORD dwData = *(DWORD*)dwRealDataFOA;

		//printf("需要重定位的第%d个数据 RVA：%08X  VA:%08X  DATA:%08X\n",
		//		i + 1, dwRelocDataRVA, dwRealDataVA, dwData);
			CString tempstr2;
			tempstr2.Format(L"需要重定位的第%d个数据 RVA：%08X  VA:%08X  DATA:%08X", i + 1, dwRelocDataRVA, dwRealDataVA, dwData);
			m_RelocList += tempstr2;
			CString tempstr3 = L"\r\n";
			m_RelocList += tempstr3;

			//下一个重定位数据位置
			pOffset++;
		}
		      
		//下一页
		pReloc =
			(PIMAGE_BASE_RELOCATION)(pReloc->SizeOfBlock + (DWORD)pReloc);
	}

}


BEGIN_MESSAGE_MAP(CDiaRelocList, CDialogEx)
END_MESSAGE_MAP()


// CDiaRelocList 消息处理程序
