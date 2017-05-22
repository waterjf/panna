#ifndef pxShapeRect_h__
#define pxShapeRect_h__

#include "pxShapeBase.h"

class pxShapeRect :
	public pxShapeBase
{
public:
	pxShapeRect(void);
	virtual ~pxShapeRect(void);

public:
	//type
	virtual pxShapeType GetShapeType();
	virtual ulong GetShapeMemSize(){return sizeof(*this);};

	//points
	virtual int GetPointNr() const; //获取坐标数量
	virtual int GetControlPointNr(); //可设点坐标数量
	virtual bool GetRotationControlPoints(pxPoint& pointAnchor, pxPoint& pointC);
	virtual bool GetPoint(int point_index, pxPoint& point) const; //获取坐标
	virtual void SetPoint(int point_index, const pxPoint& point); //设置坐标

	//persistence
	virtual int Write( pxShapeIOStream* stream );
	virtual int Read( pxShapeIOStream* stream );

	//GDI
	virtual void Draw(pxCanvas* canvas);

	//helpers
	virtual void GetBoundsRect(pxPoint& leftup, pxPoint& rightbottom);
	virtual void GetBoundsRect(pxPoint point[4]);
	virtual double GetRotationAngle(); //计算旋转角
	virtual void SetRect(const pxRect& rect); //设置坐标

	//control test
	virtual pxShapeBase* HitTest( const pxPoint& point, pxShapeBase* prev_shape );
	virtual pxTransformMark ControlPointTransformMarkTest(const pxPoint& point); //测试坐标点能否成为变换控制点

	virtual void Flip(const pxPoint& refPoint, bool flip_horz, bool flip_vert);

protected:

	//make sure this one is the first member data
	pxPoint m_otherPoints[3];

	bool m_flipped;
};

#endif // pxShapeRect_h__
