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
//----- MyMenu.cpp : Implementation of CMyMenu
//-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "resource.h"
#include "MyMenu.h"
#include "MyButton.h"

extern std::vector<CMyButton*> pBtns;
extern std::vector<CAdUiPaletteSet *> pPaletteSets;

// The file name of the persisted paletteset
#define PALETTESET_FILENAME _T("TestPaletteSet.xml")

//-----------------------------------------------------------------------------
IMPLEMENT_DYNAMIC (CMyMenu, CAdUiPaletteSet)

BEGIN_MESSAGE_MAP(CMyMenu, CAdUiPaletteSet)
	ON_WM_CREATE()
	ON_WM_GETMINMAXINFO()
END_MESSAGE_MAP()

//-----------------------------------------------------------------------------
CMyMenu::CMyMenu (): m_pDlg(NULL)
{
}

//-----------------------------------------------------------------------------
CMyMenu::~CMyMenu () 
{
	if(m_pDlg)
	{
		m_pDlg->DestroyWindow();
		delete m_pDlg;
		m_pDlg = NULL;
	}
	for (size_t i = 0;i<pBtns.size();i++)
	{
		delete pBtns[i];
		pBtns[i] = NULL;
	}
	for (size_t i = 0;i<pPaletteSets.size();i++)
	{
		delete pPaletteSets[i];
		pPaletteSets[i] = NULL;
	}
}

//-----------------------------------------------------------------------------
#ifdef _DEBUG
//- Please uncomment the 2 following lines to avoid linker error when compiling
//- in release mode. But make sure to uncomment these lines only once per project
//- if you derive multiple times from CAdUiDockControlBar/CAcUiDockControlBar
//- classes.

//void CAdUiDockControlBar::AssertValid () const {
//}
#endif

//-----------------------------------------------------------------------------
//- Called by the palette set framework to determine size constraints.
//- Override these methods to provide minimum and maximum palette set sizes.
void CMyMenu::GetMinimumSize (CSize &size) {
	//- Call the default implementation by default
	size.cx = 200;
	size.cy = 400;
	CAdUiPaletteSet::GetMinimumSize (size) ;
}

//-----------------------------------------------------------------------------
void CMyMenu::GetMaximumSize (CSize &size) {
	//- Call the default implementation by default
	size.cx = 400;
	size.cy = 800;
	CAdUiPaletteSet::GetMaximumSize (size) ;
}

BOOL CMyMenu::Create(LPCTSTR lpszWindowName, CWnd* pParentWnd)
{
	CString strWndClass ;
	strWndClass = AfxRegisterWndClass (CS_DBLCLKS, LoadCursor (NULL, IDC_ARROW)) ;
	CRect rect (0, 0, 120, 200) ;
	if (!CAdUiPaletteSet::Create(lpszWindowName, WS_VISIBLE | WS_CHILD | WS_CLIPCHILDREN, rect, pParentWnd ,PSS_EDIT_NAME | PSS_PROPERTIES_MENU | PSS_AUTO_ROLLUP | PSS_CLOSE_BUTTON))
		//WS_EX_TRANSPARENT | WS_EX_WINDOWEDGE | WS_EX_CONTEXTHELP |WS_EX_RIGHT))
		//if (!CAcUiDockControlBar::Create(strWndClass, lpszTitle, WS_VISIBLE | WS_CHILD | WS_CLIPCHILDREN, rect, pParent, 0))
		return (FALSE) ;

	//SetToolID (&clsCDockControlBar) ;

	return TRUE;

}

//BOOL CMyMenu::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext)
//{
//	// TODO: 在此添加专用代码和/或调用基类
//
//	return CAdUiPaletteSet::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);
//}

int CMyMenu::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CAdUiPaletteSet::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	CAcModuleResourceOverride resourceOverride;
	m_pDlg = new CMyDlg();
	m_pDlg->Create(IDD_MYDLG, this);
	m_pDlg->MoveWindow(0, 20, 200, 1600, TRUE);
	m_pDlg->ShowWindow(SW_SHOW);

	GetWindowRect(&m_rect);
	m_p0.x = (m_rect.right - m_rect.left); //m_rect.Size().cx;
	m_p0.y = (m_rect.bottom - m_rect.top); //m_rect.Size().cy;

	GetClientRect(&m_rect); //取客户区大小   
	m_OldSize.x = m_rect.right - m_rect.left;
	m_OldSize.y = m_rect.bottom - m_rect.top;

	return 0;
}

void CMyMenu::OnDestroy( )
{
	SavePaletteSet();
    CAdUiPaletteSet::OnDestroy();
}

void CMyMenu::OnGetMinMaxInfo(MINMAXINFO* lpMMI)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (m_p0.x>0)
	{
		lpMMI->ptMinTrackSize = m_p0; //限制最小宽和高  
	}
	else
	{
		CPoint pt(400,800); //定义宽和高  
		lpMMI->ptMinTrackSize = pt; //限制最小宽和高  
	}

	CAdUiPaletteSet::OnGetMinMaxInfo(lpMMI);
}



void CMyMenu::LoadPaletteSet()
{
	MSXML::IXMLDOMDocumentPtr pDoc = NULL;
	HRESULT hr = pDoc.CreateInstance(MSXML::CLSID_DOMDocument);

	// Get the file where the palette set's properties are saved
	const TCHAR *pRoam;
	TCHAR paletteBuffer[MAX_PATH];
	BOOL bResult=acdbHostApplicationServices()->getRoamableRootFolder(pRoam);
	_tcscpy(paletteBuffer, pRoam);
	_tcscat(paletteBuffer, PALETTESET_FILENAME);
	CComVariant var(paletteBuffer);

	VARIANT_BOOL bReturn = pDoc->load(var);
	if (bReturn == VARIANT_TRUE) // success!
	{
		MSXML::IXMLDOMNodePtr pNode;
		GetChildNode(pDoc->documentElement,_T("PS"),&pNode);

		// Call the base class version of Load.
		// This would re-establish the properties of
		// the palette set
		Load(pNode);
	}
}

void CMyMenu::SavePaletteSet()
{
	// Persist the palette set data in TestPaletteSet.xml
	MSXML::IXMLDOMDocumentPtr pDoc = NULL;
	HRESULT hr = pDoc.CreateInstance(MSXML::CLSID_DOMDocument);

	// Save pallete set into roaming profile
	const TCHAR *pRoam;
	TCHAR paletteBuffer[MAX_PATH];
	BOOL bResult=acdbHostApplicationServices()->getRoamableRootFolder(pRoam);
	_tcscpy(paletteBuffer, pRoam);
	_tcscat(paletteBuffer, PALETTESET_FILENAME);
	CComVariant var(paletteBuffer);

	// create a root element
	MSXML::IXMLDOMNodePtr pNode;

	if (AddChildNode(pDoc, _T("PS"), MSXML::NODE_ELEMENT, &pNode) == FALSE)
		return;

	// This calls the base class implementation of Save 
	// The base class adds its properties (as XML nodes) into pNode
	Save(pNode);

	// Save the xml document
	hr = pDoc->save(var);
}

//Utility functions to add and get the child node from the parent XML-DOM node
BOOL GetChildNode(MSXML::IXMLDOMNode* pNode, 
				  LPCTSTR pszNodeName, 
				  MSXML::IXMLDOMNode** ppChild)
{
	try {
		ASSERT(ppChild != NULL);
		if (ppChild == NULL)
			return FALSE;

		*ppChild = NULL;

		ASSERT(pNode != NULL && pszNodeName != NULL);
		if (pNode == NULL || pszNodeName == NULL)
			return FALSE;

		USES_CONVERSION;

		CComBSTR bstrName;
		pNode->get_nodeName(&bstrName);

		CString sName = OLE2T(bstrName);
		if (sName.CompareNoCase(pszNodeName) == 0) {
			*ppChild = pNode;
			// AddRef before returning
			if (*ppChild != NULL)
				(*ppChild)->AddRef();
			return TRUE;
		}
		// Get the named child element from the list of children
		//
		MSXML::IXMLDOMNodeListPtr pChildren; 
		pNode->get_childNodes(&pChildren);
		ASSERT(pChildren != NULL);
		if (pChildren == NULL) {
			ASSERT(FALSE);
			return FALSE;
		}

		int nCount = pChildren->Getlength();
		if (nCount < 1) {
			// No children
			return FALSE;
		}

		for (long i=0; i<nCount; i++) {
			MSXML::IXMLDOMNodePtr pNode;
			pChildren->get_item(i, &pNode);
			ASSERT(pNode != NULL);

			CComBSTR bstrName;
			pNode->get_nodeName(&bstrName);

			CString sName = OLE2T(bstrName);
			if (sName.CompareNoCase(pszNodeName) == 0) {
				*ppChild = pNode.GetInterfacePtr();
				// AddRef before returning
				if (*ppChild != NULL)
					(*ppChild)->AddRef();
				return TRUE;
			}
		}
	} catch (_com_error  &e) {
		ASSERT(FALSE);
		SetLastError(e.Error());
		return FALSE;
	} catch (...) {
		TRACE(_T("Exception\n"));        
		return FALSE;
	}

	// Named child not found
	return FALSE;
}

BOOL AddChildNode(MSXML::IXMLDOMNode* pNode, LPCTSTR pszChildNodeName,
				  short nNodeType, MSXML::IXMLDOMNode** ppChild)
{
	USES_CONVERSION;

	try {
		ASSERT(ppChild != NULL);
		if (ppChild == NULL)
			return FALSE;

		*ppChild = NULL;

		ASSERT(pNode != NULL && pszChildNodeName != NULL );
		if (pNode == NULL || pszChildNodeName == NULL)
			return FALSE;

		CString strName(pszChildNodeName);

		MSXML::IXMLDOMDocumentPtr pDoc;
		MSXML::DOMNodeType type;
		pNode->get_nodeType(&type);
		if (type == MSXML::NODE_DOCUMENT)
			pDoc = pNode;
		else {
			pNode->get_ownerDocument(&pDoc);
			ASSERT( pDoc != NULL);
			if (pDoc == NULL)
				return FALSE;
		}

		_variant_t varType(nNodeType);
		_bstr_t varName(strName);//pszChildNodeName);

		MSXML::IXMLDOMNodePtr pNewNode = pDoc->createNode(varType, varName, _T(""));;
		*ppChild = pNewNode.GetInterfacePtr();
		// AddRef before returning
		if (*ppChild != NULL)
			(*ppChild)->AddRef();

		pNode->appendChild(*ppChild);
	} catch (_com_error  &e) {
		ASSERT(FALSE);
		SetLastError(e.Error());
		return FALSE;
	} catch (...) {
		TRACE(_T("Exception\n"));        
		return FALSE;
	}

	return TRUE;
}


