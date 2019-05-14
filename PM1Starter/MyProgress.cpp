// MyProgress.cpp: 实现文件
//

#include "stdafx.h"
#include "PM1Starter.h"
#include "MyProgress.h"


// CMyProgress

IMPLEMENT_DYNAMIC(CMyProgress, CProgressCtrl)

CMyProgress::CMyProgress()
{
	m_brFore.CreateSolidBrush(RGB(6, 176, 37));
	m_brBack.CreateSolidBrush(RGB(230, 230, 230));
}

CMyProgress::~CMyProgress()
{
	m_brFore.DeleteObject();
	m_brBack.DeleteObject();
}

BEGIN_MESSAGE_MAP(CMyProgress, CProgressCtrl)
	ON_WM_PAINT()
END_MESSAGE_MAP()


void CMyProgress::OnPaint()
{
	CPaintDC dc(this);
	CRect rect;
	GetClientRect(rect);
	// 双缓冲
	CDC memDC;
	CBitmap memBmp;
	memDC.CreateCompatibleDC(NULL);
	memBmp.CreateCompatibleBitmap(&dc, rect.Width(), rect.Height());
	memDC.SelectObject(&memBmp);
	// 绘制背景色
	memDC.FillRect(rect, &m_brBack);
	// 绘制前景色
	int lower, upper;
	GetRange(lower, upper);
	CRect foreRect(rect.left, rect.top, rect.Width() * GetPos() / (upper - lower), rect.bottom);
	memDC.FillRect(foreRect, &m_brFore);
	// 拷贝内存图到屏幕显示
	dc.BitBlt(rect.left, rect.top, rect.Width(), rect.Height(), &memDC, 0, 0, SRCCOPY);
	// 清理
	memBmp.DeleteObject();
	memDC.DeleteDC();
}
