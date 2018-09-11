#pragma once

class CDlgComboxExTest : public CDialogEx
{
public:
	CDlgComboxExTest(CWnd* pParent = NULL);
    virtual ~CDlgComboxExTest();

	enum { IDD = IDD_ComboxExTestApp_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);
    virtual BOOL OnInitDialog();
    afx_msg void OnSysCommand( UINT nID, LPARAM lParam );
    afx_msg void OnPaint();
    afx_msg HCURSOR OnQueryDragIcon();
    DECLARE_MESSAGE_MAP()

protected:
	HICON               m_hIcon;	

private:
    CComboBoxEx         m_cmbBmp;
    CImageList          m_imgList;
    HBITMAP             m_hBitMap;
public:
    afx_msg void OnCbnSelchangeComboBmp();
    afx_msg void OnCbnKillfocusComboBmp();
};