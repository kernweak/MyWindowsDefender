
// MFC_PE.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CMFC_PEApp: 
// �йش����ʵ�֣������ MFC_PE.cpp
//

class CMFC_PEApp : public CWinApp
{
public:
	CMFC_PEApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CMFC_PEApp theApp;