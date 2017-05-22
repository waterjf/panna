// pxShapeBase.h: interface for the pxShapeBase class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PXSHAPEBASE_H__6E0DCBA7_7FAD_4351_AD75_2CF9D11412E5__INCLUDED_)
#define AFX_PXSHAPEBASE_H__6E0DCBA7_7FAD_4351_AD75_2CF9D11412E5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "pxDef.h"
#include "pxShapeIOStream.h"
#include "pxCanvas.h"
#include "px2DGeometry.h"
#include "pxShapePropery.h"

extern pxLineStyle g_default_line_style;
extern pxFillStyle g_default_fill_style;
extern pxFontInfo g_default_font_info;
extern pxTextStyle g_default_text_style;

class pxShapePool;
class pxShapeBase
{
public:
	pxShapeBase();
	virtual ~pxShapeBase();

public: 
	//type
	virtual pxShapeType GetShapeType() = 0;
	virtual ulong GetShapeMemSize() = 0;

	virtual void BuildPropertyCollection(pxShapeProperyCategory& PropertyCollection);
	virtual void QueryProperty(pxShapeProperyCategory& PropertyCollection);
	virtual void SetProperty(const pxShapeProperyCategory& PropertyCollection);

	//points
	virtual pxPoint* GetPointsPtr() const;
	virtual int GetPointNr() const; //获取坐标数量
	virtual int GetControlPointNr(); //可设点坐标数量
	virtual bool GetPoint(int point_index, pxPoint& point) const; //获取坐标
	virtual void SetPoint(int point_index, const pxPoint& point); //设置坐标
	virtual bool GetEndPoint(pxPoint point[2]); //获取端点左边，只有线型图元有端点
	virtual void SetContrlPoint(int point_index, const pxPoint& point); //移动控制点

	//persistence
	virtual int Write(pxShapeIOStream* stream);
	virtual int Read(pxShapeIOStream* stream);
	void CloneStyle(pxShapeBase* shape);
	void Clone(pxShapeBase* shape);

	//GDI
	virtual void Draw(pxCanvas* canvas) = 0;
	virtual void DrawSelectedMark(pxCanvas* canvas, bool with_corner_mark = true, bool with_rotation_mark = true); //画被单选时的轮廓
	virtual void DrawMultipleSelectedMark(pxCanvas* canvas, bool bMainTarget = false ); //画被多选时的轮廓
	virtual void DrawControlPointMark(pxCanvas* canvas); //画设点标记
	virtual void DrawRotationMark(pxCanvas* canvas); //画旋转标记

	//transform
	virtual void Translate(pxCoord x, pxCoord y); //平移
	virtual void Scale(const pxPoint& rotate_ref, double rotation, 
		const pxPoint& scale_ref, double scale_x, double scale_y); //缩放
	virtual void Rotate(const pxPoint& refPoint, double angle); //顺时针旋转
	virtual void Flip(const pxPoint& refPoint, bool flip_horz, bool flip_vert);

	//control test
	virtual pxShapeBase* HitTest(const pxPoint& point, pxShapeBase* prev_shape); //测试坐标点是否能选中当前图元
	virtual pxShapeBase* RectSelectTest(const pxPoint& leftup, const pxPoint& rightbottom);//测试矩形框是否能包含当前图元
	
	virtual pxTransformMark TransformMarkTest(const pxPoint& point); //测试坐标点能否成为变换控制点
	virtual pxTransformMark ControlPointTransformMarkTest(const pxPoint& point); //测试坐标点能否成为单点变换控制点

	//helpers
	virtual void GetBoundsRect(pxPoint& leftup, pxPoint& rightbottom);
	virtual void GetBoundsRect(pxPoint point[4]);
	virtual double GetRotationAngle(); //计算旋转角
	virtual bool GetRotationControlPoints(pxPoint& pointAnchor, pxPoint& pointC) = 0;
	virtual void SetRect(const pxRect& rect); //设置坐标

public:
	bool IsLineShape();
	void RevertEndPoints();//颠倒端点

	//style
	pxLineStyle LineStyle() const;
	void LineStyle(const pxLineStyle& val);

	pxFillStyle FillStyle() const;
	void FillStyle(const pxFillStyle& val);

	virtual pxTextStyle TextStyle() const;
	virtual void TextStyle(const pxTextStyle& val);

public:
	static ulong GetShapeMemSize(pxShapeType shape_type);

	static pxShapeBase* NewShape(pxShapeType shape_type);//使用new创建一个shape
	static void FreeShape(pxShapeBase* shape); //配合NewShape使用

	static void ConstructShape(pxShapeBase* shape, pxShapeType shape_type, pxShapePool* pool);
	static void DestructShape(pxShapeBase* shape);
	static bool IsLineShape(pxShapeType type);

	static void DrawSelectedMark(pxCanvas* canvas, pxPoint point[4], bool with_corner_mark = true, bool with_rotation_mark = true); //画被单选时的轮廓
	static void DrawSelectLine(pxCanvas* canvas, pxCoord x1, pxCoord y1, pxCoord x2, pxCoord y2);
	static void DrawSquareMark(pxCanvas* canvas, const pxPoint& point);
	static void DrawRotationMark(pxCanvas* canvas, pxPoint point[4]); //画旋转标记
	static void DrawRoundMark(pxCanvas* canvas, const pxPoint& point);

	static void DrawMultipleSelectedLine(pxCanvas* canvas, pxCoord x1, pxCoord y1, pxCoord x2, pxCoord y2, bool bMainTarget);
	static void GetRotationControlPoints(const pxPoint point[4], pxPoint& pointAnchor, pxPoint& pointC, bool flipped = false);

	static pxTransformMark G_TransformMarkTest(const pxPoint& point, const pxPoint& lu, const pxPoint& rb);
	static pxTransformMark G_TransformMarkTest(const pxPoint& point, const pxPoint rectPoint[4]);

protected:
	int WriteLineStyle(pxShapeIOStream* stream);
	int ReadLineStyle(pxShapeIOStream* stream);
	int WriteFillStyle(pxShapeIOStream* stream);
	int ReadFillStyle(pxShapeIOStream* stream);

	int WriteTextStyle(pxTextStyle* style, pxShapeIOStream* stream);
	int ReadTextStyle(pxTextStyle* style, pxShapeIOStream* stream);
	
	int WriteString(pxShapeIOStream* stream, const char* sz);
	int ReadString(pxShapeIOStream* stream, char* sz, int max_len);
	int ReadReallocString(pxShapeIOStream* stream, char** sz, int* len, int max_len);

	virtual void PrepareDraw(pxCanvas* canvas);
	virtual void EndDraw(pxCanvas* canvas);

private:
	void PrepareBrush(pxCanvas* canvas);
	void PreparePen(pxCanvas* canvas);

protected:
	pxLineStyle m_LineStyle;
	pxFillStyle m_FillStyle;

	//make sure this one is the last member;
	pxPoint m_Point;
};

#endif // !defined(AFX_PXSHAPEBASE_H__6E0DCBA7_7FAD_4351_AD75_2CF9D11412E5__INCLUDED_)
