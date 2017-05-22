#ifndef pxShapeFreeLine_h__
#define pxShapeFreeLine_h__

#include "pxShapeBase.h"
#include "pxShapePool.h"

class pxShapeFreeLine :
	public pxShapeBase
{
public:
	pxShapeFreeLine(void);
	virtual ~pxShapeFreeLine(void);

protected:
	shape_list m_LineList;	
};

#endif // pxShapeFreeLine_h__
