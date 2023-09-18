#include "dose.h"
#include <stdio.h>

int main() {
    // Define some test parameters to test out the working code. 
    u8 kv = 80;
    u8 thickness = 50;
    u8 target = TARGET_MO;
    u8 filter = FILTER_MATERIAL_RH;
    u8 magnification = 0;
    u16 radOutput = 5000;
    u16 mAs = 200;

    // Call the calcMGD function
    u16 mgd_result = calcMGD(kv, thickness, target, filter, magnification, radOutput, mAs);

    // Call the getIED function
    u16 ied_result = getIED();
    //int squre = square(5);
    /*printf("Square: %d\n", squre);*/
    // Print the results
    printf("MGD Result: %u\n", mgd_result);
    printf("IED Result: %u\n", ied_result);

    return 0;
}
