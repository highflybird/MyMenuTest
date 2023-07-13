#pragma once
#include "BtnST.h"

//��ť״̬
#define BUT_STATUS_DOWN   1		// ��ͷ���µı����
#define BUT_STATUS_RIGHT  2		// ��ͷ���ҵı����


// CMyButton

class CMyButton : public CButton
{
	DECLARE_DYNAMIC(CMyButton)
public:
	bool    m_isTop;
	int	    m_id;				//�������Դ����
	CString m_title;			//��ť�ı�
	CString m_cmd;				//��Ҫִ�е�������ߺ���
	CString m_bmp;				//ͼ����
	CWnd*   m_pParent;
	CRect   m_rc;

private:
	int      m_butStatus;		// ��ť״̬
	COLORREF m_ForeColor;		// �ı���ɫ
	COLORREF m_BkColor;			// ����ɫ
	BOOL     b_InRect;			// �������־
	BOOL     b_Click;			// �Ƿ񵥻���
	CRect    m_InvalRect;		// ��Ч���ߴ�
	CFont	 m_Fonts;


public:
	CMyButton();
    CMyButton(int res,const CString & szTitle,const CString & szCmd,const CString & szBmp,CWnd* pWnd,CRect rect);
	virtual ~CMyButton();

	// Attributes
public:
	void SetForeColor(COLORREF color);		// �����ı���ɫ
	void SetBkColor(COLORREF color);		// ���ñ�����ɫ
	int  GetButStatus();					// ��ȡ��ť״̬
	void SetButStatus(int Status);			// ���ð�ť״̬
	BOOL ClickBut();						// �Ƿ񵥻�����ť������
	BOOL ToCreate();

public:
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnBnClicked();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnKillFocus(CWnd* pNewWnd);

protected:
	virtual void PreSubclassWindow();
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	DECLARE_MESSAGE_MAP()
};


