#include <afxwin.h>
#include <windows.h>
#include <objidl.h>
#include <gdiplus.h>
#include <iostream>
#include <TCHAR.H>
#include <vld.h>

using namespace std;
using namespace Gdiplus;

int GetEncoderClsid( const WCHAR* format, CLSID* pClsid );

bool GetName( WCHAR* &wcName );

void CopyScrToBitmap( LPRECT lpRect );

#pragma comment (lib,"Gdiplus.lib")

#define ULONG_PTR ULONG
ULONG_PTR   gdpToken;

int GetEncoderClsid( const WCHAR* format, CLSID* pClsid )
{
    UINT  num = 0;          // number of image encoders
    UINT  size = 0;         // size of the image encoder array in bytes
    ImageCodecInfo* pImageCodecInfo = NULL;
    GetImageEncodersSize( &num, &size );
    if ( size == 0 ) {
        return -1;  // Failure
    }

    pImageCodecInfo = (ImageCodecInfo*)( malloc( size ) );
    if ( pImageCodecInfo == NULL ) {
        return -1;  // Failure
    }

    GetImageEncoders( num, size, pImageCodecInfo );
    for ( UINT j = 0; j < num; ++j ) {
        if ( 0 == wcscmp( pImageCodecInfo[j].MimeType, format ) ) {
            *pClsid = pImageCodecInfo[j].Clsid;

            free( pImageCodecInfo );

            return j;  // Success
        }
    }

    free( pImageCodecInfo );

    return -1;  // Failure    
}

bool GetName( WCHAR* &wcName ) {
    CTime ct = CTime::GetCurrentTime();
    char cName[30] = "";
    sprintf_s( cName, sizeof( cName ) / sizeof( char ), 
        "%04d%02d%02d%02d%02d%02d.jpeg", 
        ct.GetYear(), ct.GetMonth(), ct.GetDay(), 
        ct.GetHour(), ct.GetMinute(), ct.GetSecond() );
    DWORD dwSize = MultiByteToWideChar( CP_ACP, 0, cName, -1, NULL, 0 );
    if ( NULL != wcName ) {
        delete[] wcName;
        wcName = NULL;
    }

    wcName = new WCHAR[dwSize];
    if ( NULL == wcName ) {
        return false;
    }

    MultiByteToWideChar( CP_ACP, 0, cName, -1, wcName, dwSize );

    return true;
}

void CopyScrToBitmap( /*LPRECT lpRect*/ )
{
    HDC hScrDC = NULL;
    HDC hMemDC = NULL;

    HBITMAP hBitmap, hOldBitmap;

    // bitmap width&height
    /*int nWidth, nHeight;*/

    // screen resolution
    int xScrn, yScrn;

    // makesure the rectangle not NULL
    /*if ( IsRectEmpty( lpRect ) )
    {
        return;
    }*/

    // create desktop screen dc 
    HWND pScrWnd = ::GetDesktopWindow();
    hScrDC = ::GetDC( pScrWnd );

    // create mem dc
    hMemDC = CreateCompatibleDC( hScrDC );

    // get resolutions
    xScrn = GetDeviceCaps( hScrDC, HORZRES );
    yScrn = GetDeviceCaps( hScrDC, VERTRES );

    if ( xScrn < 0 || yScrn < 0 ) {
        return;
    }
    /*if ( lpRect->left < 0 )
        lpRect->left = 0;

    if ( lpRect->top < 0 )
        lpRect->top = 0;

    // 屏幕
    nWidth = ( lpRect->right - lpRect->left );
    nHeight = ( lpRect->bottom - lpRect->top );*/

    // create bitmap
    hBitmap = CreateCompatibleBitmap( hScrDC, xScrn/*nWidth*/, yScrn/*nHeight*/ );

    // select new bitmap
    hOldBitmap = (HBITMAP)SelectObject( hMemDC, hBitmap );

    // copy scr dc to mem dc
    BitBlt( hMemDC, 0, 0, xScrn/*nWidth*/, yScrn/*nHeight*/, hScrDC, 0, 0, SRCCOPY );

    // get bitmap handle
    hBitmap = (HBITMAP)SelectObject( hMemDC, hOldBitmap );

    // clean 
    ::ReleaseDC( pScrWnd, hScrDC );
    DeleteDC( hMemDC );   

    //初始化GDI+接口
    Gdiplus::GdiplusStartupInput gdpStartinput;
    GdiplusStartup( &gdpToken, &gdpStartinput, NULL );

    CLSID csdType;
    if ( -1 != GetEncoderClsid( L"image/jpeg", &csdType ) ) {
        Gdiplus::Bitmap* gdpBm = Gdiplus::Bitmap::FromHBITMAP( (HBITMAP)hBitmap, NULL );
        if ( 0 >= gdpBm->GetHeight() || 0 >= gdpBm->GetWidth() ) {
            cout << "Transfer format error!" << endl;
        } else {
            WCHAR* wcName = NULL;
            if ( GetName( wcName ) ) {
                //wcout<<wcName<<endl;
                gdpBm->Save( wcName, &csdType, NULL );
                delete wcName;
            }
        }
    } else {
        cout << "Get encoder class id failed." << endl;
    }

    ::DeleteObject( hBitmap );

    //注销GDI+资源
    GdiplusShutdown( gdpToken );
    ::EmptyClipboard();
    ::CloseClipboard();

    return;
}

int main()
{
    CopyScrToBitmap( /*CRect( 0, 0, 1920, 1080 )*/ );

    return 0;
}