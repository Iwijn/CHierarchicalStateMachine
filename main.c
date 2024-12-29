#include <stdio.h>
#include "unit_tests.h"
#include "integration_tests.h"


int main() {
    run_unit_tests();
    printf("\n\n\n");
    run_integration_tests();
}


