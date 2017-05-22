#ifndef pxShapeLineSec_h__
#define pxShapeLineSec_h__

#include "pxShapeBase.h"

class pxShapeLineSec : public pxShapeBase
{
public:
	pxShapeLineSec();
	virtual ~pxShapeLineSec();
	
public:
	//type
	virtual pxShapeType GetShapeType();
	virtual ulong GetShapeMemSize(){return sizeof(*this);};

	//points
	virtual bool GetRotationControlPoints(pxPoint& pointAnchor, pxPoint& pointC);
	virtual void SetPoint(int point_index, const pxPoint& point); //��������
	virtual bool GetEndPoint(pxPoint point[2]); //��ȡ�˵���ߣ�ֻ������ͼԪ�ж˵�
	virtual void SetContrlPoint(int point_index, const pxPoint& point); //�ƶ����Ƶ�

	//persistence
	virtual int Write( pxShapeIOStream* stream );
	virtual int Read( pxShapeIOStream* stream );

	//GDI
	virtual void Draw(pxCanvas* canvas);
	virtual void DrawControlPointMark(pxCanvas* canvas);

	//control test
	virtual pxShapeBase* HitTest( const pxPoint& point, pxShapeBase* prev_shape );
	virtual pxTransformMark TransformMarkTest( const pxPoint& point );	
	virtual pxTransformMark ControlPointTransformMarkTest( const pxPoint& point );	

protected:
	pxPoint m_PointEnd;
};

#endif // pxShapeLineSec_h__
