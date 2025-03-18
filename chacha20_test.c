#include <CUnit/Basic.h>
#include <CUnit/CUnit.h>
#include <CUnit/TestDB.h>
#include <string.h>

#include "chacha20.h"

void test_chacha20_qround(void);
void test_chacha20_qround_on_state(void);

int main(void) {
  CU_initialize_registry();
  CU_pSuite suite = CU_add_suite("chacha20_suite", 0, 0);
  CU_add_test(suite, "test basic qround function", test_chacha20_qround);
  CU_add_test(suite, "test QROUND macro on Chacha20 state struct",
              test_chacha20_qround_on_state);
  CU_basic_run_tests();
  CU_cleanup_registry();

  return 0;
}

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

static Chacha20 CHACHA20_QROUND_TEST_INITIAL_VECTOR = {
    .words = {0x879531e0, 0xc5ecf37d, 0x516461b1, 0xc9a62f8a, 0x44c20ef3,
              0x3390af7f, 0xd9fc690b, 0x2a5f714c, 0x53372767, 0xb00a5631,
              0x974c541a, 0x359e9963, 0x5c971061, 0x3d631689, 0x2098d9d6,
              0x91dbd320}};

static Chacha20 CHACHA20_QROUND_TEST_EXPECTED_VECTOR = {
    .words = {0x879531e0, 0xc5ecf37d, 0xbdb886dc, 0xc9a62f8a, 0x44c20ef3,
              0x3390af7f, 0xd9fc690b, 0xcfacafd2, 0xe46bea80, 0xb00a5631,
              0x974c541a, 0x359e9963, 0x5c971061, 0xccc07c79, 0x2098d9d6,
              0x91dbd320}};

void test_chacha20_qround_on_state(void) {
  Chacha20 state;
  memcpy(state.words, CHACHA20_QROUND_TEST_INITIAL_VECTOR.words,
         16 * sizeof(uint32_t));
  QROUND(&state, 2, 7, 8, 13);

  for (int i = 0; i < 16; i++) {
    CU_ASSERT_EQUAL(state.words[i],
                    CHACHA20_QROUND_TEST_EXPECTED_VECTOR.words[i]);
  }
}
