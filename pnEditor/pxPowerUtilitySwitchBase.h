#pragma once

#include "pxPowerUtilityBase.h"
#include "pxShapeBase.h"

class pxPowerUtilitySwitchBase :
	public pxPowerUtilityBase,
	public pxShapeBase
{
public:
	pxPowerUtilitySwitchBase(void);
	virtual ~pxPowerUtilitySwitchBase(void);
};

