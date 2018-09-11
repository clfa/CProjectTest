// IWebBrw2Dlg.cpp : implementation file
//

#include "stdafx.h"
#include "IWebBrw2.h"
#include "IWebBrw2Dlg.h"
//#include "base64_code.hpp"
#include "b64.h"
#include "cxinc/ximage.h"

#pragma comment( lib, "cximagecrtd.lib" )

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CIWebBrw2Dlg dialog

CIWebBrw2Dlg::CIWebBrw2Dlg(CWnd* pParent /*=NULL*/)
	: CDialog(CIWebBrw2Dlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CIWebBrw2Dlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	AfxEnableControlContainer();
	m_hrInit = OleInitialize( NULL );
}

CIWebBrw2Dlg::~CIWebBrw2Dlg()
{
	if( SUCCEEDED( m_hrInit ) )	 {
		OleUninitialize();
	}
}

void CIWebBrw2Dlg::OpenUrl()
{
	HRESULT hr = S_OK;
    TCHAR tcInUrl[MAX_PATH+1];
    m_editUrlAddr.GetWindowText( tcInUrl, MAX_PATH );
    m_strUrl.Format( "%s", tcInUrl );

    IWebBrowser2* pBrowser2 = NULL;     
	CoCreateInstance( CLSID_InternetExplorer, NULL, CLSCTX_LOCAL_SERVER, IID_IWebBrowser2, (void**)&pBrowser2 );
	if( NULL != pBrowser2 )   {
		VARIANT vEmpty;
		VariantInit( &vEmpty );
		
		BSTR bstrURL = SysAllocString( m_strUrl.AllocSysString() );
		TRACE( "initial url: %s\n", bstrURL );
		hr = pBrowser2->Navigate(bstrURL, &vEmpty, &vEmpty, &vEmpty, &vEmpty);
		if( SUCCEEDED( hr ) ) {
			pBrowser2->put_Visible(VARIANT_TRUE);
		}
		else {
			pBrowser2->Quit();
		}
		
		SysFreeString(bstrURL);
		pBrowser2->Release();
	}
}

void CIWebBrw2Dlg::LocalDisplayUrl()
{
    LPUNKNOWN lpUnk = m_hContainer.GetControlUnknown();
    HRESULT hrRet = lpUnk->QueryInterface( IID_IWebBrowser2, (void**)&m_pIWebBrwII );
    if( SUCCEEDED( hrRet ) ) {
        TCHAR tcInUrl[MAX_PATH+1];
        m_editUrlAddr.GetWindowText( tcInUrl, MAX_PATH );
        m_strUrl.Format( "%s", tcInUrl );
        CComVariant ccvUrl( m_strUrl );
        VARIANT vEmpty;
        VariantInit(&vEmpty);
        m_pIWebBrwII->Navigate2( &ccvUrl, &vEmpty, &vEmpty, &vEmpty, &vEmpty );
    }
}

void CIWebBrw2Dlg::RefreshUrl()
{
    if( m_pIWebBrwII ) {
        TCHAR tcInUrl[MAX_PATH+1];
        m_editUrlAddr.GetWindowText( tcInUrl, MAX_PATH );
        m_strUrl.Format( "%s", tcInUrl );
        CComVariant ccvUrl( m_strUrl );
        VARIANT vEmpty;
        VariantInit(&vEmpty);
        m_pIWebBrwII->Navigate2( &ccvUrl, &vEmpty, &vEmpty, &vEmpty, &vEmpty );
    }
}

void CIWebBrw2Dlg::GetChartImage( BSTR bsJSFunNm )
{
    if( m_pIWebBrwII ) {
        HRESULT hr = S_OK;

        CComQIPtr<IDispatch> pDocID;
        hr = m_pIWebBrwII->get_Document(&pDocID); 
        if( FAILED( hr ) ) {
            AtlTrace( L"Get IDispatch failed.\n" );
            return;
        }

        CComQIPtr<IHTMLDocument2> pDocHtml;
        hr = pDocID->QueryInterface(IID_IHTMLDocument2, (void**)&pDocHtml);
        if( FAILED( hr ) ) {
            AtlTrace( L"Get IHTMLDocument2 failed.\n" );
            return;
        }

        BSTR bsTit = NULL;
        hr = pDocHtml->get_title( &bsTit );
        if( FAILED( hr ) ) {
            AtlTrace( L"Get document title failed.\n" );
            return;
        }
        AtlTrace( L"HTML title : %s \n", bsTit );
        ::SysFreeString( bsTit );

        CComQIPtr<IDispatch> pJS;
        hr = pDocHtml->get_Script( (IDispatch**)&pJS );
        if( FAILED( hr ) ) {
            AtlTrace( L"Get javascript failed.\n" );
            return;
        }

        AtlTrace( L"bsJSFunNm: %s\n", bsJSFunNm );
        DISPID dspID;
        hr = pJS->GetIDsOfNames( IID_NULL, &bsJSFunNm, 1, LOCALE_SYSTEM_DEFAULT, &dspID);
        if( FAILED(hr) ) {
            AtlTrace( L"Get idsofname failed.\n" );
            return;
        }

        DISPPARAMS dspParms = { NULL, NULL, 0, 0 };
        EXCEPINFO ecpInfo;
        memset( &ecpInfo, 0, sizeof( ecpInfo ) );
        CComVariant VariRet;
        UINT uiArgErr = ( UINT )-1;
        hr = pJS->Invoke( dspID, IID_NULL, 0, DISPATCH_METHOD, &dspParms, &VariRet, &ecpInfo, &uiArgErr );
        if( FAILED( hr ) ) {
            TRACE( "Invoke javascript failed.\n" );
            return;
        }
        if( VariRet.vt == VT_BSTR ) {
            bstr_t bstRet( VariRet.bstrVal );
            std::string strRet( bstRet );
			size_t sztCoPos = strRet.find_first_of( ',' );
			strRet = strRet.substr( sztCoPos+1 );
			ZBase64 zb;
			int iOutBytes = 0;
			std::string strRetDec = zb.Decode( strRet.c_str(), strRet.size(), iOutBytes );
			int iLen = strRetDec.size();
			BYTE* byBuf = new BYTE[iLen];
			for( int i=0; i<iLen; ++i ) byBuf[i] = strRetDec[i];
			CxImage img( byBuf, iLen, CXIMAGE_FORMAT_PNG );
			img.Save( "gci.png", CXIMAGE_FORMAT_PNG );
			delete [] byBuf;
        }
    }
}

void CIWebBrw2Dlg::DrawUI()
{
	int nLeft = 10;
	int nTop  = 10;
	int nWidth, nHeight;
	
	CRect rcParent;
	GetClientRect( &rcParent );
	
	CRect rcAddrHead;
	m_staticHead.GetWindowRect( &rcAddrHead );
	ScreenToClient( &rcAddrHead );
	nWidth				= rcAddrHead.Width();
	nHeight				= rcAddrHead.Height();
	rcAddrHead.left		= nLeft;
	rcAddrHead.top		= nTop;
	rcAddrHead.right	= rcAddrHead.left + nWidth;
	rcAddrHead.bottom	= rcAddrHead.top + nHeight;
	m_staticHead.MoveWindow( rcAddrHead );
	
	CRect rcAddress;
	m_editUrlAddr.GetWindowRect( &rcAddress );
	ScreenToClient( &rcAddress );
	nHeight				= rcAddress.Height();
	rcAddress.left		= rcAddrHead.right + 5;
	rcAddress.top		= nTop;
	rcAddress.right		= rcAddress.left + rcParent.Width() / 2;
	rcAddress.bottom	= rcAddress.top + nHeight;
	m_editUrlAddr.MoveWindow( rcAddress );
	
	CRect rcOK;
	m_btnOK.GetWindowRect( &rcOK );
	ScreenToClient( &rcOK );
	nWidth				= rcOK.Width();
	nHeight				= rcOK.Height();
	rcOK.right			= rcParent.right - 10;
	rcOK.left			= rcParent.right - nWidth;
	rcOK.top			= rcAddress.top;
	rcOK.bottom			= rcOK.top + nHeight;
	m_btnOK.MoveWindow( rcOK );

	if( NULL != m_hContainer.GetSafeHwnd() ) {
		m_rcContainer.left		= nLeft;
		m_rcContainer.top		= rcAddress.bottom + 10;
		m_rcContainer.right		= m_rcContainer.left + rcParent.Width() - 20;
		m_rcContainer.bottom	= m_rcContainer.top + rcParent.Height() - 20;
		m_hContainer.MoveWindow( &m_rcContainer );
	}
#ifdef _DEBUG
// 	CString strValue;
// 	strValue.Format( 
// 		"父窗口：%d %d %d %d\n地址头：%d %d %d %d\n地址输入栏：%d %d %d %d\n执行按钮：%d %d %d %d\n容器窗口：%d %d %d %d", 
// 		rcParent.left,		rcParent.top,		rcParent.right,		rcParent.bottom,
// 		rcAddrHead.left,	rcAddrHead.top,		rcAddrHead.right,	rcAddrHead.bottom,
// 		rcAddress.left,		rcAddress.top,		rcAddress.right,	rcAddress.bottom,
// 		rcOK.left,			rcOK.top,			rcOK.right,			rcOK.bottom,
// 		m_rcContainer.left,	m_rcContainer.top,	m_rcContainer.right,m_rcContainer.bottom );
// 	AfxMessageBox( strValue );
#endif
}

void CIWebBrw2Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CIWebBrw2Dlg)
	DDX_Control(pDX, IDC_STATIC_ADDRESS, m_staticHead);
	DDX_Control(pDX, IDOK, m_btnOK);
	DDX_Control(pDX, IDC_EDIT_ADDRESS, m_editUrlAddr);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CIWebBrw2Dlg, CDialog)
	//{{AFX_MSG_MAP(CIWebBrw2Dlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CIWebBrw2Dlg message handlers

BOOL CIWebBrw2Dlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	ShowWindow( SW_SHOWMAXIMIZED );

	m_strUrl.Format("%s", "http://144.20.80.12:8081/HappyServer/common/echarts/designer/Index.html");
	//m_strUrl.Format("%s", "");
    m_editUrlAddr.SetWindowText( m_strUrl.GetBuffer(0) );
    m_strUrl.ReleaseBuffer();

	DrawUI();

	if( NULL == m_hContainer.GetSafeHwnd() ) {
		GetClientRect( &m_rcContainer );
		m_rcContainer.left		+= 10;
		m_rcContainer.top		+= 43;
		m_rcContainer.right		-= 10;
		m_rcContainer.bottom	-= 20;

		m_hContainer.CreateControl( 
			CLSID_WebBrowser, TEXT(""), 
			WS_VISIBLE | WS_CHILD, 
			m_rcContainer, 
			this, 
			AFX_IDW_PANE_FIRST );
		m_hContainer.SetParent( this );
		LocalDisplayUrl();
	}

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CIWebBrw2Dlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CIWebBrw2Dlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CIWebBrw2Dlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CIWebBrw2Dlg::OnOK() 
{
	//m_urlInput.SetWindowText( "http://144.20.80.12:8081/HappyServer/common/echarts/designer/Index.html" );
	RefreshUrl();
	//GetDocInfo( ::SysAllocString( L"" ) );
	//RefreshUrl();
	//BSTR bstNm = ::SysAllocString( L"getChartImage" );
	//GetChartImage( bstNm );
	//::SysFreeString( bstNm );
	//CDialog::OnOK();
}

void CIWebBrw2Dlg::GetDocInfo(BSTR bsJSFuc)
{
    HRESULT hr;
    CComQIPtr<IDispatch> pDocID;
	hr = m_pIWebBrwII->get_Document(&pDocID); 
    if( SUCCEEDED( hr ) ) {
        CComQIPtr<IHTMLDocument2> pDocHtml;
        hr = pDocID->QueryInterface(IID_IHTMLDocument2, (void**)&pDocHtml);
        if( SUCCEEDED( hr ) ) {
            BSTR bsTit;
            pDocHtml->get_title( &bsTit );
            TRACE( "title : %s \n", bsTit );
            ::SysFreeString( bsTit );
        }
    }
}

void CIWebBrw2Dlg::OnSize(UINT nType, int cx, int cy) 
{
	if( m_pIWebBrwII ) {
		DrawUI();
	}
	CDialog::OnSize(nType, cx, cy);
}