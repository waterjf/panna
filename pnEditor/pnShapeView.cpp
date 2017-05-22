#include "precomp.h"
#include "pnShapeView.h"
#include "pxShapeBase.h"
#include "pnShapeModel.h"

pnShapeView::pnShapeView(void)
{
	ResetTransform();
}

pnShapeView::~pnShapeView(void)
{
}

void pnShapeView::Paint( pxCanvas* canvas )
{
	canvas->SetTransform(m_translate, m_scale);

	pnShapeModel* model = GetShapeModel();

	shape_node pos = model->GetStartPostion();
	pxShapeBase* shape = 0;

	while(model->GetNextShape(pos, shape))
	{
		shape->Draw(canvas);
	}
}

void pnShapeView::SetViewSize( double cx, double cy, double xscale, double yscale )
{
	m_size.m_x = cx;
	m_size.m_y = cy;
	m_scale.m_x = xscale;
	m_scale.m_y = yscale;
}

void pnShapeView::ResetTransform()
{
	m_translate = pxPoint(0, 0);
	m_scale = pxPoint(1, 1);
	m_size = pxPoint(0, 0);
}
