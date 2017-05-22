#if !defined(_PXCANVASWX2DEX_H_)
#define _PXCANVASWX2DEX_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "pxCanvasWx2D.h"
#include "wx/graphics.h"

class pxCanvasWx2DEx :
	public pxCanvasWx2D
{
public:
	pxCanvasWx2DEx(wxDC* dc);
	virtual ~pxCanvasWx2DEx(void);

public:
	//GDI属性设置
	virtual void SetFont(const pxFont& font, const pxColour& color);
	virtual void SetBrush(const pxBrush& brush);
	virtual void SetPen(const pxPen& pen);

	//几何图形绘制
	virtual void BeginDrawConnectedLines();
	virtual void EndDrawConnectedLines(bool pathClosed);
	virtual void DrawLine(pxCoord x1, pxCoord y1, pxCoord x2, pxCoord y2);
	virtual void DrawEllipticArc(const pxPoint& leftup, const pxPoint& rightbuttom,
		double angle_start, double angle_end, double rotation);
	virtual void DrawPolyLine(pxPoint* points, int points_nr);
	virtual void DrawPolygon(pxPoint* points, int points_nr);
	virtual void DrawEllipse( pxCoord x1, pxCoord y1, pxCoord x2, pxCoord y2, double rotation );
	virtual void DrawText(const char* sz, pxCoord x1, pxCoord y1, double rotation, bool flipped);
	virtual void DrawBitmap(const pxBitmap& bitmap, pxCoord x1, pxCoord y1, double rotation, bool flipped);
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

protected:
	wxGraphicsPath m_GraphicsPath;
	
	wxGraphicsPath& GetGraphicsPath();
	void FreeGraphicsPath(wxGraphicsPath& path);

protected:
	wxGraphicsContext* m_gc;
};

#endif //_PXCANVASWX2DEX_H_
