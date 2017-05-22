// pxShapeImg.cpp: implementation of the pxShapeImg class.
//
//////////////////////////////////////////////////////////////////////
#include "precomp.h"
#include "pxShapeImg.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

pxShapeImg::pxShapeImg()
{
	m_Bitmap = wxNullBitmap;
	wxImage img;
	img.LoadFile("logo.bmp", wxBITMAP_TYPE_ANY);
	img.SetMaskColour(255,255,255);
	m_Bitmap = wxBitmap(img);
}

pxShapeImg::~pxShapeImg()
{

}

void pxShapeImg::Draw( pxCanvas* canvas )
{
	canvas->DrawBitmap(m_Bitmap, m_Point.m_x, m_Point.m_y, GetRotationAngle(), m_flipped);
}

pxShapeType pxShapeImg::GetShapeType()
{
	return pxShapeType_Img;
}

int pxShapeImg::Write( pxShapeIOStream* stream )
{
	int len = 0;
	int ret = 0;

	CA_RETURN(pxShapeRect::Write(stream), len, ret);
	
	//todo
	return len;
}

int pxShapeImg::Read( pxShapeIOStream* stream )
{
	int len = 0;
	int ret = 0;

	CA_RETURN(pxShapeRect::Read(stream), len, ret);
	
	//todo
	return len;
}
