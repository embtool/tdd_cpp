#include "CppUTest/TestHarness.h"
#include "embedded.h"

TEST_GROUP (Embedded)
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

TEST(Embedded, SetupClearsLoopCounter)
{
    embedded_setup();

    CHECK_EQUAL(0, get_loop_count());
}

TEST(Embedded, LoopIncrementsLoopCounter)
{
    embedded_setup();
    embedded_loop();

    CHECK_EQUAL(1, get_loop_count());
}

TEST(Embedded, LoopIncrementsLoopCounter_2)
{
    embedded_setup();
    embedded_loop();
    embedded_loop();

    CHECK_EQUAL(2, get_loop_count());
}
