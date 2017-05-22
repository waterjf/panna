// pxShapeArc.cpp: implementation of the pxShapeArc class.
//
//////////////////////////////////////////////////////////////////////
#include "precomp.h"
#include "pxShapeArc.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

pxShapeArc::pxShapeArc()
{
	m_start_angle = 0;
	m_end_angle = PI*3/2;
}

pxShapeArc::~pxShapeArc()
{

}

void pxShapeArc::Draw( pxCanvas* canvas )
{
	PrepareDraw(canvas);

	pxPoint* p = GetPointsPtr();

	canvas->DrawEllipticArc(p[0], p[2], m_start_angle, m_end_angle, GetRotationAngle());

	EndDraw(canvas);
}

pxShapeType pxShapeArc::GetShapeType()
{
	return pxShapeType_Arc;
}

pxShapeBase* pxShapeArc::HitTest( const pxPoint& pt, pxShapeBase* prev_shape )
{
	pxShapeBase* ret = 0;

	if(pxShapeEllipse::HitTest(pt, prev_shape))
	{//在椭圆上
		pxPoint* p = GetPointsPtr();

		pxPoint lu = p[0];
		pxPoint rb = p[2];
		pxPoint point = pt;

		double rotation = GetRotationAngle();

		//转成水平矩形
		px2DGeometry::RotatePoint(lu, -rotation, rb);
		px2DGeometry::RotatePoint(lu, -rotation, point);

		pxPoint c;
		c.m_x = (lu.m_x + rb.m_x) / 2;
		c.m_y = (lu.m_y + rb.m_y) / 2;

		if(!px2DGeometry::PointNearPoint(point, c))
		{//计算角度
			double angle = px2DGeometry::LineAngle(c, point);

			if(px2DGeometry::AngleBetween(angle, m_start_angle, m_end_angle))
				ret = this;
		}
		else
			ret = this;
	}

	return ret;
}

int pxShapeArc::Write( pxShapeIOStream* stream )
{
	int len = 0;
	int ret = 0;

	len = pxShapeEllipse::Write(stream);

	if(len >= 0)
	{
		CA_WRITE_RETURN(stream, m_start_angle, len, ret);
		CA_WRITE_RETURN(stream, m_end_angle, len, ret);
	}

	return len;
}

int pxShapeArc::Read( pxShapeIOStream* stream )
{
	int len = 0;
	int ret = 0;

	len = pxShapeEllipse::Read(stream);

	if(len >= 0)
	{
		CA_READ_RETURN(stream, m_start_angle, len, ret);
		CA_READ_RETURN(stream, m_end_angle, len, ret);
	}

	return len;
}

void pxShapeArc::DrawSelectedMark( pxCanvas* canvas, bool with_corner_mark /*= true*/, bool with_rotation_mark /*= true*/ )
{
	pxShapeEllipse::DrawSelectedMark(canvas, with_corner_mark, with_rotation_mark);

	pxPoint arcPoint[2];
	CalcArcPoints(arcPoint);

	pxShapeBase::DrawSquareMark(canvas, arcPoint[0]);
	pxShapeBase::DrawSquareMark(canvas, arcPoint[1]);
}

void pxShapeArc::DrawControlPointMark( pxCanvas* canvas )
{
	pxPoint arcPoint[2];
	CalcArcPoints(arcPoint);

	pxShapeBase::DrawSquareMark(canvas, arcPoint[0]);
	pxShapeBase::DrawSquareMark(canvas, arcPoint[1]);
}

void pxShapeArc::CalcArcPoints( pxPoint point[2] )
{
	double rotation = GetRotationAngle();
	pxPoint* p = GetPointsPtr();
	pxPoint lu = p[0];
	pxPoint rb = p[2];
	px2DGeometry::RotatePoint(lu, -rotation, rb);

	double a = (rb.m_x - lu.m_x) / 2;
	double b = (rb.m_y - lu.m_y) / 2;

	pxPoint c;
	c.m_x = lu.m_x + a;
	c.m_y = lu.m_y + b;

	//以y轴为基准，计算
	int n = 0;
	for(n = 0; n < 2; n ++)
	{
		double angle = m_start_angle;
		if(n > 0)
			angle = m_end_angle;

		double cosa = cos(angle);
		if(cosa < COORD_EPSILON && cosa > -COORD_EPSILON)
		{//角度为90°或270°
			point[n].m_x = 0;
			if(angle > PI)
				point[n].m_y = -b;
			else
				point[n].m_y = b;
		}
		else
		{
			//求以b为半径，角度为angle的圆上的点
			pxCoord x = cosa*b;
			pxCoord y = sin(angle)*b;

			//相应的在椭圆上的交点
			point[n].m_y = y;
			point[n].m_x = x * a / b;
		}

		point[n] += c;

		px2DGeometry::RotatePoint(lu, rotation, point[n]);
	}
}

pxTransformMark pxShapeArc::TransformMarkTest( const pxPoint& point )
{
	pxTransformMark mark = LinePointTransformMarkTest(point);
	if(mark == pxTransformMark_None)
		mark = pxShapeEllipse::TransformMarkTest(point);

	return mark;
}

pxTransformMark pxShapeArc::LinePointTransformMarkTest( const pxPoint& point )
{
	pxTransformMark mark = pxTransformMark_None;

	pxPoint arcPoint[2];
	CalcArcPoints(arcPoint);

	if(px2DGeometry::PointNearPoint(point, arcPoint[0]))
		mark = pxTransformMark_MovePoint_0;
	else if(px2DGeometry::PointNearPoint(point, arcPoint[1]))
		mark = pxTransformMark_MovePoint_1;

	return mark;
}

void pxShapeArc::SetPoint( int point_index, const pxPoint& point )
{
	if(point_index < GetPointNr())
	{
		pxShapeEllipse::SetPoint(point_index, point);
	}
}

bool pxShapeArc::GetEndPoint( pxPoint point[2] )
{
	CalcArcPoints(point);

	return true;
}

int pxShapeArc::GetControlPointNr()
{
	return 2;
}

void pxShapeArc::SetContrlPoint( int point_index, const pxPoint& point )
{
	if(point_index < 2)
	{
		double rotation = GetRotationAngle();
		pxPoint* p = GetPointsPtr();
		pxPoint lu = p[0];
		pxPoint rb = p[2];
		px2DGeometry::RotatePoint(lu, -rotation, rb);

		double a = (rb.m_x - lu.m_x) / 2;
		double b = (rb.m_y - lu.m_y) / 2;

		pxPoint c;
		c.m_x = lu.m_x + a;
		c.m_y = lu.m_y + b;

		pxPoint target = point;
		px2DGeometry::RotatePoint(lu, -rotation, target);

		if(b > COORD_EPSILON && !px2DGeometry::PointEqual(target, c))
		{
			pxPoint lp = target - c;
			double angle = 0;
			if(lp.m_x < COORD_EPSILON && lp.m_x > -COORD_EPSILON)
			{//90或270°
				if(lp.m_y > 0)
					angle = PI/2;
				else
					angle = PI*3/2;
			}
			else
			{
				angle = lp.m_y / lp.m_x * a / b;
				angle = atan(angle); //输 出: -pi/2~pi/2
				if(lp.m_x < 0)
					if(lp.m_y < 0)
						angle = PI+angle;
					else
						angle = PI+angle;
				else
					if(lp.m_y < 0)
						angle = PI*2+angle;
					else
						angle = angle;


				if(point_index == 0)
					m_start_angle = angle;
				else
					m_end_angle = angle;
			}
		}
	}
}
