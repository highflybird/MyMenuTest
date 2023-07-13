#pragma once
#include "BtnST.h"

//��ť״̬
#define BUT_STATUS_DOWN   1		// ��ͷ���µı����
#define BUT_STATUS_RIGHT  2		// ��ͷ���ҵı����

#define IS_CBUTTON 0
#if IS_CBUTTON
typedef CBitmapButton  CCButton; 
#else
typedef CButtonST  CCButton; 
#endif

// CMyButton

class CMyButton : public CCButton
{
	DECLARE_DYNAMIC(CMyButton)
public:
	bool    m_isTop;
	bool    m_isExpand;
	int	    m_id;				//�������Դ����
	CString m_szTitle;			//��ť�ı�
	CString m_szCmd;				//��Ҫִ�е�������ߺ���
	CString m_szFile;			//ͼ����
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
    CDC*     m_pDC;
	HDC      m_CDC;
	CBitmap  m_bitmap;
	CBitmap* m_pOldbmp;

public:
	CMyButton();
    CMyButton(int res,const CString & szTitle,const CString & szCmd,const CString & szBmp,CWnd* pWnd,CRect rect,bool isTop= false);
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


