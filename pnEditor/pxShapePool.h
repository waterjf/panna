#pragma once

#include "flxPool.h"

class pxShapeBase;
class pxShapeIOStream;

typedef flxList<pxShapeBase*> shape_list;
typedef iterator<pxShapeBase*> shape_iterator;
typedef list_node<pxShapeBase*>* shape_node;

class pxShapePool
{
public:
	pxShapePool(void);
	virtual ~pxShapePool(void);

	void Init();
	void Destroy();
	void ClearPool();

	pxShapeBase* CreateShape(pxShapeType shape_type); //分配一个shape
	virtual void FreeShape(pxShapeBase* shape);

	pxShapeBase* CloneShape(pxShapeBase* shape); //克隆一个shape

	void FreeShapes(const shape_list& shapes);

	void RegisterShape(pxShapeBase* shape, pxShapeBase* before_which = 0);
	void RegisterShapes(const shape_list& shapes, pxShapeBase* before_which = 0);

	pxShapeBase* FindBefore(pxShapeBase* shape); //返回在哪个图元前面
	pxShapeBase* FindAfter(pxShapeBase* shape); //返回在哪个图元前面
	pxShapeBase* UnregisterShape(pxShapeBase* shape); //返回在哪个图元前面
	void UnregisterShapes(const shape_list& shapes); //返回在哪个图元前面

	virtual int Write(pxShapeIOStream* stream, const shape_list& list );//write shape in the list
	virtual int Read(pxShapeIOStream* stream, shape_list& list ); //read shape, and add to list

	shape_node GetStartPostion();
	shape_node FindShape(pxShapeBase* shape);

	bool GetNextShape(shape_node& pos, pxShapeBase*& shape);
	bool GetPrevShape(shape_node& pos, pxShapeBase*& shape);

	uint GetShapeCount() { return m_ShapeList.GetCount(); };

protected:
	flxPool* m_ShapePool;
	shape_list m_ShapeList;
};
