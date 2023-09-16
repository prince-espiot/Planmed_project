#include "types.h"
#ifndef _DOSE_H
#define _DOSE_H

u16 getIED( void );
u16 calcMGD(u8 kv, u8 thickness, u8 target, u8 filter, u8 magnification, u16 radOutput, u16 mAs );

static u16 IED;
static u16 calcIED(u8 kv, u8 thickness, u8 target, u8 filter, u8 magnification, u16 radOutput, u16 mAs);
static u16 calcMgdIedAnode(const u16 material[][16], u8 kv, u8 p, u32 interpol, u8 filter);


#endif