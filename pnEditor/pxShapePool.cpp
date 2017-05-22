#include "precomp.h"

#include "pxShapeIOStream.h"
#include "pxShapePool.h"
#include "pxShapeBase.h"
#include "pxShapeGroup.h"

pxShapePool::pxShapePool(void)
{
	m_ShapePool = 0; 
}

void pxShapePool::Init()
{
	m_ShapePool = new flxPool[pxShapeType_COUNT];
	int n;
	for(n = pxShapeType_MIN; n <= pxShapeType_MAX; n ++)
		m_ShapePool[n-1].Init(pxShapeBase::GetShapeMemSize((pxShapeType)n)); 
}

void pxShapePool::Destroy()
{
	if(m_ShapePool)
	{
		ClearPool();

		delete[] m_ShapePool;
		m_ShapePool = 0;
	}
}

void pxShapePool::ClearPool()
{
	if(m_ShapePool)
	{
		shape_iterator itor = m_ShapeList.begin();
		while(itor != m_ShapeList.end())
		{
			FreeShape(*itor);
			itor ++;
		}
	}

	m_ShapeList.clear();
}

pxShapePool::~pxShapePool(void)
{
	Destroy();
}

int pxShapePool::Write( pxShapeIOStream* stream, const shape_list& list )
{
	int len = 0;
	int ret = 0;

	uint32 count = list.GetCount();
	CA_WRITE_RETURN(stream, count, len, ret);
	
	shape_iterator itor = list.begin();
	while(itor != list.end())
	{
		pxShapeBase* shape = *itor;
		uint16 shape_type = shape->GetShapeType();
		CA_WRITE_RETURN(stream, shape_type, len, ret);
		CA_RETURN(shape->Write(stream), len ,ret);

		itor ++;
	}

	return len;
}

int pxShapePool::Read( pxShapeIOStream* stream, shape_list& list)
{
	int len = 0;
	int ret = 0;

	uint32 count = 0;
	CA_READ_RETURN(stream, count, len, ret);

	uint n;
	for(n = 0; n < count; n ++)
	{
		pxShapeBase* shape = 0;
		uint16 shape_type;
		CA_READ_RETURN(stream, shape_type, len, ret);

		if(shape_type < pxShapeType_MIN || shape_type > pxShapeType_MAX)
			return -1;

		flxPool* pool = &m_ShapePool[shape_type-1];
		shape = (pxShapeBase*)pool->AllocElement();
		if(!shape)
			return -2;

		pxShapeBase::ConstructShape(shape, (pxShapeType)shape_type, this);

		CA_RETURN(shape->Read(stream), len, ret);

		list.push_back(shape);
	}

	return len;
}

void pxShapePool::RegisterShape( pxShapeBase* shape, pxShapeBase* before_which )
{
	shape_iterator ret = m_ShapeList.push_back(shape);

	if(before_which)
	{
		shape_iterator itor = m_ShapeList.begin();
		while(itor != m_ShapeList.end())
		{
			pxShapeBase* p = *itor;
			if(p == before_which)
			{
				m_ShapeList.link_before(ret, itor);
				break;
			}

			itor++;
		}
	}
}

void pxShapePool::RegisterShapes( const shape_list& shapes, pxShapeBase* before_which /*= 0*/ )
{
	shape_iterator itor = shapes.begin();
	while(itor != shapes.end())
	{
		pxShapeBase* p = *itor;
	
		RegisterShape(p, before_which);

		itor++;
	}
}

pxShapeBase* pxShapePool::FindBefore( pxShapeBase* shape )
{
	pxShapeBase* before_which = 0;

	shape_iterator itor = m_ShapeList.begin();
	while(itor != m_ShapeList.end())
	{
		pxShapeBase* p = *itor;

		if(p == shape)
		{
			itor ++;
			before_which = *itor;
			break;
		}

		itor ++;
	}

	return before_which;
}

pxShapeBase* pxShapePool::FindAfter( pxShapeBase* shape )
{
	pxShapeBase* after_which = 0;

	shape_iterator itor = m_ShapeList.begin();
	while(itor != m_ShapeList.end())
	{
		pxShapeBase* p = *itor;

		if(p == shape)
		{
			itor --;
			after_which = *itor;
			break;
		}

		itor ++;
	}

	return after_which;
}

pxShapeBase* pxShapePool::UnregisterShape( pxShapeBase* shape )
{
	pxShapeBase* before_which = 0;

	shape_iterator itor = m_ShapeList.begin();
	while(itor != m_ShapeList.end())
	{
		pxShapeBase* p = *itor;

		if(p == shape)
		{
			m_ShapeList.remove(itor);

			itor ++;
			before_which = *itor;
			break;
		}

		itor ++;
	}

	return before_which;
}

shape_node pxShapePool::FindShape(pxShapeBase* shape)
{
	shape_node node = 0;

	shape_iterator itor = m_ShapeList.begin();
	while(itor != m_ShapeList.end())
	{
		pxShapeBase* p = *itor;

		if(p == shape)
		{
			node = (shape_node)&*itor;
			break;
		}

		itor ++;
	}

	return node;
}

void pxShapePool::FreeShape( pxShapeBase* shape )
{
	flxPool* pool = &m_ShapePool[shape->GetShapeType()-1];
	pxShapeBase::DestructShape(shape);
	pool->RecycleElement((byte*)shape);
}

void pxShapePool::FreeShapes( const shape_list& shapes )
{
	shape_iterator itor = shapes.begin();
	while(itor != shapes.end())
	{
		FreeShape(*itor);
		itor++;
	}
}

void pxShapePool::UnregisterShapes( const shape_list& shapes )
{
	shape_iterator itor = shapes.begin();
	while(itor != shapes.end())
	{
		UnregisterShape(*itor);
		itor++;
	}
}

shape_node pxShapePool::GetStartPostion()
{
	shape_node pos = 0;

	return pos;
}

bool pxShapePool::GetNextShape( shape_node& pos, pxShapeBase*& shape )
{
	shape_iterator itor;

	if(!m_ShapeList.IsEmpty())
	{
		if(pos == 0)
			itor = m_ShapeList.begin();
		else
		{
			itor = (list_node<pxShapeBase*>*)pos;
			itor++;
		}

		pos = (shape_node)&*itor; 

		if(itor != m_ShapeList.end())
		{
			shape = *itor;

			return true;
		}
	}

	return false;
}

bool pxShapePool::GetPrevShape( shape_node& pos, pxShapeBase*& shape )
{
	shape_iterator itor;

	if(!m_ShapeList.IsEmpty())
	{
		if(pos == 0)
		{
			itor = m_ShapeList.tail();
		}
		else
		{
			itor = shape_iterator((list_node<pxShapeBase*>*)pos);
			itor--;
		}

		pos = (shape_node)&*itor; 

		if(itor != m_ShapeList.end())
		{
			shape = *itor;

			return true;
		}
	}

	return false;
}

pxShapeBase* pxShapePool::CreateShape( pxShapeType shape_type )
{
	pxShapeBase* shape = 0;
	if(shape_type >= pxShapeType_MIN && shape_type <= pxShapeType_MAX)
	{
		flxPool* pool = &m_ShapePool[shape_type-1];
		shape = (pxShapeBase*)pool->AllocElement();
		pxShapeBase::ConstructShape(shape, shape_type, this);
	}

	return shape;
}

pxShapeBase* pxShapePool::CloneShape( pxShapeBase* shape )
{
	pxShapeBase* cloned = CreateShape(shape->GetShapeType());
	cloned->Clone(shape);

	return cloned;
}
