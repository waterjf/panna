// pxShapePolygon.cpp: implementation of the pxShapePolygon class.
//
//////////////////////////////////////////////////////////////////////
#include "precomp.h"
#include "pxShapePolygon.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

pxShapePolygon::pxShapePolygon()
{
	m_nPointNr = 0;
	m_pPolyPoints = 0;
}

pxShapePolygon::~pxShapePolygon()
{
	SetPointNumber(0);
}

void pxShapePolygon::Draw( pxCanvas* canvas )
{
	PrepareDraw(canvas);

	canvas->DrawPolygon(GetPointsPtr(), m_nPointNr);

	EndDraw(canvas);
}

pxShapeType pxShapePolygon::GetShapeType()
{
	return pxShapeType_Polygon;
}

pxPoint* pxShapePolygon::GetPointsPtr() const
{
	if(m_nPointNr <= 4)
		return (pxPoint*)&m_Point;

	return m_pPolyPoints;
}

void pxShapePolygon::SetPointNumber( int nr )
{
	m_nPointNr = nr;

	if(m_nPointNr > 4)
	{
		m_pPolyPoints = new pxPoint[m_nPointNr];
	}
	else
	{
		if(m_pPolyPoints)
		{
			delete[] m_pPolyPoints;
			m_pPolyPoints = 0;
		}
	}
}

bool pxShapePolygon::GetPoint( int point_index, pxPoint& point ) const
{
	pxPoint* ptr = GetPointsPtr();
	if(point_index >= 0 && point_index < m_nPointNr)
	{
		point = ptr[point_index];
		return true;
	}

	return false;
}

void pxShapePolygon::SetPoint( int point_index, const pxPoint& point )
{
	pxPoint* ptr = GetPointsPtr();
	point_index = point_index % m_nPointNr;

	ptr[point_index] = point;
}

void pxShapePolygon::SetRect( const pxRect& rect )
{
	SetPointNumber(4);

	pxPoint point;
	point.m_x = rect.m_x;
	point.m_y = rect.m_y;
	SetPoint(0, point);

	point.m_y += rect.m_height;
	SetPoint(1, point);

	point.m_x += rect.m_width;
	SetPoint(2, point);

	point.m_y -= rect.m_height;
	SetPoint(3, point);
}

pxShapeBase* pxShapePolygon::HitTest( const pxPoint& p, pxShapeBase* prev_shape)
{
	pxPoint point = p;

	if(pxShapeBase::HitTest(point, prev_shape))
	{
		pxPoint* pPoints = GetPointsPtr();
		pxPoint line[2]; //两条线段

		int n;
		int hit_count = 0;

		for(n = 0; n < m_nPointNr; n++)
		{
			line[0] = pPoints[n];
		
			if(n == m_nPointNr - 1)
				line[1] = pPoints[0];
			else
				line[1] = pPoints[n+1];

			if(px2DGeometry::PointNearLine(point, line))//靠近一条边可认为选中
				return this;

			if(m_FillStyle.pattern != wxBRUSHSTYLE_TRANSPARENT)
			{//实心多边形，进一步用射线法计算
				if( ((point.m_y - line[0].m_y) < COORD_EPSILON && (point.m_y - line[0].m_y) > -COORD_EPSILON) ||
					((point.m_y - line[1].m_y) < COORD_EPSILON && (point.m_y - line[1].m_y) > -COORD_EPSILON) )
				{
					//与端点在一水平线上, 略移一些
					point.m_y += COORD_EPSILON;
				
					if(px2DGeometry::PointLeftShotLine(point, line))
						hit_count ++;
	
					point.m_y -= COORD_EPSILON;
				}
				else
				{
					if(px2DGeometry::PointLeftShotLine(point, line))
						hit_count ++;
				}
			}
		}

		if((hit_count % 2) != 0)
			return this;
	}

	return 0;
}

void pxShapePolygon::GetBoundsRect( pxPoint& leftup, pxPoint& rightbottom )
{
	pxPoint* pPoints = GetPointsPtr();
	leftup = pPoints[0];
	rightbottom = leftup;

	int n;
	for(n = 1; n < m_nPointNr; n ++)
	{
		if(leftup.m_x > pPoints[n].m_x)
			leftup.m_x = pPoints[n].m_x;
		if(leftup.m_y > pPoints[n].m_y)
			leftup.m_y = pPoints[n].m_y;

		if(rightbottom.m_x < pPoints[n].m_x)
			rightbottom.m_x = pPoints[n].m_x;
		if(rightbottom.m_y < pPoints[n].m_y)
			rightbottom.m_y = pPoints[n].m_y;
	}
}

int pxShapePolygon::GetPointNr() const
{
	return m_nPointNr;
}

pxTransformMark pxShapePolygon::LinePointTransformMarkTest( const pxPoint& point )
{
	pxTransformMark mark =  pxTransformMark_None;
	int n;
	pxPoint* p = GetPointsPtr();

	for(n = 0; n < GetPointNr(); n ++)
	{
		if(px2DGeometry::PointNearPoint(point, p[n]))
		{
			mark = (pxTransformMark)(pxTransformMark_MovePoint_0 + n);
		}
	}

	return mark;
}

void pxShapePolygon::DrawSelectedMark( pxCanvas* canvas, bool with_corner_mark /*= true*/ )
{
	pxPoint leftup,rightbottom;
	GetBoundsRect(leftup, rightbottom);
	DrawSelectedBound(canvas, leftup, rightbottom);
	if(with_corner_mark)
		DrawMarkOnBound(canvas, leftup, rightbottom);
}

void pxShapePolygon::DrawLinePointMark( pxCanvas* canvas )
{
	int n;
	pxPoint* p = GetPointsPtr();

	for(n = 0; n < GetPointNr(); n ++)
	{
		pxShapeBase::DrawPointMark(canvas, p[n]);
	}
}

void pxShapePolygon::DrawMultipleSelectedMark( pxCanvas* canvas, bool bMainTarget /*= false */ )
{
	pxPoint lu, rb;
	GetBoundsRect(lu, rb);
	DrawMutipleSelectedBound(canvas, lu, rb, bMainTarget);
}
