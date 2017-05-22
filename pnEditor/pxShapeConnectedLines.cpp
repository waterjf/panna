#include "precomp.h"
#include "pxShapeConnectedLines.h"

pxShapeConnectedLines::pxShapeConnectedLines(void)
{
}


pxShapeConnectedLines::~pxShapeConnectedLines(void)
{

}

pxShapeType pxShapeConnectedLines::GetShapeType()
{
	return pxShapeType_ConnectedLines;
}

void pxShapeConnectedLines::Draw( pxCanvas* canvas )
{
	if(m_ShapeList.GetCount() > 0)
	{
		shape_iterator itor = m_ShapeList.begin();
		canvas->BeginDrawConnectedLines();

		while(itor != m_ShapeList.end())
		{
			pxShapeBase* shape = *itor;
			shape->Draw(canvas);

			itor++;
		}

		canvas->EndDrawConnectedLines(PathClosed());
	}
}

pxShapeBase* pxShapeConnectedLines::HitTest( const pxPoint& point, pxShapeBase* )
{
	pxShapeBase* ret = 0;
	if(pxShapeRect::HitTest(point, 0))
		ret = this;

	return ret;
}

void pxShapeConnectedLines::DrawControlPointMark( pxCanvas* canvas )
{
	shape_iterator itor = m_ShapeList.begin();

	while(itor != m_ShapeList.end())
	{
		pxShapeBase* shape = *itor;
		shape->DrawControlPointMark(canvas);

		itor++;
	}
}

bool pxShapeConnectedLines::GetEndPoint( pxPoint point[2] )
{
	if(m_ShapeList.GetCount() > 0)
	{
		pxShapeBase* shape;
		shape_iterator itor;
		
		itor = m_ShapeList.begin();
		shape = *itor;
		shape->GetPoint(0, point[0]);

		itor = --m_ShapeList.end();
		shape = *itor;
		shape->GetPoint(shape->GetPointNr() - 1, point[1]);

		if(px2DGeometry::PointDistance(point[1], point[0]) > COORD_EPSILON)//首尾不相连
			return true;
	}

	return false;
}

void pxShapeConnectedLines::AddLine( pxShapeBase* shape, int end_point_index )
{
	ReCreateBoundRect(shape);

	pxPoint line_endPoint[2];
	pxPoint cl_endPoint[2];
	shape->GetEndPoint(line_endPoint);
	GetEndPoint(cl_endPoint);

	int line_cp_count = shape->GetControlPointNr();

	if(end_point_index == 0)
	{
		if(px2DGeometry::PointNearPoint(line_endPoint[0], cl_endPoint[1]))
			shape->SetContrlPoint(0, cl_endPoint[1]);//闭合

		m_ShapeList.push_front(shape);
	}
	else
	{
		if(px2DGeometry::PointNearPoint(line_endPoint[line_cp_count-1], cl_endPoint[0]))
			shape->SetContrlPoint(line_cp_count-1, cl_endPoint[0]);//闭合

		m_ShapeList.push_back(shape);
	}

	//线型填充一致
	shape->CloneStyle(*m_ShapeList.begin());
}

pxTransformMark pxShapeConnectedLines::ControlPointTransformMarkTest( const pxPoint& point )
{
	pxTransformMark mark = pxTransformMark_None;

	shape_iterator itor = m_ShapeList.begin();
	pxTransformMark tmp = pxTransformMark_None;

	while(itor != m_ShapeList.end())
	{
		pxShapeBase* shape = *itor;
		pxTransformMark sub_mark = shape->ControlPointTransformMarkTest(point);
		if(sub_mark != pxTransformMark_None)
		{
			mark = pxTransformMark(tmp + sub_mark);
			break;
		}

		tmp = pxTransformMark(tmp + shape->GetControlPointNr() - 1);
		itor++;
	}

	return mark;
}

int pxShapeConnectedLines::GetControlPointNr()
{
	int count = 0;

	shape_iterator itor = m_ShapeList.begin();
	while(itor != m_ShapeList.end())
	{
		pxShapeBase* shape = *itor;
		count += shape->GetControlPointNr() - 1;

		itor++;
	}

	if(m_ShapeList.GetCount() > 1)
	{
		if(!PathClosed())
			count++;
	}

	return count;
}

bool pxShapeConnectedLines::PathClosed()
{
	if(m_ShapeList.GetCount() > 0)
	{
		pxPoint point[2];

		pxShapeBase* shape;
		shape_iterator itor;

		itor = m_ShapeList.begin();
		shape = *itor;
		shape->GetPoint(0, point[0]);

		itor = --m_ShapeList.end();
		shape = *itor;
		shape->GetPoint(shape->GetPointNr() - 1, point[1]);

		if(px2DGeometry::PointDistance(point[1], point[0]) < COORD_EPSILON)//首尾不相连
			return true;
	}

	return false;
}

void pxShapeConnectedLines::SetContrlPoint( int point_index, const pxPoint& point )
{
	int count = 0;

	shape_iterator itor;
	pxShapeBase* shape;

	if(point_index == 0)
	{
		if(PathClosed())
		{
			itor = --m_ShapeList.end();
			shape = *itor;
			shape->SetContrlPoint(shape->GetControlPointNr() - 1, point);
		}
	}

	itor = m_ShapeList.begin();
	while(itor != m_ShapeList.end())
	{
		shape = *itor;
		count = shape->GetControlPointNr();
		if(count > point_index)
		{
			shape->SetContrlPoint(point_index, point);
			if(point_index == count - 1)
			{//下一个图元的第一个点也跟着调
				itor++;
				shape = *itor;
				if(shape)
					shape->SetContrlPoint(0, point);
			}
			break;
		}

		point_index = point_index - count + 1;
		itor ++;
	}
}

void pxShapeConnectedLines::ReCreateBoundRect(pxShapeBase* new_shape)
{
	if(m_ShapeList.GetCount() > 0)
	{
		pxPoint leftup(DBL_MAX, DBL_MAX);
		pxPoint rightbottom(-DBL_MAX, -DBL_MAX);

		pxPoint point[4];

		double rotation = GetRotationAngle();
		pxPoint refPoint;
		GetPoint(0, refPoint);

		//计算已有图元水平后的外边框
		shape_iterator itor = m_ShapeList.begin();
		while(itor != m_ShapeList.end())
		{
			pxShapeBase* shape = *itor;

			shape->GetBoundsRect(point);
			px2DGeometry::RotateRect(refPoint, -rotation, point);
			px2DGeometry::ConcateRect(leftup, rightbottom, point);

			itor ++;
		}

		//把新图元的边框接上
		new_shape->GetBoundsRect(point);
		px2DGeometry::RotateRect(refPoint, -rotation, point);
		px2DGeometry::ConcateRect(leftup, rightbottom, point);

		//再转成原来的角度
		px2DGeometry::Init4PointRect(leftup, rightbottom, point);
		px2DGeometry::RotateRect(refPoint, rotation, point);

		SetPoint(0, point[0]);
		SetPoint(1, point[1]);
		SetPoint(2, point[2]);
		SetPoint(3, point[3]);
	}
}
