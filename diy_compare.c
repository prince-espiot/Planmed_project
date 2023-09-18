#include "types.h"
#include "tables.h"
#include "parameters.h"

static u16 IED;

static u16 getIEDTest(void)
{
    return IED;
}

static u16 calcIEDTest(u8 kv, u8 thickness, u8 target, u8 filter, u8 magnification, u16 radOutput, u16 mAs);

// AGD calculation
static u16 calcMGDTest(u8 kv, u8 thickness, u8 target, u8 filter, u8 magnification, u16 radOutput, u16 mAs)
{
    u32 mgd = 0;
    u32 interpol;
    u8 limited_mm;
    u8 p;

    if (thickness == 0)
        return 0;

    if (thickness > 120) // esp. in man mode could expose with paddle up and then values become totally bogus
    {
        IED = 0;
        return 0;
    }

    // thickness must be limited between 10 - 100 mm
    if (thickness < 10)
        limited_mm = 10;
    else if (thickness > 100)
        limited_mm = 100;
    else
        limited_mm = thickness;

    IED = calcIEDTest(kv, limited_mm, target, filter, magnification, radOutput, mAs);

    // find correct thickness index 0 - 7 for table seek
    // starting from high mm seek until equal or lower value is found from the table
    for (p = 7; p != 0; p--) {
        if (mm_index[p] <= limited_mm)
            break;
    }

    // calculate thickness interpolation factor

    interpol = (1000 * (limited_mm - mm_index[p])) / (mm_index[p + 1] - mm_index[p]);
    printf("interpol_com value:%d\n", interpol);
    // thickness interpolate AGD-value from table that has pre-calculated (*10000) values for
    // EUREF sheet D0001629-2 c, g and s-factors

    if (target == TARGET_MO)
    {      // Mo anode
        if (filter == FILTER_MATERIAL_RH) {
            mgd = MGD_Rh[p][kv - 20] - ((interpol * (MGD_Rh[p][kv - 20] - MGD_Rh[p + 1][kv - 20]) + 500) / 1000);
            printf("mgd AFTER FILTER_COM value:%d\n", mgd);
        }
        else
            mgd = MGD_Mo[p][kv - 20] - ((interpol * (MGD_Mo[p][kv - 20] - MGD_Mo[p + 1][kv - 20]) + 500) / 1000);
        
    }
    else
    {          // Tungsten anode
        if (filter == FILTER_MATERIAL_AG)
            mgd = MGD_W_Ag[p][kv - 20] - ((interpol * (MGD_W_Ag[p][kv - 20] - MGD_W_Ag[p + 1][kv - 20]) + 500) / 1000);
        else
            mgd = MGD_W_Rh[p][kv - 20] - ((interpol * (MGD_W_Rh[p][kv - 20] - MGD_W_Rh[p + 1][kv - 20]) + 500) / 1000);
    }

    mgd = mgd * IED;

    mgd = (u16)((mgd + 5000) / 10000); // to make LSB of mgd equal 10 uGy for protocol spec and accuracy correctio / 1000
    printf("mgd_com value:%d\n", mgd);
    return (u16)mgd;
}


// IED (Incident Entrance Dose) calculation

static u16 calcIEDTest(u8 kv, u8 thickness, u8 target, u8 filter, u8 magnification, u16 radOutput, u16 mAs)
{
    u32 ied = 0;
    u8 p;
    u32 interpol;
    u8 limited_mm = thickness;

    // find correct thickness index   0 - 7 for table seek
    // starting from high mm seek until equal or lower value is found from the table
    for (p = 7; p != 0; p--) {
        if (mm_index[p] <= limited_mm)
            break;
    }

    // calculate thickness interpolation factor

    interpol = (1000 * (limited_mm - mm_index[p])) / (mm_index[p + 1] - mm_index[p]);
    printf("interpol_com_ied value:%d\n", interpol);

    // thickness interpolate IED-value from table that has pre-calculated (*10000) values for
    // kV and thickness dependent radiation output factor 

    if (target == TARGET_MO)
    {          // Mo anode
        if (magnification > 170)
        {
            if (filter == FILTER_MATERIAL_MO)
                ied = IED_mag18_Mo[p][kv - 20] + ((interpol * (IED_mag18_Mo[p + 1][kv - 20] - IED_mag18_Mo[p][kv - 20]) + 500) / 1000);
            else
                ied = IED_mag18_Rh[p][kv - 20] + ((interpol * (IED_mag18_Rh[p + 1][kv - 20] - IED_mag18_Rh[p][kv - 20]) + 500) / 1000);
        }
        else if (magnification > 130)
        {
            if (filter == FILTER_MATERIAL_MO)
                ied = IED_mag16_Mo[p][kv - 20] + ((interpol * (IED_mag16_Mo[p + 1][kv - 20] - IED_mag16_Mo[p][kv - 20]) + 500) / 1000);
            else
                ied = IED_mag16_Rh[p][kv - 20] + ((interpol * (IED_mag16_Rh[p + 1][kv - 20] - IED_mag16_Rh[p][kv - 20]) + 500) / 1000);
        }
        else
        {
            if (filter == FILTER_MATERIAL_MO)
                ied = IED_Mo[p][kv - 20] + ((interpol * (IED_Mo[p + 1][kv - 20] - IED_Mo[p][kv - 20]) + 500) / 1000);
            else
                ied = IED_Rh[p][kv - 20] + ((interpol * (IED_Rh[p + 1][kv - 20] - IED_Rh[p][kv - 20]) + 500) / 1000);
        }
    }
    else
    {         // W anode
        if (magnification > 170)
        {
            if (filter == FILTER_MATERIAL_AG)
                ied = IED_W_mag18_Ag[p][kv - 20] + ((interpol * (IED_W_mag18_Ag[p + 1][kv - 20] - IED_W_mag18_Ag[p][kv - 20]) + 500) / 1000);
            else
                ied = IED_W_mag18_Rh[p][kv - 20] + ((interpol * (IED_W_mag18_Rh[p + 1][kv - 20] - IED_W_mag18_Rh[p][kv - 20]) + 500) / 1000);
        }
        else if (magnification > 130)
        {
            if (filter == FILTER_MATERIAL_AG)
                ied = IED_W_mag16_Ag[p][kv - 20] + ((interpol * (IED_W_mag16_Ag[p + 1][kv - 20] - IED_W_mag16_Ag[p][kv - 20]) + 500) / 1000);
            else
                ied = IED_W_mag16_Rh[p][kv - 20] + ((interpol * (IED_W_mag16_Rh[p + 1][kv - 20] - IED_W_mag16_Rh[p][kv - 20]) + 500) / 1000);
        }
        else
        {
            if (filter == FILTER_MATERIAL_AG)
                ied = IED_W_Ag[p][kv - 20] + ((interpol * (IED_W_Ag[p + 1][kv - 20] - IED_W_Ag[p][kv - 20]) + 500) / 1000);
            else
                ied = IED_W_Rh[p][kv - 20] + ((interpol * (IED_W_Rh[p + 1][kv - 20] - IED_W_Rh[p][kv - 20]) + 500) / 1000);
        }
    }

    // IED value is multiplied by mAs and tube radiation output µGy/mAs

    ied = ((ied * mAs) + 500) / 1000;
    ied = ((ied * radOutput) + 500) / 1000;
    printf("ied_com value:%d\n", interpol);
    return (u16)ied;
}