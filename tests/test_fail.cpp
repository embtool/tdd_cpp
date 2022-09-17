#include "CppUTest/TestHarness.h"

TEST_GROUP (Fail)
{
    /** @brief Test set-up.
     *
     * Executed before each test.
     */
    void setup()
    {
    }

    /** @brief Test tear-down.
     *
     * Executed after each test.
     */
    void teardown()
    {
    }
};

TEST(Fail, JustPass)
{
    CHECK_EQUAL(0, 0);
}

TEST(Fail, JustFail)
{
    CHECK_EQUAL(0, 1);
}
