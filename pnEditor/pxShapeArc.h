#ifndef pxShapeArc_h__
#define pxShapeArc_h__

#include "pxShapeEllipse.h"

class pxShapeArc : public pxShapeEllipse  
{
public:
	pxShapeArc();
	virtual ~pxShapeArc();

public: 
	//type
	virtual pxShapeType GetShapeType();
	virtual ulong GetShapeMemSize(){return sizeof(*this);};

	//points
	virtual int GetControlPointNr(); //可设点坐标数量
	virtual void SetPoint( int point_index, const pxPoint& point );
	virtual bool GetEndPoint(pxPoint point[2]); //获取端点左边，只有线型图元有端点
	virtual void SetContrlPoint(int point_index, const pxPoint& point); //移动控制点

	//persistence
	virtual int Write( pxShapeIOStream* stream );
	virtual int Read( pxShapeIOStream* stream );

	//GDI
	virtual void Draw(pxCanvas* canvas);
	virtual void DrawSelectedMark(pxCanvas* canvas, bool with_corner_mark = true, bool with_rotation_mark = true); //画被单选时的轮廓
	virtual void DrawControlPointMark(pxCanvas* canvas);

	//control test
	virtual pxShapeBase* HitTest( const pxPoint& point, pxShapeBase* prev_shape );
	virtual pxTransformMark TransformMarkTest(const pxPoint& point); //测试坐标点能否成为变换控制点
	virtual pxTransformMark LinePointTransformMarkTest( const pxPoint& point );	

protected:
	void CalcArcPoints(pxPoint point[2]);

	double m_start_angle; //起始角度
	double m_end_angle; //终止角度
};

#endif // pxShapeArc_h__
