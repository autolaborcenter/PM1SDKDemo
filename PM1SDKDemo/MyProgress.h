#pragma once


// CMyProgress

class CMyProgress : public CProgressCtrl
{
	DECLARE_DYNAMIC(CMyProgress)

public:
	CMyProgress();
	virtual ~CMyProgress();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();

private:
	CBrush m_brFore;	// ǰ��ɫ
	CBrush m_brBack;	// ����ɫ
};


