#ifndef _PNEDITOR_FRAME_H_
#define _PNEDITOR_FRAME_H_

#include "pnEditorControlID.h"

class pnToolBox;
class pnEditorFrame : public wxMDIParentFrame
{
public:
    pnEditorFrame(wxWindow* parent,
            wxWindowID id,
            const wxString& title,
            const wxPoint& pos = wxDefaultPosition,
            const wxSize& size = wxDefaultSize,
            long style = wxDEFAULT_FRAME_STYLE | wxSUNKEN_BORDER);

    virtual ~pnEditorFrame();

	bool ToolboxChangeNotify(int id);

	wxPropertyGridManager* GetPropGridManager() const { return m_pPropGridManager; }

protected:
	void CreateMenus();
	void CreateAccel();
	void CreateToolbar();
	void CreateToolBox();
	void CreateShapeCanvas();
	void CreatePropertyGridMgr();

private:
	void OnNew(wxCommandEvent& evt);
	void OnOpen(wxCommandEvent& evt);
	void OnExit(wxCommandEvent& evt);

	void OnAbout(wxCommandEvent& evt);
	void OnUpdateUI(wxUpdateUIEvent& evt);

private:
	pnToolBox* m_ToolBox;
	wxAuiManager m_AuiManager;

	wxWindow* m_pProGridPanel;
	wxBoxSizer* m_pProGridSizer;
	wxPropertyGridManager* m_pPropGridManager;
	
	DECLARE_EVENT_TABLE()
};

#endif
