#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CComboxExTestApp: 
// �йش����ʵ�֣������ ComboxExTestApp.cpp
//

class CComboxExTestApp : public CWinApp
{
public:
	CComboxExTestApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CComboxExTestApp theApp;