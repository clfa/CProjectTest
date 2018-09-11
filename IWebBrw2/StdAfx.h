// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__17F60BFF_1C21_4902_BE24_1A3C53320C22__INCLUDED_)
#define AFX_STDAFX_H__17F60BFF_1C21_4902_BE24_1A3C53320C22__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxdisp.h>        // MFC Automation classes
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#include <EXDISP.H>
#include <ATLBASE.H>
CComModule _Module;
#include <ATLCOM.H>
#include <OAIDL.H>
#include <MSHTML.H>
#include <STRING>
#include <IOSTREAM>
#include <COMDEF.H>
#include <FSTREAM>
using std::ofstream;

//#include <MSHTML.IDL>
#endif // _AFX_NO_AFXCMN_SUPPORT


//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__17F60BFF_1C21_4902_BE24_1A3C53320C22__INCLUDED_)
