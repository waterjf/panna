// pxShapeText.h: interface for the pxShapeText class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PXSHAPETEXT_H__85DC8868_726F_4E82_BC52_CBB890AA4F64__INCLUDED_)
#define AFX_PXSHAPETEXT_H__85DC8868_726F_4E82_BC52_CBB890AA4F64__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "pxShapeRect.h"

#define MAX_TEXT_SHAPE_STR_LEN 0x400000 //4M

class pxShapeText : public pxShapeRect  
{
public:
	pxShapeText();
	virtual ~pxShapeText();

public:
	//type
	virtual pxShapeType GetShapeType();
	virtual ulong GetShapeMemSize(){return sizeof(*this);};

	//persistence
	virtual int Write( pxShapeIOStream* stream );
	virtual int Read( pxShapeIOStream* stream );

	//GDI
	virtual void Draw(pxCanvas* canvas);
	
	//style
	virtual pxTextStyle TextStyle() const;
	virtual void TextStyle( const pxTextStyle& val );

protected:
	void PreapreFont(pxCanvas* canvas);

	pxTextStyle m_TextStyle;

	char* m_pszText;
};

#endif // !defined(AFX_PXSHAPETEXT_H__85DC8868_726F_4E82_BC52_CBB890AA4F64__INCLUDED_)
