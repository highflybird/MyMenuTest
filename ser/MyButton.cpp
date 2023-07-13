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



// CMyButton ��Ϣ�������

#if IS_CBUTTON

void DrawImages3(HDC &hdc,CString fileName,int h)  
{  
	RECT rect = {0,0,h,h};  
	FillRect(hdc, &rect, (HBRUSH)::GetStockObject(NULL_BRUSH));  //GRAY_BRUSH

	Bitmap bmp1(fileName);  
	//CGdiplusConvertHelper::DumpBitmap(bmp1);  

	HBITMAP hBitmap= NULL;  
	Color clr(0,0,0,0);   
	if (Gdiplus::Ok == bmp1.GetHBITMAP(clr, &hBitmap))//ת����ɵ���Ԥ��Alpha��ͼ��  
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
	// TODO:  ������Ĵ����Ի���ָ����
	CDC *pDC = CDC::FromHandle( lpDrawItemStruct->hDC );
	CRect m_butRect = lpDrawItemStruct->rcItem;		// ��ȡ��ť�ߴ�
	GetWindowText( m_szTitle );						// ��ȡ��ť�ı�?
	CPoint m_Center = m_butRect.CenterPoint();		// ��ť���ĵ�
	int nSavedDC = pDC->SaveDC();
	VERIFY( pDC );
	//acutPrintf(_T("\n��ʽ��:%ld,btn��ʽ��%ld"), GetStyle(),GetButtonStyle() );
	m_BkColor = m_pParent->GetDC()->GetBkColor();   //�ͱ���ɫһ��
	//pDC->FillSolidRect( &m_butRect, m_BkColor);	// ����ť����


	CSize Extent = pDC->GetTextExtent( m_szTitle ); // ����ť����
	pDC->SetTextColor( m_ForeColor );
	//pDC->SetBkMode( TRANSPARENT );
	CRect m_textRect;  // �ı�λ��
	m_textRect.SetRect( m_butRect.left + 20, m_Center.y - Extent.cy / 2, m_butRect.left + 20 + Extent.cx, m_Center.y + Extent.cy / 2 );
	pDC->TextOut( m_textRect.left+20, m_textRect.top, m_szTitle );
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
	else
	{
		;
		//pDC->Draw3dRect(m_butRect, RGB(0,0,255), RGB(255,255,0));
	}


	//CImage image; //����ͼƬ��  
	//image.Load(m_szFile); //����ͼƬ·������ͼƬ  
	 
 
	CRect rc;//���������  
	///int cx = image.GetWidth();//��ȡͼƬ���  
	//int cy = image.GetHeight();//��ȡͼƬ�߶� 

	GetClientRect(&rc);//���pictrue�ؼ����ڵľ�������  
	
	int h = rc.Height();
	rc.right = h+10;
	rc.left = 10;
	rc.top = 2;
	rc.bottom = h-2;

	HDC hDc = pDC->GetSafeHdc();
	DrawImages3(hDc,m_szFile,h);
 
 
	//if (image.GetBPP() == 32) //ȷ�ϸ�ͼ�����Alphaͨ��
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
	//image.Draw(m_pDC->m_hDC, rc); //��ͼƬ����Picture�ؼ���ʾ�ľ�������  
	

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
	SetFocus();
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	acutPrintf(_T("%s��������%s.\n"),m_szTitle ,m_szCmd);
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
	int extStyle = WS_EX_DLGMODALFRAME|WS_EX_TOPMOST|WS_EX_LEFTSCROLLBAR|WS_EX_NOPARENTNOTIFY; 
	BOOL isOk = CreateEx(0,_T("Button"),m_szTitle,0x5001400B,m_rc,m_pParent,m_id);  //0x5001400B  ,0x5001010B,

	m_Fonts.CreatePointFont(100,_T("΢���ź�"),NULL);
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
		acutPrintf(_T("\n�޷�װ��ָ��ͼ���ļ�!"));
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
