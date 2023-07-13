#pragma once
#include "BtnST.h"

//按钮状态
#define BUT_STATUS_DOWN   1		// 箭头向下的标题框
#define BUT_STATUS_RIGHT  2		// 箭头向右的标题框

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
	int	    m_id;				//分配的资源号码
	CString m_szTitle;			//按钮文本
	CString m_szCmd;				//所要执行的命令或者函数
	CString m_szFile;			//图标名
	CWnd*   m_pParent;
	CRect   m_rc;

private:
	int      m_butStatus;		// 按钮状态
	COLORREF m_ForeColor;		// 文本颜色
	COLORREF m_BkColor;			// 背景色
	BOOL     b_InRect;			// 鼠标进入标志
	BOOL     b_Click;			// 是否单击过
	CRect    m_InvalRect;		// 有效区尺寸
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
	void SetForeColor(COLORREF color);		// 设置文本颜色
	void SetBkColor(COLORREF color);		// 设置背景颜色
	int  GetButStatus();					// 获取按钮状态
	void SetButStatus(int Status);			// 设置按钮状态
	BOOL ClickBut();						// 是否单击过按钮文字区
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


