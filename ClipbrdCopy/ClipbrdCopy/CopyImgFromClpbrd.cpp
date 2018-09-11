// CopyImgFromClpbrd.cpp : 定义控制台应用程序的入口点。
//

#include "CopyImgFromClpbrd.h"
#include <afxole.h>

#define ULONG_PTR ULONG
ULONG_PTR   gdpToken;

#define COPYIMGFRMCLIPBRD
#define COPYMETAFILEPICT

class DefFmtClpStruct {
public:
    DefFmtClpStruct( const int& nValue ) {
        nEnumNum = nValue;
        pCopyData = (ULONG_PTR*)&nEnumNum;
    }

    DefFmtClpStruct( DefFmtClpStruct* value ) {
        Copy( value );
    }

    ~DefFmtClpStruct() {

    }

    void Print()  {
        cout << endl << "=========================================================================================" << endl;
        cout << "nEnumNum = " << nEnumNum << ", &nEnumNum = " << &nEnumNum << ", pCopyData = " << pCopyData << endl;
        cout << endl << "=========================================================================================" << endl;
    }

    void Copy( const DefFmtClpStruct& value ) {
        nEnumNum = value.nEnumNum;
        pCopyData = value.pCopyData;
    }

    void Copy( DefFmtClpStruct* value ) {
        nEnumNum = value->nEnumNum;
        pCopyData = value->pCopyData;
    }

private:
    int nEnumNum;
    ULONG_PTR* pCopyData;
};

BOOL CALLBACK MyProc( EmfPlusRecordType recordType, UINT flags, UINT dataSize, const BYTE* data, VOID *callbackData );

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

int CopyTxtFromClpbrd()
{
    HWND hWnd = AfxGetMainWnd()->GetSafeHwnd();             // 获取安全窗口句柄
    if (!::OpenClipboard( hWnd )) {                         // 打开剪贴板
        GdiplusShutdown( gdpToken );                        // 注销GDI+资源
        cout << "Open clipboard Failed!" << endl;
        return 1;
    }

    HANDLE hHandle = NULL;
    if (IsClipboardFormatAvailable( CF_TEXT )) {
        hHandle = ::GetClipboardData( CF_TEXT);          // passing a CF_METAFILEPICT handle by means of DDE
        char szText[50];
        LPCSTR pData = (LPCSTR)GlobalLock( hHandle );
        memcpy( szText, pData, 50 );
        cout << szText << endl;
        ::GlobalUnlock( hHandle );
        ::GlobalFree( hHandle );
    }

    ::EmptyClipboard();
    ::CloseClipboard();

    return 0;
}

int CopyImgFromClpbrd()
{
    //初始化GDI+接口
    Gdiplus::GdiplusStartupInput gdpStartinput;
    GdiplusStartup( &gdpToken, &gdpStartinput, NULL );

    HWND hWnd = AfxGetMainWnd()->GetSafeHwnd();             // 获取安全窗口句柄
    if (!::OpenClipboard( hWnd )) {                         // 打开剪贴板
        GdiplusShutdown( gdpToken );                        // 注销GDI+资源
        cout << "Open clipboard Failed!" << endl;
        return 1;
    }
       
    HGLOBAL hGlobal = NULL;
    LPMETAFILEPICT  lpMFP = NULL;
    HANDLE hHandle = NULL;
    Gdiplus::Bitmap* gdpBm = NULL;
    UINT nImgType = 0;

    if (IsClipboardFormatAvailable( CF_METAFILEPICT )) {
        nImgType = CF_METAFILEPICT;
        hGlobal = (HGLOBAL)::GetClipboardData( CF_METAFILEPICT );          // passing a CF_METAFILEPICT handle by means of DDE
        lpMFP = (LPMETAFILEPICT)GlobalLock( hGlobal );
        GlobalUnlock( hGlobal );
    } else if (IsClipboardFormatAvailable( CF_BITMAP )) {
        nImgType = CF_BITMAP;
        hHandle = ::GetClipboardData( CF_BITMAP );              // 获取剪贴板数据句柄
        gdpBm = Gdiplus::Bitmap::FromHBITMAP( (HBITMAP)hHandle, NULL );
    }

    if (NULL == hHandle && NULL == hGlobal) {
        //注销GDI+资源
        GdiplusShutdown( gdpToken );
        ::CloseClipboard();                                 // 关闭剪贴板
        cout << "No screen images to save!" << endl;
        return 2;
    }

    CLSID csdType;
    if (-1 != GetEncoderClsid( L"image/jpeg", &csdType )) {
        WCHAR* wcName = NULL;
        if (GetName( wcName )) {
            switch (nImgType) {
                case CF_METAFILEPICT:
                    {
                        const int nWidth = 1920;
                        const int nHeight = 1200;
                        Gdiplus::Bitmap CacheImage( nWidth, nHeight, 4, PixelFormat32bppARGB, 0 );
                        Gdiplus::Graphics CacheGraphics( &CacheImage );
                        SolidBrush brush( Color( 255, 255, 255, 255 ) );
                        CacheGraphics.FillRectangle( &brush, 0, 0, nWidth, nHeight );
                        Gdiplus::Metafile metafile( lpMFP->hMF, 0 );
                        CacheGraphics.EnumerateMetafile( &metafile, PointF( 0, 0 ), MyProc, &metafile );
                        HDC hDC = ::GetDC( NULL );
                        ASSERT( NULL != hDC );
                        if (NULL != hDC) {
                            Gdiplus::Graphics Graphic( hDC );
                            Graphic.DrawImage( &CacheImage, 0, 0 );
                            if (CacheImage.GetHeight() > 0 && CacheImage.GetWidth() > 0) {
                                Gdiplus::Status stRet = CacheImage.Save( wcName, &csdType, NULL );
                                VERIFY( Gdiplus::Ok == stRet );
                            }
                        }
                    }
                    break;
                case CF_BITMAP:
                    {
                        if (0 >= gdpBm->GetHeight() || 0 >= gdpBm->GetWidth()) {
                            cout << "Transfer format error!" << endl;
                        } else {
                            WCHAR* wcName = NULL;
                            if (GetName( wcName )) {
                                gdpBm->Save( wcName, &csdType, NULL );
                            }
                        }
                    }
                    break;
            }
            
            delete wcName;
            wcName = NULL;
        }        
    } else {
        cout << "Get encoder class id failed." << endl;
    }

    //注销GDI+资源
    GdiplusShutdown( gdpToken );
    ::EmptyClipboard();
    ::CloseClipboard(); 
    GlobalFree( hGlobal );

    return 0;
}

void ManualFormatClipboard()
{
    //自定义数据结构体
    typedef struct _MYSTRUCT {
        CHAR charVal[16];
        int iVal;
        long lVal;
        double fVal;
        
    } MYSTRUCT, *LPMYSTRUCT;

    MYSTRUCT wrStruct = { "ABCD", 0, 1, 2.0 };
    MYSTRUCT rdStruct = { "DCBA", 0, 2, 4.0 };

    DWORD dwError = 0;
    UINT uFormat = 0;

    //注册自定义格式
    if (dwError == 0) {
        uFormat = RegisterClipboardFormat( _T( "ManulaFormat" ) );
        if (uFormat == 0) {    //注册格式失败        
            dwError = GetLastError();
            TRACE( _T( "RegisterClipboardFormat Error (%ld)\n" ), dwError );
        }
    }

    //将数据写入剪切板  
    if (dwError == 0) {
        if (OpenClipboard( NULL )) {    //打开剪切板        
            if (EmptyClipboard()) {     //清空剪切板
                DWORD dwSize = sizeof( wrStruct );
                HGLOBAL hMem = GlobalAlloc( GMEM_MOVEABLE | GMEM_DDESHARE, dwSize ); //分配内存  
                if (hMem) { //分配内存有效                  
                    memcpy( GlobalLock( hMem ), &wrStruct, dwSize );    //拷贝数据  
                    GlobalUnlock( hMem );//解锁  

                    if (SetClipboardData( uFormat, hMem ) == NULL) {    //数据写入剪切板                    
                        dwError = GetLastError();
                        TRACE( _T( "SetClipboardData Error for write(%ld)\n" ), dwError );
                    }
                } else {    //分配内存出错
                    dwError = GetLastError();
                    TRACE( _T( "GlobalAlloc Error for write(%ld)\n" ), dwError );
                }
            } else { //清空剪切板失败  
                dwError = GetLastError();
                TRACE( _T( "EmptyClipboard Error for write(%ld)\n" ), dwError );
            }

            if (!CloseClipboard()) {    //关闭剪切板            
                dwError = GetLastError();
                TRACE( _T( "CloseClipboard Error for write(%ld)\n" ), dwError );
            }
        } else { //打开剪切板失败          
            dwError = GetLastError();
            TRACE( _T( "OpenClipboard Error for write(%ld)\n" ), dwError );
        }
    }

    //从剪切板读出数据  
    if (dwError == 0) {
        if (OpenClipboard( NULL )) {    //打开剪切板        
            if (IsClipboardFormatAvailable( uFormat )) {    //检查格式有效            
                HGLOBAL hMem = GetClipboardData( uFormat ); //读取数据句柄  
                if (hMem != NULL) {
                    MYSTRUCT* p = (MYSTRUCT*)GlobalLock( hMem );
                    cout << "\"" << p->charVal << ",\" " << p->iVal << ", " << p->lVal << ", " << p->fVal << endl;
                    GlobalUnlock( hMem );

                    memcpy( &rdStruct, GlobalLock( hMem ), sizeof( rdStruct ) );//读出数据  
                    GlobalUnlock( hMem );//解锁  
                } else {                //读取句柄失败                
                    dwError = GetLastError();
                    TRACE( _T( "GetClipboardData Error for read (%ld)\n" ), dwError );
                }
            } else {                    //格式无效            
                dwError = GetLastError();
                TRACE( _T( "IsClipboardFormatAvailable Error for read (%ld)\n" ), dwError );
            }

            if (!CloseClipboard()) {    //关闭剪切板            
                dwError = GetLastError();
                TRACE( _T( "CloseClipboard Error for read (%ld)\n" ), dwError );
            }
        } else {                        //打开剪切板失败        
            dwError = GetLastError();
            TRACE( _T( "OpenClipboard Error for read (%ld)\n" ), dwError );
        }
    }

    //比较数据  
    if (dwError == 0) {    
        if (memcmp( &wrStruct, &rdStruct, sizeof( wrStruct ) ) == 0) {  //相等        
        } else {
            TRACE( _T( "failed to compare data!\n" ) );
            ASSERT( FALSE );
        }
    }
}

void CopyImgFromOLEClpbrd()
{
    //初始化GDI+接口
    Gdiplus::GdiplusStartupInput gdpStartinput;
    GdiplusStartup( &gdpToken, &gdpStartinput, NULL );

    OleInitialize( NULL );

#ifdef SET_OLE_CLILPBRD
    // Create an OLE data source on the heap
    COleDataSource* pData = new COleDataSource;
    if (NULL == pData) {
        cout << "Memory is empty." << endl;
        OleUninitialize();
        return;
    }
    char szContent[] = "Current selection\r\n";
    // Get the currently selected data
    DWORD nSize = sizeof( szContent );
    HGLOBAL hGlob = GlobalAlloc( GMEM_MOVEABLE|GMEM_SHARE, nSize );
    strcpy_s( (char*)GlobalLock(hGlob), nSize, szContent );
    GlobalUnlock( hGlob );
    // For the appropriate data formats...
    pData->CacheGlobalData( CF_TEXT, hGlob );
     //The Clipboard now owns the allocated memory
     //and will delete this data object
     //when new data is put on the Clipboard
    pData->SetClipboard();
#endif

    COleDataObject pdo;
    pdo.AttachClipboard();
    UINT nImgType = 0;

    if (pdo.IsDataAvailable( CF_TEXT )) {
        HANDLE hData = pdo.GetGlobalData( CF_TEXT );
        if (hData != NULL) {
            char szText[50];
            LPCSTR pData = (LPCSTR)GlobalLock( hData );
            memcpy( szText, pData, 50 );
            cout << szText << endl;
            ::GlobalUnlock( hData );
            ::GlobalFree( hData );
        }
    } 

    CLSID csdType;
    if (-1 != GetEncoderClsid( L"image/jpeg", &csdType )) {
        WCHAR* wcName = NULL;
        if (GetName( wcName )) {
            if (pdo.IsDataAvailable( CF_METAFILEPICT )) {
                LPMETAFILEPICT  lpMFP = NULL;
                HANDLE hHandle = NULL;
                HGLOBAL hGlobal = (HGLOBAL)pdo.GetGlobalData( CF_METAFILEPICT );
                if (NULL == hGlobal) {
                    delete wcName;
                    wcName = NULL;
                    pdo.Release();                    
                    OleUninitialize();
                    GdiplusShutdown( gdpToken );
                }
                lpMFP = (LPMETAFILEPICT)GlobalLock( hGlobal );
                GlobalUnlock( hGlobal );

                const int nWidth = 1920;
                const int nHeight = 1200;
                Gdiplus::Bitmap CacheImage( nWidth, nHeight, 4, PixelFormat32bppARGB, 0 );
                Gdiplus::Graphics CacheGraphics( &CacheImage );
                SolidBrush brush( Color( 255, 255, 255, 255 ) );
                CacheGraphics.FillRectangle( &brush, 0, 0, nWidth, nHeight );
                Gdiplus::Metafile metafile( lpMFP->hMF, 0 );
                CacheGraphics.EnumerateMetafile( &metafile, PointF( 0, 0 ), MyProc, &metafile );
                HDC hDC = ::GetDC( NULL );
                ASSERT( NULL != hDC );
                if (NULL != hDC) {
                    Gdiplus::Graphics Graphic( hDC );
                    Graphic.DrawImage( &CacheImage, 0, 0 );
                    if (CacheImage.GetHeight() > 0 && CacheImage.GetWidth() > 0) {
                        Gdiplus::Status stRet = CacheImage.Save( wcName, &csdType, NULL );
                        VERIFY( Gdiplus::Ok == stRet );
                    }
                }
            } else if (pdo.IsDataAvailable( CF_BITMAP )) {

            }

            delete wcName;
            wcName = NULL;
        }
    } else {
        cout << "Get encoder class id failed." << endl;
    }

    pdo.Release();
    //GlobalFree( hGlob );
    OleUninitialize();
    GdiplusShutdown( gdpToken );
}

BOOL CALLBACK MyProc(   EmfPlusRecordType recordType, 
                        UINT flags, 
                        UINT dataSize, 
                        const BYTE* data, 
                        VOID *callbackData )
{
    Metafile *pMtf = (Metafile *)callbackData;
    pMtf->PlayRecord( recordType, flags, dataSize, data );
    return TRUE;
}

void DefineFormatClpbrd()
{
    DWORD dwError = 0;
    UINT uFormat = 0;
    DefFmtClpStruct test( 35 );
    //注册自定义格式
    if (dwError == 0) {
        uFormat = RegisterClipboardFormat( _T( "DefFmtClpStruct" ) );
        if (uFormat == 0) {    //注册格式失败        
            dwError = GetLastError();
            TRACE( _T( "RegisterClipboardFormat Error (%ld)\n" ), dwError );
        }
    }

    //将数据写入剪切板  
    if (dwError == 0) {
        if (OpenClipboard( NULL )) {    //打开剪切板        
            if (EmptyClipboard()) {     //清空剪切板
                DWORD dwSize = sizeof( test );
                HGLOBAL hMem = GlobalAlloc( GMEM_MOVEABLE | GMEM_DDESHARE, dwSize ); //分配内存  
                if (hMem) { //分配内存有效                  
                    memcpy( GlobalLock( hMem ), &test, dwSize );    //拷贝数据  
                    GlobalUnlock( hMem );//解锁  

                    if (SetClipboardData( uFormat, hMem ) == NULL) {    //数据写入剪切板                    
                        dwError = GetLastError();
                        TRACE( _T( "SetClipboardData Error for write(%ld)\n" ), dwError );
                    }
                } else {    //分配内存出错
                    dwError = GetLastError();
                    TRACE( _T( "GlobalAlloc Error for write(%ld)\n" ), dwError );
                }
            } else { //清空剪切板失败  
                dwError = GetLastError();
                TRACE( _T( "EmptyClipboard Error for write(%ld)\n" ), dwError );
            }

            if (!CloseClipboard()) {    //关闭剪切板            
                dwError = GetLastError();
                TRACE( _T( "CloseClipboard Error for write(%ld)\n" ), dwError );
            }
        } else { //打开剪切板失败          
            dwError = GetLastError();
            TRACE( _T( "OpenClipboard Error for write(%ld)\n" ), dwError );
        }
    }

    //从剪切板读出数据  
    if (dwError == 0) {
        if (OpenClipboard( NULL )) {    //打开剪切板        
            if (IsClipboardFormatAvailable( uFormat )) {    //检查格式有效            
                HGLOBAL hMem = GetClipboardData( uFormat ); //读取数据句柄  
                if (hMem != NULL) {
                    DefFmtClpStruct* p = (DefFmtClpStruct*)GlobalLock( hMem );
                    p->Print();
                    GlobalUnlock( hMem );

                    DefFmtClpStruct ct( (DefFmtClpStruct*) GlobalLock( hMem ) );           //读出数据  
                    GlobalUnlock( hMem );//解锁
                    ct.Print();
                } else {                //读取句柄失败                
                    dwError = GetLastError();
                    TRACE( _T( "GetClipboardData Error for read (%ld)\n" ), dwError );
                }
            } else {                    //格式无效            
                dwError = GetLastError();
                TRACE( _T( "IsClipboardFormatAvailable Error for read (%ld)\n" ), dwError );
            }

            if (!CloseClipboard()) {    //关闭剪切板            
                dwError = GetLastError();
                TRACE( _T( "CloseClipboard Error for read (%ld)\n" ), dwError );
            }
        } else {                        //打开剪切板失败        
            dwError = GetLastError();
            TRACE( _T( "OpenClipboard Error for read (%ld)\n" ), dwError );
        }
    }
}