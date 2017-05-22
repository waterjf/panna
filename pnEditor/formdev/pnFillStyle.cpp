///-----------------------------------------------------------------
///
/// @file      pnFillStyle.cpp
/// @author    w
/// Created:   2013/9/16 15:37:59
/// @section   DESCRIPTION
///            pnFillStyle class implementation
///
///------------------------------------------------------------------

#include "pnFillStyle.h"

//Do not add custom headers
//wxDev-C++ designer will remove them
////Header Include Start
////Header Include End

//----------------------------------------------------------------------------
// pnFillStyle
//----------------------------------------------------------------------------
//Add Custom Events only in the appropriate block.
//Code added in other places will be removed by wxDev-C++
////Event Table Start
BEGIN_EVENT_TABLE(pnFillStyle,wxDialog)
	////Manual Code Start
	////Manual Code End
	
	EVT_CLOSE(pnFillStyle::OnClose)
	EVT_BUTTON(ID_WXBUTTON1,pnFillStyle::WxButton1Click)
END_EVENT_TABLE()
////Event Table End

pnFillStyle::pnFillStyle(wxWindow *parent, wxWindowID id, const wxString &title, const wxPoint &position, const wxSize& size, long style)
: wxDialog(parent, id, title, position, size, style)
{
	CreateGUIControls();
}

pnFillStyle::~pnFillStyle()
{
} 

void pnFillStyle::CreateGUIControls()
{
	//Do not add custom code between
	//GUI Items Creation Start and GUI Items Creation End.
	//wxDev-C++ designer will remove them.
	//Add the custom code before or after the blocks
	////GUI Items Creation Start

	WxStaticLine1 = new wxStaticLine(this, ID_WXSTATICLINE1, wxPoint(38, 8), wxSize(150, -1), wxLI_HORIZONTAL);
	WxStaticLine1->SetBackgroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_SCROLLBAR));

	WxButton1 = new wxButton(this, ID_WXBUTTON1, _("WxButton1"), wxPoint(324, 271), wxSize(75, 25), 0, wxDefaultValidator, _("WxButton1"));

	= new wxStaticText(this, ID_, _("ÃÓ≥‰"), wxPoint(7, 4), wxDefaultSize, 0, _(""));

	SetTitle(_("ÃÓ≥‰ Ù–‘"));
	SetIcon(wxNullIcon);
	SetSize(8,8,432,344);
	Center();
	
	////GUI Items Creation End
}

void pnFillStyle::OnClose(wxCloseEvent& /*event*/)
{
	Destroy();
}

/*
 * WxButton1Click
 */
void pnFillStyle::WxButton1Click(wxCommandEvent& event)
{
	// insert your code here
}
