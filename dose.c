//#include "types.h"
#include "tables.h"
#include "parameters.h"
#include <stdio.h>
#include "dose.h"



u16 getIED( void )
{
  return IED; 
}

u16 calcMgdIedAnode(const u16 material[][16], u8 kv, u8 p, u32 interpol, u8 filter)
{
    return material[p][kv - 20] - ((interpol * (material[p][kv - 20] - material[p + 1][kv - 20]) + 500) / 1000);
}

// AGD calculation
u16 calcMGD(u8 kv, u8 thickness, u8 target, u8 filter, u8 magnification, u16 radOutput, u16 mAs ) 
{
  u32 mgd = 0;
  u32 interpol;
  u8 limited_mm;
  u8 p;

  //simplify this if statments with ||
  if (thickness == 0 || thickness > 120) // use OR
      IED = 0;
      return 0;
  // esp. in man mode could expose with paddle up and then values become totally bogus

  // thickness must be limited between 10 - 100 mm
  // i CAN USE THE  ternary operator here c? T:F
  limited_mm = thickness < 10 ? 10 : (thickness > 100 ? 100 : thickness);

  IED = calcIED(kv, limited_mm, target, filter, magnification, radOutput, mAs );

  // find correct thickness index 0 - 7 for table seek
  // starting from high mm seek until equal or lower value is found from the table
 // simplify the for and if with a while loop
  p = 7;
  while (p != 0 && mm_index[p] >= limited_mm)
  {
      p--;
  }
  // calculate thickness interpolation factor

  interpol = (1000 * (limited_mm - mm_index[p])) / (mm_index[p+1] - mm_index[p]);

  // thickness interpolate AGD-value from table that has pre-calculated (*10000) values for
  // EUREF sheet D0001629-2 c, g and s-factors
  if (target == TARGET_MO) {
      mgd = filter == FILTER_MATERIAL_RH ? calcMgdIedAnode(MGD_Rh, kv, p, interpol, filter) : calcMgdIedAnode(MGD_Mo, kv, p, interpol, filter);
  }
  else {
      mgd = filter == FILTER_MATERIAL_AG ? calcMgdIedAnode(MGD_W_Ag, kv, p, interpol, filter) : calcMgdIedAnode(MGD_W_Rh, kv, p, interpol, filter);
  }

  mgd = mgd * IED;

  mgd = (u16)((mgd+5000) / 10000); // to make LSB of mgd equal 10 uGy for protocol spec and accuracy correctio / 1000
  return (u16)mgd;
}


// IED (Incident Entrance Dose) calculation

static u16 calcIED(u8 kv, u8 thickness, u8 target, u8 filter, u8 magnification, u16 radOutput, u16 mAs )
{
  u32 ied = 0;
  u8 p;
  u32 interpol;
  u8 limited_mm=thickness;

  // find correct thickness index   0 - 7 for table seek
  // starting from high mm seek until equal or lower value is found from the table
  for( p=7;p!=0;p-- ) {
    if( mm_index[p]<=limited_mm )
      break;
  }
  
  // calculate thickness interpolation factor

  interpol = (1000 * (limited_mm - mm_index[p])) / (mm_index[p+1] - mm_index[p]);

  // thickness interpolate IED-value from table that has pre-calculated (*10000) values for
  // kV and thickness dependent radiation output factor 

  if (target == TARGET_MO ) 
  {          // Mo anode
    if (magnification > 170) 
    {
      ied = filter == FILTER_MATERIAL_MO ? calcMgdIedAnode(IED_mag18_Mo, kv, p, interpol, filter) : calcMgdIedAnode(IED_mag18_Rh, kv, p, interpol, filter);
    }
    else if (magnification > 130)
    {
      ied = filter == FILTER_MATERIAL_MO ? calcMgdIedAnode(IED_mag16_Mo, kv, p, interpol, filter) : calcMgdIedAnode(IED_mag16_Rh, kv, p, interpol, filter);
    }
    else 
    {
      ied = filter == FILTER_MATERIAL_MO ? calcMgdIedAnode(IED_Mo, kv, p, interpol, filter) : calcMgdIedAnode(IED_Rh, kv, p, interpol, filter);
    }
  }   
  else 
  {         // W anode
    if (magnification >170) 
    {
     ied = filter == FILTER_MATERIAL_MO ? calcMgdIedAnode(IED_W_mag18_Ag, kv, p, interpol, filter) : calcMgdIedAnode(IED_W_mag18_Rh, kv, p, interpol, filter);
    }
    else if ( magnification >130)
    {
     ied = filter == FILTER_MATERIAL_MO ? calcMgdIedAnode(IED_W_mag16_Ag, kv, p, interpol, filter) : calcMgdIedAnode(IED_W_mag16_Rh, kv, p, interpol, filter);
    }
    else 
    {
    ied = filter == FILTER_MATERIAL_MO ? calcMgdIedAnode(IED_W_Ag, kv, p, interpol, filter) : calcMgdIedAnode(IED_W_Rh, kv, p, interpol, filter);

    }
  }
 
  // IED value is multiplied by mAs and tube radiation output µGy/mAs

  ied = ((ied * mAs ) + 500) / 1000; 
  ied = ((ied * radOutput ) + 500 ) / 1000;

  return (u16)ied;
}



int main() {
    // Define some test parameters
    u8 kv = 80;
    u8 thickness = 50;
    u8 target = TARGET_MO;
    u8 filter = FILTER_MATERIAL_RH;
    u8 magnification = 150;
    u16 radOutput = 5000;
    u16 mAs = 200;

    // Call the calcMGD function
    u16 mgd_result = calcMGD(kv, thickness, target, filter, magnification, radOutput, mAs);

    // Call the getIED function
    u16 ied_result = getIED();

    // Print the results
    printf("MGD Result: %u\n", mgd_result);
    printf("IED Result: %u\n", ied_result);

    return 0;
}
