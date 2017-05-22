#include "precomp.h"
#include "pxShapeLineSec.h"

pxShapeLineSec::pxShapeLineSec()
{
}

pxShapeLineSec::~pxShapeLineSec()
{

}

void pxShapeLineSec::Draw( pxCanvas* canvas )
{
	PrepareDraw(canvas);

	canvas->DrawLine(m_Point.m_x, m_Point.m_y,
		m_PointEnd.m_x, m_PointEnd.m_y);	

	EndDraw(canvas);
}

pxShapeType pxShapeLineSec::GetShapeType()
{
	return pxShapeType_LineSec;
}

pxShapeBase* pxShapeLineSec::HitTest( const pxPoint& point, pxShapeBase*  )
{
	if(px2DGeometry::PointNearLine(point, GetPointsPtr()))
		return this;

	return 0;
}

void pxShapeLineSec::DrawControlPointMark( pxCanvas* canvas )
{
	DrawSelectLine(canvas, m_Point.m_x, m_Point.m_y, m_PointEnd.m_x, m_PointEnd.m_y);

	pxShapeBase::DrawSquareMark(canvas, m_Point);
	pxShapeBase::DrawSquareMark(canvas, m_PointEnd);
}

pxTransformMark pxShapeLineSec::TransformMarkTest( const pxPoint& point )
{
	pxTransformMark mark = pxTransformMark_None;
	if(pxShapeBase::HitTest(point, 0))
	{
		if(px2DGeometry::PointNearPoint(point, m_Point))
			mark = pxTransformMark_MovePoint_0;
		else if(px2DGeometry::PointNearPoint(point, m_PointEnd))
			mark = (pxTransformMark)(pxTransformMark_MovePoint_0 + 1);
		else if(px2DGeometry::PointNearLine(point, GetPointsPtr()))
			mark = pxTransformMark_Move;
	}

	return mark;
}

int pxShapeLineSec::Write( pxShapeIOStream* stream )
{
	int len = 0;
	int ret = 0;

	len = pxShapeBase::Write(stream);

	if(len >= 0)
	{
		CA_WRITE_RETURN(stream, m_PointEnd.m_x, len, ret);
		CA_WRITE_RETURN(stream, m_PointEnd.m_y, len, ret);
	}

	return len;
}

int pxShapeLineSec::Read( pxShapeIOStream* stream )
{
	int len = 0;
	int ret = 0;

	len = pxShapeBase::Read(stream);

	if(len >= 0)
	{
		CA_READ_RETURN(stream, m_PointEnd.m_x, len, ret);
		CA_READ_RETURN(stream, m_PointEnd.m_y, len, ret);
	}

	return len;
}

void pxShapeLineSec::SetPoint( int point_index, const pxPoint& point )
{
	point_index = point_index % 2;

	pxShapeBase::SetPoint(point_index, point);
}

bool pxShapeLineSec::GetEndPoint( pxPoint point[2] )
{
	point[0] = m_Point;
	point[1] = m_PointEnd;

	return true;
}

pxTransformMark pxShapeLineSec::ControlPointTransformMarkTest( const pxPoint& point )
{
	pxTransformMark mark = TransformMarkTest(point);
	if(mark < pxTransformMark_MovePoint_0)
		mark = pxTransformMark_None;

	return mark;
}

void pxShapeLineSec::SetContrlPoint( int point_index, const pxPoint& point )
{
	SetPoint(point_index, point);
}

bool pxShapeLineSec::GetRotationControlPoints( pxPoint& pointAnchor, pxPoint& pointC )
{
	pointAnchor;
	pointC;

	return false;
}
