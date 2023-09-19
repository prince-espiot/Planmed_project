#include <stdio.h>
#include "dose.h"

u16 getIED(void)
{
    return IED;
}
static u32 calcP(u8 p, u8 limited_mm, const u8 mm_index[]) {  
    // find correct thickness index 0 - 7 for table seek
    // starting from high mm seek until equal or lower value is found from the table
    while (p != 0 && mm_index[p] >= limited_mm) {
        p--;
    }
    return p;
}
static u32 calcInterpolate(u8 p, u8 limited_mm, const u8 mm_index[9]) {  
    // calculate thickness interpolation factor
    return (1000 * (limited_mm - mm_index[p])) / (mm_index[p + 1] - mm_index[p]);
}
static u16 calcMgdAnode(const u16 material[][16], u8 kv, u8 p, u32 interpol){//this substract
    // thickness interpolate AGD-value from table that has pre-calculated (*10000) values for
    // EUREF sheet D0001629-2 c, g and s-factors
        return material[p][kv - 20] - ((interpol * (material[p][kv - 20] - material[p + 1][kv - 20]) + 500) / 1000);
}
static u16 calcIedAnode(const u16 material[][16], u8 p, u8 kv, u32 interpol) { //this add take note!!!
    return material[p][kv - 20] + ((interpol * (material[p + 1][kv - 20] - material[p][kv - 20]) + 500) / 1000);
}
// AGD calculation
u16 calcMGD(u8 kv, u8 thickness, u8 target, u8 filter, u8 magnification, u16 radOutput, u16 mAs)
{
    u32 mgd = 0;
    u32 interpol;
    u8 limited_mm;
    u8 p = 7;

    //simplify this if statements with ||
    if (thickness <= 0 || thickness > 120) {
        IED = 0;
        return 0;
    }
    // thickness must be limited between 10 - 100 mm
    limited_mm = thickness < 10 ? 10 : (thickness > 100 ? 100 : thickness);

    IED = calcIED(kv, limited_mm, target, filter, magnification, radOutput, mAs);

    p = calcP(p, limited_mm, mm_index);
        
    interpol = calcInterpolate(p, limited_mm, mm_index);

    if (target == TARGET_MO) {
        mgd = (filter == FILTER_MATERIAL_RH) ?
            calcMgdAnode(MGD_Rh, kv, p, interpol) :
            calcMgdAnode(MGD_Mo, kv, p, interpol);
    }
    else {
        mgd = (filter == FILTER_MATERIAL_AG) ?
            calcMgdAnode(MGD_W_Ag, kv, p, interpol) :
            calcMgdAnode(MGD_W_Rh, kv, p, interpol);
    }
    mgd = mgd * IED;
    mgd = (u16)((mgd + 5000) / 10000); // to make LSB of mgd equal 10 uGy for protocol spec and accuracy correctio / 1000
    return (u16)mgd;
}

// IED (Incident Entrance Dose) calculation
static u16 calcIED(u8 kv, u8 thickness, u8 target, u8 filter, u8 magnification, u16 radOutput, u16 mAs)
{
    u32 ied = 0;
    u8 p=7;
    u32 interpol;
    u8 limited_mm = thickness;

    p = calcP(p, limited_mm, mm_index);

    interpol = calcInterpolate(p, limited_mm, mm_index);

    if (target == TARGET_MO)
    {          // Mo anode
        if (magnification > 170)
        {
            ied = (filter == FILTER_MATERIAL_MO) ?
                calcIedAnode(IED_mag18_Mo, p, kv, interpol) :
                calcIedAnode(IED_mag18_Rh, p, kv, interpol);
        }
        else if (magnification > 130)
        {
            ied = (filter == FILTER_MATERIAL_MO) ?
                calcIedAnode(IED_mag16_Mo, p, kv, interpol) :
                calcIedAnode(IED_mag16_Rh, p, kv, interpol);
        }
        else
        {
            ied = (filter == FILTER_MATERIAL_MO) ?
                calcIedAnode(IED_Mo, p, kv, interpol) :
                calcIedAnode(IED_Rh, p, kv, interpol);
        }
    }
    else
    {         // W anode
        if (magnification > 170)
        {
            ied = (filter == FILTER_MATERIAL_AG) ?
                calcIedAnode(IED_W_mag18_Ag, p, kv, interpol) :
                calcIedAnode(IED_W_mag18_Rh, p, kv, interpol);
        }
        else if (magnification > 130)
        {
            ied = (filter == FILTER_MATERIAL_AG) ?
                calcIedAnode(IED_W_mag16_Ag, p, kv, interpol) :
                calcIedAnode(IED_W_mag16_Rh, p, kv, interpol);
        }
        else
        {
            ied = (filter == FILTER_MATERIAL_AG) ?
                calcIedAnode(IED_W_Ag, p, kv, interpol) :
                calcIedAnode(IED_W_Rh, p, kv, interpol);
        }
    }
    // IED value is multiplied by mAs and tube radiation output µGy/mAs
    ied = ((ied * mAs) + 500) / 1000;
    ied = ((ied * radOutput) + 500) / 1000;
    return (u16)ied;
}