#include "precomp.h"
#include "pnEditorFrame.h"
#include "pnEditorChildFrame.h"
#include "pnEditorCanvas.h"
#include "pnToolBox.h"
#include "pxFileIOStream.h"

BEGIN_EVENT_TABLE(pnEditorFrame, wxMDIParentFrame)
	EVT_MENU(wxID_NEW, pnEditorFrame::OnNew)
	EVT_MENU(wxID_OPEN, pnEditorFrame::OnOpen)
	EVT_MENU(wxID_EXIT, pnEditorFrame::OnExit)
	EVT_MENU(wxID_ABOUT, pnEditorFrame::OnAbout)
	EVT_UPDATE_UI_RANGE(wxID_LOWEST,ID_FLIP_HORZ,pnEditorFrame::OnUpdateUI)
END_EVENT_TABLE()

pnEditorFrame::pnEditorFrame(wxWindow* parent,
							 wxWindowID id,
							 const wxString& title,
							 const wxPoint& pos,
							 const wxSize& size,
							 long style)
							 : wxMDIParentFrame(parent, id, title, pos, size, style)
{
	m_AuiManager.SetManagedWindow(this);

	CreateMenus();	
	CreateToolbar();
	CreateToolBox();
	CreateStatusBar();
	GetStatusBar()->SetStatusText(_("Ready"));

	CreateShapeCanvas();
	CreatePropertyGridMgr();

	Maximize(true);

	//
	m_AuiManager.AddPane(m_pProGridPanel,
		wxAuiPaneInfo().Name(wxT("PropertyGrid")).Right());

	m_AuiManager.Update();
}

pnEditorFrame::~pnEditorFrame()
{
	m_AuiManager.UnInit();
}

void pnEditorFrame::OnExit(wxCommandEvent& WXUNUSED(event))
{
    Close(true);
}

void pnEditorFrame::OnAbout(wxCommandEvent& WXUNUSED(event))
{
    wxMessageBox(_("pnEditor\n图形文件编辑器\n(c) Copyright 20013-2016, Captain Corporation"), _("关于pnEditor"), wxOK, this);
}


void save_one_art(const wxString art_name, wxBitmapType type)
{
	wxString type_ext = ".png";
	wxArtProvider::GetBitmap(art_name, wxART_MENU, wxSize(24, 24)).SaveFile(art_name + "_24_24" + type_ext, type);
	wxArtProvider::GetBitmap(art_name, wxART_MENU, wxSize(16, 16)).SaveFile(art_name + "_16_16" + type_ext, type);
}

void save_one_art(const wxString art_name)
{
	save_one_art(art_name, wxBITMAP_TYPE_ICO);
	save_one_art(art_name, wxBITMAP_TYPE_PNG);
}

void save_art()
{
	save_one_art(wxART_ADD_BOOKMARK);
	save_one_art(wxART_DEL_BOOKMARK);
	save_one_art(wxART_HELP_SIDE_PANEL);
	save_one_art(wxART_HELP_SETTINGS);
	save_one_art(wxART_HELP_BOOK);
	save_one_art(wxART_HELP_FOLDER);
	save_one_art(wxART_HELP_PAGE);
	save_one_art(wxART_GO_BACK);
	save_one_art(wxART_GO_FORWARD);
	save_one_art(wxART_GO_UP);
	save_one_art(wxART_GO_DOWN);
	save_one_art(wxART_GO_TO_PARENT);
	save_one_art(wxART_GO_HOME);
	save_one_art(wxART_GOTO_FIRST);
	save_one_art(wxART_GOTO_LAST);
	save_one_art(wxART_FILE_OPEN);
	save_one_art(wxART_FILE_SAVE);
	save_one_art(wxART_FILE_SAVE_AS);
	save_one_art(wxART_PRINT);
	save_one_art(wxART_HELP);
	save_one_art(wxART_TIP);
	save_one_art(wxART_REPORT_VIEW);
	save_one_art(wxART_LIST_VIEW);
	save_one_art(wxART_NEW_DIR);
	save_one_art(wxART_HARDDISK);
	save_one_art(wxART_FLOPPY);
	save_one_art(wxART_CDROM);
	save_one_art(wxART_REMOVABLE);
	save_one_art(wxART_FOLDER);
	save_one_art(wxART_FOLDER_OPEN);
	save_one_art(wxART_GO_DIR_UP);
	save_one_art(wxART_EXECUTABLE_FILE);
	save_one_art(wxART_NORMAL_FILE);
	save_one_art(wxART_TICK_MARK);
	save_one_art(wxART_CROSS_MARK);
	save_one_art(wxART_ERROR);
	save_one_art(wxART_QUESTION);
	save_one_art(wxART_WARNING);
	save_one_art(wxART_INFORMATION);
	save_one_art(wxART_MISSING_IMAGE);
	save_one_art(wxART_COPY);
	save_one_art(wxART_CUT);
	save_one_art(wxART_PASTE);
	save_one_art(wxART_DELETE);
	save_one_art(wxART_NEW);
	save_one_art(wxART_UNDO);
	save_one_art(wxART_REDO);
	save_one_art(wxART_PLUS);
	save_one_art(wxART_MINUS);
	save_one_art(wxART_CLOSE);
	save_one_art(wxART_QUIT);
	save_one_art(wxART_FIND);
	save_one_art(wxART_FIND_AND_REPLACE);
}

void pnEditorFrame::CreateMenus( void )
{
	//save_art();

	wxMenuBar* mb = new wxMenuBar;
	SetMenuBar(mb);

	wxMenu* menu;
	wxMenuItem* item;
	wxBitmap bmp;
	wxSize menuicon_size(32,32);

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

	menu->AppendSeparator();

	item = menu->Append(wxID_PRINT_SETUP,	"打印设置");
	menu->AppendSeparator();

	item = new wxMenuItem(menu, wxID_EXIT, 	"退出		Alt+F4");
	bmp = wxArtProvider::GetBitmap(wxART_QUIT, wxART_MENU, menuicon_size);
	item->SetBitmap(bmp);
	menu->Append(item);
	mb->Append(menu, _("文件"));

	//helpmenu
	menu = new wxMenu;
	item = new wxMenuItem(menu, wxID_HELP,	"帮助");
	bmp = wxArtProvider::GetBitmap(wxART_HELP, wxART_MENU, menuicon_size);
	item->SetBitmap(bmp);
	menu->Append(item);

	menu->Append(wxID_ABOUT,					"关于");
	mb->Append(menu, _("帮助"));
}

void pnEditorFrame::CreateAccel()
{
#if 0
	wxAcceleratorEntry entries[256];
	int aeid = 0;

	entries[aeid++].Set(wxACCEL_CTRL, (int) 'N', wxID_NEW);
	entries[aeid++].Set(wxACCEL_CTRL, (int) 'O', wxID_OPEN);
	entries[aeid++].Set(wxACCEL_CTRL, (int) 'S', wxID_SAVE);
	entries[aeid++].Set(wxACCEL_CTRL, (int) 'P', wxID_PRINT);
	entries[aeid++].Set(wxACCEL_CTRL, (int) 'Z', wxID_UNDO);
	entries[aeid++].Set(wxACCEL_CTRL, (int) 'Y', wxID_REDO);
	entries[aeid++].Set(wxACCEL_CTRL, (int) 'X', wxID_CUT);
	entries[aeid++].Set(wxACCEL_CTRL, (int) 'C', wxID_COPY);
	entries[aeid++].Set(wxACCEL_CTRL, (int) 'V', wxID_PASTE);
	entries[aeid++].Set(wxACCEL_CTRL, (int) 'A', wxID_SELECTALL);

	wxAcceleratorTable accel(aeid, entries);
	SetAcceleratorTable(accel);
#endif
}

void pnEditorFrame::CreateToolbar( void )
{
	CreateToolBar(wxTB_FLAT | wxTB_HORIZONTAL);
	wxToolBar* toolbar = GetToolBar();

	wxSize menuicon_size(24, 24);
	toolbar->SetToolBitmapSize(menuicon_size);

	wxBitmap bmp;

	bmp = wxArtProvider::GetBitmap(wxART_NEW, wxART_TOOLBAR, menuicon_size);
	toolbar->AddTool(wxID_NEW, "", bmp, "新建文件");
	bmp = wxArtProvider::GetBitmap(wxART_FILE_OPEN, wxART_TOOLBAR, menuicon_size);
	toolbar->AddTool(wxID_OPEN, "", bmp, "打开文件");
	bmp = wxArtProvider::GetBitmap(wxART_FILE_SAVE, wxART_TOOLBAR, menuicon_size);
	toolbar->AddTool(wxID_SAVE, "", bmp, "保存文件");
	bmp = wxArtProvider::GetBitmap(wxART_FILE_SAVE_AS, wxART_TOOLBAR, menuicon_size);
	toolbar->AddTool(wxID_SAVEAS, "", bmp, "另存文件");
	
	toolbar->AddSeparator();

	bmp = wxArtProvider::GetBitmap(wxART_UNDO, wxART_TOOLBAR, menuicon_size);
	toolbar->AddTool(wxID_UNDO, "", bmp, "撤销");
	bmp = wxArtProvider::GetBitmap(wxART_REDO, wxART_TOOLBAR, menuicon_size);
	toolbar->AddTool(wxID_REDO, "", bmp, "重做");

	toolbar->AddSeparator();

	toolbar->Realize();
}

void pnEditorFrame::CreateToolBox( void )
{
	m_ToolBox = new pnToolBox(this);
	wxSize size = GetClientSize();
	wxSize wsize = GetSize();
	wxSize tbsize = m_ToolBox->GetSize();

	m_ToolBox->Move(size.GetWidth() - tbsize.GetWidth() - 24,
		wsize.GetHeight() - size.GetHeight());

	m_ToolBox->Show();
}

void pnEditorFrame::CreateShapeCanvas( void )
{
	new pnEditorChildFrame(this);
}

void pnEditorFrame::OnNew( wxCommandEvent&  )
{
	new pnEditorChildFrame(this);
}

void pnEditorFrame::OnOpen( wxCommandEvent& )
{
	wxFileDialog loadFileDialog(this, "打开图形文件", wxEmptyString, wxEmptyString,
		"图形文件(*.pns)|*.pns", wxFD_OPEN | wxFD_FILE_MUST_EXIST);

	loadFileDialog.CentreOnParent();
	
	if (loadFileDialog.ShowModal() == wxID_OK)
	{
		pnEditorChildFrame* child = new pnEditorChildFrame(this);
		if(!child->Load(loadFileDialog.GetPath()))
			delete child;
	}
}

bool pnEditorFrame::ToolboxChangeNotify( int id )
{
	wxMDIChildFrame* child = GetActiveChild();
	if(child)
		return ((pnEditorChildFrame*)child)->ToolboxChangeNotify(id);

	return false;
}

void pnEditorFrame::OnUpdateUI( wxUpdateUIEvent& evt )
{
	evt.Enable(true);
	switch (evt.GetId())
	{
	case wxID_NEW:
	case wxID_OPEN:
	case wxID_PRINT_SETUP:
	case wxID_EXIT:
	case wxID_ABOUT:
		break;
	default:
		{
			wxMDIChildFrame* child = GetActiveChild();
			if(child)
			{
				evt.StopPropagation();
				GetActiveChild()->GetEventHandler()->ProcessEvent(evt);
			}
		}
		break;
	}
}

void pnEditorFrame::CreatePropertyGridMgr()
{
	m_pProGridPanel = new wxPanel(this, wxID_ANY,	wxPoint(0, 0), wxSize(400, 400), wxTAB_TRAVERSAL);
	// Column
	m_pProGridSizer = new wxBoxSizer(wxVERTICAL);

	int style = // default style
		wxPG_BOLD_MODIFIED |
		wxPG_SPLITTER_AUTO_CENTER |
		wxPG_TOOLBAR |
		wxPG_DESCRIPTION;

	m_pPropGridManager = new wxPropertyGridManager(m_pProGridPanel,
		// Don't change this into wxID_ANY in the sample, or the
		// event handling will obviously be broken.
		wxID_ANY, /*wxID_ANY*/
		wxDefaultPosition,
		wxSize(400, 640), // FIXME: wxDefaultSize gives assertion in propgrid.
		// But calling SetInitialSize in manager changes the code
		// order to the grid gets created immediately, before SetExtraStyle
		// is called.
		style);

	m_pPropGridManager->SetColumnCount(2);
	m_pPropGridManager->ShowHeader(false);

	m_pPropGridManager->SetValidationFailureBehavior(wxPG_VFB_MARK_CELL |
		wxPG_VFB_SHOW_MESSAGEBOX);

	m_pPropGridManager->GetGrid()->SetVerticalSpacing(2);

// 	wxPGCell cell;
// 	cell.SetText("Unspecified");
// 	cell.SetFgCol(*wxLIGHT_GREY);
// 	m_pPropGridManager->GetGrid()->SetUnspecifiedValueAppearance(cell);

	m_pProGridSizer->Add(m_pPropGridManager, 1, wxEXPAND);

	m_pProGridPanel->SetSizer(m_pProGridSizer);
	m_pProGridSizer->SetSizeHints(m_pProGridPanel);

	wxPropertyGridManager* pgman = m_pPropGridManager;
	wxPropertyGridPage* pg = pgman->AddPage("CommonProperty");

	pg->Append(new wxPropertyCategory(wxT("基本属性"), wxPG_LABEL));
	pg->SetPropertyHelpString(wxT("基本属性"), wxT("Editing this will change font used in the property grid."));
	
	pg->Append(new wxStringProperty(wxT("标题"), wxPG_LABEL, ""));
	pg->Append(new wxFontProperty(wxT("字体"), wxPG_LABEL));
	pg->SetPropertyHelpString(wxT("字体"), wxT("Editing this will change font used in the property grid."));
	pg->Append(new wxSystemColourProperty(wxT("Margin Colour"), wxPG_LABEL,
		pg->GetGrid()->GetMarginColour()));
	pg->Append(new wxSystemColourProperty(wxT("Cell Colour"), wxPG_LABEL,
		pg->GetGrid()->GetCellBackgroundColour()));
	pg->Append(new wxSystemColourProperty(wxT("Cell Text Colour"), wxPG_LABEL,
		pg->GetGrid()->GetCellTextColour()));
	pg->Append(new wxSystemColourProperty(wxT("Line Colour"), wxPG_LABEL,
		pg->GetGrid()->GetLineColour()));
}
