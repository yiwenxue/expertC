#include <stdio.h>
#include <stdlib.h>
#include <fib.h>
#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>

void fib_test(){
    CU_ASSERT(fib(10) == 55);
    CU_ASSERT(fib(22) == 17711);
    CU_ASSERT(fib(40) == 102334155);
}

int main(int argc , char * argv[]){

    CU_initialize_registry();
    CU_pSuite suite1 = CU_add_suite("fib_test", 0, 0);

    CU_add_test(suite1, "fib_test", fib_test);

    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    CU_cleanup_registry();

    return 0;
}

