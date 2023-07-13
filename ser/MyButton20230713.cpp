// MyButton.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MyButton.h"
#include <strsafe.h>
#include "resource.h"
 
// CMyButton

IMPLEMENT_DYNAMIC(CMyButton, CCButton)

CMyButton::CMyButton():m_pDC(NULL),m_pOldbmp(NULL)
{
	m_ForeColor = RGB(0, 0, 0);         // ������ɫ
	m_BkColor = RGB(240, 240, 240);     // ����ɫ
	m_butStatus = BUT_STATUS_DOWN;      // ��ť״̬
	b_InRect = false;                   // �������־
	b_Click = false;                    // ������־
    m_isExpand = false;
	m_isTop = false;
	
}

CMyButton::~CMyButton()
{
}

                   
CMyButton::CMyButton(int res,const CString & szTitle,const CString & szCmd,const CString & szBmp,CWnd* pWnd,CRect rect,bool isTop):m_pDC(NULL),m_isTop(isTop),m_pOldbmp(NULL)
{
	m_ForeColor = RGB(0, 0, 0);         // ������ɫ
	m_BkColor = RGB(177, 195, 217);     // ����ɫ
	m_butStatus = BUT_STATUS_DOWN;      // ��ť״̬
	b_InRect = false;                   // �������־
	b_Click = false;                    // ������־

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



// CMyButton ��Ϣ�������

#if IS_CBUTTON

void CMyButton::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	// TODO:  ������Ĵ����Ի���ָ����
	CDC *pDC = CDC::FromHandle( lpDrawItemStruct->hDC );
	CRect m_butRect = lpDrawItemStruct->rcItem;		// ��ȡ��ť�ߴ�
	GetWindowText( m_title );						// ��ȡ��ť�ı�?
	CPoint m_Center = m_butRect.CenterPoint();		// ��ť���ĵ�
	int nSavedDC = pDC->SaveDC();
	VERIFY( pDC );
	//acutPrintf(_T("\n��ʽ��:%ld,btn��ʽ��%ld"), GetStyle(),GetButtonStyle() );
	m_BkColor = m_pParent->GetDC()->GetBkColor();   //�ͱ���ɫһ��
	//pDC->FillSolidRect( &m_butRect, m_BkColor);	// ����ť����

	CPen bkPen;
	bkPen.CreatePen( PS_SOLID, 2, RGB(227, 233, 241) );
	bkPen.CreatePen( PS_SOLID, 2, m_BkColor );
	pDC->SelectObject( &bkPen );
	pDC->MoveTo( m_butRect.left, m_butRect.top + 2 );
	pDC->LineTo( m_butRect.right, m_butRect.top + 2 );
	pDC->MoveTo( m_butRect.left, m_butRect.bottom - 2 );
	pDC->LineTo( m_butRect.right, m_butRect.bottom - 2 );

	CBitmap m_Bmp;    // ����ť��ߵ�λͼ
	CPoint pt;
	pt.x = m_butRect.left;
	pt.y = m_butRect.top + (m_butRect.Height() - 16) / 2;
	CDC dcMem;
	dcMem.CreateCompatibleDC( pDC );
	dcMem.SelectObject(&m_Bmp[0]);
	pDC->BitBlt( pt.x, pt.y, 16, 16, &dcMem, 0, 0, SRCAND );
	dcMem.DeleteDC();

	POINT  m_pt[3];		         // ��ͷ����
	pt.x = m_butRect.left + 8;   // ����ͷ
	pt.y = m_butRect.top + m_butRect.Height() /2 ;
	switch( m_butStatus )
	{
	case BUT_STATUS_DOWN:  // ��ͷ���µı����
		m_pt[0].x = pt.x - 5;
		m_pt[0].y = pt.y - 3;
		m_pt[1].x = pt.x + 6;
		m_pt[1].y = pt.y - 3;
		m_pt[2].x = pt.x;
		m_pt[2].y = pt.y + 3;
		break;
	case BUT_STATUS_RIGHT:  // ��ͷ���ҵı����
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

	CSize Extent = pDC->GetTextExtent( m_title ); // ����ť����
	pDC->SetTextColor( m_ForeColor );
	pDC->SetBkMode( TRANSPARENT );
	CRect m_textRect;  // �ı�λ��
	m_textRect.SetRect( m_butRect.left + 20, m_Center.y - Extent.cy / 2, m_butRect.left + 20 + Extent.cx, m_Center.y + Extent.cy / 2 );
	pDC->TextOut( m_textRect.left, m_textRect.top, m_title );
	m_InvalRect.SetRect( m_textRect.left - 17, m_textRect.top, m_textRect.right, m_textRect.bottom + 2 ); // ��Ч����������ͷ������


	if ( b_InRect )  // ������ʱ
	{
		CPen bkPen;
		bkPen.CreatePen( PS_SOLID, 2, RGB(0, 0, 0) );   // �������¼ӻ���
		pDC->SelectObject( &bkPen );
		//pDC->MoveTo( m_textRect.left, m_textRect.bottom + 1 );
		//pDC->LineTo( m_textRect.right, m_textRect.bottom + 1 );
		pDC->Draw3dRect(m_butRect, ::GetSysColor(COLOR_BTNSHADOW), ::GetSysColor(COLOR_BTNHILIGHT));
	}


	CImage image; //����ͼƬ��  
	image.Load(_T("d:\\temp\\main.ico")); //����ͼƬ·������ͼƬ  

	CRect rc;//���������  
	int cx = image.GetWidth();//��ȡͼƬ���  
	int cy = image.GetHeight();//��ȡͼƬ�߶� 

	GetClientRect(&rc);//���pictrue�ؼ����ڵľ�������  
	
	int h = rc.Height();
	rc.right = h+10;
	rc.left = 10;
	rc.top = 2;
	rc.bottom = h-2;
 
	if (image.GetBPP() == 32) //ȷ�ϸ�ͼ�����Alphaͨ��
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
	//image.Draw(m_pDC->m_hDC, rc); //��ͼƬ����Picture�ؼ���ʾ�ľ�������  
	

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
	ReleaseDC(m_pDC);//�ͷ�picture�ؼ���DC  

}
#endif



void CMyButton::PreSubclassWindow()
{
	// TODO: �ڴ����ר�ô����/����û���

	CCButton::PreSubclassWindow();
	ModifyStyle(0, BS_OWNERDRAW);
}

void CMyButton::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if( !b_InRect )
	{
		if ( m_InvalRect.PtInRect(point) ) // ��������Ч��
		{
			b_InRect = true;
			CBrush Brush;
			Brush.CreateSolidBrush( m_BkColor );
			SetCapture();
			InvalidateRect( &m_InvalRect, false ); // �ػ水ť
		}
	}
	if( b_InRect )
	{
		if ( !m_InvalRect.PtInRect(point) )	 // ����뿪��Ч��
		{
			b_InRect = false;
			ReleaseCapture();
			InvalidateRect( &m_InvalRect, false ); // �ػ水ť
		}
	}
	CCButton::OnMouseMove(nFlags, point);
}

void CMyButton::OnBnClicked()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	acutPrintf(_T("%s��������%s.\n"),m_title ,m_cmd);
	if (m_isTop)
	{
		m_isExpand = !m_isExpand;
	}
	//::SetWindowLong(GetSafeHwnd(),GWL_EXSTYLE,4);
}

void CMyButton::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if ( b_InRect ) // ֻ������Ч����������Ӧ
	{
		m_butStatus = BUT_STATUS_DOWN;
		InvalidateRect( &m_InvalRect, false ); // �ػ水ť
		b_Click = true;	 // ������־
		//acutPrintf(_T("\n����OnLButtonDown����������%s"),m_cmd);
	}
	CCButton::OnLButtonDown(nFlags, point);
}

void CMyButton::OnKillFocus(CWnd* pNewWnd)
{
	CCButton::OnKillFocus(pNewWnd);

	// TODO: �ڴ˴������Ϣ����������
	if (b_InRect == FALSE) return;

	if (b_InRect == TRUE)
	{
		b_InRect = FALSE;
		Invalidate();
	}
}


// �����ı���ɫ
void CMyButton::SetForeColor(COLORREF color)
{
	m_ForeColor = color;
}

// ���ñ�����ɫ
void CMyButton::SetBkColor(COLORREF color)
{
	m_BkColor = color;
}

// ��ȡ��ť״̬
int CMyButton::GetButStatus()
{
	return m_butStatus;
}

// ���ð�ť״̬
void CMyButton::SetButStatus(int Status)
{
	m_butStatus = Status;
	InvalidateRect( &m_InvalRect, false );
}

// �Ƿ񵥻���ť��Ч��
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
 
	// TODO: �ڴ����ר�ô����/����û���
	int btnStyle = WS_CHILD|WS_VISIBLE|WS_TABSTOP ;
	INT extStyle = WS_EX_DLGMODALFRAME|WS_EX_TOPMOST|WS_EX_LEFTSCROLLBAR|WS_EX_NOPARENTNOTIFY; 
	BOOL isOk = CreateEx(0,_T("Button"),m_title,0x5001400B,m_rc,m_pParent,m_id);  //0x5001400B  ,0x5001010B,

	//m_pParent->GetFont();
	m_Fonts.CreatePointFont(100,_T("΢���ź�"),NULL);
	//SetFont(m_pParent->GetFont(),true);
	SetFont(&m_Fonts,true);
	//SetFont(acedGetAcadFrame()->GetFont(),true);

	//CBitmap bmp;
    GetClientRect(m_rc);
    int H = m_rc.Height();
 //   Bitmap bm(m_bmp);
	//int newSize = rc.Height();

	//// ��ȡ���ͼƬ�Ŀ�͸�
	//float width = bm.getWidth();
	//float height = bm.getHeight();
	//// ��������ͼƬ�õ�matrix����
	//Matrix matrix = new Matrix();
	//// ������������
	//float scaleWidth = ((float) newSize) / width;
	//float scaleHeight = ((float) newSize) / height;
	//// ����ͼƬ����
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
		acutPrintf(_T("\n�޷�װ��ָ��ͼ���ļ�!"));
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
