#include "stdafx.h"
#include "ComboxExTestApp.h"
#include "DlgComboxExTest.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);

protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()

CDlgComboxExTest::CDlgComboxExTest(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgComboxExTest::IDD, pParent)
    , m_hBitMap(NULL)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

CDlgComboxExTest::~CDlgComboxExTest()
{
    if ( NULL != m_hBitMap ) {
        DeleteObject( m_hBitMap );
    }
}

void CDlgComboxExTest::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
    DDX_Control( pDX, IDC_COMBO_BMP, m_cmbBmp );
}

BEGIN_MESSAGE_MAP(CDlgComboxExTest, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
    ON_CBN_SELCHANGE( IDC_COMBO_BMP, &CDlgComboxExTest::OnCbnSelchangeComboBmp )
    ON_CBN_KILLFOCUS( IDC_COMBO_BMP, &CDlgComboxExTest::OnCbnKillfocusComboBmp )
END_MESSAGE_MAP()

BOOL CDlgComboxExTest::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

    //m_list.Create( IDB_BITMAP_TEST, 16, 16, 5, RGB( 255, 255, 255 ) );
    m_imgList.Create( 16, 16, ILC_COLOR32 | ILC_MASK, 1, 0 );
    //m_list.Add( AfxGetApp()->LoadIcon( IDI_ICON1 ) );
    //m_list.Add( AfxGetApp()->LoadIcon( IDI_ICON2 ) );
    //m_list.Add( AfxGetApp()->LoadIcon( IDI_ICON3 ) );
    //m_list.Add( AfxGetApp()->LoadIcon( IDI_ICON4 ) );
    //m_list.Add( AfxGetApp()->LoadIcon( IDI_ICON5 ) );
    CBitmap bitMap;
    m_hBitMap = ( HBITMAP )LoadImage( NULL, _T( "test.bmp" ), IMAGE_BITMAP, 0, 0,
        LR_CREATEDIBSECTION | LR_LOADFROMFILE | LR_DEFAULTSIZE );
    if ( NULL != m_hBitMap ) {
        bitMap.Detach();
        VERIFY( bitMap.Attach( m_hBitMap ) );
    }
    //bitMap.LoadBitmap( IDB_BITMAP_TEST );
    m_imgList.Add( &bitMap, RGB( 255, 255, 255 ) );
    m_cmbBmp.SetImageList( &m_imgList );

    const int nId = 5;
    auto lambda = [&]( const CString& strVal, int nVal ) {
        COMBOBOXEXITEM insItem;
        insItem.mask = CBEIF_IMAGE | CBEIF_INDENT|  CBEIF_OVERLAY |
            CBEIF_SELECTEDIMAGE | CBEIF_TEXT;
        TRACE( _T( "%d\n" ), nVal );
        insItem.iItem = nVal;
        insItem.iImage = nVal;// nId - 1 - nVal;
        insItem.iSelectedImage = nVal;// nId - 1 - nVal;
        insItem.iOverlay = 0;
        insItem.iIndent = ( 0 & 0x03);
        CString strTmp;
        strTmp.Format( _T( "%s %d" ), strVal, nVal + 1 ); 
        insItem.pszText = strTmp.LockBuffer();
        strTmp.UnlockBuffer();
        int nIdex = m_cmbBmp.InsertItem( &insItem );
        VERIFY( CB_ERR != nIdex );
    };

    for ( int i = 0; i < nId; ++i )
        lambda( _T( "ͼ��" ), i );

    m_cmbBmp.SetCurSel( 0 );

    int nItemCnt = m_cmbBmp.GetCount();
    VERIFY( nItemCnt == nId );

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CDlgComboxExTest::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CDlgComboxExTest::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CDlgComboxExTest::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CDlgComboxExTest::OnCbnSelchangeComboBmp()
{
    int nCurSel = m_cmbBmp.GetCurSel();
}

void CDlgComboxExTest::OnCbnKillfocusComboBmp()
{
    // TODO:  �ڴ���ӿؼ�֪ͨ����������
}
