#include "precomp.h"
#include "pnShapeEditorModel.h"
#include "pxShapeBase.h"

pnShapeEditorModel::pnShapeEditorModel(void)
{
}

pnShapeEditorModel::~pnShapeEditorModel(void)
{
}

void pnShapeEditorModel::SelectShape( shape_list& shapes, const pxPoint& leftup, const pxPoint& rightbottom)
{
	shape_node pos = GetStartPostion();
	pxShapeBase* shape = 0;
	while(GetPrevShape(pos, shape))
	{
		if(shape->RectSelectTest(leftup, rightbottom))
		{
			shapes.push_front(shape);
		}
	}
}

void pnShapeEditorModel::TranslateShapes( const shape_list& shapes, const pxPoint& offset )
{
	shape_iterator itor = shapes.begin();
	while(itor != shapes.end())
	{
		pxShapeBase* shape = *itor;
		shape->Translate(offset.m_x, offset.m_y);
		itor ++;
	}
}

void pnShapeEditorModel::ScaleShapes( const shape_list& shapes, const pxPoint& rotate_ref, double rotation,
									 const pxPoint& scale_ref, const pxPoint& scale )
{
	shape_iterator itor = shapes.begin();
	while(itor != shapes.end())
	{
		pxShapeBase* shape = *itor;
		shape->Scale(rotate_ref, rotation, scale_ref, scale.m_x, scale.m_y);
		itor ++;
	}
}

void pnShapeEditorModel::RotateShapes( const shape_list& shapes, const pxPoint& refPoint, double angle )
{
	shape_iterator itor = shapes.begin();
	while(itor != shapes.end())
	{
		pxShapeBase* shape = *itor;
		shape->Rotate(refPoint, angle);
		itor ++;
	}
}

void pnShapeEditorModel::GetShapesBoundRect( const shape_list& shapes, pxPoint& leftup, pxPoint& rightbottom, int enlarge )
{
	leftup = pxPoint(DBL_MAX, DBL_MAX);
	rightbottom = pxPoint(-DBL_MAX, -DBL_MAX);

	if(shapes.GetCount() > 0)
	{
		shape_iterator itor = shapes.begin();
		while( itor != shapes.end() )
		{
			pxPoint lu, rb;
			pxShapeBase* shape = *itor;
			shape->GetBoundsRect(lu, rb);

			if(leftup.m_x > lu.m_x)
				leftup.m_x = lu.m_x;
			if(leftup.m_y > lu.m_y)
				leftup.m_y = lu.m_y;
			if(rightbottom.m_x < rb.m_x)
				rightbottom.m_x = rb.m_x;
			if(rightbottom.m_y < rb.m_y)
				rightbottom.m_y = rb.m_y;

			itor ++;
		}

		leftup.m_x -= enlarge;
		leftup.m_y -= enlarge;
		rightbottom.m_x += enlarge;
		rightbottom.m_y += enlarge;
	}
}

void pnShapeEditorModel::GetShapesBoundRect( pxPoint& leftup, pxPoint& rightbottom, int enlarge )
{
	GetShapesBoundRect(m_ShapeList, leftup, rightbottom, enlarge);
}
