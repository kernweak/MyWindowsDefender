#pragma once
#include<iostream>
#include<fstream>
#include<Windows.h>
#include<stdio.h>
#include<string>
#include<iomanip>
#include"PECalcu.h"
using namespace std;
extern void DirectoryString(DWORD dwIndex);

// CPEeditor �Ի���

class CPEeditor : public CDialogEx
{
	DECLARE_DYNAMIC(CPEeditor)

public:
	CPEeditor(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CPEeditor();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOGPEeditor };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
public:
	CString m_PEload;//�ļ���·��
	void showPE();
	CString m_AddressOfEntryPoint;
	CString m_ImageBase;
	CString m_SizeOfImage;
	CString m_BaseOfCode;
	CString m_BaseOfData;
	CString m_SectionAlignment;
	CString m_FileAlignment;
	CString m_Magic;
	CString m_Subsystem;
	CString m_NumberOfSections;
	CString m_TimeDateStamp;
	CString m_SizeOfHeaders;
	CString m_DllCharacteristics;
	CString m_CheckSum;
	CString m_SizeOfOptionalHeader;
	CString m_NumberOfRvaAndSizes;
	afx_msg void OnBnClickedCalcu();
};
