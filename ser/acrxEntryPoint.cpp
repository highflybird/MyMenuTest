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
//----- acrxEntryPoint.cpp
//-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "resource.h"
#include "MyMenu.h"
#include "MyDlg.h"
#include "Modalless.h"
#include "MyButton.h"

//-----------------------------------------------------------------------------
#define szRDS _RXST("")

//-----------------------------------------------------------------------------

CMyMenu* pMenu = NULL;
CModalless* pDlg = NULL; 
std::vector<CMyButton*> pBtns;
std::vector<CAdUiPaletteSet *> pPaletteSets;
Gdiplus::GdiplusStartupInput gdiplusStartupInput;
ULONG_PTR gdiplusToken;

//----- ObjectARX EntryPoint
class CserApp : public AcRxArxApp {

public:
	CserApp () : AcRxArxApp () {}

	virtual AcRx::AppRetCode On_kInitAppMsg (void *pkt) {
		// TODO: Load dependencies here

		// You *must* call On_kInitAppMsg here
		AcRx::AppRetCode retCode =AcRxArxApp::On_kInitAppMsg (pkt) ;
		
		// TODO: Add your initialization code here
		//初始化GDI Plus应用环境
		GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
        serccc();
		return (retCode) ;
	}

	virtual AcRx::AppRetCode On_kUnloadAppMsg (void *pkt) {
		// TODO: Add your code here
		if (pMenu)
		{
			pMenu->DestroyWindow();
			delete pMenu;
			pMenu = NULL;
		}

		if (pDlg)
		{
			pDlg->DestroyWindow();
			delete pDlg;
			pDlg = NULL;
		}
		//释放GDI Plus的应用环境
		GdiplusShutdown(gdiplusToken);

		// You *must* call On_kUnloadAppMsg here
		AcRx::AppRetCode retCode =AcRxArxApp::On_kUnloadAppMsg (pkt) ;
		// TODO: Unload dependencies here

		return (retCode) ;
	}

	virtual void RegisterServerComponents () {
	}


	// - ser.ccc command (do not rename)
	static void serccc(void)
	{
		// Add your code for command ser.ccc here
		CMDIFrameWnd* pAcadFrame = acedGetAcadFrame();
		if (!pMenu)
		{
			pMenu = new CMyMenu;
			pMenu->Create(_T("我的菜单"),pAcadFrame);
			pMenu->EnableDocking(CBRS_ALIGN_ANY);
			pMenu->SetWindowText(_T("Highflybird"));
          
			//pMenu->AddCustomMenuItems();
            const int buttonHeight = 30;
            CRect rect,rect1;
			pMenu->GetClientRect(rect);
			rect1.left = rect.left = 0;
			rect1.right = rect.right = 150;
			rect.top = buttonHeight+5;
			rect.bottom = rect.top+buttonHeight;

	        CMyButton *pBtn  = NULL;  
			CAdUiPaletteSet *pPalette = NULL;
			int ButtonID = 50000;
			CString str1,str2,str3;

			pBtn = new CMyButton(ButtonID,_T("图层管理"),_T("级别1"),_T("d:\\temp\\main.ico"),pMenu,rect,true);
			pBtn->ToCreate();
			pBtns.push_back(pBtn);
			rect.bottom = rect.top+11*buttonHeight;
			pPalette = new CAdUiPaletteSet;
			pPaletteSets.push_back(pPalette);
			pPalette->Create(_T("图层管理"), WS_CHILD | WS_CLIPCHILDREN|WS_VISIBLE, rect, pMenu ,0);
			//pPalette->Create(WS_CHILD|WS_CLIPCHILDREN|WS_VISIBLE,_T("图层管理"),pMenu);
			//pMenu->AddPalette(pPalette);
			//pPalette->SetOpacity(0);
			pPalette->SetPaletteSetStyle(0);
			ButtonID++;

			rect1.top = 0;
			rect1.bottom = buttonHeight;  
			str3 = _T("d:\\temp\\folder1.bmp");
			for(int i = 0;i<10;i++,ButtonID++)
			{
				rect1.top += buttonHeight;
				rect1.bottom +=  buttonHeight;   
				str1.Format(_T("图管菜单%d"),i);
				str2.Format(_T("图管命令%d"),i);
				//str3.Format(_T("图管图标%d"),i);
				pBtn = new CMyButton(ButtonID,str1,str2,str3,pPalette,rect1);
				if (pBtn->ToCreate()) 
					pBtns.push_back(pBtn);
			}
            
			ButtonID++;
			rect.top = rect.bottom;
			rect.bottom = rect.top+buttonHeight;
			pBtn = new CMyButton(ButtonID,_T("实用工具"),_T("级别1"),_T("d:\\temp\\test2.png"),pMenu,rect,true);
			pBtn->ToCreate();
			pBtns.push_back(pBtn);
			rect.bottom = rect.top + 11*buttonHeight;
			pPalette = new CAdUiPaletteSet;
			pPalette->Create(_T("实用工具"), WS_CHILD | WS_CLIPCHILDREN|WS_VISIBLE, rect, pMenu ,0);
			pPaletteSets.push_back(pPalette);
			//pPalette->Create(WS_CHILD|WS_CLIPCHILDREN|WS_VISIBLE,_T("图层管理"),pMenu);
			//pMenu->AddPalette(pPalette);
			//pPalette->SetOpacity(0);
			pPalette->SetPaletteSetStyle(0);

			ButtonID++;

			rect1.top = 0;
			rect1.bottom = buttonHeight;  
			for(int i = 0;i<10;i++,ButtonID++)
			{
				rect1.top += buttonHeight;
				rect1.bottom +=  buttonHeight;   
				str1.Format(_T("工具菜单%d"),i);
				str2.Format(_T("工具命令%d"),i);
				//str3.Format(_T("工具图标%d"),i);
				pBtn = new CMyButton(ButtonID,str1,str2,str3,pPalette,rect1);
				if (pBtn->ToCreate()) 
					pBtns.push_back(pBtn);
			}

		}
		pMenu->RestoreControlBar();
		pMenu->LoadPaletteSet();
		pAcadFrame->ShowControlBar(pMenu, TRUE, FALSE);
	}

	// - ser.ccc command (do not rename)
	static void serddd(void)
	{
		// Add your code for command ser.ccc here
        if (pDlg)
        {
			pDlg->ShowWindow(SW_SHOW);
        }
		else
		{
			pDlg = new CModalless(acedGetAcadFrame());
			pDlg->Create(IDD_MODALLESS);
			pDlg->ShowWindow(SW_SHOW);
		}
	}
} ;

//-----------------------------------------------------------------------------
IMPLEMENT_ARX_ENTRYPOINT(CserApp)

ACED_ARXCOMMAND_ENTRY_AUTO(CserApp, ser, ccc, ccc, ACRX_CMD_TRANSPARENT, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CserApp, ser, ddd, ddd, ACRX_CMD_TRANSPARENT, NULL)
