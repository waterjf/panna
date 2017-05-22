#ifndef pxShapeBezier_h__
#define pxShapeBezier_h__

#include "pxShapeRect.h"

class pxShapeBezier :
	public pxShapeRect
{
public:
	pxShapeBezier(void);
	virtual ~pxShapeBezier(void);

	virtual pxShapeType GetShapeType();
	virtual ulong GetShapeMemSize(){return sizeof(*this);};

	//points
	virtual bool GetEndPoint(pxPoint point[2]); //获取端点左边，只有线型图元有端点
	virtual int GetControlPointNr(); //可设点坐标数量
	virtual void SetContrlPoint(int point_index, const pxPoint& point); //移动控制点

	//GDI
	virtual void Draw(pxCanvas* canvas);
	virtual void DrawSelectedMark(pxCanvas* canvas, bool with_corner_mark = true, bool with_rotation_mark = true);
	virtual void DrawControlPointMark( pxCanvas* canvas );

	//control test
	virtual pxShapeBase* HitTest( const pxPoint& point, pxShapeBase* prev_shape );
	virtual pxTransformMark TransformMarkTest( const pxPoint& point ); //测试坐标点能否成为变换控制点
	virtual pxTransformMark LinePointTransformMarkTest( const pxPoint& point );	

protected:
	pxPoint Bezier(const pxPoint& start, const pxPoint& end, double t);
	pxPoint Bezier(double t);
};

#endif // pxShapeBezier_h__
