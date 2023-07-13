// MyButton.cpp : 实现文件
//

#include "stdafx.h"
#include "MyButton.h"
#include <strsafe.h>
#include "resource.h"
 
// CMyButton

IMPLEMENT_DYNAMIC(CMyButton, CCButton)

CMyButton::CMyButton():m_pDC(NULL),m_pOldbmp(NULL)
{
	m_ForeColor = RGB(0, 0, 0);         // 文字颜色
	m_BkColor = RGB(240, 240, 240);     // 背景色
	m_butStatus = BUT_STATUS_DOWN;      // 按钮状态
	b_InRect = false;                   // 鼠标进入标志
	b_Click = false;                    // 单击标志
    m_isExpand = false;
	m_isTop = false;
	
}

CMyButton::~CMyButton()
{
}

                   
CMyButton::CMyButton(int res,const CString & szTitle,const CString & szCmd,const CString & szBmp,CWnd* pWnd,CRect rect,bool isTop):m_pDC(NULL),m_isTop(isTop),m_pOldbmp(NULL)
{
	m_ForeColor = RGB(0, 0, 0);         // 文字颜色
	m_BkColor = RGB(177, 195, 217);     // 背景色
	m_butStatus = BUT_STATUS_DOWN;      // 按钮状态
	b_InRect = false;                   // 鼠标进入标志
	b_Click = false;                    // 单击标志

	m_id = res;
    m_szTitle = szTitle;
	m_szFile = szBmp;
	m_szCmd = szCmd;
	m_pParent = pWnd;
	m_rc = rect;
	m_isExpand = false;
}


BEGIN_MESSAGE_MAP(CMyButton, CCButton)
	ON_WM_MOUSEMOVE()
	ON_CONTROL_REFLECT(BN_CLICKED, &CMyButton::OnBnClicked)
	ON_WM_LBUTTONDOWN()
	ON_WM_KILLFOCUS()
END_MESSAGE_MAP()



// CMyButton 消息处理程序

#if IS_CBUTTON

void DrawImages3(HDC &hdc,CString fileName,int h)  
{  
	RECT rect = {0,0,h,h};  
	FillRect(hdc, &rect, (HBRUSH)::GetStockObject(NULL_BRUSH));  //GRAY_BRUSH

	Bitmap bmp1(fileName);  
	//CGdiplusConvertHelper::DumpBitmap(bmp1);  

	HBITMAP hBitmap= NULL;  
	Color clr(0,0,0,0);   
	if (Gdiplus::Ok == bmp1.GetHBITMAP(clr, &hBitmap))//转换完成的是预乘Alpha的图像  
	{  
		HDC hMemDC = ::CreateCompatibleDC(hdc);  
		HBITMAP hOld = (HBITMAP)SelectObject(hMemDC, hBitmap);  
		BLENDFUNCTION blendfunc = {AC_SRC_OVER, 0, 255, AC_SRC_ALPHA};  
		AlphaBlend(hdc,   
			0, 0, bmp1.GetWidth(), bmp1.GetHeight(),  
			hMemDC,  
			0, 0, bmp1.GetWidth(), bmp1.GetHeight(),  
			blendfunc);  
        //StretchBlt(hdc,0,0,h,h,hMemDC,0,0,bmp1.GetWidth(),bmp1.GetHeight(),SRCCOPY);
		SelectObject(hMemDC, hOld);  
		DeleteDC(hMemDC);  
		DeleteObject(hBitmap);  
	}  
}  

void CMyButton::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	// TODO:  添加您的代码以绘制指定项
	CDC *pDC = CDC::FromHandle( lpDrawItemStruct->hDC );
	CRect m_butRect = lpDrawItemStruct->rcItem;		// 获取按钮尺寸
	GetWindowText( m_szTitle );						// 获取按钮文本?
	CPoint m_Center = m_butRect.CenterPoint();		// 求按钮中心点
	int nSavedDC = pDC->SaveDC();
	VERIFY( pDC );
	//acutPrintf(_T("\n样式是:%ld,btn样式是%ld"), GetStyle(),GetButtonStyle() );
	m_BkColor = m_pParent->GetDC()->GetBkColor();   //和背景色一致
	//pDC->FillSolidRect( &m_butRect, m_BkColor);	// 画按钮背景


	CSize Extent = pDC->GetTextExtent( m_szTitle ); // 画按钮文字
	pDC->SetTextColor( m_ForeColor );
	//pDC->SetBkMode( TRANSPARENT );
	CRect m_textRect;  // 文本位置
	m_textRect.SetRect( m_butRect.left + 20, m_Center.y - Extent.cy / 2, m_butRect.left + 20 + Extent.cx, m_Center.y + Extent.cy / 2 );
	pDC->TextOut( m_textRect.left+20, m_textRect.top, m_szTitle );
	m_InvalRect.SetRect( m_textRect.left - 17, m_textRect.top, m_textRect.right, m_textRect.bottom + 2 ); // 有效区：包括箭头和文字


	if ( b_InRect )  // 鼠标进入时
	{
		CPen bkPen;
		bkPen.CreatePen( PS_SOLID, 2, RGB(0, 0, 0) );   // 在文字下加划线
		pDC->SelectObject( &bkPen );
		//pDC->MoveTo( m_textRect.left, m_textRect.bottom + 1 );
		//pDC->LineTo( m_textRect.right, m_textRect.bottom + 1 );
		pDC->Draw3dRect(m_butRect, ::GetSysColor(COLOR_BTNSHADOW), ::GetSysColor(COLOR_BTNHILIGHT));
	}
	else
	{
		;
		//pDC->Draw3dRect(m_butRect, RGB(0,0,255), RGB(255,255,0));
	}


	//CImage image; //创建图片类  
	//image.Load(m_szFile); //根据图片路径加载图片  
	 
 
	CRect rc;//定义矩形类  
	///int cx = image.GetWidth();//获取图片宽度  
	//int cy = image.GetHeight();//获取图片高度 

	GetClientRect(&rc);//获得pictrue控件所在的矩形区域  
	
	int h = rc.Height();
	rc.right = h+10;
	rc.left = 10;
	rc.top = 2;
	rc.bottom = h-2;

	HDC hDc = pDC->GetSafeHdc();
	DrawImages3(hDc,m_szFile,h);
 
 
	//if (image.GetBPP() == 32) //确认该图像包含Alpha通道
	//{
	//	int i;
	//	int j;
	//	for (i = 0; i < image.GetWidth(); i++)
	//	{
	//		for (j = 0; j < image.GetHeight(); j++)
	//		{
	//			byte *pByte = (byte *)image.GetPixelAddress(i, j);
	//			pByte[0] = 255-pByte[0] * pByte[3] / 255;
	//			pByte[1] = 255-pByte[1] * pByte[3] / 255;
	//			pByte[2] = 255-pByte[2] * pByte[3] / 255;

	//			pDC->SetPixel(i,j,RGB(pByte[0],pByte[1],pByte[2]));
	//		}
	//	}
	//}
	//else 
	//{
		//StretchBlt(pDC->GetSafeHdc(),0,0,h,h,image.GetDC(),0,0,image.GetWidth(),image.GetHeight(),SRCAND);
	//}
	//image.Draw(m_pDC->m_hDC, rc); //将图片画到Picture控件表示的矩形区域  
	

	pDC->RestoreDC( nSavedDC );
}
#else 
void CMyButton::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	CCButton::DrawItem(lpDrawItemStruct);
}
#endif



void CMyButton::PreSubclassWindow()
{
	// TODO: 在此添加专用代码和/或调用基类

	CCButton::PreSubclassWindow();
	ModifyStyle(0, BS_OWNERDRAW);
}

void CMyButton::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if( !b_InRect )
	{
		if ( m_InvalRect.PtInRect(point) ) // 鼠标进入有效区
		{
			b_InRect = true;
			CBrush Brush;
			Brush.CreateSolidBrush( m_BkColor );
			SetCapture();
			InvalidateRect( &m_InvalRect, false ); // 重绘按钮
		}
	}
	if( b_InRect )
	{
		if ( !m_InvalRect.PtInRect(point) )	 // 鼠标离开有效区
		{
			b_InRect = false;
			ReleaseCapture();
			InvalidateRect( &m_InvalRect, false ); // 重绘按钮
		}
	}
	CCButton::OnMouseMove(nFlags, point);
}

void CMyButton::OnBnClicked()
{
	SetFocus();
	// TODO: 在此添加控件通知处理程序代码
	acutPrintf(_T("%s的命令是%s.\n"),m_szTitle ,m_szCmd);
	if (m_isTop)
	{
		m_isExpand = !m_isExpand;
	}
	//::SetWindowLong(GetSafeHwnd(),GWL_EXSTYLE,4);
}

void CMyButton::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if ( b_InRect ) // 只有在有效区单击才响应
	{
		m_butStatus = BUT_STATUS_DOWN;
		InvalidateRect( &m_InvalRect, false ); // 重绘按钮
		b_Click = true;	 // 单击标志
		//acutPrintf(_T("\n这是OnLButtonDown发出的命令%s"),m_cmd);
	}
	CCButton::OnLButtonDown(nFlags, point);
}

void CMyButton::OnKillFocus(CWnd* pNewWnd)
{
	CCButton::OnKillFocus(pNewWnd);

	// TODO: 在此处添加消息处理程序代码
	if (b_InRect == FALSE) return;

	if (b_InRect == TRUE)
	{
		b_InRect = FALSE;
		Invalidate();
	}
}


// 设置文本颜色
void CMyButton::SetForeColor(COLORREF color)
{
	m_ForeColor = color;
}

// 设置背景颜色
void CMyButton::SetBkColor(COLORREF color)
{
	m_BkColor = color;
}

// 获取按钮状态
int CMyButton::GetButStatus()
{
	return m_butStatus;
}

// 设置按钮状态
void CMyButton::SetButStatus(int Status)
{
	m_butStatus = Status;
	InvalidateRect( &m_InvalRect, false );
}

// 是否单击按钮有效区
BOOL CMyButton::ClickBut()
{
	if( b_Click )
	{
		b_Click = false;
		return true;
	}
	return false;
}


BOOL CMyButton::ToCreate()
{
	// TODO: 在此添加专用代码和/或调用基类
	int btnStyle = WS_CHILD|WS_VISIBLE|WS_TABSTOP ;
	int extStyle = WS_EX_DLGMODALFRAME|WS_EX_TOPMOST|WS_EX_LEFTSCROLLBAR|WS_EX_NOPARENTNOTIFY; 
	BOOL isOk = CreateEx(0,_T("Button"),m_szTitle,0x5001400B,m_rc,m_pParent,m_id);  //0x5001400B  ,0x5001010B,

	m_Fonts.CreatePointFont(100,_T("微软雅黑"),NULL);
	SetFont(&m_Fonts,true);
	//SetFont(m_pParent->GetFont(),true);
	//SetFont(acedGetAcadFrame()->GetFont(),true);
	//SubclassDlgItem(m_id, this);

    GetClientRect(m_rc);
    int H = m_rc.Height();
	CImage img;
	HBITMAP hBitmap = NULL;
	HRESULT res = img.Load(m_szFile);
	if (res == S_OK)
	{
		hBitmap = img.Detach();
	}
	else
	{
		acutPrintf(_T("\n无法装入指定图标文件!"));
		return FALSE; 
	}

#if IS_CBUTTON
    
#else
	SetBitmaps(hBitmap,RGB(0, 0, 0),NULL,RGB(255,255,255));
	SetAlign(CButtonST::ST_ALIGN_HORIZ);
	//SizeToContent();
#endif

	return isOk;
}
