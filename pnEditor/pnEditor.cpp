// For compilers that support precompilation, includes "wx/wx.h".

#include "precomp.h"
#include "pnEditorFrame.h"
#include "rc.h"

// -- application --

class pnEditorApp : public wxApp
{
public:
    bool OnInit();
};

DECLARE_APP(pnEditorApp)
IMPLEMENT_APP(pnEditorApp)

bool pnEditorApp::OnInit()
{
    if ( !wxApp::OnInit() )
        return false;

	LoadRc();

    wxFrame* frame = new pnEditorFrame(NULL,
                                 wxID_ANY,
                                 wxT("Í¼ÐÎ±à¼­Æ÷"),
                                 wxDefaultPosition,
                                 wxSize(800, 600));
    frame->Show();

    return true;
}
