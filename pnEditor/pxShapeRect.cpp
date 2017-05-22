#include "precomp.h"
#include "pxShapeRect.h"

pxShapeRect::pxShapeRect(void)
{
	m_flipped = false;
}

pxShapeRect::~pxShapeRect(void)
{
}

pxShapeType pxShapeRect::GetShapeType()
{
	return pxShapeType_Rect;
}

int pxShapeRect::Write( pxShapeIOStream* stream )
{
	int n;
	int len = 0;
	int ret = 0;

	len = pxShapeBase::Write(stream);

	if(len >= 0)
	{
		pxPoint* points = GetPointsPtr();
		for(n = 0 ; n < GetPointNr(); n ++)
		{
			CA_WRITE_RETURN(stream, points[n].m_x, len, ret);		
			CA_WRITE_RETURN(stream, points[n].m_y, len, ret);		
		}

		CA_WRITE_RETURN(stream, m_flipped, len, ret);		
	}

	return len;
}

int pxShapeRect::Read( pxShapeIOStream* stream )
{
	int n;
	int len = 0;
	int ret = 0;

	len = pxShapeBase::Read(stream);

	if(len >= 0)
	{
		for(n = 0 ; n < GetPointNr(); n ++)
		{
			CA_READ_RETURN(stream, GetPointsPtr()[n].m_x, len, ret);		
			CA_READ_RETURN(stream, GetPointsPtr()[n].m_y, len, ret);		
		}

		CA_READ_RETURN(stream, m_flipped, len, ret);		
	}

	return len;
}

void pxShapeRect::Draw( pxCanvas* canvas )
{
	PrepareDraw(canvas);

	canvas->DrawPolygon(GetPointsPtr(), GetPointNr());

	EndDraw(canvas);
}

int pxShapeRect::GetPointNr() const
{
	return 4;
}

void pxShapeRect::SetPoint( int point_index, const pxPoint& point )
{
	pxPoint* ptr = GetPointsPtr();
	point_index = point_index % GetPointNr();

	ptr[point_index] = point;
}

void pxShapeRect::SetRect( const pxRect& rect )
{
	pxPoint lu, rb;
	lu.m_x = rect.m_x;
	lu.m_y = rect.m_y;
	rb = lu;
	rb.m_x += rect.m_width;
	rb.m_y += rect.m_height;

	px2DGeometry::NormalizeRect(lu, rb);

	SetPoint(0, lu);
	SetPoint(1, pxPoint(lu.m_x, rb.m_y));
	SetPoint(2, rb);
	SetPoint(3, pxPoint(rb.m_x, lu.m_y));
}

pxTransformMark pxShapeRect::ControlPointTransformMarkTest( const pxPoint& point )
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

pxShapeBase* pxShapeRect::HitTest( const pxPoint& p, pxShapeBase* prev_shape)
{
	pxPoint point = p;

	if(pxShapeBase::HitTest(point, prev_shape))
	{
		pxPoint* pPoints = GetPointsPtr();
		pxPoint line[2]; //两条线段

		int n;
		int hit_count = 0;

		for(n = 0; n < GetPointNr(); n++)
		{
			line[0] = pPoints[n];

			if(n == GetPointNr() - 1)
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

bool pxShapeRect::GetPoint( int point_index, pxPoint& point ) const
{
	pxPoint* ptr = GetPointsPtr();
	if(point_index >= 0 && point_index < GetPointNr())
	{
		point = ptr[point_index];
		return true;
	}

	return false;
}

double pxShapeRect::GetRotationAngle()
{
	pxPoint* lp = GetPointsPtr();

	double rotation = px2DGeometry::LineAngle(lp[0], lp[3]);

	return rotation;
}

void pxShapeRect::GetBoundsRect( pxPoint& leftup, pxPoint& rightbottom )
{
	pxShapeBase::GetBoundsRect(leftup, rightbottom);
}

void pxShapeRect::GetBoundsRect( pxPoint point[4] )
{
	pxPoint* p = GetPointsPtr();

	point[0] = p[0];
	point[1] = p[1];
	point[2] = p[2];
	point[3] = p[3];
}

int pxShapeRect::GetControlPointNr()
{
	return 0;
}

void pxShapeRect::Flip( const pxPoint& refPoint, bool flip_horz, bool flip_vert )
{
	pxShapeBase::Flip(refPoint, flip_horz, flip_vert);

	m_flipped = m_flipped ^ flip_horz;
	m_flipped = m_flipped ^ flip_vert;
}

bool pxShapeRect::GetRotationControlPoints( pxPoint& pointAnchor, pxPoint& pointC )
{
	pxPoint point[4];
	GetBoundsRect(point);

	pxShapeBase::GetRotationControlPoints(point, pointAnchor, pointC, m_flipped);

	return true;
}
