#ifndef pnShapeModel_h__
#define pnShapeModel_h__

#include "pxShapeFactory.h"

class pnShapeView;
class pnShapeModel : public pxShapeFactory
{
public:
	pnShapeModel(void);
	virtual ~pnShapeModel(void);

	pnShapeView* GetShapeView() const;
	void SetShapeView(pnShapeView* val);

public:
	virtual int Load(pxShapeIOStream* stream);
	virtual int Save(pxShapeIOStream* stream);

	pxShapeBase* HitestShape(const pxPoint& point, pxShapeBase* prev_shape);

protected:
	pnShapeView* m_pShapeView;
};

#endif // pnShapeModel_h__
