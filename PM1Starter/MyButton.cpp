// MyButton.cpp : implementation file
// By: yanglx2022		2015-5-12
// 使用方法
// 1 拷贝文件MyButton.h与MyButton.cpp到工程目录，添加头文件#include "MyButton.h"
// 2 添加一个按钮控件(假设ID为IDC_MYBUTTON)，Owner Draw属性一定改为True(默认为False)
// 3 定义窗口类一个CMyButton类型的成员变量(如m_mbtnTest)
// 4 窗口初始化时调用m_mbtnTest.SubclassDlgItem(IDC_MYBUTTON, this);
//    或者在DoDataExchange函数中添加DDX_Control(pDX, IDC_MYBUTTON, m_mbtnTest)语句
// 5 窗口初始化时调用m_mbtnTest.LoadBitmaps()传入位图资源

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

// 清理
void CMyButton::OnDestroy()
{
	CButton::OnDestroy();

	DeleteObject(m_hBitmapNorm);
	DeleteObject(m_hBitmapOver);
	DeleteObject(m_hBitmapDown);
	DeleteObject(m_hBitmapDisable);
}

// 鼠标进入
void CMyButton::OnMouseHover(UINT nFlags, CPoint point)
{
	m_bEnter = TRUE;
	Invalidate();
	CButton::OnMouseHover(nFlags, point);
	
}

// 鼠标移出
void CMyButton::OnMouseLeave()
{
	m_bEnter = FALSE;
	Invalidate();
	CButton::OnMouseLeave();
}

void CMyButton::OnMouseMove(UINT nFlags, CPoint point)
{
	if (!m_bEnter)	// 鼠标刚刚进入
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

// 重载DrawItem
void CMyButton::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	HDC hDC = lpDrawItemStruct->hDC;
	if (m_bTabFocus)	// Tab焦点
	{
		DrawBitmap(hDC, m_hBitmapDisable);
	}
	else if (lpDrawItemStruct->itemState & ODS_DISABLED)	// 禁止状态
	{
		DrawBitmap(hDC, m_hBitmapDisable);
	}
	else if (lpDrawItemStruct->itemState & ODS_SELECTED)	// 按下状态
	{
		DrawBitmap(hDC, m_hBitmapDown);
	}
	else
	{
		if (m_bSelected)	// 选中
		{
			DrawBitmap(hDC, m_hBitmapDown);
		}
		else if (m_bEnter)	// 鼠标位于按钮上
		{
			DrawBitmap(hDC, m_hBitmapOver);

		}
		else	// 鼠标不在按钮上
		{
			DrawBitmap(hDC, m_hBitmapNorm);

		}
	}
}

// 显示位图
void CMyButton::DrawBitmap(HDC hDC, HBITMAP hBitmap)
{
	HDC hdcMem = CreateCompatibleDC(hDC);
	SelectObject(hdcMem, hBitmap);
	BitBlt(hDC, 0, 0, m_nWidth, m_nHeight, hdcMem, 0, 0, SRCCOPY);
	DeleteDC(hdcMem);
}

// 加载Bitmap
// 后三个资源可任意有无，不设时参数置0即可
// nNormBmpID - [IN]按钮正常状态的位图资源ID
// nOverBmpID - [IN][Default 0]鼠标移动到按钮上时的位图资源ID
// nDownBmpID - [IN][Default 0]按钮按下状态的位图资源ID
// m_nDisableBmpID - [IN][Default 0]按钮禁用状态的位图资源ID
void CMyButton::LoadBitmaps(int nNormBmpID, int nOverBmpID, int nDownBmpID, int m_nDisableBmpID)
{
	// 初始化
	m_bEnter = FALSE;
	m_bTabFocus = FALSE;
	// 加载Norm
	m_hBitmapNorm = (HBITMAP)LoadImage(AfxGetInstanceHandle(), MAKEINTRESOURCE(nNormBmpID), IMAGE_BITMAP, 0, 0, 0);
	// Over无用Norm代替
	if (nOverBmpID == 0)
	{
		m_hBitmapOver = m_hBitmapNorm;
	}
	else
	{
		m_hBitmapOver = (HBITMAP)LoadImage(AfxGetInstanceHandle(), MAKEINTRESOURCE(nOverBmpID), IMAGE_BITMAP, 0, 0, 0);
	}
	// Down无Over有时用Over代替, 否则用Norm代替
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
	// Disable无用Norm代替
	if (m_nDisableBmpID == 0)
	{
		m_hBitmapDisable = m_hBitmapNorm;
	}
	else
	{
		m_hBitmapDisable =  (HBITMAP)LoadImage(AfxGetInstanceHandle(), MAKEINTRESOURCE(m_nDisableBmpID), IMAGE_BITMAP, 0, 0, 0);
	}
	// 获取位图尺寸
	BITMAP bmp;
	GetObject(m_hBitmapNorm, sizeof(BITMAP), (LPBYTE)&bmp);
	m_nWidth = bmp.bmWidth;
	m_nHeight = bmp.bmHeight;
	// 修改按钮尺寸与位图相同
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
