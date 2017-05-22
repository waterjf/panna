// pxShapeText.cpp: implementation of the pxShapeText class.
//
//////////////////////////////////////////////////////////////////////
#include "precomp.h"
#include "pxShapeText.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

pxShapeText::pxShapeText()
{
	m_pszText = 0;

	m_pszText = new char[512];
	strcpy(m_pszText, "test Test ²âÊÔ");
}

pxShapeText::~pxShapeText()
{
	delete[] m_pszText;
	m_pszText = 0;
}

void pxShapeText::Draw( pxCanvas* canvas )
{
	PrepareDraw(canvas);
	PreapreFont(canvas);

	if(m_pszText)
	{
		double rotation = GetRotationAngle();
	
		canvas->DrawText(m_pszText, m_Point.m_x, m_Point.m_y, rotation, m_flipped);
	}
}

void pxShapeText::PreapreFont( pxCanvas* canvas )
{
	canvas->SetFont(m_TextStyle);
}

pxShapeType pxShapeText::GetShapeType()
{
	return pxShapeType_Text;
}

int pxShapeText::Write( pxShapeIOStream* stream )
{
	int len = 0;
	int ret = 0;

	len = pxShapeRect::Write(stream);

	if(len >= 0)
	{
		CA_RETURN(WriteTextStyle(&m_TextStyle, stream), len, ret);
		CA_RETURN(WriteString(stream, m_pszText), len, ret);	
	}

	return len;
}

int pxShapeText::Read( pxShapeIOStream* stream )
{
	int len = 0;
	int ret = 0;

	len = pxShapeRect::Read(stream);

	if(len >= 0)
	{
		CA_RETURN(ReadTextStyle(&m_TextStyle, stream), len, ret);

		if(m_pszText)
			delete[] m_pszText;
		m_pszText = 0;

		CA_RETURN(ReadReallocString(stream, &m_pszText, 0, MAX_TEXT_SHAPE_STR_LEN), len, ret);	
	}

	return len;
}

pxTextStyle pxShapeText::TextStyle() const
{
	return m_TextStyle;
}

void pxShapeText::TextStyle( const pxTextStyle& val )
{
	m_TextStyle = val;
}
