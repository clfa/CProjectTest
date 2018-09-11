// IWebBrw2Dlg.h : header file
//

#if !defined(AFX_IWEBBRW2DLG_H__1040B984_FA07_4EB9_AFE3_5C2FA251068C__INCLUDED_)
#define AFX_IWEBBRW2DLG_H__1040B984_FA07_4EB9_AFE3_5C2FA251068C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CIWebBrw2Dlg dialog

class CIWebBrw2Dlg : public CDialog
{
// Construction
public:
	CIWebBrw2Dlg(CWnd* pParent = NULL);	// standard constructor
	virtual ~CIWebBrw2Dlg();
private:
	void GetDocInfo( BSTR bsJSFuc );
    void OpenUrl();
    void LocalDisplayUrl();
    void RefreshUrl();
    void GetChartImage( BSTR bsJSFunNm );
	void DrawUI();
// Dialog Data
	//{{AFX_DATA(CIWebBrw2Dlg)
	enum { IDD = IDD_IWEBBRW2_DIALOG };
	CStatic	m_staticHead;
	CButton	m_btnOK;
	CEdit	m_editUrlAddr;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CIWebBrw2Dlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON					m_hIcon;
    CString					m_strUrl;
    CWnd					m_hContainer;
    CComPtr<IWebBrowser2>	m_pIWebBrwII;
    RECT					m_rcContainer;
	HRESULT					m_hrInit;
	// Generated message map functions
	//{{AFX_MSG(CIWebBrw2Dlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	virtual void OnOK();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_IWEBBRW2DLG_H__1040B984_FA07_4EB9_AFE3_5C2FA251068C__INCLUDED_)
