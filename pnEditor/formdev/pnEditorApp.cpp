//---------------------------------------------------------------------------
//
// Name:        pnEditorApp.cpp
// Author:      w
// Created:     2013/9/16 15:36:34
// Description: 
//
//---------------------------------------------------------------------------

#include "pnEditorApp.h"
#include "pnEditorFrm.h"

IMPLEMENT_APP(pnEditorFrmApp)

bool pnEditorFrmApp::OnInit()
{
    pnEditorFrm* frame = new pnEditorFrm(NULL);
    SetTopWindow(frame);
    frame->Show();
    return true;
}
 
int pnEditorFrmApp::OnExit()
{
	return 0;
}
