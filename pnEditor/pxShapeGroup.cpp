#include "precomp.h"
#include "pxShapeGroup.h"

pxShapeGroup::pxShapeGroup(void)
{
	m_ShapePool = 0;
}

pxShapeGroup::~pxShapeGroup(void)
{
	ClearGroup();
	m_ShapePool = 0;
}

pxShapeType pxShapeGroup::GetShapeType()
{
	return pxShapeType_Group;
}

void pxShapeGroup::Draw( pxCanvas* canvas )
{
	shape_iterator itor = m_ShapeList.begin();
	while(itor != m_ShapeList.end())
	{
		pxShapeBase* shape = *itor;
		shape->Draw(canvas);

		itor++;
	}
}

int pxShapeGroup::Write( pxShapeIOStream* stream )
{
	int len = 0;
	int ret = 0;

	CA_RETURN(pxShapeRect::Write(stream), len, ret);
	CA_RETURN(m_ShapePool->Write(stream, m_ShapeList), len ,ret);

	return len;
}

int pxShapeGroup::Read( pxShapeIOStream* stream )
{
	int len = 0;
	int ret = 0;

	ClearGroup();

	CA_RETURN(pxShapeRect::Read(stream), len, ret);
	CA_RETURN(m_ShapePool->Read(stream, m_ShapeList), len ,ret);

	return len;
}

void pxShapeGroup::Translate( pxCoord x, pxCoord y )
{
	pxShapeRect::Translate(x, y);

	if(m_ShapePool)
	{
		shape_iterator itor = m_ShapeList.begin();
		while(itor != m_ShapeList.end())
		{
			pxShapeBase* shape = *itor;
			shape->Translate(x, y);
			itor ++;
		}
	}
}

void pxShapeGroup::Scale( const pxPoint& rotate_ref, double rotation,
						 const pxPoint& scale_ref, double scale_x, double scale_y )
{
	pxShapeRect::Scale(rotate_ref, rotation, scale_ref, scale_x, scale_y);

	if(m_ShapePool)
	{
		shape_iterator itor = m_ShapeList.begin();
		while(itor != m_ShapeList.end())
		{
			pxShapeBase* shape = *itor;
			shape->Scale(rotate_ref, rotation, scale_ref, scale_x, scale_y);

			itor++;
		}
	}
}

void pxShapeGroup::Rotate( const pxPoint& refPoint, double angle )
{
	pxShapeRect::Rotate(refPoint, angle);

	if(m_ShapePool)
	{
		shape_iterator itor = m_ShapeList.begin();
		while(itor != m_ShapeList.end())
		{
			pxShapeBase* shape = *itor;
			shape->Rotate(refPoint, angle);

			itor++;
		}
	}
}

void pxShapeGroup::Assemble( const shape_list& from_shapes )
{
	shape_iterator itor = from_shapes.begin();
	while(itor != from_shapes.end())
	{
		pxShapeBase* shape = *itor;
		m_ShapeList.push_back(shape);

		itor++;
	}

	CreateBoundRect();
}

void pxShapeGroup::Dissemble( shape_list& dismiss_to )
{
	shape_iterator itor = m_ShapeList.begin();
	while(itor != m_ShapeList.end())
	{
		dismiss_to.push_back(*itor);

		itor++;
	}

	m_ShapeList.clear();
}

void pxShapeGroup::SetShapePool( pxShapePool* pool )
{
	m_ShapePool = pool;
}

void pxShapeGroup::CreateBoundRect()
{
	pxPoint leftup(DBL_MAX, DBL_MAX);
	pxPoint rightbottom(-DBL_MAX, -DBL_MAX);

	shape_iterator itor = m_ShapeList.begin();
	while(itor != m_ShapeList.end())
	{
		pxShapeBase* shape = *itor;

		pxPoint lu,rb;
		shape->GetBoundsRect(lu,rb);
		px2DGeometry::ConcateRect(leftup, rightbottom, lu, rb);

		itor++;
	}

	SetPoint(0, leftup);
	SetPoint(1, pxPoint(leftup.m_x, rightbottom.m_y));
	SetPoint(2, rightbottom);
	SetPoint(3, pxPoint(rightbottom.m_x, leftup.m_y));
}

bool pxShapeGroup::RemoveSubShape( pxShapeBase* shape )
{
	shape_iterator itor = m_ShapeList.begin();
	while(itor != m_ShapeList.end())
	{
		if(*itor == shape)
		{
			m_ShapeList.remove(itor);
			return true;
		}

		itor ++;
	}

	return false;
}

void pxShapeGroup::ClearGroup()
{
	shape_iterator itor = m_ShapeList.begin();
	while(itor != m_ShapeList.end())
	{
		m_ShapePool->FreeShape(*itor);

		itor ++;
	}

	m_ShapeList.clear();
}

void pxShapeGroup::Flip( const pxPoint& refPoint, bool flip_horz, bool flip_vert )
{
	pxShapeRect::Flip(refPoint, flip_horz, flip_vert);

	if(m_ShapePool)
	{
		shape_iterator itor = m_ShapeList.begin();
		while(itor != m_ShapeList.end())
		{
			pxShapeBase* shape = *itor;
			shape->Flip(refPoint, flip_horz, flip_vert);

			itor++;
		}
	}
}
