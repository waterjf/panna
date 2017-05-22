#ifndef pxShapeConnectedLines_h__
#define pxShapeConnectedLines_h__

#include "pxShapeGroup.h"

class pxShapeConnectedLines :
	public pxShapeGroup
{
public:
	pxShapeConnectedLines(void);
	virtual ~pxShapeConnectedLines(void);

	//points
	virtual int GetControlPointNr(); //可设点坐标数量
	virtual bool GetEndPoint(pxPoint point[2]); //获取端点左边，只有线型图元有端点
	virtual void SetContrlPoint(int point_index, const pxPoint& point); //移动控制点

	//type
	virtual pxShapeType GetShapeType();
	virtual ulong GetShapeMemSize(){return sizeof(*this);};

	//GDI
	virtual void Draw( pxCanvas* canvas );
	virtual void DrawControlPointMark( pxCanvas* canvas );

	//transform
	virtual pxShapeBase* HitTest( const pxPoint& point, pxShapeBase* prev_shape );
	virtual pxTransformMark ControlPointTransformMarkTest(const pxPoint& point); //测试坐标点能否成为单点变换控制点

	void AddLine(pxShapeBase* shape, int end_point_index);

protected:
	bool PathClosed();
	void ReCreateBoundRect(pxShapeBase* new_shape);

};

#endif // pxShapeConnectedLines_h__
