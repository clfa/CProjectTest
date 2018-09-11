#ifndef COPYIMGFROMCLPBRD_H_
#define COPYIMGFROMCLPBRD_H_

#include <iostream>
#include <TCHAR.H>
#include <afxwin.h>
#include <windows.h>
#include <objidl.h>
#include <gdiplus.h>
#include <vld.h>
#include "CopyImgFromClpbrd.h"

using namespace std;
using namespace Gdiplus;

#pragma comment (lib,"Gdiplus.lib")

int GetEncoderClsid( const WCHAR* format, CLSID* pClsid );

bool GetName( WCHAR* &wcName );

int CopyTxtFromClpbrd();

int CopyImgFromClpbrd();

void ManualFormatClipboard();

void DefineFormatClpbrd();

void CopyImgFromOLEClpbrd();

#endif