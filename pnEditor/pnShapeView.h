#ifndef pnShapeView_h__
#define pnShapeView_h__

#include "pxShapeBase.h"

class pnShapeModel;
class pxCanvas;

class pnShapeView
{
public:
	pnShapeView(void);
	virtual ~pnShapeView(void);

	virtual void ResetTransform();
	virtual pnShapeModel* GetShapeModel() = 0;

	virtual void GetCanvasSize(pxSize& size) = 0;
	virtual void RefreshClientArea() = 0;
	virtual pxCanvas* CreatePaintCanvas() = 0;
	virtual void FreePaintCanvas(pxCanvas* canvas) = 0;
	virtual void SetViewSize(double cx, double cy, double xscale, double yscale);

public:
	virtual void MouseLeftDown(const pxPoint& point) = 0;
	virtual void MouseLeftUp(const pxPoint& point) = 0;
	virtual void MouseMotion(const pxPoint& point) = 0;
	virtual void OperationCancel(bool clear_selecetion) = 0;
	virtual void OperationDelete() = 0;
	virtual void OperationSelectAll() = 0;

	virtual void Paint(pxCanvas* canvas);

protected:
	pxPoint m_translate;
	pxPoint m_scale;

	pxPoint m_size;
};

#endif // pnShapeView_h__
