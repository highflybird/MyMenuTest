// (C) Copyright 2002-2007 by Autodesk, Inc. 
//
// Permission to use, copy, modify, and distribute this software in
// object code form for any purpose and without fee is hereby granted, 
// provided that the above copyright notice appears in all copies and 
// that both that copyright notice and the limited warranty and
// restricted rights notice below appear in all supporting 
// documentation.
//
// AUTODESK PROVIDES THIS PROGRAM "AS IS" AND WITH ALL FAULTS. 
// AUTODESK SPECIFICALLY DISCLAIMS ANY IMPLIED WARRANTY OF
// MERCHANTABILITY OR FITNESS FOR A PARTICULAR USE.  AUTODESK, INC. 
// DOES NOT WARRANT THAT THE OPERATION OF THE PROGRAM WILL BE
// UNINTERRUPTED OR ERROR FREE.
//
// Use, duplication, or disclosure by the U.S. Government is subject to 
// restrictions set forth in FAR 52.227-19 (Commercial Computer
// Software - Restricted Rights) and DFAR 252.227-7013(c)(1)(ii)
// (Rights in Technical Data and Computer Software), as applicable.
//

//-----------------------------------------------------------------------------
//----- MyDlg.cpp : Implementation of CMyDlg
//-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "resource.h"
#include "MyDlg.h"

//-----------------------------------------------------------------------------
IMPLEMENT_DYNAMIC (CMyDlg, CAdUiBaseDialog)

BEGIN_MESSAGE_MAP(CMyDlg, CAdUiBaseDialog)
	ON_MESSAGE(WM_ACAD_KEEPFOCUS, OnAcadKeepFocus)
END_MESSAGE_MAP()

//-----------------------------------------------------------------------------
CMyDlg::CMyDlg (CWnd *pParent /*=NULL*/, HINSTANCE hInstance /*=NULL*/) : CAdUiBaseDialog (CMyDlg::IDD, pParent, hInstance) {
}

//-----------------------------------------------------------------------------
void CMyDlg::DoDataExchange (CDataExchange *pDX) {
	CAdUiBaseDialog::DoDataExchange (pDX) ;
	DDX_Control(pDX, IDC_BUTTON1, m_btn1);
	DDX_Control(pDX, IDC_BUTTON2, m_btn2);
	DDX_Control(pDX, IDC_BUTTON3, m_btn3);
}

//-----------------------------------------------------------------------------
//----- Needed for modeless dialogs to keep focus.
//----- Return FALSE to not keep the focus, return TRUE to keep the focus
LRESULT CMyDlg::OnAcadKeepFocus (WPARAM, LPARAM) {
	return (TRUE) ;
}

BOOL CMyDlg::OnInitDialog()
{
	CAdUiBaseDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	HBITMAP hBitmap = (HBITMAP)::LoadImage(NULL,_T("res\\folder1.bmp"), IMAGE_BITMAP,0,0,LR_LOADFROMFILE|LR_CREATEDIBSECTION);
	m_btn1.SetBitmap(hBitmap);


	m_btn1.SubclassDlgItem(IDC_BUTTON1, this);
	m_btn1.SetBitmaps(IDB_BITMAP1, RGB(255, 255, 255),IDB_BITMAP2,RGB(255,255,255));

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
