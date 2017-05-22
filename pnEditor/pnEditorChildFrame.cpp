#include "precomp.h"
#include "pnEditorChildFrame.h"
#include "pnEditorCanvas.h"
#include "pxFileIOStream.h"
#include "pnEditorControlID.h"
#include "pnEditorFrame.h"
#include "rc.h"

BEGIN_EVENT_TABLE(pnEditorChildFrame, wxMDIChildFrame)
	//file menu
	EVT_MENU(wxID_CLOSE,			pnEditorChildFrame::OnClose)
	EVT_MENU(wxID_SAVE,				pnEditorChildFrame::OnSave)
	EVT_UPDATE_UI(wxID_SAVE,		pnEditorChildFrame::OnUpdateSave)
	EVT_MENU(wxID_SAVEAS,			pnEditorChildFrame::OnSaveAs)
	//EVT_UPDATE_UI(wxID_SAVEAS,		pnEditorChildFrame::OnUpdateSave)
	EVT_MENU(wxID_PRINT_SETUP,		pnEditorChildFrame::OnPrintSetup)
	EVT_MENU(wxID_PRINT,			pnEditorChildFrame::OnPrint)
	//edit menu
	EVT_MENU(wxID_UNDO,				pnEditorChildFrame::OnUndo)
	EVT_UPDATE_UI(wxID_UNDO,		pnEditorChildFrame::OnUpdateUndo)
	EVT_MENU(wxID_REDO,				pnEditorChildFrame::OnRedo)
	EVT_UPDATE_UI(wxID_REDO,		pnEditorChildFrame::OnUpdateRedo)
	EVT_MENU(wxID_CUT,				pnEditorChildFrame::OnCut)
	EVT_UPDATE_UI(wxID_CUT,			pnEditorChildFrame::OnUpdateCut)
	EVT_MENU(wxID_COPY,				pnEditorChildFrame::OnCopy)
	EVT_UPDATE_UI(wxID_COPY,		pnEditorChildFrame::OnUpdateCopy)
	EVT_MENU(wxID_PASTE,			pnEditorChildFrame::OnPaste)
	EVT_UPDATE_UI(wxID_PASTE,		pnEditorChildFrame::OnUpdatePaste)
	EVT_MENU(wxID_DELETE,			pnEditorChildFrame::OnDelete)
	EVT_UPDATE_UI(wxID_DELETE,		pnEditorChildFrame::OnUpdateDelete)
	EVT_MENU(wxID_SELECTALL,		pnEditorChildFrame::OnSelectAll)
	//view menu
	EVT_MENU(wxID_ZOOM_IN,			pnEditorChildFrame::OnZoomIn)
	EVT_MENU(wxID_ZOOM_OUT,			pnEditorChildFrame::OnZoomOut)
	EVT_MENU(wxID_ZOOM_100,			pnEditorChildFrame::OnZoom100)
	EVT_MENU(wxID_ZOOM_FIT,			pnEditorChildFrame::OnZoomFit)
	//operation menu
	EVT_MENU(ID_ASSEMBLE,			pnEditorChildFrame::OnAssemble)
	EVT_UPDATE_UI(ID_ASSEMBLE,		pnEditorChildFrame::OnUpdateAssemble)
	EVT_MENU(ID_DISSEMBLE,			pnEditorChildFrame::OnDissemble)
	EVT_UPDATE_UI(ID_DISSEMBLE,		pnEditorChildFrame::OnUpdateDissemble)
	EVT_MENU(ID_BRING_TOP,			pnEditorChildFrame::OnBringTop)
	EVT_UPDATE_UI(ID_BRING_TOP,		pnEditorChildFrame::OnUpdateBringTop)
	EVT_MENU(ID_BRING_BOTTOM,		pnEditorChildFrame::OnBringBottom)
	EVT_UPDATE_UI(ID_BRING_BOTTOM,	pnEditorChildFrame::OnUpdateBringBottom)
	EVT_MENU(ID_BRING_UP,			pnEditorChildFrame::OnBringUp)
	EVT_UPDATE_UI(ID_BRING_UP,		pnEditorChildFrame::OnUpdateBringUp)
	EVT_MENU(ID_BRING_DOWN,			pnEditorChildFrame::OnBringDown)
	EVT_UPDATE_UI(ID_BRING_DOWN,	pnEditorChildFrame::OnUpdateBringDown)
	EVT_MENU(ID_ALGINLEFT,			pnEditorChildFrame::OnAlignLeft)
	EVT_UPDATE_UI(ID_ALGINLEFT,		pnEditorChildFrame::OnUpdateAlignLeft)
	EVT_MENU(ID_ALGINRIGHT,			pnEditorChildFrame::OnAlignRight)
	EVT_UPDATE_UI(ID_ALGINRIGHT,	pnEditorChildFrame::OnUpdateAlignRight)
	EVT_MENU(ID_ALGINHCENTER,		pnEditorChildFrame::OnAlignHCenter)
	EVT_UPDATE_UI(ID_ALGINHCENTER,	pnEditorChildFrame::OnUpdateAlignHCenter)
	EVT_MENU(ID_ALGINTOP,			pnEditorChildFrame::OnAlignTop)
	EVT_UPDATE_UI(ID_ALGINTOP,		pnEditorChildFrame::OnUpdateAlignTop)
	EVT_MENU(ID_ALGINBOTTOM,		pnEditorChildFrame::OnAlignBottom)
	EVT_UPDATE_UI(ID_ALGINBOTTOM,	pnEditorChildFrame::OnUpdateAlignBottom)
	EVT_MENU(ID_ALGINVCENTER,		pnEditorChildFrame::OnAlignVCenter)
	EVT_UPDATE_UI(ID_ALGINVCENTER,	pnEditorChildFrame::OnUpdateAlignVCenter)
	EVT_MENU(ID_HSAMEDISTANCE,		pnEditorChildFrame::OnHSameDistance)
	EVT_UPDATE_UI(ID_HSAMEDISTANCE,	pnEditorChildFrame::OnUpdateHSameDistance)
	EVT_MENU(ID_VSAMEDISTANCE,		pnEditorChildFrame::OnVSameDistance)
	EVT_UPDATE_UI(ID_VSAMEDISTANCE,	pnEditorChildFrame::OnUpdateVSameDistance)
	EVT_MENU(ID_SAME_WIDTH,			pnEditorChildFrame::OnSameWidth)
	EVT_UPDATE_UI(ID_SAME_WIDTH,	pnEditorChildFrame::OnUpdateSameWidth)
	EVT_MENU(ID_SAME_HEIGHT,		pnEditorChildFrame::OnSameHeight)
	EVT_UPDATE_UI(ID_SAME_HEIGHT,	pnEditorChildFrame::OnUpdateSameHeight)
	EVT_MENU(ID_SAME_SIZE,			pnEditorChildFrame::OnSameSize)
	EVT_UPDATE_UI(ID_SAME_SIZE,		pnEditorChildFrame::OnUpdateSameSize)
	EVT_MENU(ID_ROTATE_N90,			pnEditorChildFrame::OnRotateN90)
	EVT_UPDATE_UI(ID_ROTATE_N90,	pnEditorChildFrame::OnUpdateRotateN90)
	EVT_MENU(ID_ROTATE_P90,			pnEditorChildFrame::OnRotateP90)
	EVT_UPDATE_UI(ID_ROTATE_P90,	pnEditorChildFrame::OnUpdateRotateP90)
	EVT_MENU(ID_FLIP_VERT,			pnEditorChildFrame::OnFlipVert)
	EVT_UPDATE_UI(ID_FLIP_VERT,		pnEditorChildFrame::OnUpdateFlipVert)
	EVT_MENU(ID_FLIP_HORZ,			pnEditorChildFrame::OnFlipHorz)
	EVT_UPDATE_UI(ID_FLIP_HORZ,		pnEditorChildFrame::OnUpdateFlipHorz)
END_EVENT_TABLE()

wxMenuBar* pnEditorChildFrame::m_menuBar = 0;
int pnEditorChildFrame::m_menubar_ref_count = 0;

pnEditorChildFrame::pnEditorChildFrame(wxMDIParentFrame *parent) :
	wxMDIChildFrame(parent, wxID_ANY, "")
{
	m_pCanvas = new pnEditorCanvas(this);

	m_filePath = "";

	CreateMenus();

	Maximize();
}

pnEditorChildFrame::~pnEditorChildFrame()
{
	FreeMenus();
}

void pnEditorChildFrame::OnActivate( wxActivateEvent&  )
{
	if(m_pCanvas)
		m_pCanvas->SetFocus();
}

bool pnEditorChildFrame::ToolboxChangeNotify( int id )
{
	return m_pCanvas->SetCurrentTool( id );
}

void pnEditorChildFrame::OnSave( wxCommandEvent& evt )
{
	if(m_filePath == "")
		OnSaveAs(evt);
	else
		Save(m_filePath);
}

void pnEditorChildFrame::OnSaveAs( wxCommandEvent&  )
{
	wxFileDialog 
		saveFileDialog(this, "保存图形文件", "", "",
		"图形文件(*.pns)|*.pns", wxFD_SAVE|wxFD_OVERWRITE_PROMPT);

	saveFileDialog.CentreOnParent();

	if (saveFileDialog.ShowModal() == wxID_OK)
	{
		Save(saveFileDialog.GetPath());
	}
}

bool pnEditorChildFrame::Save( const wxString& file_path )
{
	bool ret = true;

	wxFile file;
	if(!file.Open(file_path, wxFile::write))
	{
		wxMessageBox("文件不能写");
		ret = false;
	}
	else
	{
		pxFileIOStream stream(&file);
		if(!GetCanvas()->SavePage(stream))
		{
			wxMessageBox("保存文件失败！");
			ret = false;
		}
	}

	return ret;
}

void pnEditorChildFrame::OnUpdateSave( wxUpdateUIEvent& evt )
{
	evt.Enable(GetCanvas()->GetModified());
}

bool pnEditorChildFrame::Load( const wxString& file_path )
{
	bool ret = true;
	m_filePath = file_path;

	wxFile file;
	if(!file.Open(m_filePath, wxFile::read))
	{
		wxMessageBox("文件不能读取!");
		ret = false;
	}
	else
	{
		pxFileIOStream stream(&file);

		if(!GetCanvas()->LoadPage(stream))
		{
			wxMessageBox("读取文件失败!");
			ret = false;
		}
	}

	GetCanvas()->SetModified(false);

	return ret;
}

void pnEditorChildFrame::OnClose( wxCommandEvent& evt )
{
	if(GetCanvas()->GetModified())
	{
		OnSave(evt);
		GetCanvas()->SetModified(false);
	}

	Close();
}

void pnEditorChildFrame::OnPrintSetup( wxCommandEvent& )
{

}

void pnEditorChildFrame::OnPrint( wxCommandEvent& )
{

}

void pnEditorChildFrame::OnUndo( wxCommandEvent& )
{
	GetCanvas()->OperationUndo();
}

void pnEditorChildFrame::OnUpdateUndo( wxUpdateUIEvent& evt)
{
	evt.Enable(GetCanvas()->CanUndo());
}

void pnEditorChildFrame::OnRedo( wxCommandEvent& )
{
	GetCanvas()->OperationRedo();
}

void pnEditorChildFrame::OnUpdateRedo( wxUpdateUIEvent& evt )
{
	evt.Enable(GetCanvas()->CanRedo());
}

void pnEditorChildFrame::OnCut( wxCommandEvent& )
{
	GetCanvas()->OperationCut();
}

void pnEditorChildFrame::OnUpdateCut( wxUpdateUIEvent& evt )
{
	evt.Enable(GetCanvas()->GetSelectionCount() > 0);
}

void pnEditorChildFrame::OnCopy( wxCommandEvent& )
{
	GetCanvas()->OperationCopy();
}

void pnEditorChildFrame::OnUpdateCopy( wxUpdateUIEvent& evt )
{
	evt.Enable(GetCanvas()->GetSelectionCount() > 0);
}

void pnEditorChildFrame::OnPaste( wxCommandEvent& )
{
	GetCanvas()->OperationPaste();
}

void pnEditorChildFrame::OnUpdatePaste( wxUpdateUIEvent& evt )
{
	evt.Enable(GetCanvas()->CanPaste());
}

void pnEditorChildFrame::OnDelete( wxCommandEvent& )
{
	GetCanvas()->OperationDelete();
}

void pnEditorChildFrame::OnUpdateDelete( wxUpdateUIEvent& evt )
{
	evt.Enable(GetCanvas()->GetSelectionCount() > 0);
}

void pnEditorChildFrame::OnSelectAll( wxCommandEvent& )
{
	GetCanvas()->OperationSelectAll();
}

void pnEditorChildFrame::OnZoomIn( wxCommandEvent& )
{
	GetCanvas()->ZoomIn();
}

void pnEditorChildFrame::OnZoomOut( wxCommandEvent& )
{
	GetCanvas()->ZoomOut();
}

void pnEditorChildFrame::OnZoom100( wxCommandEvent& )
{
	GetCanvas()->Zoom100();
}

void pnEditorChildFrame::OnZoomFit( wxCommandEvent& )
{
	GetCanvas()->ZoomFit();
}

void pnEditorChildFrame::OnAlignLeft( wxCommandEvent& WXUNUSED(evt) )
{
	GetCanvas()->OperationAlignLeft();
}

void pnEditorChildFrame::OnUpdateAlignLeft( wxUpdateUIEvent& evt )
{
	evt.Enable(GetCanvas()->GetSelectionCount() > 1);
}

void pnEditorChildFrame::OnAlignRight( wxCommandEvent& WXUNUSED(evt) )
{
	GetCanvas()->OperationAlignRight();
}

void pnEditorChildFrame::OnUpdateAlignRight( wxUpdateUIEvent& evt )
{
	evt.Enable(GetCanvas()->GetSelectionCount() > 1);
}

void pnEditorChildFrame::OnAlignHCenter( wxCommandEvent& WXUNUSED(evt) )
{
	GetCanvas()->OperationAlignHCenter();
}

void pnEditorChildFrame::OnUpdateAlignHCenter( wxUpdateUIEvent& evt )
{
	evt.Enable(GetCanvas()->GetSelectionCount() > 1);
}

void pnEditorChildFrame::OnAlignTop( wxCommandEvent& WXUNUSED(evt) )
{
	GetCanvas()->OperationAlignTop();
}

void pnEditorChildFrame::OnUpdateAlignTop( wxUpdateUIEvent& evt )
{
	evt.Enable(GetCanvas()->GetSelectionCount() > 1);
}

void pnEditorChildFrame::OnAlignBottom( wxCommandEvent& WXUNUSED(evt) )
{
	GetCanvas()->OperationAlignBottom();
}

void pnEditorChildFrame::OnUpdateAlignBottom( wxUpdateUIEvent& evt )
{
	evt.Enable(GetCanvas()->GetSelectionCount() > 1);
}

void pnEditorChildFrame::OnAlignVCenter( wxCommandEvent& WXUNUSED(evt) )
{
	GetCanvas()->OperationAlignVCenter();
}

void pnEditorChildFrame::OnUpdateAlignVCenter( wxUpdateUIEvent& evt )
{
	evt.Enable(GetCanvas()->GetSelectionCount() > 1);
}

void pnEditorChildFrame::OnBringTop( wxCommandEvent& WXUNUSED(evt) )
{
	GetCanvas()->OperationBringToTop();
}

void pnEditorChildFrame::OnUpdateBringTop( wxUpdateUIEvent& evt )
{
	evt.Enable(GetCanvas()->GetSelectionCount() > 0);
}

void pnEditorChildFrame::OnBringBottom( wxCommandEvent& WXUNUSED(evt) )
{
	GetCanvas()->OperationBringToBottom();
}

void pnEditorChildFrame::OnUpdateBringBottom( wxUpdateUIEvent& evt )
{
	evt.Enable(GetCanvas()->GetSelectionCount() > 0);
}

void pnEditorChildFrame::OnBringUp( wxCommandEvent& WXUNUSED(evt) )
{
	GetCanvas()->OperationBringUp();
}

void pnEditorChildFrame::OnUpdateBringUp( wxUpdateUIEvent& evt )
{
	evt.Enable(GetCanvas()->GetSelectionCount() > 0);
}

void pnEditorChildFrame::OnBringDown( wxCommandEvent& WXUNUSED(evt) )
{
	GetCanvas()->OperationBringDown();
}

void pnEditorChildFrame::OnUpdateBringDown( wxUpdateUIEvent& evt )
{
	evt.Enable(GetCanvas()->GetSelectionCount() > 0);
}

void pnEditorChildFrame::OnAssemble( wxCommandEvent& WXUNUSED(evt) )
{
	GetCanvas()->OperationAssemble();
}

void pnEditorChildFrame::OnUpdateAssemble( wxUpdateUIEvent& evt )
{
	evt.Enable(GetCanvas()->GetSelectionCount() > 1);
}

void pnEditorChildFrame::OnDissemble( wxCommandEvent& WXUNUSED(evt) )
{
	GetCanvas()->OperationDissemble();
}

void pnEditorChildFrame::OnUpdateDissemble( wxUpdateUIEvent& evt )
{
	evt.Enable(GetCanvas()->CanDissemble());
}

void pnEditorChildFrame::OnHSameDistance( wxCommandEvent& WXUNUSED(evt) )
{
	GetCanvas()->OperationHSameDistance();
}

void pnEditorChildFrame::OnUpdateHSameDistance( wxUpdateUIEvent& evt )
{
	evt.Enable(GetCanvas()->GetSelectionCount() > 2);
}

void pnEditorChildFrame::OnVSameDistance( wxCommandEvent& WXUNUSED(evt) )
{
	GetCanvas()->OperationVSameDistance();
}

void pnEditorChildFrame::OnUpdateVSameDistance( wxUpdateUIEvent& evt )
{
	evt.Enable(GetCanvas()->GetSelectionCount() > 2);
}

void pnEditorChildFrame::OnSameWidth( wxCommandEvent& WXUNUSED(evt) )
{
	GetCanvas()->OperationSameWidth();
}

void pnEditorChildFrame::OnUpdateSameWidth( wxUpdateUIEvent& evt )
{
	evt.Enable(GetCanvas()->GetSelectionCount() > 1);
}

void pnEditorChildFrame::OnSameHeight( wxCommandEvent& WXUNUSED(evt) )
{
	GetCanvas()->OperationSameHeight();
}

void pnEditorChildFrame::OnUpdateSameHeight( wxUpdateUIEvent& evt )
{
	evt.Enable(GetCanvas()->GetSelectionCount() > 1);
}

void pnEditorChildFrame::OnSameSize( wxCommandEvent& WXUNUSED(evt) )
{
	GetCanvas()->OperationSameSize();
}

void pnEditorChildFrame::OnUpdateSameSize( wxUpdateUIEvent& evt )
{
	evt.Enable(GetCanvas()->GetSelectionCount() > 1);
}

void pnEditorChildFrame::OnRotateN90( wxCommandEvent& WXUNUSED(evt) )
{
	GetCanvas()->OperationRotateN90();
}

void pnEditorChildFrame::OnUpdateRotateN90( wxUpdateUIEvent& evt )
{
	evt.Enable(GetCanvas()->GetSelectionCount() > 0);
}

void pnEditorChildFrame::OnRotateP90( wxCommandEvent& WXUNUSED(evt) )
{
	GetCanvas()->OperationRotateP90();
}

void pnEditorChildFrame::OnUpdateRotateP90( wxUpdateUIEvent& evt )
{
	evt.Enable(GetCanvas()->GetSelectionCount() > 0);
}

void pnEditorChildFrame::OnFlipVert( wxCommandEvent& WXUNUSED(evt) )
{
	GetCanvas()->OperationFlipVert();
}

void pnEditorChildFrame::OnUpdateFlipVert( wxUpdateUIEvent& evt )
{
	evt.Enable(GetCanvas()->GetSelectionCount() > 0);
}

void pnEditorChildFrame::OnFlipHorz( wxCommandEvent& WXUNUSED(evt) )
{
	GetCanvas()->OperationFlipHorz();
}

void pnEditorChildFrame::OnUpdateFlipHorz( wxUpdateUIEvent& evt )
{
	evt.Enable(GetCanvas()->GetSelectionCount() > 0);
}

void pnEditorChildFrame::CreateMenus()
{
	m_menubar_ref_count ++;
	if(!m_menuBar)
	{
		m_menuBar = new wxMenuBar;
		wxMenuItem* item;
		wxBitmap bmp;
		wxSize menuicon_size(16,16);

		wxMenu* menu;
		//file menu
		menu = new wxMenu;
		item = new wxMenuItem(menu, wxID_NEW,	"新建页面	Ctrl+N");
		bmp = wxArtProvider::GetBitmap(wxART_NEW, wxART_MENU, menuicon_size);
		item->SetBitmap(bmp);
		menu->Append(item);

		item = new wxMenuItem(menu, wxID_OPEN,	"打开页面	Ctrl+O");
		bmp = wxArtProvider::GetBitmap(wxART_FILE_OPEN, wxART_MENU, menuicon_size);
		item->SetBitmap(bmp);
		menu->Append(item);

		menu->Append(wxID_CLOSE	,				"关闭页面");
		menu->AppendSeparator();

		item = new wxMenuItem(menu, wxID_SAVE,	"保存页面	Ctrl+S");
		bmp = wxArtProvider::GetBitmap(wxART_FILE_SAVE, wxART_MENU, menuicon_size);
		item->SetBitmap(bmp);
		menu->Append(item);
	
		item = new wxMenuItem(menu, wxID_SAVEAS,"页面另存为");
		bmp = wxArtProvider::GetBitmap(wxART_FILE_SAVE_AS, wxART_MENU, menuicon_size);
		item->SetBitmap(bmp);
		menu->Append(item);

		menu->AppendSeparator();

		menu->Append(wxID_PRINT_SETUP,			"打印设置");

		item = new wxMenuItem(menu, wxID_PRINT,	"打印		Ctrl+P");
		bmp = wxArtProvider::GetBitmap(wxART_PRINT, wxART_MENU, menuicon_size);
		item->SetBitmap(bmp);
		menu->Append(item);
	
		menu->AppendSeparator();

		item = new wxMenuItem(menu, wxID_EXIT, 	"退出		Alt+F4");
		bmp = wxArtProvider::GetBitmap(wxART_QUIT, wxART_MENU, menuicon_size);
		item->SetBitmap(bmp);
		menu->Append(item);
		
		m_menuBar->Append(menu, _("文件"));
		//edit menu
		menu = new wxMenu;
		item = new wxMenuItem(menu, wxID_UNDO,	"撤销		Ctrl+Z");
		bmp = wxArtProvider::GetBitmap(wxART_UNDO, wxART_MENU, menuicon_size);
		item->SetBitmap(bmp);
		menu->Append(item);

		item = new wxMenuItem(menu, wxID_REDO,  "重做		Ctrl+Y");
		bmp = wxArtProvider::GetBitmap(wxART_REDO, wxART_MENU, menuicon_size);
		item->SetBitmap(bmp);
		menu->Append(item);
	
		menu->AppendSeparator();

		item = new wxMenuItem(menu, wxID_CUT,   "剪切		Ctrl+X");
		bmp = wxArtProvider::GetBitmap(wxART_CUT, wxART_MENU, menuicon_size);
		item->SetBitmap(bmp);
		menu->Append(item);

		item = new wxMenuItem(menu, wxID_COPY,   "拷贝		Ctrl+C");
		bmp = wxArtProvider::GetBitmap(wxART_COPY, wxART_MENU, menuicon_size);
		item->SetBitmap(bmp);
		menu->Append(item);

		item = new wxMenuItem(menu, wxID_PASTE,   "粘贴		Ctrl+V");
		bmp = wxArtProvider::GetBitmap(wxART_PASTE, wxART_MENU, menuicon_size);
		item->SetBitmap(bmp);
		menu->Append(item);

		item = new wxMenuItem(menu, wxID_DELETE,   "删除		Del");
		bmp = wxArtProvider::GetBitmap(wxART_DELETE, wxART_MENU, menuicon_size);
		item->SetBitmap(bmp);
		menu->Append(item);
	
		menu->AppendSeparator();

		menu->Append(wxID_SELECTALL,		"全选		Ctrl+A");
		m_menuBar->Append(menu, _("编辑"));
		//view menu
		menu = new wxMenu;
		wxRect toolRect(0,0,16,15);

		item = new wxMenuItem(menu, wxID_ZOOM_IN,	"放大");
		item->SetBitmap(rcToolBarBitmap.GetSubBitmap(toolRect));
		toolRect.Offset(16, 0);
		menu->Append(item);

		item = new wxMenuItem(menu, wxID_ZOOM_OUT,	"缩小");
		item->SetBitmap(rcToolBarBitmap.GetSubBitmap(toolRect));
		toolRect.Offset(16, 0);
		menu->Append(item);

		item = new wxMenuItem(menu, wxID_ZOOM_100,	"默认比例");
		item->SetBitmap(rcToolBarBitmap.GetSubBitmap(toolRect));
		toolRect.Offset(16, 0);
		menu->Append(item);

		item = new wxMenuItem(menu, wxID_ZOOM_FIT,	"适合窗口大小");
		item->SetBitmap(rcToolBarBitmap.GetSubBitmap(toolRect));
		toolRect.Offset(16, 0);
		menu->Append(item);
		m_menuBar->Append(menu, _("视图"));
		//operation menu
		menu = pnEditorCanvas::CreateOpertaionMenu();

		m_menuBar->Append(menu, _("操作"));
		//option menu
		menu = new wxMenu;
		menu->Append(wxID_PROPERTIES,	"默认属性	Ctrl+Enter");
		m_menuBar->Append(menu, _("选项"));
		//helpmenu
		menu = new wxMenu;
		menu->Append(wxID_ABOUT);
		m_menuBar->Append(menu, _("帮助"));
		SetMenuBar(m_menuBar);
	}
}

void pnEditorChildFrame::FreeMenus()
{
	m_menubar_ref_count --;
	SetMenuBar(0);
	if(m_menubar_ref_count == 0)
	{
		delete m_menuBar;
		m_menuBar = 0;
	}
}
