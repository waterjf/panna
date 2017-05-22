// pxShapeAnimator.cpp: implementation of the pxShapeAnimator class.
//
//////////////////////////////////////////////////////////////////////
#include "precomp.h"
#include "pxShapeAnimator.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

pxShapeAnimator::pxShapeAnimator()
{

}

pxShapeAnimator::~pxShapeAnimator()
{

}

void pxShapeAnimator::Draw( pxCanvas* canvas )
{
	canvas;
	//todo
}

pxShapeType pxShapeAnimator::GetShapeType()
{
	return pxShapeType_Animator;
}

int pxShapeAnimator::Write( pxShapeIOStream* stream )
{
	return pxShapeRect::Write(stream);
}

int pxShapeAnimator::Read( pxShapeIOStream* stream )
{
	return pxShapeRect::Read(stream);
}
