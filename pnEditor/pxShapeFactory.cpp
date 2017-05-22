#include "precomp.h"
#include "pxShapeFactory.h"
#include "pxShapeBase.h"

pxShapeFactory::pxShapeFactory()
{
	Initialize();
}

pxShapeFactory::~pxShapeFactory(void)
{
}

void pxShapeFactory::Initialize()
{
	m_LineStyle = g_default_line_style;
	m_FillStyle = g_default_fill_style;
	m_TextStyle = g_default_text_style;

	Init();
}

void pxShapeFactory::Finalize()
{
	Destroy();
}

void pxShapeFactory::SetDefaultLineStyle( const pxLineStyle& style )
{
	m_LineStyle = style;
}

void pxShapeFactory::SetDefaultFillStyle( const pxFillStyle& style )
{
	m_FillStyle = style;
}

void pxShapeFactory::SetDefaultTextStyle( const pxTextStyle& style )
{
	m_TextStyle = style;
}

pxShapeBase* pxShapeFactory::CreateShape( pxShapeType shape_type )
{
	pxShapeBase* shape = pxShapePool::CreateShape(shape_type);
	if(shape)
	{
		shape->LineStyle(m_LineStyle);
		shape->FillStyle(m_FillStyle);
		shape->TextStyle(m_TextStyle);
	}

	return shape;
}
