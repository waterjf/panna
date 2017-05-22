// pxCanvasWx2D.h: interface for the pxCanvasWx2D class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PXCANVASWX2D_H__6786F721_CEA3_40E1_A94D_44E661A02978__INCLUDED_)
#define AFX_PXCANVASWX2D_H__6786F721_CEA3_40E1_A94D_44E661A02978__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "pxCanvas.h"
 
class pxCanvasWx2D : public pxCanvas 
{
public:
	pxCanvasWx2D(wxDC* dc);
	virtual ~pxCanvasWx2D();

public:
	//设置坐标变换
	virtual void SetTransform(const pxPoint& offset, const pxPoint& scale);
	virtual void GetTransform(pxPoint& offset, pxPoint& scale);
	virtual void SetRotation(double angle);//顺时针旋转

public:
	virtual pxSize GetTextExtent(const char* sz);
	virtual void GetTextExtent(const char* sz, pxCoord* width, pxCoord* height);

	//GDI属性设置
	virtual void SetFont(const pxFont& font, const pxColour& color);
	virtual void SetBrush(const pxBrush& brush);
	virtual void SetPen(const pxPen& pen);

	virtual void SetFont(const pxTextStyle& text_style);
	virtual void SetBrush(const pxFillStyle& fill_style);
	virtual void SetPen(const pxLineStyle& line_style);

	//几何图形绘制
	virtual void BeginDrawConnectedLines();
	virtual void EndDrawConnectedLines(bool pathClosed);
	virtual void DrawLine(pxCoord x1, pxCoord y1, pxCoord x2, pxCoord y2);
	virtual void DrawEllipticArc(const pxPoint& leftup, const pxPoint& rightbuttom, double angle_start, double angle_end, double rotate);
	virtual void DrawRectangle(pxCoord x1, pxCoord y1, pxCoord x2, pxCoord y2);
	virtual void DrawPolyLine( pxPoint* points, int points_nr );
	virtual void DrawPolygon(pxPoint* points, int points_nr);
	virtual void DrawEllipse(pxCoord x1, pxCoord y1, pxCoord x2, pxCoord y2, double rotate);
	virtual void DrawText(const char* sz, pxCoord x1, pxCoord y1, double rotation, bool flipped);
	virtual void DrawBitmap(const pxBitmap& bitmap, pxCoord x1, pxCoord y1, double rotate, bool flipped);
	virtual void DrawBezierArc(pxPoint points[4]);
	virtual bool Blit(pxCoord xdest,
		pxCoord 	ydest,
		pxCoord 	width,
		pxCoord 	height,
		pxCanvas* 	source,
		pxCoord 	xsrc,
		pxCoord 	ysrc,
		int		 	logicalFunc,
		bool 		useMask,
		pxCoord 	xsrcMask,
		pxCoord 	ysrcMask);

	virtual pxSize GetSize();

protected:
	void TransformPoint(pxPoint* point);
	void TransformPoint(pxCoord& x, pxCoord& y);
	wxColour ConvertpxColour(const pxColour& color );

protected:
	wxDC* m_pDC;

	int m_enableTransform;

	pxPoint m_offset;// 坐标变换的原点
	pxPoint m_scale;// 缩放
	double m_rotation;

	int m_nDrawConnectedLines;
};

#endif // !defined(AFX_PXCANVASWX2D_H__6786F721_CEA3_40E1_A94D_44E661A02978__INCLUDED_)
