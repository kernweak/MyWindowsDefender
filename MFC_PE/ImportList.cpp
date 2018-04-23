// ImportList.cpp : 实现文件
//

#include "stdafx.h"
#include "MFC_PE.h"
#include "ImportList.h"
#include "afxdialogex.h"
#include "PECalcu.h"

// CImportList 对话框

IMPLEMENT_DYNAMIC(CImportList, CDialogEx)

CImportList::CImportList(PIMAGE_NT_HEADERS32 pnt, BYTE *FileBuf,CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DiaImportList, pParent),
	m_pNt(pnt),
	m_pFileBuf(FileBuf)
{

}

CImportList::~CImportList()
{
}

void CImportList::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST2, m_ListCtrl);
	DDX_Control(pDX, IDC_LIST3, m_CListCtrl2);
}

BOOL CImportList::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	//显示导入表
	showImport();
	UpdateData(FALSE);
	return TRUE;
}

void CImportList::showImport()
{
	CPECalcu temp1(m_pNt);//用于计算
	CRect rectLocal;
	m_ListCtrl.GetClientRect(rectLocal);
	m_ListCtrl.SetColumnWidth(0, rectLocal.Width() / 6);         //设置列的宽度。
	m_ListCtrl.SetColumnWidth(1, rectLocal.Width() / 6);
	m_ListCtrl.SetColumnWidth(2, rectLocal.Width() / 6);
	m_ListCtrl.SetColumnWidth(4, rectLocal.Width() / 6);
	m_ListCtrl.SetColumnWidth(5, rectLocal.Width() / 6);
	m_ListCtrl.SetColumnWidth(5, rectLocal.Width() / 6);
	m_ListCtrl.InsertColumn(0, _T("DLL名称"), LVCFMT_CENTER, 200, -1);
	m_ListCtrl.InsertColumn(1, _T("OriginalFirstThunk"), LVCFMT_CENTER, 200, -1);
	m_ListCtrl.InsertColumn(2, _T("日期时间标志"), LVCFMT_CENTER, 200, -1);
	m_ListCtrl.InsertColumn(3, L"ForwarderChain", LVCFMT_CENTER, 200, -1);
	m_ListCtrl.InsertColumn(4, _T("名称"), LVCFMT_CENTER, 200, -1);
	m_ListCtrl.InsertColumn(5, _T("FirstThunk"), LVCFMT_CENTER, 200, -1);
	//找到导入表

	CRect rectLocal1;
	m_CListCtrl2.GetClientRect(rectLocal1);
	m_CListCtrl2.SetColumnWidth(0, rectLocal1.Width() / 5);         //设置列的宽度。
	m_CListCtrl2.SetColumnWidth(1, rectLocal1.Width() / 5);
	m_CListCtrl2.SetColumnWidth(2, rectLocal1.Width() / 5);
	m_CListCtrl2.SetColumnWidth(4, rectLocal1.Width() / 5);
	m_CListCtrl2.SetColumnWidth(5, rectLocal1.Width() / 5);
	m_CListCtrl2.InsertColumn(0, _T("ThunkRVA"), LVCFMT_CENTER, 200, -1);
	m_CListCtrl2.InsertColumn(1, _T("Thunk偏移"), LVCFMT_CENTER, 200, -1);
	m_CListCtrl2.InsertColumn(2, _T("Thunk值"), LVCFMT_CENTER, 200, -1);
	m_CListCtrl2.InsertColumn(3, _T("提示"), LVCFMT_CENTER, 200, -1);
	m_CListCtrl2.InsertColumn(4, _T("API名称"), LVCFMT_CENTER, 200, -1);

                         DWORD dwImportRVA = m_pNt->OptionalHeader.DataDirectory[1].VirtualAddress;
	//找到在文件中的位置
	 DWORD T = temp1.RVAtoFOA(dwImportRVA);
	//DWORD dwImportInfile = DWORD((char*)m_pFileBuf+T);
	PIMAGE_IMPORT_DESCRIPTOR pImport = (PIMAGE_IMPORT_DESCRIPTOR)(m_pFileBuf+T);
	int i = 0;
	while (pImport->Name) {
			char* T = (char*)(temp1.RVAtoFOA(pImport->Name) + m_pFileBuf);
			CString dwName(T);
			m_ListCtrl.InsertItem(i, dwName);
			CString dwOriginalFirstThunk;
			dwOriginalFirstThunk.Format(L"%08X", pImport->OriginalFirstThunk);
			m_ListCtrl.SetItemText(i, 1, dwOriginalFirstThunk);
			CString dwTime;
			dwTime.Format(L"%08X", pImport->TimeDateStamp);
			m_ListCtrl.SetItemText(i, 2, dwTime);
			CString dwFC;
			dwFC.Format(L"%08X", pImport->ForwarderChain);
			m_ListCtrl.SetItemText(i, 3, dwFC);
			CString dwM;
			dwM.Format(L"%08X", pImport->Name);
			m_ListCtrl.SetItemText(i, 4, dwM);
			CString dwF;
			dwF.Format(L"%08X", pImport->FirstThunk);
			m_ListCtrl.SetItemText(i, 5, dwF);
		i++;
		pImport++;
	}
}


 BEGIN_MESSAGE_MAP(CImportList, CDialogEx)
	 ON_NOTIFY(NM_CLICK, IDC_LIST2, &CImportList::OnNMClickList2)
 END_MESSAGE_MAP()


// CImportList 消息处理程序


 void CImportList::OnNMClickList2(NMHDR *pNMHDR, LRESULT *pResult)
 {
	 LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	 // TODO: 在此添加控件通知处理程序代码
	 *pResult = 0;
	 int nCount = -1;
	 CPECalcu temp1(m_pNt);
	 NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	 if (pNMListView->iItem != -1)
	 {
		 nCount = pNMListView->iItem;
		 int temp = pNMItemActivate->iItem;//点了第几行
		 int count = temp;

		 DWORD dwImportRVA = m_pNt->OptionalHeader.DataDirectory[1].VirtualAddress;
		 //找到在文件中的位置
		 DWORD T = temp1.RVAtoFOA(dwImportRVA);
		 //DWORD dwImportInfile = DWORD((char*)m_pFileBuf+T);
		 PIMAGE_IMPORT_DESCRIPTOR pImport = (PIMAGE_IMPORT_DESCRIPTOR)(m_pFileBuf + T);
		 DWORD OriginalFT = 0;//存放pImport->OriginalFirstThunk
		 while (pImport->Name) {
			 if (count == 0) {
				 OriginalFT = pImport->OriginalFirstThunk;
				 break;
			}
			 count--;
			 pImport++;
		 }
		 temp2= m_ListCtrl.GetItemText(temp, 5);
		 DWORD dwFirstRVA;
		 //dwFirstRVA = _wtoi(temp2);

		 swscanf_s(temp2.GetBuffer(), L"%x", &dwFirstRVA);
		 int i = 0;
		 DWORD dwFOA = temp1.RVAtoFOA(dwFirstRVA);
		 PIMAGE_THUNK_DATA  pFirsThunk =
			 (PIMAGE_THUNK_DATA)(temp1.RVAtoFOA(dwFirstRVA) + m_pFileBuf);
		 m_CListCtrl2.DeleteAllItems();
		 while (pFirsThunk->u1.AddressOfData)
		 {
			 CString strf;
			 strf.Format(L"%08X", OriginalFT);

			 m_CListCtrl2.InsertItem(i, strf);//插入ThankRVA
			 OriginalFT += 4;
			 CString strf1;
			 strf1.Format(L"%08X", dwFOA);
			 m_CListCtrl2.SetItemText(i, 1, strf1);//FOA值
			 dwFOA += 4;

			 CString strf2;
			 strf2.Format(L"%08X", pFirsThunk->u1.AddressOfData);
			 m_CListCtrl2.SetItemText(i, 2, strf2);//FOA值
			 //判断导入方式
			 if (IMAGE_SNAP_BY_ORDINAL32(pFirsThunk->u1.AddressOfData))
			 {
				 //说明是序号导入(低16位是其序号)
				 printf("\t\t%04X \n", pFirsThunk->u1.Ordinal & 0xFFFF);
			 }
			 else
			 {
				 //名称导入
				 PIMAGE_IMPORT_BY_NAME pImportName =
					 (PIMAGE_IMPORT_BY_NAME)(temp1.RVAtoFOA(pFirsThunk->u1.AddressOfData) + m_pFileBuf );
				 //printf("\t\t%04X %s \n", pImportName->Hint, pImportName->Name);
				 CString strint;
				 strint.Format(L"%04X", pImportName->Hint);
				 m_CListCtrl2.SetItemText(i, 3, strint);//插入序号
				 CString strn(pImportName->Name);
				 m_CListCtrl2.SetItemText(i, 4, strn);//插入name
			 }
			 //
			 i++;
			 pFirsThunk++;
		 }
	 }
 }
