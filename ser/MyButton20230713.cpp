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
    m_title = szTitle;
	m_bmp = szBmp;
	m_cmd = szCmd;
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

void CMyButton::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	// TODO:  添加您的代码以绘制指定项
	CDC *pDC = CDC::FromHandle( lpDrawItemStruct->hDC );
	CRect m_butRect = lpDrawItemStruct->rcItem;		// 获取按钮尺寸
	GetWindowText( m_title );						// 获取按钮文本?
	CPoint m_Center = m_butRect.CenterPoint();		// 求按钮中心点
	int nSavedDC = pDC->SaveDC();
	VERIFY( pDC );
	//acutPrintf(_T("\n样式是:%ld,btn样式是%ld"), GetStyle(),GetButtonStyle() );
	m_BkColor = m_pParent->GetDC()->GetBkColor();   //和背景色一致
	//pDC->FillSolidRect( &m_butRect, m_BkColor);	// 画按钮背景

	CPen bkPen;
	bkPen.CreatePen( PS_SOLID, 2, RGB(227, 233, 241) );
	bkPen.CreatePen( PS_SOLID, 2, m_BkColor );
	pDC->SelectObject( &bkPen );
	pDC->MoveTo( m_butRect.left, m_butRect.top + 2 );
	pDC->LineTo( m_butRect.right, m_butRect.top + 2 );
	pDC->MoveTo( m_butRect.left, m_butRect.bottom - 2 );
	pDC->LineTo( m_butRect.right, m_butRect.bottom - 2 );

	CBitmap m_Bmp;    // 画按钮左边的位图
	CPoint pt;
	pt.x = m_butRect.left;
	pt.y = m_butRect.top + (m_butRect.Height() - 16) / 2;
	CDC dcMem;
	dcMem.CreateCompatibleDC( pDC );
	dcMem.SelectObject(&m_Bmp[0]);
	pDC->BitBlt( pt.x, pt.y, 16, 16, &dcMem, 0, 0, SRCAND );
	dcMem.DeleteDC();

	POINT  m_pt[3];		         // 箭头坐标
	pt.x = m_butRect.left + 8;   // 画箭头
	pt.y = m_butRect.top + m_butRect.Height() /2 ;
	switch( m_butStatus )
	{
	case BUT_STATUS_DOWN:  // 箭头向下的标题框
		m_pt[0].x = pt.x - 5;
		m_pt[0].y = pt.y - 3;
		m_pt[1].x = pt.x + 6;
		m_pt[1].y = pt.y - 3;
		m_pt[2].x = pt.x;
		m_pt[2].y = pt.y + 3;
		break;
	case BUT_STATUS_RIGHT:  // 箭头向右的标题框
		m_pt[0].x = pt.x;
		m_pt[0].y = pt.y - 6;
		m_pt[1].x = pt.x + 6;
		m_pt[1].y = pt.y;
		m_pt[2].x = pt.x;
		m_pt[2].y = pt.y + 6;
		break;
	}
	CBrush Brush;
	Brush.CreateSolidBrush( m_ForeColor );
	pDC->SelectObject( &Brush );
	CRgn rgn;
	rgn.CreatePolygonRgn( m_pt, 3, ALTERNATE );
	pDC->PaintRgn( &rgn );

	CSize Extent = pDC->GetTextExtent( m_title ); // 画按钮文字
	pDC->SetTextColor( m_ForeColor );
	pDC->SetBkMode( TRANSPARENT );
	CRect m_textRect;  // 文本位置
	m_textRect.SetRect( m_butRect.left + 20, m_Center.y - Extent.cy / 2, m_butRect.left + 20 + Extent.cx, m_Center.y + Extent.cy / 2 );
	pDC->TextOut( m_textRect.left, m_textRect.top, m_title );
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


	CImage image; //创建图片类  
	image.Load(_T("d:\\temp\\main.ico")); //根据图片路径加载图片  

	CRect rc;//定义矩形类  
	int cx = image.GetWidth();//获取图片宽度  
	int cy = image.GetHeight();//获取图片高度 

	GetClientRect(&rc);//获得pictrue控件所在的矩形区域  
	
	int h = rc.Height();
	rc.right = h+10;
	rc.left = 10;
	rc.top = 2;
	rc.bottom = h-2;
 
	if (image.GetBPP() == 32) //确认该图像包含Alpha通道
	{
		int i;
		int j;
		for (i = 0; i < image.GetWidth(); i++)
		{
			for (j = 0; j < image.GetHeight(); j++)
			{
				byte *pByte = (byte *)image.GetPixelAddress(i, j);
				pByte[0] = 255-pByte[0] * pByte[3] / 255;
				pByte[1] = 255-pByte[1] * pByte[3] / 255;
				pByte[2] = 255-pByte[2] * pByte[3] / 255;

				pDC->SetPixel(i,j,RGB(pByte[0],pByte[1],pByte[2]));
			}
		}
	}
	//image.Draw(m_pDC->m_hDC, rc); //将图片画到Picture控件表示的矩形区域  
	

	pDC->RestoreDC( nSavedDC );
}
#else 
void CMyButton::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	CCButton::DrawItem(lpDrawItemStruct);
	m_pDC =  CDC::FromHandle( lpDrawItemStruct->hDC );
	m_pDC->SetBkMode( TRANSPARENT );
	if (m_isTop)
	{
		if (m_isExpand)
		{
			m_pDC->TextOut(10,0,_T("-"));
		}
		else
		{
			m_pDC->TextOut(10,0,_T("+"));	
		}
	}
	ReleaseDC(m_pDC);//释放picture控件的DC  

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
	// TODO: 在此添加控件通知处理程序代码
	acutPrintf(_T("%s的命令是%s.\n"),m_title ,m_cmd);
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
	INT extStyle = WS_EX_DLGMODALFRAME|WS_EX_TOPMOST|WS_EX_LEFTSCROLLBAR|WS_EX_NOPARENTNOTIFY; 
	BOOL isOk = CreateEx(0,_T("Button"),m_title,0x5001400B,m_rc,m_pParent,m_id);  //0x5001400B  ,0x5001010B,

	//m_pParent->GetFont();
	m_Fonts.CreatePointFont(100,_T("微软雅黑"),NULL);
	//SetFont(m_pParent->GetFont(),true);
	SetFont(&m_Fonts,true);
	//SetFont(acedGetAcadFrame()->GetFont(),true);

	//CBitmap bmp;
    GetClientRect(m_rc);
    int H = m_rc.Height();
 //   Bitmap bm(m_bmp);
	//int newSize = rc.Height();

	//// 获取这个图片的宽和高
	//float width = bm.getWidth();
	//float height = bm.getHeight();
	//// 创建操作图片用的matrix对象
	//Matrix matrix = new Matrix();
	//// 计算宽高缩放率
	//float scaleWidth = ((float) newSize) / width;
	//float scaleHeight = ((float) newSize) / height;
	//// 缩放图片动作
	//matrix.postScale(scaleWidth, scaleHeight);
	//Bitmap bitmap = Bitmap.createBitmap(bgimage, 0, 0, (int) width,
	//	(int) height, matrix, true);
	//return bitmap;

 
    SubclassDlgItem(m_id, this);
  
	//CPaintDC dc(this);
	//m_pDC = GetDC();

	CImage img;
	HBITMAP hBitmap = NULL;
	HBITMAP hOldBitmap = NULL;
	HRESULT res = img.Load(m_bmp);
	if (res == S_OK)
	{
		hBitmap = img.Detach();
		//hOldBitmap = (HBITMAP)SelectObject(m_CDC,hBitmap);
		//m_bitmap.Attach(hBitmap);
	}
	else
	{
		acutPrintf(_T("\n无法装入指定图标文件!"));
		return isOk; 
	}
   
	Bitmap bm(m_bmp);
	bm.GetWidth();
	bm.GetHeight();
    Matrix mat = new Matrix;
	mat.Scale(x,y);
	Bitmap new = Bitmap.Clone()


	
	//BITMAP bm;
	//m_bitmap.GetBitmap(&bm);
   //
	//HDC  hMemDC = CreateCompatibleDC(m_CDC);
	//::StretchBlt(hMemDC,0, 0,H,H , m_CDC , 0, 0, bm.bmWidth, bm.bmHeight, SRCCOPY);
    //hBitmap = (HBITMAP)SelectObject(hMemDC ,hBitmap);
 
	//SetBitmap(hBmp);
	//SetBitmaps(HBITMAP hBitmapIn, COLORREF crTransColorIn, HBITMAP hBitmapOut, COLORREF crTransColorOut)
	SetBitmaps(hBitmap,RGB(0, 0, 0),NULL,RGB(255,255,255));

   
	return isOk;
}
