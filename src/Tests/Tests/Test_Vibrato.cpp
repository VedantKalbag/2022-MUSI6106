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
            // You can do set-up work for each test here
            fDelayInSec = 0.1f;
            fDepthInSec = 0.05f;
            fSampleRateInHz = 44100.f;
            fFrequencyInHz = 1.5f;
            iNumChannels = 1;
        }

        ~VibratoTest() override {
            // You can do clean-up work that doesn't throw exceptions here.
        }

        // If the constructor and destructor are not enough for setting up
        // and cleaning up each test, you can define the following methods:

        void SetUp() override {
            // Code here will be called immediately after the constructor (right
            // before each test).
            CVibrato::create(vibrato, fDelayInSec, fDepthInSec, fSampleRateInHz, fFrequencyInHz, iNumChannels);
        }

        void TearDown() override {
            // Code here will be called immediately after each test (right
            // before the destructor).
            CVibrato::destroy(vibrato);
        }

        // Class members declared here can be used by all tests in the test suite
        CVibrato *vibrato;
        float fDelayInSec;
        float fDepthInSec;
        float fSampleRateInHz;
        float fFrequencyInHz;
        int iNumChannels;
    };

    TEST_F(VibratoTest, SetParamTest)
    {
        EXPECT_EQ(vibrato->setParam(CVibrato::kFrequency, -1.5f), Error_t::kFunctionInvalidArgsError);
        EXPECT_EQ(vibrato->setParam(CVibrato::kDelay, -0.1f), Error_t::kFunctionInvalidArgsError);
        EXPECT_EQ(vibrato->setParam(CVibrato::kWidth, -0.1f), Error_t::kFunctionInvalidArgsError);

    }
    TEST_F(VibratoTest, SetWidthGreaterThanDelay)
    {
        vibrato->setParam(CVibrato::kDelay, 0.1f);
        EXPECT_EQ(vibrato->setParam(CVibrato::kWidth, 0.2f), Error_t::kFunctionInvalidArgsError);
    }

    TEST_F(VibratoTest, GetEqualsSet)
    {
        vibrato->setParam(CVibrato::kDelay, 0.1f);
        EXPECT_NEAR(vibrato->getParam(CVibrato::kDelay), 0.1f, 1e-4);

        vibrato->setParam(CVibrato::kWidth, 0.05f);
        EXPECT_NEAR(vibrato->getParam(CVibrato::kWidth), 0.05f, 1e-4);

        vibrato->setParam(CVibrato::kFrequency, 42.f); //FIXME: This line calls setFreq on a NULL object, the instance of LFO no longer exists here
        EXPECT_NEAR(vibrato->getParam(CVibrato::kFrequency), 42.f, 1e-4);
    }
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
