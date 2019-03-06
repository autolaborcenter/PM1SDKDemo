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
	// ע��Ҫ����󸸴�������Ӧ�Ļ���Ҫ��_EX����Ӧ��������FALSE
	ON_CONTROL_REFLECT_EX(EN_CHANGE, &CMyEdit::OnEnChange)
	ON_WM_DESTROY()
	ON_WM_CONTEXTMENU()
END_MESSAGE_MAP()



// CMyEdit message handlers

// ����
void CMyEdit::OnDestroy()
{
	CEdit::OnDestroy();

	if (m_hBkBrush)
	{
		DeleteObject(m_hBkBrush);
	}
}

// ��ý���
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

// ʧȥ����
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

// ������ɫ(ע����ON_WM_CTLCOLOR_REFLECT������ON_WM_CTLCOLOR)
HBRUSH CMyEdit::CtlColor(CDC* pDC, UINT nCtlColor)
{
	// nCtlColor��һ��ΪCTLCOLOR_EDIT, Ҳ����ΪCTLCOLOR_STATIC(�༭��ֻ��ʱ)
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
	pDC->SetBkMode(TRANSPARENT);	// �ı�����͸��
	return m_hBkBrush;
}

// �ı��ı�(ON_CONTROL_REFLECT_EX)
BOOL CMyEdit::OnEnChange()
{
	// ��ǰ����
	CString sText;
	GetWindowText(sText);
	// ��ʾ��Ϣ
	if (m_sTip == sText)
	{
		m_bTip = TRUE;
		m_bInputOk = false;
	}
	else
	{
		m_bTip = FALSE;
		sText = sText.Trim();
		// �Ϸ�����(��+-����ǰ)
		m_bInputLegal =				// ֻ�����ַ�0123456789+-.
			sText == sText.SpanIncluding(L"0123456789.+-") 
			&& sText.Replace(L".", L".") <= 1	// ���һ��.
			&& (sText.Replace(L"+", L"+") +	
				sText.Replace(L"-", L"-")) <= 1	// ���һ��+��-
			&& sText.Find(L"+") <= 0			// +-����ǰ
			&& sText.Find(L"-") <= 0;
		// ��ֵ��Χ
		if (m_bInputLegal)
		{
			double val = _ttof(sText);
			m_bInputInRange = val >= m_dMin && val <= m_dMax;
		}
		m_bInputOk = m_bInputLegal && m_bInputInRange;
	}
	Invalidate(TRUE);
	return FALSE;	// ����FALSE����������Ӧ, TRUE�����ڲ���Ӧ(ע��_EX)
}

// ������ʾ�ı�
void CMyEdit::SetTipText(CString sText)
{
	// �����ı�
	m_sTip = sText;
	SetWindowText(m_sTip);
}

//�����Ҽ��˵�
void CMyEdit::OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/)
{
	// ���κβ���, �����Ҽ��˵�
}

// ��ʼ
void CMyEdit::PreSubclassWindow()
{
	// ��ʼ��
	m_bTip = TRUE;
	m_hBkBrush = CreateSolidBrush(RGB(254, 254, 254));

	CEdit::PreSubclassWindow();
}

// �������뷶Χ
void CMyEdit::SetRange(double min, double max)
{
	m_dMin = min;
	m_dMax = max;
}
