#include <CUnit/Basic.h>
#include <CUnit/CUnit.h>
#include <CUnit/TestDB.h>
#include <string.h>

#include "chacha20.h"

void test_chacha20_qround(void);
void test_chacha20_qround_on_state(void);
void test_chacha20_setup_block(void);
void test_chacha20_block(void);

int main(void) {
  CU_initialize_registry();
  CU_pSuite suite = CU_add_suite("chacha20_suite", 0, 0);
  CU_add_test(suite, "test basic qround function", test_chacha20_qround);
  CU_add_test(suite, "test QROUND macro on Chacha20 state struct",
              test_chacha20_qround_on_state);
  CU_add_test(suite, "test chacha20_setup_block", test_chacha20_setup_block);
  CU_add_test(suite, "test chacha20_block", test_chacha20_block);
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

static const Chacha20 CHACHA20_QROUND_TEST_INITIAL_VECTOR = {
    .words = {0x879531e0, 0xc5ecf37d, 0x516461b1, 0xc9a62f8a, 0x44c20ef3,
              0x3390af7f, 0xd9fc690b, 0x2a5f714c, 0x53372767, 0xb00a5631,
              0x974c541a, 0x359e9963, 0x5c971061, 0x3d631689, 0x2098d9d6,
              0x91dbd320}};

static const Chacha20 CHACHA20_QROUND_TEST_EXPECTED_VECTOR = {
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

static const uint8_t TEST_CHACHA20_SETUP_KEY[] = {
    0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a,
    0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15,
    0x16, 0x17, 0x18, 0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f};

static const uint8_t TEST_CHACHA20_SETUP_NONCE[] = {
    0x00, 0x00, 0x00, 0x09, 0x00, 0x00, 0x00, 0x4a, 0x00, 0x00, 0x00, 0x00};

static Chacha20 TEST_CHACHA20_SETUP_EXPECTED_STATE = {
    .words = {0x61707865, 0x3320646e, 0x79622d32, 0x6b206574, 0x03020100,
              0x07060504, 0x0b0a0908, 0x0f0e0d0c, 0x13121110, 0x17161514,
              0x1b1a1918, 0x1f1e1d1c, 0x00000001, 0x09000000, 0x4a000000,
              0x00000000}};

void test_chacha20_setup_block(void) {
  Chacha20 state;
  chacha20_setup_block(&state, TEST_CHACHA20_SETUP_KEY,
                       TEST_CHACHA20_SETUP_NONCE, 1);
  for (int i = 0; i < 16; i++) {
    CU_ASSERT_EQUAL(state.words[i],
                    TEST_CHACHA20_SETUP_EXPECTED_STATE.words[i]);
  }
}

static const Chacha20 TEST_CHACHA20_BLOCK_EXPECTED_STATE = {
    .words = {0xe4e7f110, 0x15593bd1, 0x1fdd0f50, 0xc47120a3, 0xc7f4d1c7,
              0x0368c033, 0x9aaa2204, 0x4e6cd4c3, 0x466482d2, 0x09aa9f07,
              0x05d7c214, 0xa2028bd9, 0xd19c12b5, 0xb94e16de, 0xe883d0cb,
              0x4e3c50a2}};

void test_chacha20_block(void) {
  Chacha20 state;
  chacha20_block(&state, TEST_CHACHA20_SETUP_KEY, TEST_CHACHA20_SETUP_NONCE, 1);
  for (int i = 0; i < 16; i++) {
    CU_ASSERT_EQUAL(state.words[i],
                    TEST_CHACHA20_BLOCK_EXPECTED_STATE.words[i]);
  }
}
