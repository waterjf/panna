#include "precomp.h"
#include "rc.h"

#include <wx/stdpaths.h>

wxCursor rcRotationCursor;
wxBitmap rcToolBarBitmap;
wxBitmap rcToolBoxBitmap;
	
void LoadRc()
{
	wxImage::AddHandler(new wxCURHandler);
	wxImage::AddHandler(new wxICOHandler);
	wxImage::AddHandler(new wxPNGHandler);

	wxStandardPaths sp = wxStandardPaths::Get();
	wxString ep = sp.GetExecutablePath();
	ep = wxPathOnly(ep);
	ep = ep + "/resource/";

	rcRotationCursor = wxCursor(ep + "rotation.cur", wxBITMAP_TYPE_CUR);

	wxImage img;

	img = wxNullImage;
	img.LoadFile(ep + "toolbar.bmp",  wxBITMAP_TYPE_BMP);
	img.SetMaskColour(192,192,192);
	rcToolBarBitmap = wxBitmap(img);

	img = wxNullImage;
	img.LoadFile(ep + "toolbox.bmp",  wxBITMAP_TYPE_BMP);
	img.SetMaskColour(192,192,192);
	rcToolBoxBitmap = wxBitmap(img);
}
