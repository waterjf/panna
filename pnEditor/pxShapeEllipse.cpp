// pxShapeEllipse.cpp: implementation of the pxShapeEllipse class.
//
//////////////////////////////////////////////////////////////////////
#include "precomp.h"
#include "pxShapeEllipse.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

pxShapeEllipse::pxShapeEllipse()
{
}

pxShapeEllipse::~pxShapeEllipse()
{

}

void pxShapeEllipse::Draw( pxCanvas* canvas )
{
	PrepareDraw(canvas);

	pxPoint* lp = GetPointsPtr();


	canvas->DrawEllipse(lp[0].m_x, lp[0].m_y, lp[2].m_x, lp[2].m_y, GetRotationAngle());

	EndDraw(canvas);
}

pxShapeType pxShapeEllipse::GetShapeType()
{
	return pxShapeType_Ellipse;
}

pxShapeBase* pxShapeEllipse::HitTest( const pxPoint& pt, pxShapeBase* prev_shape )
{
	pxPoint point = pt;

	if(pxShapeBase::HitTest(point, prev_shape))//快速过滤
	{
		pxPoint* p = GetPointsPtr();
		
		pxPoint lu = p[0];
		pxPoint rb = p[2];
	
		double rotation = GetRotationAngle();

		//转成水平矩形
		px2DGeometry::RotatePoint(lu, -rotation, rb);
		px2DGeometry::RotatePoint(lu, -rotation, point);

		double a = (rb.m_x - lu.m_x) / 2;
		double b = (rb.m_y - lu.m_y) / 2;
	
		pxPoint c;
		c.m_x = lu.m_x + a;
		c.m_y = lu.m_y + b;

		if(a<0) a=-a;
		if(b<0) b=-b;

		//外扩一点
		a = a + px2DGeometry::GetTolerance();
		b = b + px2DGeometry::GetTolerance();

		//以中心点为原点
		point = point - c;

		double r = (point.m_x*point.m_x)/(a*a) + (point.m_y*point.m_y)/(b*b);
		if(r < 1)
		{
			if(m_FillStyle.pattern == wxBRUSHSTYLE_TRANSPARENT)
			{//空心椭圆
				a = a - px2DGeometry::GetTolerance()*2;
				b = b - px2DGeometry::GetTolerance()*2;
				if(a > COORD_EPSILON && b > COORD_EPSILON)
				{
					r = (point.m_x*point.m_x)/(a*a) + (point.m_y*point.m_y)/(b*b);
					if(r < 1)
						return 0;
				}
			}

			return this;
		}
	}

	return 0;
}
