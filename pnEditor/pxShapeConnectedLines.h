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
	virtual int GetControlPointNr(); //�������������
	virtual bool GetEndPoint(pxPoint point[2]); //��ȡ�˵���ߣ�ֻ������ͼԪ�ж˵�
	virtual void SetContrlPoint(int point_index, const pxPoint& point); //�ƶ����Ƶ�

	//type
	virtual pxShapeType GetShapeType();
	virtual ulong GetShapeMemSize(){return sizeof(*this);};

	//GDI
	virtual void Draw( pxCanvas* canvas );
	virtual void DrawControlPointMark( pxCanvas* canvas );

	//transform
	virtual pxShapeBase* HitTest( const pxPoint& point, pxShapeBase* prev_shape );
	virtual pxTransformMark ControlPointTransformMarkTest(const pxPoint& point); //����������ܷ��Ϊ����任���Ƶ�

	void AddLine(pxShapeBase* shape, int end_point_index);

protected:
	bool PathClosed();
	void ReCreateBoundRect(pxShapeBase* new_shape);

};

#endif // pxShapeConnectedLines_h__
