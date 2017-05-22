#ifndef pxShapeGroup_h__
#define pxShapeGroup_h__

#include "pxShapeRect.h"
#include "pxShapePool.h"

class pxShapeGroup :
	public pxShapeRect
{
public:
	pxShapeGroup(void);
	virtual ~pxShapeGroup(void);

public:
	//type
	virtual pxShapeType GetShapeType();
	virtual ulong GetShapeMemSize(){return sizeof(*this);};

	//persistence
	virtual int Write( pxShapeIOStream* stream );
	virtual int Read( pxShapeIOStream* stream );

	//GDI
	virtual void Draw( pxCanvas* canvas );

	//transform
	virtual void Translate(pxCoord x, pxCoord y); //平移
	virtual void Scale(const pxPoint& rotate_ref, double rotation,
		const pxPoint& scale_ref, double scale_x, double scale_y); //缩放
	virtual void Rotate(const pxPoint& refPoint, double angle); //旋转
	virtual void Flip(const pxPoint& refPoint, bool flip_horz, bool flip_vert);

public:
	void SetShapePool(pxShapePool* pool);
	virtual void Assemble(const shape_list& from_shapes); //组合
	virtual void Dissemble(shape_list& dismiss_to); //分解
	bool RemoveSubShape( pxShapeBase* shape );
	
protected:
	void CreateBoundRect();
	void ClearGroup();

	pxShapePool* m_ShapePool;
	shape_list m_ShapeList;
};

#endif // pxShapeGroup_h__
