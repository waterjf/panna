///-----------------------------------------------------------------
///
/// @file      pnEditorFrm.cpp
/// @author    w
/// Created:   2013/9/16 15:36:35
/// @section   DESCRIPTION
///            pnEditorFrm class implementation
///
///------------------------------------------------------------------

#include "pnEditorFrm.h"

//Do not add custom headers between
//Header Include Start and Header Include End
//wxDev-C++ designer will remove them
////Header Include Start
////Header Include End

//----------------------------------------------------------------------------
// pnEditorFrm
//----------------------------------------------------------------------------
//Add Custom Events only in the appropriate block.
//Code added in other places will be removed by wxDev-C++
////Event Table Start
BEGIN_EVENT_TABLE(pnEditorFrm,wxFrame)
	////Manual Code Start
	////Manual Code End
	
	EVT_CLOSE(pnEditorFrm::OnClose)
END_EVENT_TABLE()
////Event Table End

pnEditorFrm::pnEditorFrm(wxWindow *parent, wxWindowID id, const wxString &title, const wxPoint &position, const wxSize& size, long style)
: wxFrame(parent, id, title, position, size, style)
{
	CreateGUIControls();
}

pnEditorFrm::~pnEditorFrm()
{
}

void pnEditorFrm::CreateGUIControls()
{
	//Do not add custom code between
	//GUI Items Creation Start and GUI Items Creation End
	//wxDev-C++ designer will remove them.
	//Add the custom code before or after the blocks
	////GUI Items Creation Start

	SetTitle(_("pnEditor"));
	SetIcon(wxNullIcon);
	SetSize(8,8,320,334);
	Center();
	
	////GUI Items Creation End
}

void pnEditorFrm::OnClose(wxCloseEvent& event)
{
	Destroy();
}
