#include "MUSI6106Config.h"

#ifdef WITH_TESTS
#include "Vector.h"
#include "Vibrato.h"

#include "gtest/gtest.h"


namespace vibrato_lfo_test {
    void CHECK_ARRAY_CLOSE(float* buffer1, float* buffer2, int iLength, float fTolerance)
    {
        for (int i = 0; i < iLength; i++)
        {
            EXPECT_NEAR(buffer1[i], buffer2[i], fTolerance);
        }
    }

    // add setup, teardown, writewav and deletefile
    class VibratoTest : public ::testing::Test{
    protected:
        // You can remove any or all of the following functions if their bodies would
        // be empty.

        VibratoTest() {
            // You can do set-up work for each test here.
        }

        ~VibratoTest() override {
            // You can do clean-up work that doesn't throw exceptions here.
        }

        // If the constructor and destructor are not enough for setting up
        // and cleaning up each test, you can define the following methods:

        void SetUp() override {
            // Code here will be called immediately after the constructor (right
            // before each test).

        }

        void TearDown() override {
            // Code here will be called immediately after each test (right
            // before the destructor).

        }

        // Class members declared here can be used by all tests in the test suite

    };
    class LFOTest : public ::testing::Test{
    protected:
        // You can remove any or all of the following functions if their bodies would
        // be empty.

        LFOTest() {
            // You can do set-up work for each test here.
        }

        ~LFOTest() override {
            // You can do clean-up work that doesn't throw exceptions here.
        }

        // If the constructor and destructor are not enough for setting up
        // and cleaning up each test, you can define the following methods:

        void SetUp() override {
            // Code here will be called immediately after the constructor (right
            // before each test).

        }

        void TearDown() override {
            // Code here will be called immediately after each test (right
            // before the destructor).

        }

        // Class members declared here can be used by all tests in the test suite

    };
}


#endif //WITH_TESTS
