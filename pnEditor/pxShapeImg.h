// pxShapeImg.h: interface for the pxShapeImg class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PXSHAPEIMG_H__754B54AC_825C_46B9_AC07_CBE9397B9BD1__INCLUDED_)
#define AFX_PXSHAPEIMG_H__754B54AC_825C_46B9_AC07_CBE9397B9BD1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "pxShapeRect.h"

class pxShapeImg : public pxShapeRect  
{
public:
	pxShapeImg();
	virtual ~pxShapeImg();

public: 
	//type
	virtual pxShapeType GetShapeType();
	virtual ulong GetShapeMemSize(){return sizeof(*this);};

	virtual int Write( pxShapeIOStream* stream );
	virtual int Read( pxShapeIOStream* stream );
	
	//GDI
	virtual void Draw(pxCanvas* canvas);

protected:
	pxBitmap m_Bitmap;
};

#endif // !defined(AFX_PXSHAPEIMG_H__754B54AC_825C_46B9_AC07_CBE9397B9BD1__INCLUDED_)
