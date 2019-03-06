#pragma once


// CMyEdit

class CMyEdit : public CEdit
{
	DECLARE_DYNAMIC(CMyEdit)

public:
	CMyEdit();
	virtual ~CMyEdit();

protected:
	DECLARE_MESSAGE_MAP()
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);
	afx_msg BOOL OnEnChange();
	CString m_sTip;
	BOOL m_bTip;
	HBRUSH m_hBkBrush;
	double m_dMax;
	double m_dMin;

private:
	bool m_bInputLegal;
	bool m_bInputInRange;

public:
	int m_bInputOk = false;

public:
	void SetTipText(CString sText);
	afx_msg void OnDestroy();
	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);
	void SetRange(double min, double max);

protected:
	virtual void PreSubclassWindow();
};


