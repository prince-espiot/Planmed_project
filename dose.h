#ifndef _DOSE_H
#define _DOSE_H
#include "types.h"
#include "tables.h"
#include "parameters.h"

u16 getIED( void );
u16 calcMGD(u8 kv, u8 thickness, u8 target, u8 filter, u8 magnification, u16 radOutput, u16 mAs );
u32 calcP(u8 p, u8 limited_mm, const u8 mm_index[9]);

static u16 IED;
u16 calcIED(u8 kv, u8 thickness, u8 target, u8 filter, u8 magnification, u16 radOutput, u16 mAs);

u16 calcMgdAnode(const u16 material[][16], u8 kv, u8 p, u32 interpol);
u16 calcIedAnode(const u16 material[][16], u8 p, u8 kv, u32 interpol);
u32 calcInterpolate(u8 limited_mm, u8 p, const u8 mm_index[9]);


#endif
