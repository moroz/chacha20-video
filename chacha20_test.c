#include <CUnit/Basic.h>
#include <CUnit/CUnit.h>
#include <CUnit/TestDB.h>

#include "chacha20.h"

void test_chacha20_qround(void) {
  uint32_t a, b, c, d;
  a = 0x11111111;
  b = 0x01020304;
  c = 0x9b8d6f43;
  d = 0x01234567;
  chacha20_qround(&a, &b, &c, &d);
  CU_ASSERT_EQUAL(a, 0xea2a92f4);
  CU_ASSERT_EQUAL(b, 0xcb1cf8ce);
  CU_ASSERT_EQUAL(c, 0x4581472e);
  CU_ASSERT_EQUAL(d, 0x5881c4bb);
}

int main(void) {
  CU_initialize_registry();
  CU_pSuite suite = CU_add_suite("chacha20_suite", 0, 0);
  CU_add_test(suite, "test basic qround function", test_chacha20_qround);
  CU_basic_run_tests();
  CU_cleanup_registry();

  return 0;
}
