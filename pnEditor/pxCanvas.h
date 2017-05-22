// pxCanvas.h: interface for the pxCanvas class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PXCANVAS_H__2339C5E0_FDC4_481C_8692_5E5CD4CA3712__INCLUDED_)
#define AFX_PXCANVAS_H__2339C5E0_FDC4_481C_8692_5E5CD4CA3712__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "pxDef.h"
#include "pxAdapter.h"

class pxCanvas
{
public:
	pxCanvas();
	virtual ~pxCanvas();

public:
	//设置坐标变换
	virtual void SetTransform(const pxPoint& offset, const pxPoint& scale) = 0;
	virtual void GetTransform(pxPoint& offset, pxPoint& scale) = 0;
	virtual void SetRotation(double angle) = 0;//顺时针旋转

public:
	virtual pxSize GetTextExtent(const char* sz) = 0;
	
	//GDI属性设置
	virtual void SetFont(const pxFont& font, const pxColour& color) = 0;
	virtual void SetBrush(const pxBrush& brush) = 0;
	virtual void SetPen(const pxPen& pen) = 0;

	virtual void SetFont(const pxTextStyle& text_style) = 0;
	virtual void SetBrush(const pxFillStyle& fill_style) = 0;
	virtual void SetPen(const pxLineStyle& line_style) = 0;

	//几何图形绘制
	virtual void BeginDrawConnectedLines() = 0;
	virtual void EndDrawConnectedLines(bool pathClosed) = 0;
	virtual void DrawLine(pxCoord x1, pxCoord y1, pxCoord x2, pxCoord y2) = 0;
	virtual void DrawEllipticArc(const pxPoint& leftup, const pxPoint& rightbuttom, double angle_start, double angle_end, double rotate) = 0;
	virtual void DrawRectangle(pxCoord x1, pxCoord y1, pxCoord x2, pxCoord y2) = 0;
	virtual void DrawPolyLine(pxPoint* points, int points_nr) = 0;
	virtual void DrawPolygon(pxPoint* points, int points_nr) = 0;
	virtual void DrawEllipse(pxCoord x1, pxCoord y1, pxCoord x2, pxCoord y2, double rotate) = 0;
	virtual void DrawText(const char* sz, pxCoord x1, pxCoord y1, double rotation, bool flipped) = 0;
	virtual void DrawBitmap(const pxBitmap& bitmap, pxCoord x1, pxCoord y1, double rotate, bool flipped) = 0;
	virtual void DrawBezierArc(pxPoint points[4]) = 0;
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
			pxCoord 	ysrcMask) = 0;

	virtual pxSize GetSize() = 0;

protected:
	pxPen m_Pen;
	pxBrush m_Brush;
	pxFont m_Font;
};

#endif // !defined(AFX_PXCANVAS_H__2339C5E0_FDC4_481C_8692_5E5CD4CA3712__INCLUDED_)
