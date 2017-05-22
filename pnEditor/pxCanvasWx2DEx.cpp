#include "precomp.h"
#include "pxCanvasWx2DEx.h"
#include "px2DGeometry.h"
#include "wx/dcprint.h"
#include "wx/dcmemory.h"

pxCanvasWx2DEx::pxCanvasWx2DEx(wxDC* dc) : pxCanvasWx2D(dc)
{
	m_gc = 0;

	wxMemoryDC* mdc = wxDynamicCast(dc, wxMemoryDC);
	if(mdc)
	{
		m_gc = wxGraphicsContext::Create(*mdc);
	}
	
	wxWindowDC* wdc = wxDynamicCast(dc, wxWindowDC);
	if(wdc)
	{
		m_gc = wxGraphicsContext::Create(*wdc);
	}

	wxPrinterDC* pdc = wxDynamicCast(dc, wxPrinterDC);
	if(pdc)
	{
		m_gc = wxGraphicsContext::Create(*pdc);
	}

	if(m_gc)
	{
		m_gc->SetAntialiasMode(wxANTIALIAS_DEFAULT);
		m_gc->SetInterpolationQuality(wxINTERPOLATION_FAST);

		m_GraphicsPath = wxNullGraphicsPath;
	}
}

pxCanvasWx2DEx::~pxCanvasWx2DEx(void)
{
	delete m_gc;
}

void pxCanvasWx2DEx::SetFont( const pxFont& font, const pxColour& color )
{
	pxCanvasWx2D::SetFont(font, color);
	m_gc->SetFont(font, ConvertpxColour(color));
}

void pxCanvasWx2DEx::SetBrush( const pxBrush& brush )
{
	pxCanvasWx2D::SetBrush(brush);
	m_gc->SetBrush(brush);
}

void pxCanvasWx2DEx::SetPen( const pxPen& pen )
{
	pxCanvasWx2D::SetPen(pen);
	m_gc->SetPen(pen);
}

void pxCanvasWx2DEx::DrawLine( pxCoord x1, pxCoord y1, pxCoord x2, pxCoord y2 )
{
	TransformPoint(x1, y1);
	TransformPoint(x2, y2);

	wxGraphicsPath& path = GetGraphicsPath();

	if(m_nDrawConnectedLines < 3)
		path.MoveToPoint((wxDouble)x1, (wxDouble)y1);
	
	path.AddLineToPoint((wxDouble)x2, (wxDouble)y2);
	
	if(m_nDrawConnectedLines == 0)
		m_gc->StrokePath(path);

	FreeGraphicsPath(path);
}

void pxCanvasWx2DEx::DrawPolyLine( pxPoint* points, int points_nr )
{
	int n;

	if(points_nr > 1)
	{
		pxPoint pt;

		wxGraphicsPath& path = GetGraphicsPath();

		pt = points[0];
		TransformPoint(&pt);

		if(m_nDrawConnectedLines < 3)
			path.MoveToPoint(pt.m_x, pt.m_y);

		for(n = 1; n < points_nr; n ++)
		{
			pt = points[n];
			TransformPoint(&pt);
			path.AddLineToPoint(pt.m_x, pt.m_y);
		}

		if(m_nDrawConnectedLines == 0)
			m_gc->StrokePath(path);
	
		FreeGraphicsPath(path);
	}
}

void pxCanvasWx2DEx::DrawPolygon( pxPoint* points, int points_nr )
{
	int n;

	if(points_nr > 1)
	{
		pxPoint pt;

		wxGraphicsPath path = m_gc->CreatePath();

#if 0 //test LinearGradientBrush
		wxGraphicsGradientStops stops(wxColour(255,0,0), wxColour(0,255,0));
		wxGraphicsBrush br = m_gc->CreateLinearGradientBrush(points[0].m_x,points[0].m_y,
		points[2].m_x,points[2].m_y,stops);
		m_gc->SetBrush(br);
#endif

		pt = points[0];
		TransformPoint(&pt);
		path.MoveToPoint(pt.m_x, pt.m_y);

		for(n = 1; n < points_nr; n ++)
		{
			pt = points[n];
			TransformPoint(&pt);
			path.AddLineToPoint(pt.m_x, pt.m_y);
		}

		pt = points[0];
		TransformPoint(&pt);
		path.AddLineToPoint(pt.m_x, pt.m_y);

		m_gc->DrawPath(path);
	}
}

bool pxCanvasWx2DEx::Blit( pxCoord xdest, pxCoord ydest, pxCoord width, pxCoord height, pxCanvas* source, 
		pxCoord xsrc, pxCoord ysrc, int logicalFunc, bool useMask, pxCoord xsrcMask, pxCoord ysrcMask )
{
	bool ret = pxCanvasWx2D::Blit(xdest, ydest, width, height, source, xsrc, ysrc, logicalFunc, useMask, xsrcMask, ysrcMask);

	return ret;
}

void pxCanvasWx2DEx::DrawEllipticArc( const pxPoint& leftup, const pxPoint& rightbuttom, 
									 double angle_start, double angle_end, double rotate)
{
	pxPoint lu = leftup;
	pxPoint rb = rightbuttom;

	TransformPoint(&lu);
	TransformPoint(&rb);

	pxPoint c;
	c.m_x = (lu.m_x + rb.m_x) / 2;
	c.m_y = (lu.m_y + rb.m_y) / 2;

	//把外接矩形转成水平
	px2DGeometry::RotatePoint(lu, -rotate, rb);
	double a = (rb.m_x - lu.m_x) / 2;
	double b = (rb.m_y - lu.m_y) / 2;

	if(a > COORD_EPSILON && b > COORD_EPSILON)
	{
		wxGraphicsPath& path = GetGraphicsPath();

		wxGraphicsMatrix mt = m_gc->CreateMatrix();
		mt.Translate(c.m_x, c.m_y);
		mt.Rotate(rotate);
		mt.Scale(1, b/a);
		path.AddArc(0, 0, a, angle_start, angle_end, true);
		path.Transform(mt);

		if(m_nDrawConnectedLines == 0)
			m_gc->StrokePath(path);

		FreeGraphicsPath(path);
	}
}

void pxCanvasWx2DEx::DrawEllipse( pxCoord x1, pxCoord y1, pxCoord x2, pxCoord y2, double rotate )
{
	TransformPoint(x1, y1);
	TransformPoint(x2, y2);

	x2 -= x1;
	y2 -= y1;

	wxGraphicsPath path = m_gc->CreatePath();

	//以x1,y1为原点，旋转rotate的椭圆
	m_gc->PushState();
	m_gc->Translate(x1,y1);
	m_gc->Rotate(rotate);

	pxPoint point(x2,y2);
	px2DGeometry::RotatePoint(pxPoint(0,0), -rotate, point);

#if 0 //test RadialGradientBrush
	wxGraphicsGradientStops stops(wxColour(255,255,255), wxColour(0,0,0));
	wxGraphicsBrush br = m_gc->CreateRadialGradientBrush(point.m_x/4,point.m_y/2,
		point.m_x/2, point.m_y/2,max(point.m_x,point.m_y)/2,stops);
	m_gc->SetBrush(br);
#endif

	path.AddEllipse(0, 0, point.m_x, point.m_y);

	m_gc->DrawPath(path);

	m_gc->PopState();
}

void pxCanvasWx2DEx::DrawText( const char* sz, pxCoord x1, pxCoord y1, double rotation, bool flipped )
{
	TransformPoint(x1, y1);
	
	double xscale = 1;

	if(flipped)
	{
		rotation = PI-rotation;
		xscale = -xscale;
	}

	m_gc->PushState();
	m_gc->Translate(x1,y1);
	m_gc->Scale(xscale,1);
	m_gc->Rotate(rotation);
	m_gc->DrawText(wxString(sz), 0, 0);
	m_gc->PopState();
}

void pxCanvasWx2DEx::DrawBezierArc( pxPoint points[4] )
{
	pxPoint p[4];
	int n;
	for(n = 0; n < 4; n ++)
	{
		p[n] = points[n];
		TransformPoint(&p[n]);
	}

	wxGraphicsPath& path = GetGraphicsPath();

	if(m_nDrawConnectedLines < 3)
		path.MoveToPoint(p[0]);
	
	path.AddCurveToPoint(p[1], p[2], p[3]);

	if(m_nDrawConnectedLines == 0)
		m_gc->StrokePath(path);

	FreeGraphicsPath(path);
}

void pxCanvasWx2DEx::BeginDrawConnectedLines()
{
	pxCanvasWx2D::BeginDrawConnectedLines();
	
	m_GraphicsPath = m_gc->CreatePath();
}

void pxCanvasWx2DEx::EndDrawConnectedLines(bool pathClosed)
{
	pxCanvasWx2D::EndDrawConnectedLines(pathClosed);

	if(pathClosed)
		m_gc->DrawPath(m_GraphicsPath);
	else
		m_gc->StrokePath(m_GraphicsPath);

	m_GraphicsPath = wxNullGraphicsPath;
}

wxGraphicsPath& pxCanvasWx2DEx::GetGraphicsPath()
{
	if(m_nDrawConnectedLines == 0)
		m_GraphicsPath = m_gc->CreatePath();
	else if(m_nDrawConnectedLines == 1)
	{
		m_nDrawConnectedLines = 2;
	}
	else
	{
		m_nDrawConnectedLines = 3;
	}

	return m_GraphicsPath;
}

void pxCanvasWx2DEx::FreeGraphicsPath( wxGraphicsPath& path )
{
	if(m_nDrawConnectedLines == 0)
		path = wxNullGraphicsPath;
}

void pxCanvasWx2DEx::DrawBitmap( const pxBitmap& bitmap, pxCoord x1, pxCoord y1, double rotation, bool flipped)
{
	double xscale = 1;

	if(flipped)
	{
		rotation = PI-rotation;
		xscale = -xscale;
	}

	TransformPoint(x1, y1);
	
	m_gc->PushState();
	m_gc->Translate(x1, y1);
	m_gc->Scale(xscale, 1);
	m_gc->Rotate(rotation);
	m_gc->DrawBitmap(bitmap, 0, 0, bitmap.GetWidth(), bitmap.GetHeight());
	m_gc->PopState();
}
