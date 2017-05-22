//---------------------------------------------------------------------------
//
// Name:        pnEditorApp.h
// Author:      w
// Created:     2013/9/16 15:36:34
// Description: 
//
//---------------------------------------------------------------------------

#ifndef __PNEDITORFRMApp_h__
#define __PNEDITORFRMApp_h__

#ifdef __BORLANDC__
	#pragma hdrstop
#endif

#ifndef WX_PRECOMP
	#include <wx/wx.h>
#else
	#include <wx/wxprec.h>
#endif

class pnEditorFrmApp : public wxApp
{
	public:
		bool OnInit();
		int OnExit();
};

#endif
