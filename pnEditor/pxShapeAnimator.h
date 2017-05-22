// pxShapeAnimator.h: interface for the pxShapeAnimator class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PXSHAPEANIMATOR_H__4C9DB3F1_20CD_48AD_8D87_80E0DB2179A7__INCLUDED_)
#define AFX_PXSHAPEANIMATOR_H__4C9DB3F1_20CD_48AD_8D87_80E0DB2179A7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "pxShapeRect.h"

class pxShapeAnimator : public pxShapeRect
{
public:
	pxShapeAnimator();
	virtual ~pxShapeAnimator();

public:
	//type
	virtual pxShapeType GetShapeType();
	virtual ulong GetShapeMemSize(){return sizeof(*this);};

	virtual int Write( pxShapeIOStream* stream );
	virtual int Read( pxShapeIOStream* stream );

	//GDI
	virtual void Draw(pxCanvas* canvas);

protected:

};

#endif // !defined(AFX_PXSHAPEANIMATOR_H__4C9DB3F1_20CD_48AD_8D87_80E0DB2179A7__INCLUDED_)
