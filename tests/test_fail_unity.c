#include "unity.h"

/** @brief Test suite set-up.
 *
 * Executed once before the first test.
 */
void suiteSetUp(void)
{
}

/** @brief Test suite tear-down.
 *
 * Executed once after the last test.
 *
 * @param num_failures Receives the number of failures detected by the
 * test framework.
 * @return Test execution status, main()'s return value: 0 for success,
 * 1 for failure.
 */
int suiteTearDown(int num_failures)
{
    return num_failures != 0;
}

/** @brief Test set-up.
 *
 * Executed before each test.
 */
void setUp(void)
{
}

/** @brief Test tear-down.
 *
 * Executed after each test.
 */
void tearDown(void)
{
}

/*
 * For the test runner to detect the tests they must have prototype like
 * these:
 *
 * void test_*(void);
 * void spec_*(void);
 */

void test_just_pass(void)
{
    TEST_ASSERT_EQUAL(0, 0);
}

void test_just_fail(void)
{
    TEST_ASSERT_EQUAL(0, 1);
}
