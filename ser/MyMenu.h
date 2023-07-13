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
//----- MyMenu.h : Declaration of the CMyMenu
//-----------------------------------------------------------------------------
#pragma once

//-----------------------------------------------------------------------------
#include "adui.h"
#include "MyDlg.h"

//-----------------------------------------------------------------------------
class CMyMenu : public CAdUiPaletteSet {
	DECLARE_DYNAMIC (CMyMenu)

public:
	CMyMenu () ;
	virtual ~CMyMenu () ;
private:
	CMyDlg*  m_pDlg;

	CRect	m_rect;
	POINT   m_p0;
	POINT	m_OldSize;

protected:
	//- Called by the palette set framework to determine size constraints.
	//- Override these methods to provide minimum and maximum palette set sizes.
	virtual void GetMinimumSize (CSize &size) ;
	virtual void GetMaximumSize (CSize &size) ;

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL Create(LPCTSTR lpszWindowName, CWnd* pParentWnd);
	//virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy(); 

public:
	void LoadPaletteSet();
	void SavePaletteSet();
	afx_msg void OnGetMinMaxInfo(MINMAXINFO* lpMMI);
} ;

//Utility functions to add and get the child node from the parent XML-DOM node

 BOOL GetChildNode(MSXML::IXMLDOMNode* pNode, 
						 LPCTSTR pszNodeName, 
						 MSXML::IXMLDOMNode** ppChild);

 BOOL AddChildNode(MSXML::IXMLDOMNode* pNode, LPCTSTR pszChildNodeName,
						 short nNodeType, MSXML::IXMLDOMNode** ppChild);
