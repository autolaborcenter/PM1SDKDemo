#pragma once


// CMyButton

class CMyButton : public CButton
{
	DECLARE_DYNAMIC(CMyButton)

public:
	CMyButton();
	virtual ~CMyButton();

protected:
	DECLARE_MESSAGE_MAP()

	afx_msg void OnMouseHover(UINT nFlags, CPoint point);
	afx_msg void OnMouseLeave();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	void DrawBitmap(HDC hDC, HBITMAP hBitmap);
	afx_msg void OnDestroy();

	BOOL m_bEnter;	// 鼠标进入标记
	HBITMAP m_hBitmapNorm;
	HBITMAP m_hBitmapOver;
	HBITMAP m_hBitmapDown;
	HBITMAP m_hBitmapDisable;
	int m_nWidth;
	int m_nHeight;
	BOOL m_bTabFocus;
	bool m_bSelected = false;

public:
	void LoadBitmaps(int nNormBmpID, int nOverBmpID = 0, int nDownBmpID = 0, int nDisableBmpID = 0);
	void Move(int x, int y);
	void SetTabFocus();
	void KillTabFocus();
	void Select(bool selected);
};


