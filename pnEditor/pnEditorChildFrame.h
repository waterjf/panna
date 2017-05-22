#ifndef pnEditorChildFrame_h__
#define pnEditorChildFrame_h__

class pnEditorCanvas;
class pnEditorChildFrame : public wxMDIChildFrame
{
public:
	pnEditorChildFrame(wxMDIParentFrame *parent);
	virtual ~pnEditorChildFrame();
	
	pnEditorCanvas* GetCanvas() const { return m_pCanvas; }
	bool ToolboxChangeNotify( int id );

	bool Load(const wxString& file_path);
	bool Save(const wxString& file_path);

protected://menu
	//file menu
	void OnClose(wxCommandEvent& WXUNUSED(evt));
	void OnSave(wxCommandEvent& WXUNUSED(evt));
	void OnUpdateSave(wxUpdateUIEvent& evt);
	void OnSaveAs(wxCommandEvent& WXUNUSED(evt));
	void OnPrintSetup(wxCommandEvent& WXUNUSED(evt));
	void OnPrint(wxCommandEvent& WXUNUSED(evt));
	//edit menu
	void OnUndo(wxCommandEvent& WXUNUSED(evt));
	void OnUpdateUndo(wxUpdateUIEvent& evt);
	void OnRedo(wxCommandEvent& WXUNUSED(evt));
	void OnUpdateRedo(wxUpdateUIEvent& evt);
	void OnCut(wxCommandEvent& WXUNUSED(evt));
	void OnUpdateCut(wxUpdateUIEvent& evt);
	void OnCopy(wxCommandEvent& WXUNUSED(evt));
	void OnUpdateCopy(wxUpdateUIEvent& evt);
	void OnPaste(wxCommandEvent& WXUNUSED(evt));
	void OnUpdatePaste(wxUpdateUIEvent& evt);
	void OnDelete(wxCommandEvent& WXUNUSED(evt));
	void OnUpdateDelete(wxUpdateUIEvent& evt);
	void OnSelectAll(wxCommandEvent& WXUNUSED(evt));
	//view menu
	void OnZoomIn(wxCommandEvent& WXUNUSED(evt));
	void OnZoomOut(wxCommandEvent& WXUNUSED(evt));
	void OnZoom100(wxCommandEvent& WXUNUSED(evt));
	void OnZoomFit(wxCommandEvent& WXUNUSED(evt));
	//operation menu
	void OnAssemble(wxCommandEvent& WXUNUSED(evt));
	void OnUpdateAssemble(wxUpdateUIEvent& evt);
	void OnDissemble(wxCommandEvent& WXUNUSED(evt));
	void OnUpdateDissemble(wxUpdateUIEvent& evt);
	void OnBringTop(wxCommandEvent& WXUNUSED(evt));
	void OnUpdateBringTop(wxUpdateUIEvent& evt);
	void OnBringBottom(wxCommandEvent& WXUNUSED(evt));
	void OnUpdateBringBottom(wxUpdateUIEvent& evt);
	void OnBringUp(wxCommandEvent& WXUNUSED(evt));
	void OnUpdateBringUp(wxUpdateUIEvent& evt);
	void OnBringDown(wxCommandEvent& WXUNUSED(evt));
	void OnUpdateBringDown(wxUpdateUIEvent& evt);
	////////////////////////////////////////////////
	void OnAlignLeft(wxCommandEvent& WXUNUSED(evt));
	void OnUpdateAlignLeft(wxUpdateUIEvent& evt);
	void OnAlignRight(wxCommandEvent& WXUNUSED(evt));
	void OnUpdateAlignRight(wxUpdateUIEvent& evt);
	void OnAlignHCenter(wxCommandEvent& WXUNUSED(evt));
	void OnUpdateAlignHCenter(wxUpdateUIEvent& evt);
	void OnAlignTop(wxCommandEvent& WXUNUSED(evt));
	void OnUpdateAlignTop(wxUpdateUIEvent& evt);
	void OnAlignBottom(wxCommandEvent& WXUNUSED(evt));
	void OnUpdateAlignBottom(wxUpdateUIEvent& evt);
	void OnAlignVCenter(wxCommandEvent& WXUNUSED(evt));
	void OnUpdateAlignVCenter(wxUpdateUIEvent& evt);
	void OnHSameDistance(wxCommandEvent& WXUNUSED(evt));
	void OnUpdateHSameDistance(wxUpdateUIEvent& evt);
	void OnVSameDistance(wxCommandEvent& WXUNUSED(evt));
	void OnUpdateVSameDistance(wxUpdateUIEvent& evt);
	void OnSameWidth(wxCommandEvent& WXUNUSED(evt));
	void OnUpdateSameWidth(wxUpdateUIEvent& evt);
	void OnSameHeight(wxCommandEvent& WXUNUSED(evt));
	void OnUpdateSameHeight(wxUpdateUIEvent& evt);
	void OnSameSize(wxCommandEvent& WXUNUSED(evt));
	void OnUpdateSameSize(wxUpdateUIEvent& evt);
	void OnRotateN90(wxCommandEvent& WXUNUSED(evt));
	void OnUpdateRotateN90(wxUpdateUIEvent& evt);
	void OnRotateP90(wxCommandEvent& WXUNUSED(evt));
	void OnUpdateRotateP90(wxUpdateUIEvent& evt);
	void OnFlipVert(wxCommandEvent& WXUNUSED(evt));
	void OnUpdateFlipVert(wxUpdateUIEvent& evt);
	void OnFlipHorz(wxCommandEvent& WXUNUSED(evt));
	void OnUpdateFlipHorz(wxUpdateUIEvent& evt);

	////////////////////////////////////////////////

protected:
	void OnActivate(wxActivateEvent& WXUNUSED(evt));
	void CreateMenus();
	void FreeMenus();

	pnEditorCanvas* m_pCanvas;
	wxString m_filePath;
	static wxMenuBar* m_menuBar;
	static int m_menubar_ref_count;

	DECLARE_EVENT_TABLE()
};

#endif // pnEditorChildFrame_h__
