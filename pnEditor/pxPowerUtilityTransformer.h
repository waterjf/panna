#pragma once
#include "pxPowerUtilityBase.h"
#include "pxShapeGroup.h"

class pxPowerUtilityTransformer :
	public pxPowerUtilityBase,
	public pxShapeGroup
{
public:
	pxPowerUtilityTransformer(void);
	virtual ~pxPowerUtilityTransformer(void);
};

