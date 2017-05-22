// pxCanvasWx2D.cpp: implementation of the pxCanvasWx2D class.
//
//////////////////////////////////////////////////////////////////////
#include "precomp.h"
#include "pxCanvasWx2D.h"
#include "px2DGeometry.h"

pxCanvasWx2D::pxCanvasWx2D(wxDC* dc)
{
	m_pDC = dc;
	m_enableTransform = 1;
	
	m_scale.m_x = 1.0;
	m_scale.m_y = 1.0;
	m_offset.m_x = 0;
	m_offset.m_y = 0;
	m_rotation = 0;

	m_nDrawConnectedLines = false;
}

pxCanvasWx2D::~pxCanvasWx2D()
{
	delete m_pDC;
}

void pxCanvasWx2D::DrawLine( pxCoord x1, pxCoord y1, pxCoord x2, pxCoord y2 )
{
	TransformPoint(x1, y1);
	TransformPoint(x2, y2);

	m_pDC->DrawLine((wxCoord)x1, (wxCoord)y1, (wxCoord)x2, (wxCoord)y2);
}

void pxCanvasWx2D::DrawPolyLine( pxPoint* points, int points_nr )
{
	if(points_nr > 1)
	{
		int n;

		for(n = 0; n < points_nr-1; n ++)
		{
			DrawLine(points[n].m_x, points[n].m_y, points[n+1].m_x, points[n+1].m_y);
		}
	}
}

void pxCanvasWx2D::DrawPolygon( pxPoint* points, int points_nr )
{
	int n;
	
	pxPoint pt;
	wxPoint* wxpt = 0;

	if(points_nr > 1)
	{
		wxpt = new wxPoint[points_nr];
		
		for(n = 0; n < points_nr; n ++)
		{
			pt = points[n];
			TransformPoint(&pt);

			wxpt[n].x = (wxCoord)(pt.m_x+0.5);
			wxpt[n].y = (wxCoord)(pt.m_y+0.5);
		}

		m_pDC->DrawPolygon(points_nr, wxpt);

		delete[] wxpt;
	}
}

void pxCanvasWx2D::DrawEllipse( pxCoord x1, pxCoord y1, pxCoord x2, pxCoord y2, double rotate )
{
	rotate;//不支持旋转

	TransformPoint(x1, y1);
	TransformPoint(x2, y2);

	m_pDC->DrawEllipse(x1, y1, x2-x1, y2-y1);
/*
	double a = (x2 - x1) / 2;
	double b = (y2 - y1) / 2;
	double aa = a * a;
	double bb = b * b;

	pxPoint c; //中心点
	c.m_x = x1 + a;
	c.m_y = y1 + b;

	int n;
	const int count = 100;

	wxPoint* p = new wxPoint[count*2];

	if(a < b)
	{//由x求y
		double deltax = a / (count-1) * 2;
		for(n = 0; n < count; n ++)
		{
			pxPoint point;
			point.m_x = -a + deltax * n;
			point.m_y = bb - point.m_x*point.m_x*bb/aa;
			if(point.m_y < 0)
				point.m_y = 0;
			point.m_y = -sqrt(point.m_y);

			p[n].x = (wxCoord)(point.m_x + c.m_x + 0.5);
			p[n].y = (wxCoord)(point.m_y + c.m_y + 0.5);

			point.m_y = -point.m_y;
	
			p[count*2 - n - 1].x = (wxCoord)(point.m_x + c.m_x + 0.5);
			p[count*2 - n - 1].y = (wxCoord)(point.m_y + c.m_y + 0.5);
		}
	}
	else
	{//由y求x
		double deltay = b / (count-1) * 2;
		for(n = 0; n < count; n ++)
		{
			pxPoint point;
			point.m_y = -b + deltay * n;

			point.m_x = aa - point.m_y*point.m_y*aa/bb;
			if(point.m_x < 0)
				point.m_x = 0;
			point.m_x = -sqrt(point.m_x);

			p[n].x = (wxCoord)(point.m_x + c.m_x + 0.5);
			p[n].y = (wxCoord)(point.m_y + c.m_y + 0.5);

			point.m_x = -point.m_x;

			p[count*2 - n - 1].x = (wxCoord)(point.m_x + c.m_x + 0.5);
			p[count*2 - n - 1].y = (wxCoord)(point.m_y + c.m_y + 0.5);
		}
	}

	m_pDC->DrawPolygon(count*2, p);

	delete[] p;
*/
}

bool pxCanvasWx2D::Blit( pxCoord xdest, pxCoord ydest, pxCoord width, pxCoord height, 
						pxCanvas* source, pxCoord xsrc, pxCoord ysrc, int logicalFunc, 
						bool useMask, pxCoord xsrcMask, pxCoord ysrcMask )
{
	return m_pDC->Blit(xdest, ydest, width, height, ((pxCanvasWx2D*)source)->m_pDC, xsrc, ysrc,
		(wxRasterOperationMode)logicalFunc, useMask, xsrcMask, ysrcMask);
}

void pxCanvasWx2D::SetTransform( const pxPoint& offset, const pxPoint& scale )
{
	m_offset = offset;
	m_scale.m_x = scale.m_x;
	m_scale.m_y = scale.m_y;
}

void pxCanvasWx2D::GetTransform( pxPoint& offset, pxPoint& scale )
{
	offset = m_offset;
	scale = m_scale;
}

void pxCanvasWx2D::TransformPoint( pxPoint* point )
{
	TransformPoint(point->m_x, point->m_y);
}

void pxCanvasWx2D::TransformPoint( pxCoord& x, pxCoord& y )
{
	if(m_enableTransform)
	{
		x = x + m_offset.m_x;
		y = y + m_offset.m_y;
		
		x = x * m_scale.m_x;
		y = y * m_scale.m_y;	
	}
}

void pxCanvasWx2D::SetFont( const pxFont& font, const pxColour& color )
{
	m_pDC->SetTextForeground(ConvertpxColour(color));
	m_pDC->SetFont(font);
}

void pxCanvasWx2D::SetFont( const pxTextStyle& text_style )
{
	pxCoord font_size = text_style.text_font.font_size;
	font_size = font_size * min(m_scale.m_x, m_scale.m_y);

	wxFont font(font_size,
		(wxFontFamily)text_style.text_font.family,
		(wxFontStyle)text_style.text_font.font_style,
		(wxFontWeight)text_style.text_font.font_weight,
		text_style.text_font.font_underline != 0,
		text_style.text_font.font_face_name, 
		(wxFontEncoding)(text_style.text_font.font_encoding));
 
	SetFont(font, text_style.text_color);
}

void pxCanvasWx2D::SetBrush( const pxBrush& brush )
{
	m_pDC->SetBrush(brush);
}

void pxCanvasWx2D::SetBrush( const pxFillStyle& style )
{
	SetBrush(wxNullBrush);
	m_Brush.FreeResource(true);

	if(style.pattern != wxBRUSHSTYLE_TRANSPARENT)
	{
		wxColour clr(style.color.red, 
		style.color.green,
		style.color.blue,
		style.color.alpha);

		m_Brush = wxBrush(clr, (wxBrushStyle)style.pattern);
	
		SetBrush(m_Brush);
	}
}

void pxCanvasWx2D::SetPen( const pxPen& pen )
{
	m_pDC->SetPen(pen);
}

void pxCanvasWx2D::SetPen( const pxLineStyle& style )
{
	SetPen((pxPen&)wxNullPen);
	m_Pen.FreeResource(true);

	if(style.style != wxPENSTYLE_TRANSPARENT && style.width != 0)
	{
		wxColour clr(style.color.red, 
			style.color.green,
			style.color.blue,
			style.color.alpha);

		m_Pen = wxPen(clr, (int)style.width, (wxPenStyle)style.style);

		SetPen((pxPen&)m_Pen);
	}
}

pxSize pxCanvasWx2D::GetTextExtent( const char* sz )
{
	wxCoord width,height;
	m_pDC->GetTextExtent(sz, &width, &height);

	pxSize size;
	size.x = (pxCoord)width;
	size.y = (pxCoord)height;

	return size;
}

void pxCanvasWx2D::GetTextExtent( const char* sz, pxCoord* width, pxCoord* height )
{
	pxSize size = GetTextExtent(sz);
	*width = (wxCoord)size.x;
	*height = (wxCoord)size.y;
}

void pxCanvasWx2D::DrawRectangle( pxCoord x1, pxCoord y1, pxCoord x2, pxCoord y2 )
{
	TransformPoint(x1, y1);
	TransformPoint(x2, y2);

	m_pDC->DrawRectangle(x1, y1, x2-x1, y2-y1);
}

void pxCanvasWx2D::DrawText( const char* sz, pxCoord x1, pxCoord y1, double rotation, bool flipped )
{
	flipped;

	TransformPoint(x1, y1);

	m_pDC->DrawRotatedText(sz, x1, y1, -rotation/PI*180);
}

void pxCanvasWx2D::DrawEllipticArc( const pxPoint& leftup, const pxPoint& rightbuttom, 
								   double angle_start, double angle_end, double rotate )
{
	rotate;//不支持旋转

	pxPoint lu = leftup;
	pxPoint rb = rightbuttom;

	TransformPoint(&lu);
	TransformPoint(&rb);

	m_pDC->DrawEllipticArc(lu.m_x, lu.m_y, rb.m_x - lu.m_x, rb.m_y - lu.m_y, angle_start, angle_end);
}

void pxCanvasWx2D::DrawBitmap( const pxBitmap& bitmap, pxCoord x1, pxCoord y1, double rotate, bool flipped)
{
	rotate;	
	flipped;

	TransformPoint(x1, y1);

	if(bitmap.IsOk())
		m_pDC->DrawBitmap(bitmap, x1, y1);
}

pxSize pxCanvasWx2D::GetSize()
{
	return m_pDC->GetSize();
}

void pxCanvasWx2D::DrawBezierArc( pxPoint points[4] )
{
	const int devide_count = 40;
	const double delta_t = 1.0 / devide_count;

	wxPoint wxp[2];
	wxPoint xp;

	int n;
	double t = 0;
	for(n = 0; n < devide_count + 1; n ++)
	{
		pxPoint p = px2DGeometry::Bezier(points, 4, t);

		TransformPoint(&p);
		xp.x = p.m_x;
		xp.y = p.m_y;

		if(n > 0)
		{
			wxp[1] = xp;
			m_pDC->DrawLine(wxp[0], wxp[1]);
			wxp[0] = xp;
		}
		else
			wxp[0] = xp;

		t += delta_t;
	}
}

void pxCanvasWx2D::BeginDrawConnectedLines()
{
	m_nDrawConnectedLines = 1;
}

void pxCanvasWx2D::EndDrawConnectedLines(bool pathClosed)
{
	pathClosed;
	m_nDrawConnectedLines = 0;
}

void pxCanvasWx2D::SetRotation( double angle )
{
	m_rotation = angle;
}

wxColour pxCanvasWx2D::ConvertpxColour( const pxColour& color )
{
	return wxColour(color.red, color.green, color.blue, color.alpha);
}
