// IWebBrw2.h : main header file for the IWEBBRW2 application
//

#if !defined(AFX_IWEBBRW2_H__BD746C5B_EE13_40FC_BD04_5CF25F9D0D02__INCLUDED_)
#define AFX_IWEBBRW2_H__BD746C5B_EE13_40FC_BD04_5CF25F9D0D02__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CIWebBrw2App:
// See IWebBrw2.cpp for the implementation of this class
//

class CIWebBrw2App : public CWinApp
{
public:
	CIWebBrw2App();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CIWebBrw2App)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CIWebBrw2App)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_IWEBBRW2_H__BD746C5B_EE13_40FC_BD04_5CF25F9D0D02__INCLUDED_)
