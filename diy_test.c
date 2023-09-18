#include "dose.h"
#include <stdio.h>
#include "diy_compare.c"

int main() {
    // Define some test parameters to test out the working code. 
    u8 kv = 80;
    u8 thickness = 50;
    u8 target = TARGET_MO;
    u8 filter = FILTER_MATERIAL_AG;
    u8 magnification = 180;
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


    //Now testing the results of the refactoring are equal....

    u16 mgd_test = calcMGDTest(kv, thickness, target, filter, magnification, radOutput, mAs);
    u16 ied_test = calcIEDTest(kv, thickness, target, filter, magnification, radOutput, mAs);

    // Compare the results
    if (mgd_result == mgd_test && ied_test == ied_result) {
        printf("Passed: MGD Test: %d\n", mgd_test);
        printf("        MGD Result: %d\n", mgd_result);
        printf("        IED Test: %d\n", ied_test);
        printf("        IED Result: %d\n", ied_result);
    }
    else {
        printf("Failed: MGD Test: %d\n", mgd_test);
        printf("        MGD Result: %d\n", mgd_result);
        printf("        IED Test: %d\n", ied_test);
        printf("        IED Result: %d\n", ied_result);
    }

    return 0;
}
