
#if !defined(_PNSHAPE_CANVAS_H_)
#define _PNSHAPE_CANVAS_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "pnShapeEditorModel.h"
#include "pnShapeEditorView.h"

class pxFileIOStream;
class wxPropertyGridManager;

class pnEditorCanvas :
	public wxScrolledCanvas,
	public pnShapeEditorModel,
	public pnShapeEditorView
{
public:
	pnEditorCanvas(wxWindow* parent, wxWindowID id = wxID_ANY,
		const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxDefaultSize);

	static wxMenu* CreateOpertaionMenu();

	virtual ~pnEditorCanvas(void);

	virtual void GetCanvasSize(pxSize& size);
	virtual void RefreshClientArea();
	virtual pxCanvas* CreatePaintCanvas();
	virtual void FreePaintCanvas(pxCanvas* canvas);

	virtual void SetMarkCursor(pxTransformMark cursor_type);
	virtual void SetToolCursor(pxShapeType cursor_type);

	virtual void SetViewSize(double cx, double cy, double xscale, double yscale);

public://menu event redirect
	bool LoadPage(pxFileIOStream& stream);
	bool SavePage(pxFileIOStream& stream);

private:
	void OnPaint(wxPaintEvent& WXUNUSED(evt));
	void OnEraseBackground(wxEraseEvent& WXUNUSED(evt));
	void OnSize(wxSizeEvent& WXUNUSED(evt));
	void OnLeftDown(wxMouseEvent& WXUNUSED(evt));
	void OnLeftUp(wxMouseEvent& WXUNUSED(evt));
	void OnContextMenu(wxContextMenuEvent& evt);
	void OnMouseMove(wxMouseEvent& WXUNUSED(evt));
	void OnMouseWheel(wxMouseEvent& evt);
	void OnKeyDown(wxKeyEvent& WXUNUSED(evt));
	void OnKillFocus(wxFocusEvent& WXUNUSED(evt));
	void OnSetFocus(wxFocusEvent& WXUNUSED(evt));
	void OnScroll(wxScrollWinEvent& WXUNUSED(evt));
	void OnCaptureLost(wxMouseCaptureLostEvent& WXUNUSED(evt));
	
	void RefreshScrollState();

protected:
	wxMenu* m_pPopupMenu;
	wxPropertyGridManager* GetPropertyGridManager();

	DECLARE_EVENT_TABLE()
};

#endif
