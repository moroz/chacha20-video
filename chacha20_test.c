#include <CUnit/Basic.h>
#include <CUnit/CUnit.h>
#include <CUnit/TestDB.h>

void test_chacha20_qround(void) {}

int main(void) {
  CU_initialize_registry();
  CU_pSuite suite = CU_add_suite("chacha20_suite", 0, 0);
  CU_add_test(suite, "test basic qround function", test_chacha20_qround);
  CU_basic_run_tests();
  CU_cleanup_registry();

  return 0;
}
