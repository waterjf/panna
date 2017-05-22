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
	virtual int GetControlPointNr(); //�������������
	virtual void SetPoint( int point_index, const pxPoint& point );
	virtual bool GetEndPoint(pxPoint point[2]); //��ȡ�˵���ߣ�ֻ������ͼԪ�ж˵�
	virtual void SetContrlPoint(int point_index, const pxPoint& point); //�ƶ����Ƶ�

	//persistence
	virtual int Write( pxShapeIOStream* stream );
	virtual int Read( pxShapeIOStream* stream );

	//GDI
	virtual void Draw(pxCanvas* canvas);
	virtual void DrawSelectedMark(pxCanvas* canvas, bool with_corner_mark = true, bool with_rotation_mark = true); //������ѡʱ������
	virtual void DrawControlPointMark(pxCanvas* canvas);

	//control test
	virtual pxShapeBase* HitTest( const pxPoint& point, pxShapeBase* prev_shape );
	virtual pxTransformMark TransformMarkTest(const pxPoint& point); //����������ܷ��Ϊ�任���Ƶ�
	virtual pxTransformMark LinePointTransformMarkTest( const pxPoint& point );	

protected:
	void CalcArcPoints(pxPoint point[2]);

	double m_start_angle; //��ʼ�Ƕ�
	double m_end_angle; //��ֹ�Ƕ�
};

#endif // pxShapeArc_h__
