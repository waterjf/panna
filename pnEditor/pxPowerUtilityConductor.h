#pragma once
#include "pxPowerUtilityBase.h"
#include "pxShapeLineSec.h"

class pxPowerUtilityConductor :
	public pxPowerUtilityBase,
	public pxShapeLineSec
{
public:
	pxPowerUtilityConductor(void);
	virtual ~pxPowerUtilityConductor(void);
};

