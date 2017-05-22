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
	virtual bool GetEndPoint(pxPoint point[2]); //��ȡ�˵���ߣ�ֻ������ͼԪ�ж˵�
	virtual int GetControlPointNr(); //�������������
	virtual void SetContrlPoint(int point_index, const pxPoint& point); //�ƶ����Ƶ�

	//GDI
	virtual void Draw(pxCanvas* canvas);
	virtual void DrawSelectedMark(pxCanvas* canvas, bool with_corner_mark = true, bool with_rotation_mark = true);
	virtual void DrawControlPointMark( pxCanvas* canvas );

	//control test
	virtual pxShapeBase* HitTest( const pxPoint& point, pxShapeBase* prev_shape );
	virtual pxTransformMark TransformMarkTest( const pxPoint& point ); //����������ܷ��Ϊ�任���Ƶ�
	virtual pxTransformMark LinePointTransformMarkTest( const pxPoint& point );	

protected:
	pxPoint Bezier(const pxPoint& start, const pxPoint& end, double t);
	pxPoint Bezier(double t);
};

#endif // pxShapeBezier_h__
