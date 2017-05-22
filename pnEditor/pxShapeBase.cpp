// pxShapeBase.cpp: implementation of the pxShapeBase class.
//
//////////////////////////////////////////////////////////////////////
#include "precomp.h"
#include "pxShapeBase.h"
#include "pxShapeLineSec.h"
#include "pxShapeRect.h"
#include "pxShapePolygon.h"
#include "pxShapeText.h"
#include "pxShapeEllipse.h"
#include "pxShapeArc.h"
#include "pxShapeBezier.h"
#include "pxShapeImg.h"
#include "pxShapeAnimator.h"
#include "pxShapeGroup.h"
#include "pxMemIOStream.h"
#include "pxShapeConnectedLines.h"

pxLineStyle g_default_line_style = {
	wxPENSTYLE_SOLID,
	1,
	0,
	0,
	0,
	0,
	0,
	{0,0,0,wxALPHA_OPAQUE,},
};

pxFillStyle g_default_fill_style = {
	wxBRUSHSTYLE_SOLID,
	{255,255,255,wxALPHA_OPAQUE,},
	{255,255,255,wxALPHA_OPAQUE,},
};

pxFontInfo g_default_font_info = {
	wxFONTFAMILY_DEFAULT,
	wxFONTSTYLE_NORMAL,
	wxFONTWEIGHT_NORMAL,
	0,
	wxFONTENCODING_DEFAULT,
	10,
	"宋体",
};

pxTextStyle g_default_text_style = {
	g_default_font_info,
	{0,0,0,wxALPHA_OPAQUE,},
};

//////////////////////////////////////////////////////////////
pxShapeBase::pxShapeBase()
{
}

pxShapeBase::~pxShapeBase()
{
}

int pxShapeBase::Write( pxShapeIOStream* stream )
{
	int len = 0;
	int ret = 0;

	CA_RETURN(WriteLineStyle(stream), len, ret);
	CA_RETURN(WriteFillStyle(stream), len, ret);

	CA_WRITE_RETURN(stream, m_Point.m_x, len, ret);
	CA_WRITE_RETURN(stream, m_Point.m_y, len, ret);

	return len;
}

int pxShapeBase::Read( pxShapeIOStream* stream )
{
	int len = 0;
	int ret = 0;

	CA_RETURN(ReadLineStyle(stream), len, ret);
	CA_RETURN(ReadFillStyle(stream), len, ret);

	CA_READ_RETURN(stream, m_Point.m_x, len, ret);
	CA_READ_RETURN(stream, m_Point.m_y, len, ret);
	
	return len;
}

int pxShapeBase::WriteLineStyle( pxShapeIOStream* stream )
{
	int len = 0;
	int ret = 0;

	CA_WRITE_RETURN(stream, m_LineStyle.style, len, ret);
	CA_WRITE_RETURN(stream, m_LineStyle.width, len, ret);
	CA_WRITE_RETURN(stream, m_LineStyle.terminal_style, len, ret);

	CA_WRITE_RETURN(stream, m_LineStyle.start_style, len, ret);
	CA_WRITE_RETURN(stream, m_LineStyle.end_style, len, ret);
	CA_WRITE_RETURN(stream, m_LineStyle.start_size, len, ret);
	CA_WRITE_RETURN(stream, m_LineStyle.end_size, len, ret);

	CA_WRITE_RETURN(stream, m_LineStyle.color.red, len, ret);
	CA_WRITE_RETURN(stream, m_LineStyle.color.green, len, ret);
	CA_WRITE_RETURN(stream, m_LineStyle.color.blue, len, ret);	
	CA_WRITE_RETURN(stream, m_LineStyle.color.alpha, len, ret);
	
	return len;
}

int pxShapeBase::ReadLineStyle( pxShapeIOStream* stream )
{
	int len = 0;
	int ret = 0;
	
	CA_READ_RETURN(stream, m_LineStyle.style, len, ret);
	CA_READ_RETURN(stream, m_LineStyle.width, len, ret);
	CA_READ_RETURN(stream, m_LineStyle.terminal_style, len, ret);
	
	CA_READ_RETURN(stream, m_LineStyle.start_style, len, ret);
	CA_READ_RETURN(stream, m_LineStyle.end_style, len, ret);
	CA_READ_RETURN(stream, m_LineStyle.start_size, len, ret);
	CA_READ_RETURN(stream, m_LineStyle.end_size, len, ret);
	
	CA_READ_RETURN(stream, m_LineStyle.color.red, len, ret);
	CA_READ_RETURN(stream, m_LineStyle.color.green, len, ret);
	CA_READ_RETURN(stream, m_LineStyle.color.blue, len, ret);	
	CA_READ_RETURN(stream, m_LineStyle.color.alpha, len, ret);
	
	return len;	
}

int pxShapeBase::WriteFillStyle( pxShapeIOStream* stream )
{
	int len = 0;
	int ret = 0;

	CA_WRITE_RETURN(stream, m_FillStyle.pattern, len, ret);

	CA_WRITE_RETURN(stream, m_FillStyle.pattern_color.red, len, ret);
	CA_WRITE_RETURN(stream, m_FillStyle.pattern_color.green, len, ret);
	CA_WRITE_RETURN(stream, m_FillStyle.pattern_color.blue, len, ret);	
	CA_WRITE_RETURN(stream, m_FillStyle.pattern_color.alpha, len, ret);
	
	CA_WRITE_RETURN(stream, m_FillStyle.color.red, len, ret);
	CA_WRITE_RETURN(stream, m_FillStyle.color.green, len, ret);
	CA_WRITE_RETURN(stream, m_FillStyle.color.blue, len, ret);	
	CA_WRITE_RETURN(stream, m_FillStyle.color.alpha, len, ret);
	
	return len;
}

int pxShapeBase::ReadFillStyle( pxShapeIOStream* stream )
{
	int len = 0;
	int ret = 0;
	
	CA_READ_RETURN(stream, m_FillStyle.pattern, len, ret);
	
	CA_READ_RETURN(stream, m_FillStyle.pattern_color.red, len, ret);
	CA_READ_RETURN(stream, m_FillStyle.pattern_color.green, len, ret);
	CA_READ_RETURN(stream, m_FillStyle.pattern_color.blue, len, ret);	
	CA_READ_RETURN(stream, m_FillStyle.pattern_color.alpha, len, ret);
	
	CA_READ_RETURN(stream, m_FillStyle.color.red, len, ret);
	CA_READ_RETURN(stream, m_FillStyle.color.green, len, ret);
	CA_READ_RETURN(stream, m_FillStyle.color.blue, len, ret);	
	CA_READ_RETURN(stream, m_FillStyle.color.alpha, len, ret);
	
	return len;
}

int pxShapeBase::WriteTextStyle( pxTextStyle* style, pxShapeIOStream* stream )
{
	int len = 0;
	int ret = 0;

	CA_WRITE_RETURN(stream, style->text_font.family, len, ret);
	CA_WRITE_RETURN(stream, style->text_font.font_style, len, ret);
	CA_WRITE_RETURN(stream, style->text_font.font_weight, len, ret);
	CA_WRITE_RETURN(stream, style->text_font.font_underline, len, ret);
	CA_WRITE_RETURN(stream, style->text_font.font_encoding, len, ret);
	CA_WRITE_RETURN(stream, style->text_font.font_size, len, ret);

	CA_RETURN(WriteString(stream, style->text_font.font_face_name), len, ret);

	CA_WRITE_RETURN(stream, style->text_color.red, len, ret);
	CA_WRITE_RETURN(stream, style->text_color.green, len, ret);
	CA_WRITE_RETURN(stream, style->text_color.blue, len, ret);	
	CA_WRITE_RETURN(stream, style->text_color.alpha, len, ret);

	return len;
}

int pxShapeBase::ReadTextStyle( pxTextStyle* style, pxShapeIOStream* stream )
{
	int len = 0;
	int ret = 0;

	CA_READ_RETURN(stream, style->text_font.family, len, ret);
	CA_READ_RETURN(stream, style->text_font.font_style, len, ret);
	CA_READ_RETURN(stream, style->text_font.font_weight, len, ret);
	CA_READ_RETURN(stream, style->text_font.font_underline, len, ret);
	CA_READ_RETURN(stream, style->text_font.font_encoding, len, ret);
	CA_READ_RETURN(stream, style->text_font.font_size, len, ret);

	CA_RETURN(ReadString(stream, style->text_font.font_face_name, MAX_FONT_FACE_NAME_LEN),
		len, ret);

	CA_READ_RETURN(stream, style->text_color.red, len, ret);
	CA_READ_RETURN(stream, style->text_color.green, len, ret);
	CA_READ_RETURN(stream, style->text_color.blue, len, ret);	
	CA_READ_RETURN(stream, style->text_color.alpha, len, ret);

	return len;
}

void pxShapeBase::PrepareBrush(pxCanvas* canvas)
{
	canvas->SetBrush(m_FillStyle);	
}

void pxShapeBase::PreparePen(pxCanvas* canvas)
{
	canvas->SetPen(m_LineStyle);
}

int pxShapeBase::WriteString( pxShapeIOStream* stream, const char* sz )
{
	int len = 0;
	int ret = 0;

	int sz_len = 0;
	if(sz)
		sz_len = strlen(sz);

	CA_WRITE_RETURN(stream, sz_len, len, ret);

	if(sz_len > 0)
	{
		CA_WRITE_BYTES_RETURN(stream, sz[0], sz_len, len, ret);
	}

	return len;
}

int pxShapeBase::ReadReallocString( pxShapeIOStream* stream, char** sz, int* ret_len, int max_len )
{
	int len = 0;
	int ret = 0;

	*sz = 0;
	
	if(ret_len)
		*ret_len = 0;

	int sz_len = 0;
	CA_READ_RETURN(stream, sz_len, len, ret);

	sz_len = min(sz_len, max_len);

	if(sz_len > 0)
	{
		if(ret_len)
			*ret_len = sz_len;

		if(*sz)
			delete[] *sz;

		*sz = new char[sz_len + 1];

		CA_READ_BYTES_RETURN(stream, (*sz)[0], sz_len, len, ret);
		(*sz)[sz_len] = 0;
	}

	return len;
}

int pxShapeBase::ReadString( pxShapeIOStream* stream, char* sz, int max_len )
{
	int len = 0;
	int ret = 0;

	int sz_len = 0;
	CA_READ_RETURN(stream, sz_len, len, ret);

	if(sz_len > max_len)
		return -1;

	if(sz_len > 0)
	{
		CA_READ_BYTES_RETURN(stream, sz[0], sz_len, len, ret);
	}
	
	sz[sz_len] = 0;

	return len;
}

bool pxShapeBase::GetPoint( int point_index, pxPoint& point ) const
{
	if(point_index >= 0 && point_index < 2)
	{
		point = GetPointsPtr()[point_index];
		return true;
	}

	return false;
}

void pxShapeBase::SetPoint( int point_index, const pxPoint& point )
{
	if(point_index >= 0 && point_index < 2)
	{
		GetPointsPtr()[point_index] = point;
	}
}

void pxShapeBase::Translate( pxCoord x, pxCoord y )
{
	int n = 0;
	
	pxPoint point;
	while(GetPoint(n, point))
	{
		point.m_x += x;
		point.m_y += y;

		SetPoint(n, point);
		n++;
	}
}

void pxShapeBase::Scale(const pxPoint& rotate_ref, double rotation,
						const pxPoint& scale_ref, double scale_x, double scale_y)
{
	pxPoint point;
	int n = 0;

	while(GetPoint(n, point))
	{
		px2DGeometry::RotatePoint(rotate_ref, -rotation, point);

		pxPoint offset = point - scale_ref;
		offset.m_x *= scale_x;
		offset.m_y *= scale_y;
		point = scale_ref + offset;

		px2DGeometry::RotatePoint(rotate_ref, rotation, point);
		SetPoint(n, point);
		n++;
	}
}

pxLineStyle pxShapeBase::LineStyle() const
{
	return m_LineStyle;
}

void pxShapeBase::LineStyle( const pxLineStyle& val )
{
	m_LineStyle = val;
}

pxFillStyle pxShapeBase::FillStyle() const
{
	return m_FillStyle;
}

void pxShapeBase::FillStyle( const pxFillStyle& val )
{
	m_FillStyle = val;
}

pxTextStyle pxShapeBase::TextStyle() const
{
	return g_default_text_style;
}

void pxShapeBase::TextStyle( const pxTextStyle& )
{

}

void pxShapeBase::SetRect( const pxRect& rect )
{
	pxPoint point;
	point.m_x = rect.m_x;
	point.m_y = rect.m_y;

	SetPoint(0, point);

	point.m_x += rect.m_width;
	point.m_y += rect.m_height;

	SetPoint(1, point);
}

pxShapeBase* pxShapeBase::HitTest( const pxPoint& point, pxShapeBase*  )
{//完成基本的矩形过滤
	pxPoint leftup, rightbottom;
	GetBoundsRect(leftup, rightbottom);

	px2DGeometry::RectAddTolerance(leftup, rightbottom);

	if( px2DGeometry::PointInRectangle(point, leftup, rightbottom) )
	{
		return this;
	}

	return 0;
}

pxTransformMark pxShapeBase::G_TransformMarkTest( const pxPoint& point, const pxPoint rectPoint[4])
{
	pxTransformMark mark = pxTransformMark_None;

	int n;
	for(n = 0; n < 4; n ++)
	{
		pxPoint p0, p1;
		p0 = rectPoint[n];
		p1 = rectPoint[(n+1)%4];

		pxPoint c;
		c.m_x = (p1.m_x + p0.m_x) / 2;
		c.m_y = (p1.m_y + p0.m_y) / 2;

		if(px2DGeometry::PointNearPoint(point, p0))
			mark = (pxTransformMark)(pxTransformMark_Rotate + 1 + n * 2);
		else if(px2DGeometry::PointNearPoint(point, p1))
			mark = (pxTransformMark)(pxTransformMark_Rotate + 1 + n * 2 + 2);
		else if(px2DGeometry::PointNearPoint(point, c))
			mark = (pxTransformMark)(pxTransformMark_Rotate + 1 + n * 2 + 1);

		if(mark != pxTransformMark_None)
			break;
	}

	return mark;
}

pxTransformMark pxShapeBase::G_TransformMarkTest( const pxPoint& point, const pxPoint& lu, const pxPoint& rb )
{
	pxPoint recPoint[4];
	px2DGeometry::Init4PointRect(lu, rb, recPoint);

	return G_TransformMarkTest(point, recPoint);
}

pxTransformMark pxShapeBase::TransformMarkTest( const pxPoint& point )
{
	pxTransformMark mark = pxTransformMark_None;
	
	if(HitTest(point, 0))
	{
		mark = G_TransformMarkTest(point, GetPointsPtr());
		if(mark == pxTransformMark_None)
			mark = pxTransformMark_Move;
	}
	else
	{
		pxPoint pointAnchor, pointC;
		GetRotationControlPoints(pointAnchor, pointC);

		if(px2DGeometry::PointNearPoint(point, pointAnchor))
			mark = pxTransformMark_Rotate;
	}

	return mark;
}

pxShapeBase* pxShapeBase::RectSelectTest( const pxPoint& leftup, const pxPoint& rightbottom )
{
	pxPoint lu, rb;
	GetBoundsRect(lu, rb);

	if(px2DGeometry::PointInRectangle(lu, leftup, rightbottom) && 
		px2DGeometry::PointInRectangle(rb, leftup, rightbottom))
	{
		return this;
	}

	return 0;
}

void pxShapeBase::DrawSquareMark( pxCanvas* canvas, const pxPoint& point )
{
	pxPoint of, scale;
	canvas->GetTransform(of,scale);

	const double mark_width = 3 / scale.m_x;
	const double mark_height = 3 / scale.m_y;

	pxLineStyle style = g_default_line_style;
	style.width = 1;
	style.color.red = 0;
	style.color.green = 168;
	style.color.blue = 255;
	canvas->SetPen(style);
	canvas->SetBrush(*wxWHITE_BRUSH);
	canvas->DrawRectangle(point.m_x-mark_width, point.m_y-mark_height, point.m_x+mark_width, point.m_y+mark_height);
}

void pxShapeBase::DrawRoundMark( pxCanvas* canvas, const pxPoint& point )
{
	pxPoint of, scale;
	canvas->GetTransform(of,scale);

	const double mark_width = 3 / scale.m_x;
	const double mark_height = 3 / scale.m_y;

	pxLineStyle style = g_default_line_style;
	style.width = 1;
	style.color.red = 0;
	style.color.green = 168;
	style.color.blue = 255;
	canvas->SetPen(style);
	canvas->SetBrush(*wxWHITE_BRUSH);
	canvas->DrawEllipse(point.m_x-mark_width, point.m_y-mark_height, point.m_x+mark_width, point.m_y+mark_height, 0);
}

void pxShapeBase::GetBoundsRect( pxPoint& leftup, pxPoint& rightbottom )
{
	pxPoint* pPoints = GetPointsPtr();
	leftup = pPoints[0];
	rightbottom = leftup;

	int n;
	for(n = 1; n < GetPointNr(); n ++)
	{
		if(leftup.m_x > pPoints[n].m_x)
			leftup.m_x = pPoints[n].m_x;
		if(leftup.m_y > pPoints[n].m_y)
			leftup.m_y = pPoints[n].m_y;

		if(rightbottom.m_x < pPoints[n].m_x)
			rightbottom.m_x = pPoints[n].m_x;
		if(rightbottom.m_y < pPoints[n].m_y)
			rightbottom.m_y = pPoints[n].m_y;
	}
}

void pxShapeBase::GetBoundsRect( pxPoint point[4] )
{
	pxPoint lu,rb;
	GetBoundsRect(lu,rb);

	px2DGeometry::Init4PointRect(lu, rb, point);
}

void pxShapeBase::DrawSelectLine( pxCanvas* canvas, pxCoord x1, pxCoord y1, pxCoord x2, pxCoord y2)
{
	pxLineStyle style = g_default_line_style;
	style.style = wxPENSTYLE_LONG_DASH;
	style.width = 1;
	style.color.red = 0;
	style.color.green = 168;
	style.color.blue = 255;

	canvas->SetPen(style);
	canvas->SetBrush(*wxTRANSPARENT_BRUSH);

	canvas->DrawLine(x1, y1, x2, y2);
}

void pxShapeBase::DrawMultipleSelectedLine( pxCanvas* canvas, pxCoord x1, pxCoord y1, pxCoord x2, pxCoord y2, bool bMainTarget )
{
	pxLineStyle style = g_default_line_style;
	style.style = wxPENSTYLE_SOLID;
	style.width = 1;
	style.color.red = 255;
	style.color.green = 100;
	style.color.blue = 255;
	if(bMainTarget)
		style.width = 3;

	canvas->SetPen(style);
	canvas->SetBrush(*wxTRANSPARENT_BRUSH);

	canvas->DrawLine(x1, y1, x2, y2);
}

void pxShapeBase::Clone( pxShapeBase* shape )
{
	pxMemIOStream stream;
	int len = shape->Write(&stream);

	stream.Alloc(len);
	shape->Write(&stream);

	stream.Seek(0);
	Read(&stream);
}

ulong pxShapeBase::GetShapeMemSize( pxShapeType shape_type )
{
	ulong size = 0;
	pxShapeBase* shape = (pxShapeBase*)new byte[64*1024];//just big enough
	pxShapeBase::ConstructShape(shape, shape_type, 0);

	size = shape->GetShapeMemSize();

	pxShapeBase::DestructShape(shape);
	delete[] (byte*)shape;

	return size;
}

pxShapeBase* pxShapeBase::NewShape( pxShapeType shape_type)
{
	pxShapeBase* shape = 0;

	if(shape_type > pxShapeType_Invalid && shape_type < pxShapeType_COUNT)
	{
		ulong len = GetShapeMemSize(shape_type);
		shape = (pxShapeBase*)new byte[len];
		ConstructShape(shape, shape_type, 0);

		shape->LineStyle(g_default_line_style);
		shape->FillStyle(g_default_fill_style);
		shape->TextStyle(g_default_text_style);
	}

	return shape;
}

void pxShapeBase::FreeShape( pxShapeBase* shape )
{
	delete[] shape;
}

void pxShapeBase::ConstructShape( pxShapeBase* shape, pxShapeType shape_type, pxShapePool* pool )
{
	if(shape_type >= pxShapeType_MIN && shape_type <= pxShapeType_MAX)
	{
		switch (shape_type)
		{
		case pxShapeType_LineSec:
			((pxShapeLineSec*)shape)->pxShapeLineSec::pxShapeLineSec();
			break;
		case pxShapeType_Rect:
			((pxShapeRect*)shape)->pxShapeRect::pxShapeRect();
			break;
		case pxShapeType_Text:
			((pxShapeText*)shape)->pxShapeText::pxShapeText();
			break;
		case pxShapeType_Ellipse:
			((pxShapeEllipse*)shape)->pxShapeEllipse::pxShapeEllipse();
			break;
		case pxShapeType_Arc:
			((pxShapeArc*)shape)->pxShapeArc::pxShapeArc();
			break;
		case pxShapeType_BezierArc:
			((pxShapeBezier*)shape)->pxShapeBezier::pxShapeBezier();
			break;
		case pxShapeType_Img:
			((pxShapeImg*)shape)->pxShapeImg::pxShapeImg();
			break;
		case pxShapeType_Animator:
			((pxShapeAnimator*)shape)->pxShapeAnimator::pxShapeAnimator();
			break;
		case pxShapeType_Group:
			((pxShapeGroup*)shape)->pxShapeGroup::pxShapeGroup();
			((pxShapeGroup*)shape)->SetShapePool(pool);
			break;
		case pxShapeType_ConnectedLines:
			((pxShapeConnectedLines*)shape)->pxShapeConnectedLines::pxShapeConnectedLines();
			((pxShapeConnectedLines*)shape)->SetShapePool(pool);
			break;
		default:
			break;
		}
	}
}

void pxShapeBase::DestructShape( pxShapeBase* shape )
{
	int memsize = shape->GetShapeMemSize();
	shape->~pxShapeBase();
	memset(shape, 0, memsize);
}

void pxShapeBase::DrawSelectedMark( pxCanvas* canvas, bool with_corner_mark /*= true*/, bool with_rotation_mark/* = true*/ )
{
	if(with_rotation_mark)
		DrawRotationMark(canvas);

	pxPoint* p = GetPointsPtr();
	int point_count = GetPointNr();

	int n;
	for(n = 0; n < point_count; n ++)
	{
		pxPoint p0, p1;
		p0 = p[n];
		p1 = p[(n+1)%point_count];

		DrawSelectLine(canvas, p0.m_x, p0.m_y, p1.m_x, p1.m_y);

		if(with_corner_mark)
		{
			pxPoint c;
			c.m_x = (p1.m_x + p0.m_x) / 2;
			c.m_y = (p1.m_y + p0.m_y) / 2;
			DrawSquareMark(canvas, p0);

			if(point_count > 2)
				DrawSquareMark(canvas, c);

			if(n == point_count - 1)
				DrawSquareMark(canvas, p1);
		}
	}
}

void pxShapeBase::DrawSelectedMark( pxCanvas* canvas, pxPoint p[4], bool with_corner_mark /*= true*/, bool with_rotation_mark /*= true*/ )
{
	if(with_rotation_mark)
		DrawRotationMark(canvas, p);

	int n;
	int point_count = 4;

	for(n = 0; n < point_count; n ++)
	{
		pxPoint p0, p1;
		p0 = p[n];
		p1 = p[(n+1)%point_count];

		DrawSelectLine(canvas, p0.m_x, p0.m_y, p1.m_x, p1.m_y);

		if(with_corner_mark)
		{
			pxPoint c;
			c.m_x = (p1.m_x + p0.m_x) / 2;
			c.m_y = (p1.m_y + p0.m_y) / 2;
			DrawSquareMark(canvas, p0);

			if(point_count > 2)
				DrawSquareMark(canvas, c);

			if(n == point_count - 1)
				DrawSquareMark(canvas, p1);
		}
	}
}

void pxShapeBase::DrawMultipleSelectedMark( pxCanvas* canvas, bool bMainTarget /*= false*/ )
{
	pxPoint* p = GetPointsPtr();
	int point_count = GetPointNr();

	int n;
	for(n = 0; n < point_count; n ++)
	{
		pxPoint p0, p1;
		p0 = p[n];
		p1 = p[(n+1)%point_count];

		DrawMultipleSelectedLine(canvas, p0.m_x, p0.m_y, p1.m_x, p1.m_y, bMainTarget);
	}
}

void pxShapeBase::DrawControlPointMark( pxCanvas* canvas )
{
	DrawSelectedMark(canvas, false, false);
}

pxPoint* pxShapeBase::GetPointsPtr() const
{
	return (pxPoint*)&m_Point;
}

bool pxShapeBase::IsLineShape()
{
	return IsLineShape(GetShapeType());
}

bool pxShapeBase::IsLineShape( pxShapeType type )
{
	if(type == pxShapeType_LineSec ||
		type == pxShapeType_Arc ||
		type == pxShapeType_BezierArc ||
		type == pxShapeType_ConnectedLines)
	{
		return true;
	}

	return false;
}

pxTransformMark pxShapeBase::ControlPointTransformMarkTest( const pxPoint& )
{
	return pxTransformMark_None;
}

int pxShapeBase::GetPointNr() const
{
	return 2;
}

void pxShapeBase::PrepareDraw( pxCanvas* canvas )
{
	PreparePen(canvas);
	PrepareBrush(canvas);
}

void pxShapeBase::EndDraw( pxCanvas* )
{
}

void pxShapeBase::Rotate( const pxPoint& refPoint, double angle )
{
	int n;
	pxPoint* lp = GetPointsPtr();

	for(n = 0; n < GetPointNr(); n ++)
	{
		px2DGeometry::RotatePoint(refPoint, angle, lp[n]);
	}
}

void pxShapeBase::DrawRotationMark( pxCanvas* canvas )
{
	pxPoint pointAnchor, pointC;

	if(GetRotationControlPoints(pointAnchor, pointC))
	{
		DrawSelectLine(canvas, pointC.m_x, pointC.m_y, pointAnchor.m_x, pointAnchor.m_y);
		DrawRoundMark(canvas, pointAnchor);
	}
}

void pxShapeBase::DrawRotationMark( pxCanvas* canvas, pxPoint point[4] )
{
	pxPoint pointAnchor, pointC;
	GetRotationControlPoints(point, pointAnchor, pointC);

	DrawSelectLine(canvas, pointC.m_x, pointC.m_y, pointAnchor.m_x, pointAnchor.m_y);
	DrawRoundMark(canvas, pointAnchor);
}

void pxShapeBase::GetRotationControlPoints( const pxPoint point[4], pxPoint& pointAnchor, pxPoint& pointC, bool flipped )
{
	double rotation = px2DGeometry::LineAngle(point[0], point[3]);

	pxPoint ru = point[3];
	px2DGeometry::RotatePoint(point[0], -rotation, ru);

	pointC = (ru + point[0]) / 2; //第三条边中点
	pointAnchor = pointC; //旋转控制点

	const int rotation_line_len = 30;

	if(flipped)
		pointAnchor.m_y += rotation_line_len; 
	else
		pointAnchor.m_y -= rotation_line_len; 

	px2DGeometry::RotatePoint(point[0], rotation, pointC);
	px2DGeometry::RotatePoint(point[0], rotation, pointAnchor);
}

double pxShapeBase::GetRotationAngle()
{
	return 0;
}

bool pxShapeBase::GetEndPoint( pxPoint point[2] )
{
	point;

	return false;
}

int pxShapeBase::GetControlPointNr()
{
	return GetPointNr();
}

void pxShapeBase::SetContrlPoint( int point_index, const pxPoint& point )
{
	//do nothing
	point_index;
	point;
}

void pxShapeBase::RevertEndPoints()
{
	pxPoint tmp;
	int n;
	int count =  GetPointNr();

	pxPoint* p = GetPointsPtr();

	for(n = 0; n < count / 2; n ++)
	{
		tmp = p[n];
		p[n] = p[count-n-1];
		p[count-n-1] = tmp;
	}
}

void pxShapeBase::CloneStyle( pxShapeBase* shape )
{
	pxLineStyle ls = shape->LineStyle();
	pxFillStyle fs = shape->FillStyle();
	pxTextStyle ts = shape->TextStyle();

	LineStyle(ls);
	FillStyle(fs);
	TextStyle(ts);
}

void pxShapeBase::Flip( const pxPoint& refPoint, bool flip_horz, bool flip_vert )
{
	int point_nr = GetPointNr();
	int n;
	for(n = 0; n < point_nr; n ++)
	{
		pxPoint point = GetPointsPtr()[n];

		px2DGeometry::FlipPoint(refPoint, flip_horz, flip_vert, point);
	
		SetPoint(n, point);
	}
}

void pxShapeBase::BuildPropertyCollection(pxShapeProperyCategory& PropertyCollection)
{
	pxShapeProperyCategory& subcollection = PropertyCollection.AddCategory("基本属性");
	subcollection.AddItem("线型",		SPIT_LONG, &m_LineStyle.style, sizeof(m_LineStyle.style));
	subcollection.AddItem("线宽",		SPIT_LONG, &m_LineStyle.width, sizeof(m_LineStyle.width));
	subcollection.AddItem("线端形状",	SPIT_LONG, &m_LineStyle.terminal_style, sizeof(m_LineStyle.terminal_style));
	subcollection.AddItem("线色",		SPIT_LONG, &m_LineStyle.color, sizeof(m_LineStyle.color));
}

void pxShapeBase::QueryProperty(pxShapeProperyCategory& PropertyCollection)
{
	if (PropertyCollection.GetPropertyItemCount() > 0)
	{//填充指定的属性
		pxShapeProperyCategory col;
		BuildPropertyCollection(col);
		col.FetchValue();

		PropertyCollection.FillFrom(col);
	}
	else
	{
		BuildPropertyCollection(PropertyCollection);
		PropertyCollection.FetchValue();
	}
}

void pxShapeBase::SetProperty(const pxShapeProperyCategory& PropertyCollection)
{
	
}
