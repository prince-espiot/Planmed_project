#include "tables.h"
#include "parameters.h"
#include <stdio.h>
#include "dose.h"



#define SEVEN 7 // why seven??   because the thickness can not be greater the 100---SO 103 doesnt matter
#define THOUSAND 1000

u16 getIED( void )
{
  return IED; 
}

u32 calcInterpolate(u8 p, u8 limited_mm, const u8 mm_index[9]) {
    // find correct thickness index 0 - 7 for table seek
    // starting from high mm seek until equal or lower value is found from the table
    while (p != 0 && mm_index[p] >= limited_mm) {
        printf("p value:%d\n", p);
        p--;
    }
    if (p != 0) {
        //printf("mm_index[p] value:%d\n", mm_index[p]);
        return (THOUSAND * (limited_mm - mm_index[p])) / (mm_index[p + 1] - mm_index[p]);
    }
    else {
        //if p is 0 which can not be the case
        printf("error");
        return 0;
    }
}

u16 calcMgdIedAnode(const u16 material[][16], u8 kv, u8 p, u32 interpol)
{
    return material[p][kv - 20] - ((interpol * (material[p][kv - 20] - material[p + 1][kv - 20]) + 500) / THOUSAND);
}

// AGD calculation
u16 calcMGD(u8 kv, u8 thickness, u8 target, u8 filter, u8 magnification, u16 radOutput, u16 mAs ) 
{
  u32 mgd = 0;
  u32 interpol;
  u8 limited_mm;
  u8 p = SEVEN;
  
  //simplify this if statements with ||
  if (thickness <= 0 || thickness > 120) {
      IED = 0;
      return 0;
  }
  // esp. in man mode could expose with paddle up and then values become totally bogus

  // thickness must be limited between 10 - 100 mm
  // i CAN USE THE  ternary operator here c? T:F
  limited_mm = thickness < 10 ? 10 : (thickness > 100 ? 100 : thickness);
 
  IED = calcIED(kv, limited_mm, target, filter, magnification, radOutput, mAs );

  interpol = calcInterpolate(p, limited_mm, mm_index);

  // thickness interpolate AGD-value from table that has pre-calculated (*10000) values for
  // EUREF sheet D0001629-2 c, g and s-factors
  if (target == TARGET_MO) {
      mgd = filter == FILTER_MATERIAL_RH ? calcMgdIedAnode(MGD_Rh, kv, p, interpol) : calcMgdIedAnode(MGD_Mo, kv, p, interpol);
  }
  else {
      mgd = filter == FILTER_MATERIAL_AG ? calcMgdIedAnode(MGD_W_Ag, kv, p, interpol) : calcMgdIedAnode(MGD_W_Rh, kv, p, interpol);
  }

  mgd = mgd * IED;
  mgd = (u16)((mgd+5000) / 10000); // to make LSB of mgd equal 10 uGy for protocol spec and accuracy correctio / 1000
  return (u16)mgd;
}

// IED (Incident Entrance Dose) calculation
static u16 calcIED(u8 kv, u8 thickness, u8 target, u8 filter, u8 magnification, u16 radOutput, u16 mAs )
{
  u32 ied = 0;
  u8 p = SEVEN;
  u32 interpol;
  u8 limited_mm=thickness;

  interpol = calcInterpolate(p, limited_mm, mm_index);

  // thickness interpolate IED-value from table that has pre-calculated (*10000) values for
  // kV and thickness dependent radiation output factor 
  // USE THE  ternary operator here c? T:F
  if (target == TARGET_MO ) 
  {          // Mo anode
    if (magnification > 170) 
    {
      ied = filter == FILTER_MATERIAL_MO ? calcMgdIedAnode(IED_mag18_Mo, kv, p, interpol) : calcMgdIedAnode(IED_mag18_Rh, kv, p, interpol);
    }
    else if (magnification > 130)
    {
      ied = filter == FILTER_MATERIAL_MO ? calcMgdIedAnode(IED_mag16_Mo, kv, p, interpol) : calcMgdIedAnode(IED_mag16_Rh, kv, p, interpol);
    }
    else 
    {
      ied = filter == FILTER_MATERIAL_MO ? calcMgdIedAnode(IED_Mo, kv, p, interpol) : calcMgdIedAnode(IED_Rh, kv, p, interpol);
    }
  }   
  else 
  {         // W anode
    if (magnification >170) 
    {
       
     ied = filter == FILTER_MATERIAL_AG ? calcMgdIedAnode(IED_W_mag18_Ag, kv, p, interpol) : calcMgdIedAnode(IED_W_mag18_Rh, kv, p, interpol);
    }
    else if ( magnification >130)
    {
     ied = filter == FILTER_MATERIAL_AG ? calcMgdIedAnode(IED_W_mag16_Ag, kv, p, interpol) : calcMgdIedAnode(IED_W_mag16_Rh, kv, p, interpol);
    }
    else 
    {
    ied = filter == FILTER_MATERIAL_AG ? calcMgdIedAnode(IED_W_Ag, kv, p, interpol) : calcMgdIedAnode(IED_W_Rh, kv, p, interpol);

    }
  }
  // IED value is multiplied by mAs and tube radiation output µGy/mAs
  ied = ((ied * mAs ) + 500) / THOUSAND;
  ied = ((ied * radOutput ) + 500 ) / THOUSAND;
  return (u16)ied;
}



//int main() {
//    // Define some test parameters
//    u8 kv = 80;
//    u8 thickness = 50;
//    u8 target = TARGET_MO;
//    u8 filter = FILTER_MATERIAL_RH;
//    u8 magnification = 0;
//    u16 radOutput = 5000;
//    u16 mAs = 200;
//
//    // Call the calcMGD function
//    u16 mgd_result = calcMGD(kv, thickness, target, filter, magnification, radOutput, mAs);
//
//    // Call the getIED function
//    u16 ied_result = getIED();
//    //int squre = square(5);
//    /*printf("Square: %d\n", squre);*/
//    // Print the results
//    printf("MGD Result: %u\n", mgd_result);
//    printf("IED Result: %u\n", ied_result);
//
//    return 0;
//}
