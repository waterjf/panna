#include "precomp.h"
#include "pxShapeBezier.h"
#include "pxCanvasWx2D.h"

pxShapeBezier::pxShapeBezier(void)
{
}

pxShapeBezier::~pxShapeBezier(void)
{
}

pxShapeType pxShapeBezier::GetShapeType()
{
	return pxShapeType_BezierArc;
}

void pxShapeBezier::Draw( pxCanvas* canvas )
{
	PrepareDraw(canvas);

#if DRAW_MYSELF
	pxPoint* p = new pxPoint[bezier_devide_count+1];

	double t = 0;

	int n;
	for(n = 0; n < bezier_devide_count+1; n++)
	{
		p[n] = Bezier(t);
		t += delta_t;
	}

	canvas->DrawPolyLine(p, bezier_devide_count+1);

	delete[] p;
#else
	canvas->DrawBezierArc(GetPointsPtr());
#endif
}

pxShapeBase* pxShapeBezier::HitTest( const pxPoint& point, pxShapeBase*  )
{
	if(pxShapeBase::HitTest(point, 0))
	{
		if(px2DGeometry::PointNearBezier(point, GetPointsPtr(), GetPointNr()))
			return this;
	}

	return 0;
}

void pxShapeBezier::DrawSelectedMark( pxCanvas* canvas, bool with_corner_mark /*= true*/, bool with_rotation_mark /*= true*/ )
{
	pxShapeRect::DrawSelectedMark(canvas, false, with_rotation_mark);

	if(with_corner_mark)
	{
		pxPoint* p = GetPointsPtr();

		DrawSquareMark(canvas, p[0]);
		DrawSquareMark(canvas, p[GetPointNr() - 1]);
	}
}

void pxShapeBezier::DrawControlPointMark( pxCanvas* canvas )
{
	int n;
	pxPoint* p = GetPointsPtr();
	for(n = 0; n < GetPointNr(); n ++)
	{
		pxShapeBase::DrawSquareMark(canvas, p[n]);
	}
}

pxTransformMark pxShapeBezier::TransformMarkTest( const pxPoint& point )
{
	pxTransformMark mark = pxShapeRect::TransformMarkTest(point);
	if(mark != pxTransformMark_None)
	{
		pxPoint* p = GetPointsPtr();
		int count = GetPointNr();

		if(px2DGeometry::PointNearPoint(point, p[0]))
			mark = pxTransformMark_MovePoint_0;
		else if(px2DGeometry::PointNearPoint(point, p[count - 1]))
			mark = (pxTransformMark)(pxTransformMark_MovePoint_0 + count - 1);
	}

	return mark;
}

pxTransformMark pxShapeBezier::LinePointTransformMarkTest( const pxPoint& point )
{
	pxTransformMark mark = pxTransformMark_None;

	pxPoint* lp = GetPointsPtr();
	int n;
	for(n = 1; n < GetPointNr() - 1; n ++)
	{
		if(px2DGeometry::PointNearPoint(point, lp[n]))
		{
			mark = (pxTransformMark)(pxTransformMark_MovePoint_1 + n - 1);
			break;
		}
	}

	return mark;
}

bool pxShapeBezier::GetEndPoint( pxPoint point[2] )
{
	pxPoint* p = GetPointsPtr();

	point[0] = p[0];
	point[1] = p[GetPointNr() - 1];

	return true;
}

void pxShapeBezier::SetContrlPoint( int point_index, const pxPoint& point )
{
	SetPoint(point_index, point);
}

int pxShapeBezier::GetControlPointNr()
{
	return 4;
}
