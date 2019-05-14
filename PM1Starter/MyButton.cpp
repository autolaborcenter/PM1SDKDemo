// MyButton.cpp : implementation file
// By: yanglx2022		2015-5-12
// ʹ�÷���
// 1 �����ļ�MyButton.h��MyButton.cpp������Ŀ¼�����ͷ�ļ�#include "MyButton.h"
// 2 ���һ����ť�ؼ�(����IDΪIDC_MYBUTTON)��Owner Draw����һ����ΪTrue(Ĭ��ΪFalse)
// 3 ���崰����һ��CMyButton���͵ĳ�Ա����(��m_mbtnTest)
// 4 ���ڳ�ʼ��ʱ����m_mbtnTest.SubclassDlgItem(IDC_MYBUTTON, this);
//    ������DoDataExchange���������DDX_Control(pDX, IDC_MYBUTTON, m_mbtnTest)���
// 5 ���ڳ�ʼ��ʱ����m_mbtnTest.LoadBitmaps()����λͼ��Դ

#include "stdafx.h"
#include "MyButton.h"

// CMyButton

IMPLEMENT_DYNAMIC(CMyButton, CButton)

CMyButton::CMyButton()
{
}

CMyButton::~CMyButton()
{
}


BEGIN_MESSAGE_MAP(CMyButton, CButton)
	ON_WM_MOUSEHOVER()
	ON_WM_MOUSELEAVE()
	ON_WM_MOUSEMOVE()
	ON_WM_DESTROY()
END_MESSAGE_MAP()



// CMyButton message handlers

// ����
void CMyButton::OnDestroy()
{
	CButton::OnDestroy();

	DeleteObject(m_hBitmapNorm);
	DeleteObject(m_hBitmapOver);
	DeleteObject(m_hBitmapDown);
	DeleteObject(m_hBitmapDisable);
}

// ������
void CMyButton::OnMouseHover(UINT nFlags, CPoint point)
{
	m_bEnter = TRUE;
	Invalidate();
	CButton::OnMouseHover(nFlags, point);
	
}

// ����Ƴ�
void CMyButton::OnMouseLeave()
{
	m_bEnter = FALSE;
	Invalidate();
	CButton::OnMouseLeave();
}

void CMyButton::OnMouseMove(UINT nFlags, CPoint point)
{
	if (!m_bEnter)	// ���ոս���
    {
		TRACKMOUSEEVENT tme;
		tme.cbSize = sizeof(TRACKMOUSEEVENT);
		tme.hwndTrack = this->GetSafeHwnd();
		tme.dwFlags = TME_LEAVE|TME_HOVER;
		tme.dwHoverTime = 1;
		m_bEnter = _TrackMouseEvent(&tme);
    }

      CWnd::OnMouseMove(nFlags,   point);  
}

// ����DrawItem
void CMyButton::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	HDC hDC = lpDrawItemStruct->hDC;
	if (m_bTabFocus)	// Tab����
	{
		DrawBitmap(hDC, m_hBitmapDisable);
	}
	else if (lpDrawItemStruct->itemState & ODS_DISABLED)	// ��ֹ״̬
	{
		DrawBitmap(hDC, m_hBitmapDisable);
	}
	else if (lpDrawItemStruct->itemState & ODS_SELECTED)	// ����״̬
	{
		DrawBitmap(hDC, m_hBitmapDown);
	}
	else
	{
		if (m_bSelected)	// ѡ��
		{
			DrawBitmap(hDC, m_hBitmapDown);
		}
		else if (m_bEnter)	// ���λ�ڰ�ť��
		{
			DrawBitmap(hDC, m_hBitmapOver);

		}
		else	// ��겻�ڰ�ť��
		{
			DrawBitmap(hDC, m_hBitmapNorm);

		}
	}
}

// ��ʾλͼ
void CMyButton::DrawBitmap(HDC hDC, HBITMAP hBitmap)
{
	HDC hdcMem = CreateCompatibleDC(hDC);
	SelectObject(hdcMem, hBitmap);
	BitBlt(hDC, 0, 0, m_nWidth, m_nHeight, hdcMem, 0, 0, SRCCOPY);
	DeleteDC(hdcMem);
}

// ����Bitmap
// ��������Դ���������ޣ�����ʱ������0����
// nNormBmpID - [IN]��ť����״̬��λͼ��ԴID
// nOverBmpID - [IN][Default 0]����ƶ�����ť��ʱ��λͼ��ԴID
// nDownBmpID - [IN][Default 0]��ť����״̬��λͼ��ԴID
// m_nDisableBmpID - [IN][Default 0]��ť����״̬��λͼ��ԴID
void CMyButton::LoadBitmaps(int nNormBmpID, int nOverBmpID, int nDownBmpID, int m_nDisableBmpID)
{
	// ��ʼ��
	m_bEnter = FALSE;
	m_bTabFocus = FALSE;
	// ����Norm
	m_hBitmapNorm = (HBITMAP)LoadImage(AfxGetInstanceHandle(), MAKEINTRESOURCE(nNormBmpID), IMAGE_BITMAP, 0, 0, 0);
	// Over����Norm����
	if (nOverBmpID == 0)
	{
		m_hBitmapOver = m_hBitmapNorm;
	}
	else
	{
		m_hBitmapOver = (HBITMAP)LoadImage(AfxGetInstanceHandle(), MAKEINTRESOURCE(nOverBmpID), IMAGE_BITMAP, 0, 0, 0);
	}
	// Down��Over��ʱ��Over����, ������Norm����
	if (nDownBmpID == 0)
	{
		if (nOverBmpID != 0)
		{
			m_hBitmapDown = m_hBitmapOver;
		}
		else
		{
			m_hBitmapDown = m_hBitmapNorm;
		}
	}
	else
	{
		m_hBitmapDown = (HBITMAP)LoadImage(AfxGetInstanceHandle(), MAKEINTRESOURCE(nDownBmpID), IMAGE_BITMAP, 0, 0, 0);
	}
	// Disable����Norm����
	if (m_nDisableBmpID == 0)
	{
		m_hBitmapDisable = m_hBitmapNorm;
	}
	else
	{
		m_hBitmapDisable =  (HBITMAP)LoadImage(AfxGetInstanceHandle(), MAKEINTRESOURCE(m_nDisableBmpID), IMAGE_BITMAP, 0, 0, 0);
	}
	// ��ȡλͼ�ߴ�
	BITMAP bmp;
	GetObject(m_hBitmapNorm, sizeof(BITMAP), (LPBYTE)&bmp);
	m_nWidth = bmp.bmWidth;
	m_nHeight = bmp.bmHeight;
	// �޸İ�ť�ߴ���λͼ��ͬ
	RECT rect, rect1;
	GetWindowRect(&rect);
	GetParent()->GetWindowRect(&rect1);
	MoveWindow(rect.left-rect1.left, rect.top-rect1.top, m_nWidth, m_nHeight);
}

void CMyButton::Move(int x, int y)
{
	CRect rect;
	GetWindowRect(rect);
	MoveWindow(x, y, rect.Width(), rect.Height());
}

void CMyButton::SetTabFocus()
{
	m_bTabFocus = TRUE;
}

void CMyButton::KillTabFocus()
{
	m_bTabFocus = FALSE;
}

void CMyButton::Select(bool selected)
{
	m_bSelected = selected;
	this->Invalidate(FALSE);
}
