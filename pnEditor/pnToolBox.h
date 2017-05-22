#ifndef pnToolBox_h__
#define pnToolBox_h__

class pnEditorFrame;
class pnToolBox :
	public wxMiniFrame
{
public:
	pnToolBox(pnEditorFrame* frame);
	virtual ~pnToolBox(void);

	void ToolClicked(wxCommandEvent& evt);

protected:
	pnEditorFrame* m_pFrame;
	wxBitmapButton* m_pButtons;

	DECLARE_EVENT_TABLE()
};

#endif // pnToolBox_h__
