// pxShapeEllipse.h: interface for the pxShapeEllipse class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PXSHAPEELLIPSE_H__94D5E05D_A9A0_4470_9CE5_4069CA79911E__INCLUDED_)
#define AFX_PXSHAPEELLIPSE_H__94D5E05D_A9A0_4470_9CE5_4069CA79911E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "pxShapeRect.h"

class pxShapeEllipse : public pxShapeRect  
{
public:
	pxShapeEllipse();
	virtual ~pxShapeEllipse();

public:
	//type
	virtual pxShapeType GetShapeType();
	virtual ulong GetShapeMemSize(){ return sizeof(*this); };

	//GDI
	virtual void Draw(pxCanvas* canvas);

	//control test
	virtual pxShapeBase* HitTest( const pxPoint& point, pxShapeBase* prev_shape );

protected:
	
};

#endif // !defined(AFX_PXSHAPEELLIPSE_H__94D5E05D_A9A0_4470_9CE5_4069CA79911E__INCLUDED_)
