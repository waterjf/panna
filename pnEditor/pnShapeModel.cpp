#include "precomp.h"
#include "pnShapeModel.h"
#include "pxShapeFactory.h"
#include "pxShapeBase.h"

pnShapeModel::pnShapeModel(void)
{
	m_pShapeView = 0;
}

pnShapeModel::~pnShapeModel(void)
{
}

int pnShapeModel::Load( pxShapeIOStream* stream )
{
	ClearPool();
	return Read(stream, m_ShapeList);
}

int pnShapeModel::Save( pxShapeIOStream* stream )
{
	return pxShapePool::Write(stream, m_ShapeList);
}

pxShapeBase* pnShapeModel::HitestShape( const pxPoint& point, pxShapeBase* prev_shape )
{
	shape_node pos = GetStartPostion();
	pxShapeBase* shape = 0;
	pxShapeBase* fisrt_shape = 0;
	pxShapeBase* select_shape = 0;

	bool start_test = true;
	if(prev_shape && prev_shape->HitTest(point, 0))
		start_test = false;

	select_shape = 0;

	while(GetPrevShape(pos, shape))
	{
		//找到第一个选中图元
		if(!fisrt_shape)
		{
			pxShapeBase* sel_shape = shape->HitTest(point, prev_shape);
			if(sel_shape)
				fisrt_shape = sel_shape;
		}

		//从prev_shape开始找
		if(!start_test)
		{
			if(shape == prev_shape)
				start_test = true;

			continue;
		}

		if(shape->HitTest(point, prev_shape))
		{
			select_shape = shape;
			break;
		}
	}

	if(!select_shape)
		select_shape = fisrt_shape;

	return select_shape;
}

pnShapeView* pnShapeModel::GetShapeView() const
{
	return m_pShapeView;
}

void pnShapeModel::SetShapeView( pnShapeView* val )
{
	m_pShapeView = val;
}
