///-----------------------------------------------------------------
///
/// @file      pnFillStyle.h
/// @author    w
/// Created:   2013/9/16 15:37:59
/// @section   DESCRIPTION
///            pnFillStyle class declaration
///
///------------------------------------------------------------------

#ifndef __PNFILLSTYLE_H__
#define __PNFILLSTYLE_H__

#ifdef __BORLANDC__
	#pragma hdrstop
#endif

#ifndef WX_PRECOMP
	#include <wx/wx.h>
	#include <wx/dialog.h>
#else
	#include <wx/wxprec.h>
#endif

//Do not add custom headers between 
//Header Include Start and Header Include End.
//wxDev-C++ designer will remove them. Add custom headers after the block.
////Header Include Start
#include <wx/statline.h>
#include <wx/button.h>
#include <wx/stattext.h>
////Header Include End

////Dialog Style Start
#undef pnFillStyle_STYLE
#define pnFillStyle_STYLE wxCAPTION | wxCLOSE_BOX
////Dialog Style End

class pnFillStyle : public wxDialog
{
	private:
		DECLARE_EVENT_TABLE();
		
	public:
		pnFillStyle(wxWindow *parent, wxWindowID id = 1, const wxString &title = wxT("ÃÓ≥‰ Ù–‘"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = pnFillStyle_STYLE);
		virtual ~pnFillStyle();
		void WxButton1Click(wxCommandEvent& event);
	
	private:
		//Do not add custom control declarations between 
		//GUI Control Declaration Start and GUI Control Declaration End.
		//wxDev-C++ will remove them. Add custom code after the block.
		////GUI Control Declaration Start
		wxStaticLine *WxStaticLine1;
		wxButton *WxButton1;
		wxStaticText *;
		////GUI Control Declaration End
		
	private:
		//Note: if you receive any error with these enum IDs, then you need to
		//change your old form code that are based on the #define control IDs.
		//#defines may replace a numeric value for the enum names.
		//Try copy and pasting the below block in your old form header files.
		enum
		{
			////GUI Enum Control ID Start
			ID_WXSTATICLINE1 = 1004,
			ID_WXBUTTON1 = 1002,
			ID_ = 1001,
			////GUI Enum Control ID End
			ID_DUMMY_VALUE_ //don't remove this value unless you have other enum values
		};
	
	private:
		void OnClose(wxCloseEvent& event);
		void CreateGUIControls();
};

#endif
