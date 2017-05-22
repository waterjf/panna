#ifndef pxShapeFactory_h__
#define pxShapeFactory_h__

#include "pxDef.h"
#include "pxShapePool.h"

class pxShapeFactory : public pxShapePool
{
public:
	pxShapeFactory();
	virtual ~pxShapeFactory(void);

	virtual pxShapeBase* CreateShape( pxShapeType shape_type );

public:
	void Initialize();
	void Finalize();

	void SetDefaultLineStyle(const pxLineStyle& style);
	void SetDefaultFillStyle(const pxFillStyle& style);
	void SetDefaultTextStyle(const pxTextStyle& style);

protected:
	pxLineStyle m_LineStyle;
	pxFillStyle m_FillStyle;
	pxTextStyle m_TextStyle;
};

#endif // pxShapeFactory_h__
