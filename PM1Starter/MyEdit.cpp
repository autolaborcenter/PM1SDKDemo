// MyEdit.cpp : implementation file
//

#include "stdafx.h"
#include "MyEdit.h"


// CMyEdit

IMPLEMENT_DYNAMIC(CMyEdit, CEdit)

CMyEdit::CMyEdit()
{
	m_bTip = TRUE;
	m_hBkBrush = NULL;
	m_dMin = -100;
	m_dMax = 100;
	m_bInputLegal = true;
	m_bInputInRange = true;
}

CMyEdit::~CMyEdit()
{

}


BEGIN_MESSAGE_MAP(CMyEdit, CEdit)
	ON_WM_SETFOCUS()
	ON_WM_KILLFOCUS()
	ON_WM_CTLCOLOR_REFLECT()
	// 注意要反射后父窗口仍响应的话需要加_EX且响应函数返回FALSE
	ON_CONTROL_REFLECT_EX(EN_CHANGE, &CMyEdit::OnEnChange)
	ON_WM_DESTROY()
	ON_WM_CONTEXTMENU()
END_MESSAGE_MAP()



// CMyEdit message handlers

// 清理
void CMyEdit::OnDestroy()
{
	CEdit::OnDestroy();

	if (m_hBkBrush)
	{
		DeleteObject(m_hBkBrush);
	}
}

// 获得焦点
void CMyEdit::OnSetFocus(CWnd* pOldWnd)
{
	CEdit::OnSetFocus(pOldWnd);

	CString sText;
	GetWindowText(sText);
	if (sText == m_sTip)
	{
		SetWindowText(L"");
	}
}

// 失去焦点
void CMyEdit::OnKillFocus(CWnd* pNewWnd)
{
	CEdit::OnKillFocus(pNewWnd);

	CString sText;
	GetWindowText(sText);
	if (sText.IsEmpty())
	{
		SetWindowText(m_sTip);
	}
}

// 设置颜色(注意是ON_WM_CTLCOLOR_REFLECT而不是ON_WM_CTLCOLOR)
HBRUSH CMyEdit::CtlColor(CDC* pDC, UINT nCtlColor)
{
	// nCtlColor不一定为CTLCOLOR_EDIT, 也可能为CTLCOLOR_STATIC(编辑框只读时)
	if (m_bTip && CTLCOLOR_EDIT == nCtlColor)
	{
		pDC->SetTextColor(RGB(180, 180 ,180));
	}
	else
	{
		if (m_bInputLegal && m_bInputInRange)
		{
			pDC->SetTextColor(RGB(0, 0, 0));
		}
		else if (m_bInputLegal)
		{
			pDC->SetTextColor(RGB(255, 0, 255));
		}
		else
		{
			pDC->SetTextColor(RGB(255, 0, 0));
		}
	}
	pDC->SetBkMode(TRANSPARENT);	// 文本背景透明
	return m_hBkBrush;
}

// 文本改变(ON_CONTROL_REFLECT_EX)
BOOL CMyEdit::OnEnChange()
{
	// 当前输入
	CString sText;
	GetWindowText(sText);
	// 提示信息
	if (m_sTip == sText)
	{
		m_bTip = TRUE;
		m_bInputOk = false;
	}
	else
	{
		m_bTip = FALSE;
		sText = sText.Trim();
		// 合法数字(且+-在最前)
		m_bInputLegal =				// 只包含字符0123456789+-.
			sText == sText.SpanIncluding(L"0123456789.+-") 
			&& sText.Replace(L".", L".") <= 1	// 最多一个.
			&& (sText.Replace(L"+", L"+") +	
				sText.Replace(L"-", L"-")) <= 1	// 最多一个+或-
			&& sText.Find(L"+") <= 0			// +-在最前
			&& sText.Find(L"-") <= 0;
		// 数值范围
		if (m_bInputLegal)
		{
			double val = _ttof(sText);
			m_bInputInRange = val >= m_dMin && val <= m_dMax;
		}
		m_bInputOk = m_bInputLegal && m_bInputInRange;
	}
	Invalidate(TRUE);
	return FALSE;	// 返回FALSE父窗口仍响应, TRUE父窗口不响应(注意_EX)
}

// 设置提示文本
void CMyEdit::SetTipText(CString sText)
{
	// 设置文本
	m_sTip = sText;
	SetWindowText(m_sTip);
}

//屏蔽右键菜单
void CMyEdit::OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/)
{
	// 无任何操作, 屏蔽右键菜单
}

// 初始
void CMyEdit::PreSubclassWindow()
{
	// 初始化
	m_bTip = TRUE;
	m_hBkBrush = CreateSolidBrush(RGB(254, 254, 254));

	CEdit::PreSubclassWindow();
}

// 设置输入范围
void CMyEdit::SetRange(double min, double max)
{
	m_dMin = min;
	m_dMax = max;
}
