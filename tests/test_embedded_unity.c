#include "embedded.h"
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

void test_setup_clears_loop_counter(void)
{
    embedded_setup();

    TEST_ASSERT_EQUAL(0, get_loop_count());
}

void test_loop_increments_loop_counter(void)
{
    embedded_setup();
    embedded_loop();

    TEST_ASSERT_EQUAL(1, get_loop_count());
}

void test_loop_increments_loop_counter_2(void)
{
    embedded_setup();
    embedded_loop();
    embedded_loop();

    TEST_ASSERT_EQUAL(2, get_loop_count());
}
