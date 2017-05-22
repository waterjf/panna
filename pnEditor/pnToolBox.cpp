#include "precomp.h"
#include "pnToolBox.h"
#include "pnEditorFrame.h"
#include "pxDef.h"
#include "rc.h"

BEGIN_EVENT_TABLE(pnToolBox, wxMiniFrame)
	EVT_MENU_RANGE(pxShapeType_Invalid, pxShapeType_MAX, pnToolBox::ToolClicked)
END_EVENT_TABLE()

pnToolBox::pnToolBox(pnEditorFrame* frame) : wxMiniFrame(frame, wxID_ANY, "π§æﬂœ‰", wxDefaultPosition, wxSize(88,100),
	 wxFRAME_FLOAT_ON_PARENT | wxFRAME_TOOL_WINDOW | wxCAPTION | wxSYSTEM_MENU )
{
	m_pFrame = frame;
	m_pButtons = 0;

	wxBitmap bmp = rcToolBoxBitmap;

	wxToolBar* toolbar = new wxToolBar(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTB_VERTICAL|wxTB_LEFT);
	SetToolBar(toolbar);

	wxRect rect(0,0,16,15);

	int n;
	for(n = pxShapeType_Invalid; n < pxShapeType_ConnectedLines; n ++)
	{
		toolbar->AddTool(n, "", bmp.GetSubBitmap(rect), "Õº‘™", wxITEM_RADIO);
		rect.Offset(16,0);
	}
	toolbar->Realize();

	toolbar->SetRows(8);
	wxSize size = toolbar->GetToolSize();
	SetClientSize(0, size.GetHeight()*8+1);
}

pnToolBox::~pnToolBox(void)
{

}

void pnToolBox::ToolClicked( wxCommandEvent& evt )
{
	int id = evt.GetId();

	if(m_pFrame->ToolboxChangeNotify(id))
	{//button sunk
		
	}
}
