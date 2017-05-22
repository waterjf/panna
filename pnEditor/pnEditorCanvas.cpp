#include "precomp.h"
#include "pnEditorCanvas.h"
#include "pxCanvasWx2DEx.h"
#include "pxShapeFactory.h"
#include "pnShapeModel.h"
#include "pxShapeGroup.h"
#include "pxShapeConnectedLines.h"
#include "pxFileIOStream.h"
#include "pnEditorControlID.h"
#include "pnEditorChildFrame.h"
#include "pnEditorFrame.h"

#include "rc.h"

BEGIN_EVENT_TABLE(pnEditorCanvas, wxControl)
	EVT_PAINT(pnEditorCanvas::OnPaint)
	EVT_SIZE(pnEditorCanvas::OnSize)
	EVT_ERASE_BACKGROUND(pnEditorCanvas::OnEraseBackground)
	EVT_LEFT_DOWN(pnEditorCanvas::OnLeftDown)
	EVT_LEFT_UP(pnEditorCanvas::OnLeftUp)
	EVT_CONTEXT_MENU(pnEditorCanvas::OnContextMenu)
	EVT_MOTION(pnEditorCanvas::OnMouseMove)
	EVT_KEY_DOWN(pnEditorCanvas::OnKeyDown)
	EVT_KILL_FOCUS(pnEditorCanvas::OnKillFocus)
	EVT_SET_FOCUS(pnEditorCanvas::OnSetFocus)
	EVT_SCROLLWIN(pnEditorCanvas::OnScroll)
	EVT_MOUSE_CAPTURE_LOST(pnEditorCanvas::OnCaptureLost)
	EVT_MOUSEWHEEL(pnEditorCanvas::OnMouseWheel)
END_EVENT_TABLE()

pnEditorCanvas::pnEditorCanvas(wxWindow* parent,
	wxWindowID id /*= wxID_ANY*/, 
	const wxPoint& pos /*= wxDefaultPosition*/,
	const wxSize& size /*= wxDefaultSize*/)
		: wxScrolled(parent, id, pos, size, wxBORDER),
		  pnShapeEditorView(this)
{
	m_pPopupMenu = CreateOpertaionMenu();
	
	SetShapeView(this);

	pxRect rect(50,50,200,100);
	pxPoint ofst(50,50);

	pxFillStyle fs;
	fs.pattern = wxBRUSHSTYLE_SOLID;
	fs.pattern_color.red = 255;
	fs.pattern_color.green = 0;
	fs.pattern_color.blue = 0;
	fs.pattern_color.alpha = 255;
	
	fs.color.red = 255;
	fs.color.green = 0;
	fs.color.blue = 0;
	fs.color.alpha = 255;
	pxShapeFactory::SetDefaultFillStyle(fs);

#if 1
	pxShapeBase* shape;
	pxShapeGroup* group = 0;
	pxShapeConnectedLines* scl = 0;

	int n;
	for(n = pxShapeType_MIN; n <= pxShapeType_Group; n ++)
	{
		shape = m_pShapeModel->CreateShape((pxShapeType)n);
		m_pShapeModel->RegisterShape(shape);

		shape->SetRect(rect);
	
		pxLineStyle style = shape->LineStyle();
		style.width = 2;
		style.color.red = 255;
		shape->LineStyle(style);

		pxFillStyle fill_style = shape->FillStyle();
		fill_style.color.green = 255;
		shape->FillStyle(fill_style);

		rect.Offset(ofst);

		if(n == pxShapeType_Group)
			group = (pxShapeGroup*)shape;

		if(n == pxShapeType_ConnectedLines)
			scl = (pxShapeConnectedLines*)shape;

//		if(n == pxShapeType_Polygon)
		{
			if(n == pxShapeType_Text)
			{
				pxShapeBase* shape1 = m_pShapeModel->CreateShape((pxShapeType)n);
				m_pShapeModel->RegisterShape(shape1);
				shape1->Clone(shape);
			}
			pxPoint lu;
			shape->GetPoint(0, lu);
			shape->Rotate(lu, PI/12);
		}
	}
#endif
	//create connected lines
	if(scl)
	{
		shape_list scl_list;

		pxShapeBase* p;
		pxPoint pointOrgn(300, 10);
		pxPoint point = pointOrgn;
		//add line sec
		p = m_pShapeModel->CreateShape(pxShapeType_LineSec);
		//m_pShapeModel->RegisterShape(p);
		scl_list.push_back(p);
		p->SetPoint(0, point); point += pxPoint(100,10);
		p->SetPoint(1, point);
		//add line sec
		p = m_pShapeModel->CreateShape(pxShapeType_LineSec);
		//m_pShapeModel->RegisterShape(p);
		scl_list.push_back(p);
		p->SetPoint(0, point); point += pxPoint(10,100);
		p->SetPoint(1, point);
#if 1
		//add bezier line
		p = m_pShapeModel->CreateShape(pxShapeType_BezierArc);
		//m_pShapeModel->RegisterShape(p);
		scl_list.push_back(p);
		p->SetPoint(0, point); point += pxPoint(-50,-20);
		p->SetPoint(1, point); point += pxPoint(-50, 40);
		p->SetPoint(2, point); point += pxPoint(-50, -20);
		p->SetPoint(3, point);
		//add bezier line
		p = m_pShapeModel->CreateShape(pxShapeType_BezierArc);
		//m_pShapeModel->RegisterShape(p);
		scl_list.push_back(p);
		p->SetPoint(0, point); point += pxPoint(-20,-50);
		p->SetPoint(1, point); point += pxPoint(-20, 100);
		p->SetPoint(2, point); point += pxPoint(-20, -50);
		p->SetPoint(3, point);
#endif
		//add LineSec
		p = m_pShapeModel->CreateShape(pxShapeType_LineSec);
		//m_pShapeModel->RegisterShape(p);
		scl_list.push_back(p);
		p->SetPoint(0, point); point = pointOrgn; //回到原点
		p->SetPoint(1, point);

		scl->Assemble(scl_list);
		pxFillStyle fill_style = scl->FillStyle();
		pxColour fc = {255,0,0,255,};
		fill_style.color=fc;
		scl->FillStyle(fill_style);

		
		p = m_pShapeModel->CreateShape(pxShapeType_ConnectedLines);
		m_pShapeModel->RegisterShape(p);

		p->Clone(scl);
		p->Translate(100,100);

		pxPoint lu,rb;
		p->GetBoundsRect(lu,rb);
		p->Rotate(lu, PI/12);
	}

#if 1
	shape_list list;
	for(n = pxShapeType_MIN; n < pxShapeType_ConnectedLines; n ++)
	{
		pxShapeType type = (pxShapeType)n;
	
		shape = m_pShapeModel->CreateShape(type);
		shape->SetRect(rect);

		pxLineStyle style = shape->LineStyle();
		style.width = 1;
		style.color.blue = 255;
		shape->LineStyle(style);

		pxFillStyle fill_style = shape->FillStyle();
		fill_style.pattern = wxBRUSHSTYLE_TRANSPARENT;
		shape->FillStyle(fill_style);

		rect.Offset(ofst);

		list.push_back(shape);
	}

	//create group
	if(group)
	{
		group->SetShapePool(m_pShapeModel);
		group->Assemble(list);
	}

#endif

	Zoom100();
}

pnEditorCanvas::~pnEditorCanvas(void)
{
	delete m_pPopupMenu;
	m_pPopupMenu = 0;
}

void pnEditorCanvas::OnPaint(wxPaintEvent& WXUNUSED(evt))
{
	wxPaintDC* dc = new wxPaintDC(this);
#if 0
	pxCanvasWx2DEx canvas(dc);

	wxSize clsz = GetClientSize();
	canvas.SetPen( wxPen(wxColour(0,0,0),1,wxSOLID) );
	canvas.SetBrush( *wxWHITE_BRUSH );
	canvas.DrawLine( 50, 50, clsz.x-50, clsz.y-50);
	canvas.DrawLine( 50, clsz.y-50, clsz.x-50, 50);
#endif

#if 0
	//wxStopWatch sw;
	wxGraphicsContext* gc = wxGraphicsContext::Create(*dc);

	wxSize clsz = GetClientSize();
	gc->SetPen( wxPen(wxColour(0,0,0),1,wxSOLID) );
	gc->SetBrush( *wxWHITE_BRUSH );

	// Clear the background
	gc->DrawRectangle( 0, 0, clsz.x, clsz.y);
	// Do the drawing

	wxGraphicsPath path = gc->CreatePath();
	path.MoveToPoint(50, 50);
	path.AddLineToPoint(clsz.x-50, clsz.y-50);
	gc->StrokePath(path);

	path.MoveToPoint(50, clsz.y-50);
	path.AddLineToPoint(clsz.x-50, 50);
	gc->StrokePath(path);

	//gc->StrokeLine( 50, 50, clsz.x-50, clsz.y-50);
	//gc->StrokeLine( 50, clsz.y-50, clsz.x-50, 50);

	delete gc;
//	sw.Pause();
#endif 

#if 0
	pxCanvasWx2DEx canvas(dc);

	wxSize size = GetClientSize();
	wxString s;
	pxCoord h = 0, w = 0;
	pxCoord height = 0;

	s.Printf(wxT("Size: %d x %d"), size.x, size.y); 
	canvas.GetTextExtent(s, &w, &height);
	height += 3;

	canvas.SetFont(*wxNORMAL_FONT);
	canvas.SetBrush(*wxWHITE_BRUSH);
	canvas.SetPen(*wxWHITE_PEN);
	canvas.DrawRectangle(0, 0, size.x, size.y);
	canvas.SetPen(*wxLIGHT_GREY_PEN);
	canvas.DrawLine(0, 0, size.x, size.y);
	canvas.DrawLine(0, size.y, size.x, 0);
	canvas.DrawText(s, (size.x-w)/2, ((size.y-(height*5))/2));
#endif

#if 1
	wxSize size = GetClientSize();
	pxCanvasWx2D canvas(dc);

	Paint(&canvas);

#endif
}

void pnEditorCanvas::OnEraseBackground(wxEraseEvent& WXUNUSED(evt))
{
	// intentionally empty
}

void pnEditorCanvas::OnSize(wxSizeEvent& WXUNUSED(evt))
{
	Refresh(false);
}

void pnEditorCanvas::OnLeftDown( wxMouseEvent& evt)
{
	pxPoint point = evt.GetPosition();
	MouseLeftDown(point);
	CaptureMouse();
}

void pnEditorCanvas::OnLeftUp( wxMouseEvent& evt )
{
	pxPoint point = evt.GetPosition();
	MouseLeftUp(point);
	if(GetCapture() == this)
		ReleaseMouse();

	//todo
	if (m_selected_shapes.GetCount() > 0)
	{
		pxShapeBase* shape = *m_selected_shapes.begin();

		pxShapeProperyCategory spc;
		shape->QueryProperty(spc);
	
		
	}
}

void pnEditorCanvas::OnMouseMove( wxMouseEvent& evt )
{
	pxPoint point = evt.GetPosition();
	MouseMotion(point);
}

void pnEditorCanvas::RefreshClientArea()
{
	Refresh(false);
}

pxCanvas* pnEditorCanvas::CreatePaintCanvas()
{
	wxWindowDC* dc = new wxWindowDC(this);
	pxCanvasWx2D* canvas = new pxCanvasWx2D(dc);

	return canvas;
}

void pnEditorCanvas::FreePaintCanvas(pxCanvas* canvas)
{
	delete canvas;
}

bool pnEditorCanvas::SavePage(pxFileIOStream& stream)
{
	return Save(&stream) >= 0;
}

void pnEditorCanvas::SetMarkCursor( pxTransformMark cursor_type )
{
	static bool cursors_Loaded = false;
	static wxCursor g_cursors[pxTransformMark_MovePoint_0];

	if(!cursors_Loaded)
	{
		int index = 0;
		g_cursors[index++] = wxCursor(wxCURSOR_ARROW);
		g_cursors[index++] = wxCursor(wxCURSOR_SIZING);
		g_cursors[index++] = rcRotationCursor;
		g_cursors[index++] = wxCursor(wxCURSOR_SIZENWSE);
		g_cursors[index++] = wxCursor(wxCURSOR_SIZEWE);
		g_cursors[index++] = wxCursor(wxCURSOR_SIZENESW);
		g_cursors[index++] = wxCursor(wxCURSOR_SIZENS);
		g_cursors[index++] = wxCursor(wxCURSOR_SIZENWSE);
		g_cursors[index++] = wxCursor(wxCURSOR_SIZEWE);
		g_cursors[index++] = wxCursor(wxCURSOR_SIZENESW);
		g_cursors[index++] = wxCursor(wxCURSOR_SIZENS);
		cursors_Loaded = true;
	}

	wxCursor cursor;
	if(cursor_type < pxTransformMark_MovePoint_0)
		cursor = g_cursors[cursor_type];
	else
		cursor = wxCURSOR_HAND;

	SetCursor(cursor);
}

void pnEditorCanvas::SetToolCursor( pxShapeType cursor_type )
{
	if(cursor_type > pxShapeType_Invalid)
		SetCursor(wxCURSOR_CROSS);
	else
		SetCursor(wxCURSOR_ARROW);
}

void pnEditorCanvas::OnKeyDown( wxKeyEvent& evt )
{
	int key = evt.GetKeyCode();
	switch (key)
	{
	case WXK_ESCAPE:
		OperationCancel(true);
		if(GetCapture() == this)
			ReleaseMouse();
		break;
	case WXK_DELETE:
		OperationDelete();
		break;
	case WXK_UP:
		OperationMove(0, -1);
		break;
	case WXK_DOWN:
		OperationMove(0, 1);
		break;
	case WXK_LEFT:
		OperationMove(-1, 0);
		break;
	case WXK_RIGHT:
		OperationMove(1, 0);
		break;
	default:
		break;
	}
}

void pnEditorCanvas::OnKillFocus(wxFocusEvent& )
{
	OperationCancel(false);
	if(GetCapture() == this)
		ReleaseMouse();
}

void pnEditorCanvas::OnSetFocus( wxFocusEvent& WXUNUSED(evt) )
{
	SetCurrentTool(m_eCurrentShapeType);
}

void pnEditorCanvas::OnScroll( wxScrollWinEvent& evt )
{
	wxScrolledCanvas::HandleOnScroll(evt);
	RefreshScrollState();
}

bool pnEditorCanvas::LoadPage( pxFileIOStream& stream )
{
	bool ret = true;
	//clear undo redo
	m_UndoRedo.Clear();

	if(Load(&stream) < 0)
		ret = false;

	ResetView();
	Refresh(false);

	return ret;
}

void pnEditorCanvas::GetCanvasSize( pxSize& size )
{
	size = wxWindow::GetSize();
}

void pnEditorCanvas::OnCaptureLost( wxMouseCaptureLostEvent& WXUNUSED(evt) )
{
	if(GetCapture() == this)
		ReleaseMouse();
}

void pnEditorCanvas::OnContextMenu( wxContextMenuEvent& evt )
{
	PopupMenu(m_pPopupMenu, ScreenToClient(evt.GetPosition()));
}

wxMenu* pnEditorCanvas::CreateOpertaionMenu()
{
	wxMenuItem* item;
	wxRect toolRect(16*4,0,16,15);

	wxMenu* menu = new wxMenu;

	item = new wxMenuItem(menu, ID_ASSEMBLE,	"组合");
	item->SetBitmap(rcToolBarBitmap.GetSubBitmap(toolRect));
	toolRect.Offset(16, 0);
	menu->Append(item);

	item = new wxMenuItem(menu, ID_DISSEMBLE,	"分解");
	item->SetBitmap(rcToolBarBitmap.GetSubBitmap(toolRect));
	toolRect.Offset(16, 0);
	menu->Append(item);

	menu->AppendSeparator();

	item = new wxMenuItem(menu, ID_BRING_TOP,	"提至顶层");
	item->SetBitmap(rcToolBarBitmap.GetSubBitmap(toolRect));
	toolRect.Offset(16, 0);
	menu->Append(item);

	item = new wxMenuItem(menu, ID_BRING_BOTTOM,	"沉至底层");
	item->SetBitmap(rcToolBarBitmap.GetSubBitmap(toolRect));
	toolRect.Offset(16, 0);
	menu->Append(item);

	item = new wxMenuItem(menu, ID_BRING_UP,	"上移一层");
	item->SetBitmap(rcToolBarBitmap.GetSubBitmap(toolRect));
	toolRect.Offset(16, 0);
	menu->Append(item);

	item = new wxMenuItem(menu, ID_BRING_DOWN,	"下移一层");
	item->SetBitmap(rcToolBarBitmap.GetSubBitmap(toolRect));
	toolRect.Offset(16, 0);
	menu->Append(item);

	menu->AppendSeparator();

	item = new wxMenuItem(menu, ID_ALGINLEFT,	"左对齐");
	item->SetBitmap(rcToolBarBitmap.GetSubBitmap(toolRect));
	toolRect.Offset(16, 0);
	menu->Append(item);

	item = new wxMenuItem(menu, ID_ALGINRIGHT,	"右对齐");
	item->SetBitmap(rcToolBarBitmap.GetSubBitmap(toolRect));
	toolRect.Offset(16, 0);
	menu->Append(item);

	item = new wxMenuItem(menu, ID_ALGINHCENTER,	"水平居中对齐");
	item->SetBitmap(rcToolBarBitmap.GetSubBitmap(toolRect));
	toolRect.Offset(16, 0);
	menu->Append(item);

	item = new wxMenuItem(menu, ID_ALGINTOP,	"上对齐");
	item->SetBitmap(rcToolBarBitmap.GetSubBitmap(toolRect));
	toolRect.Offset(16, 0);
	menu->Append(item);

	item = new wxMenuItem(menu, ID_ALGINBOTTOM,	"下对齐");
	item->SetBitmap(rcToolBarBitmap.GetSubBitmap(toolRect));
	toolRect.Offset(16, 0);
	menu->Append(item);

	item = new wxMenuItem(menu, ID_ALGINVCENTER,	"垂直居中对齐");
	item->SetBitmap(rcToolBarBitmap.GetSubBitmap(toolRect));
	toolRect.Offset(16, 0);
	menu->Append(item);

	menu->AppendSeparator();

	item = new wxMenuItem(menu, ID_HSAMEDISTANCE,	"水平等距");
	item->SetBitmap(rcToolBarBitmap.GetSubBitmap(toolRect));
	toolRect.Offset(16, 0);
	menu->Append(item);

	item = new wxMenuItem(menu, ID_VSAMEDISTANCE,	"垂直等距");
	item->SetBitmap(rcToolBarBitmap.GetSubBitmap(toolRect));
	toolRect.Offset(16, 0);
	menu->Append(item);

	menu->AppendSeparator();

	item = new wxMenuItem(menu, ID_SAME_HEIGHT,	"等高");
	item->SetBitmap(rcToolBarBitmap.GetSubBitmap(toolRect));
	toolRect.Offset(16, 0);
	menu->Append(item);

	item = new wxMenuItem(menu, ID_SAME_WIDTH,	"等宽");
	item->SetBitmap(rcToolBarBitmap.GetSubBitmap(toolRect));
	toolRect.Offset(16, 0);
	menu->Append(item);

	item = new wxMenuItem(menu, ID_SAME_SIZE,	"等尺寸");
	item->SetBitmap(rcToolBarBitmap.GetSubBitmap(toolRect));
	toolRect.Offset(16, 0);
	menu->Append(item);

	menu->AppendSeparator();

	item = new wxMenuItem(menu, ID_ROTATE_N90,	"逆时针旋转");
	item->SetBitmap(rcToolBarBitmap.GetSubBitmap(toolRect));
	toolRect.Offset(16, 0);
	menu->Append(item);

	item = new wxMenuItem(menu, ID_ROTATE_P90,	"顺时针旋转");
	item->SetBitmap(rcToolBarBitmap.GetSubBitmap(toolRect));
	toolRect.Offset(16, 0);
	menu->Append(item);

	item = new wxMenuItem(menu, ID_FLIP_VERT,	"垂直翻转");
	item->SetBitmap(rcToolBarBitmap.GetSubBitmap(toolRect));
	toolRect.Offset(16, 0);
	menu->Append(item);

	item = new wxMenuItem(menu, ID_FLIP_HORZ,	"水平翻转");
	item->SetBitmap(rcToolBarBitmap.GetSubBitmap(toolRect));
	toolRect.Offset(16, 0);
	menu->Append(item);

	return menu;
}

#define SCROLL_STEP 4
void pnEditorCanvas::OnMouseWheel( wxMouseEvent& evt )
{
	if(evt.GetWheelAxis() == wxMOUSE_WHEEL_VERTICAL)
	{
		if(wxGetKeyState(WXK_CONTROL))
		{
			if(evt.GetWheelRotation() > 0)
				ZoomIn();
			else
				ZoomOut();
		}
		else if(wxGetKeyState(WXK_SHIFT))
		{
			
			if(evt.GetWheelRotation() > 0)
				Scroll(GetScrollPos(wxHORIZONTAL)-SCROLL_STEP, GetScrollPos(wxVERTICAL));
			else
				Scroll(GetScrollPos(wxHORIZONTAL)+SCROLL_STEP, GetScrollPos(wxVERTICAL));
			RefreshScrollState();
		}
		else
		{
			if(evt.GetWheelRotation() > 0)
				Scroll(GetScrollPos(wxHORIZONTAL), GetScrollPos(wxVERTICAL)-SCROLL_STEP);
			else
				Scroll(GetScrollPos(wxHORIZONTAL), GetScrollPos(wxVERTICAL)+SCROLL_STEP);
			RefreshScrollState();
		}
	}
}

void pnEditorCanvas::RefreshScrollState()
{
	wxPoint point = GetViewStart();
	int x,y;
	GetScrollPixelsPerUnit(&x,&y);
	point.x *= x;
	point.y *= y;
	point.x /= m_scale.m_x;
	point.y /= m_scale.m_y;

	m_translate = -point;

	RefreshClientArea();
}

void pnEditorCanvas::SetViewSize( double cx, double cy, double xscale, double yscale )
{
	int xpos = GetScrollPos(wxHORIZONTAL);
	int ypos = GetScrollPos(wxVERTICAL);

	double xpos_scale = xscale / m_scale.m_x;
	double ypos_scale = yscale / m_scale.m_y;

	const int scroll_units = 10;
	SetScrollbars(scroll_units, scroll_units, cx*xscale/scroll_units,
		cy*yscale/scroll_units, xpos*xpos_scale, ypos*ypos_scale, true);

	pnShapeView::SetViewSize(cx, cy, xscale, yscale);

	RefreshScrollState();
}

wxPropertyGridManager* pnEditorCanvas::GetPropertyGridManager()
{
	pnEditorChildFrame* childframe = (pnEditorChildFrame*)GetParent();
	pnEditorFrame* frame = (pnEditorFrame*)childframe->GetParent();

	return frame->GetPropGridManager();
}
