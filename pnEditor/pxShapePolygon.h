// pxShapePolygon.h: interface for the pxShapePolygon class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PXSHAPEPOLYGON_H__9CB251F6_24B6_441C_8343_F643BE6E72CE__INCLUDED_)
#define AFX_PXSHAPEPOLYGON_H__9CB251F6_24B6_441C_8343_F643BE6E72CE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "pxShapeBase.h"

class pxShapePolygon : public pxShapeBase  
{
public:
	pxShapePolygon();
	virtual ~pxShapePolygon();

public: 
	//type
	virtual pxShapeType GetShapeType();
	virtual ulong GetShapeMemSize(){return sizeof(*this);};

	//persistence
	virtual int Write( pxShapeIOStream* stream );
	virtual int Read( pxShapeIOStream* stream );

	//GDI
	virtual void Draw(pxCanvas* canvas);
	virtual void DrawSelectedMark(pxCanvas* canvas, bool with_corner_mark = true); //画被单选时的轮廓
	virtual void DrawMultipleSelectedMark(pxCanvas* canvas, bool bMainTarget = false ); //画被多选时的轮廓
	virtual void DrawLinePointMark(pxCanvas* canvas);

	//transform
	virtual int GetPointNr() const; //获取坐标数量
	virtual bool GetPoint( int point_index, pxPoint& point ) const ;
	virtual void SetPoint( int point_index, const pxPoint& point );
	virtual void SetRect( const pxRect& rect );
	virtual pxPoint* GetPointsPtr() const;

	virtual void GetBoundsRect(pxPoint& leftup, pxPoint& rightbottom);

	virtual pxShapeBase* HitTest( const pxPoint& point, pxShapeBase* prev_shape );
	virtual pxTransformMark LinePointTransformMarkTest(const pxPoint& point); //测试坐标点能否成为变换控制点

protected:
	void SetPointNumber(int nr);

protected:
	pxPoint m_pFixedPoints[3];

	int m_nPointNr;
	pxPoint* m_pPolyPoints;
};

#endif // !defined(AFX_PXSHAPEPOLYGON_H__9CB251F6_24B6_441C_8343_F643BE6E72CE__INCLUDED_)
