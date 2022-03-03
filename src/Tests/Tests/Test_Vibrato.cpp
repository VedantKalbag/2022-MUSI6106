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
            fWidthInSec = 0.05f;
            fSampleRateInHz = 44100.f;
            fFrequencyInHz = 1.5f;
            iNumChannels = 1;
            vibrato = nullptr;
        }

        ~VibratoTest() override {
            // You can do clean-up work that doesn't throw exceptions here.
        }

        // If the constructor and destructor are not enough for setting up
        // and cleaning up each test, you can define the following methods:

        void SetUp() override {
            // Code here will be called immediately after the constructor (right
            // before each test).
            CVibrato::create(vibrato, fDelayInSec, fWidthInSec, fSampleRateInHz, fFrequencyInHz, iNumChannels);
            ppfInputBuffer = new float*[iNumChannels];
            ppfOutputBuffer = new float*[iNumChannels];
            for (int channel = 0; channel < iNumChannels; channel++)
            {
                ppfInputBuffer[channel] = new float[iBufferLength];
                ppfOutputBuffer[channel] = new float[iBufferLength];
            }
        }

        void TearDown() override {
            // Code here will be called immediately after each test (right
            // before the destructor).
            CVibrato::destroy(vibrato);
            for (int channel = 0; channel < iNumChannels; channel++)
            {
                delete[] ppfInputBuffer[channel];
                delete[] ppfOutputBuffer[channel];
            }
            delete[] ppfInputBuffer;
            delete[] ppfOutputBuffer;
            ppfInputBuffer = nullptr;
            ppfOutputBuffer = nullptr;
        }

        // Class members declared here can be used by all tests in the test suite
        CVibrato *vibrato;
        float fDelayInSec;
        float fWidthInSec;
        float fSampleRateInHz;
        float fFrequencyInHz;
        int iNumChannels;
        float** ppfInputBuffer = nullptr;
        float** ppfOutputBuffer = nullptr;
        int iBufferLength = 1024;
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

        vibrato->setParam(CVibrato::kFrequency, 42.f);
        EXPECT_NEAR(vibrato->getParam(CVibrato::kFrequency), 42.f, 1e-4);
    }

    TEST_F(VibratoTest, BlockSizeTest)
    {
        float** tmpOutput = new float* [iNumChannels];
        for (int channel = 0; channel < iNumChannels; channel++)
        {
            tmpOutput[channel] = new float[iBufferLength];
            CSynthesis::generateNoise(ppfInputBuffer[channel], iBufferLength);
        }

        EXPECT_EQ(vibrato->process(ppfInputBuffer, ppfOutputBuffer, iBufferLength),Error_t::kNoError);
        EXPECT_EQ(vibrato->process(ppfInputBuffer,tmpOutput,1021),Error_t::kNoError);
        for (int c = 0; c < iNumChannels; c++)
        {
            CHECK_ARRAY_CLOSE(tmpOutput[c], ppfOutputBuffer[c], iNumChannels, 1E-5);
        }
        for(int i=0;i<iNumChannels;i++)
            delete[] tmpOutput[i];
        delete[] tmpOutput;
    }

    TEST_F(VibratoTest, OutputEqualsDelayedInput)
    {
        for (int channel = 0; channel < iNumChannels; channel++)
        {
            CSynthesis::generateSine(ppfInputBuffer[channel],fFrequencyInHz, fSampleRateInHz, iBufferLength, 1);
        }
        vibrato->setParam(CVibrato::CVibratoParam::kWidth, 0);
        vibrato->process(ppfInputBuffer, ppfOutputBuffer, iBufferLength);
        int total_delay_samples = static_cast<int>(2 + (fWidthInSec * 2 * fSampleRateInHz));//+ (fDelayInSec * fSampleRate);


        for (int channel = 0; channel < iNumChannels; channel++)
        {
            CHECK_ARRAY_CLOSE(ppfInputBuffer[channel] + total_delay_samples, ppfOutputBuffer[channel],
                              iBufferLength - total_delay_samples, 1e-3);
        }
    }

    TEST_F(VibratoTest, DCStaysDC)
    {
        for (int channel = 0; channel < iNumChannels; channel++)
        {
            CSynthesis::generateDc(ppfInputBuffer[channel], iBufferLength, 1);
        }
        vibrato->process(ppfInputBuffer, ppfOutputBuffer, iBufferLength);
        // Since the output file will be delayed, we need to calculate how many samples it is delayed by and offset that when checking the buffers
        // L=2+DELAY+WIDTH*2; - delay accounted for elsewhere? Trial and error showed the below value is right
        int total_delay_samples = static_cast<int>(2 + (fWidthInSec * 2 * fSampleRateInHz));//+ (fDelayInSec * fSampleRate);


        for (int channel = 0; channel < iNumChannels; channel++)
        {
            CHECK_ARRAY_CLOSE(ppfInputBuffer[channel] + total_delay_samples, ppfOutputBuffer[channel],
                              iBufferLength - total_delay_samples, 1e-3);
        }
    }



    class LFOTest : public ::testing::Test{
    protected:
        // You can remove any or all of the following functions if their bodies would
        // be empty.

        LFOTest() {
            // You can do set-up work for each test here.
            lfoSine = nullptr;
            lfoDC = nullptr;
            fSampleRate = 44100;
            waveformSine = LFO::Sine;
            waveformDC = LFO::Dc;
            fFrequencyInHertz = 10;
            fWidth = 5;
        }

        ~LFOTest() override {
            // You can do clean-up work that doesn't throw exceptions here.
        }

        // If the constructor and destructor are not enough for setting up
        // and cleaning up each test, you can define the following methods:

        void SetUp() override {
            // Code here will be called immediately after the constructor (right
            // before each test).
            lfoSine = new LFO(fSampleRate, waveformSine, fFrequencyInHertz, fWidth);
            lfoDC = new LFO(fSampleRate, waveformDC, fFrequencyInHertz, fWidth);

        }

        void TearDown() override {
            // Code here will be called immediately after each test (right
            // before the destructor).
            delete lfoSine;
            delete lfoDC;

        }

        // Class members declared here can be used by all tests in the test suite
        LFO* lfoSine;
        LFO* lfoDC;
        float fSampleRate;
        LFO::Wavetable waveformSine;
        LFO::Wavetable waveformDC;
        float fFrequencyInHertz;
        float fWidth;

    };

    TEST_F(LFOTest, InitTest)
    {
        EXPECT_TRUE(lfoSine);
        EXPECT_TRUE(lfoDC);
    }

    TEST_F(LFOTest, WidthTest)
    {
        float tolerance = 0.05; 
        float maxValue = 0;
        float auxValue = 0;
        int lfoArrayLength = lfoSine->getLength();
        float* pWavetableArray_1 = new float[lfoArrayLength];
        
        for (int i = 0; i < lfoArrayLength; i++) {
            auxValue = lfoSine->readSample();
            if (auxValue > maxValue)
                maxValue = auxValue;
        }
        EXPECT_NEAR(maxValue, fWidth, tolerance);

        float testFreq = 5;
        lfoSine->setFreq(testFreq);
        int lfoArrayLengthTest2 = static_cast<int> (lfoArrayLength / (fFrequencyInHertz / testFreq));
        float* pWavetableArray_2 = new float[lfoArrayLengthTest2];

        auxValue = 0;
        maxValue = 0;
        for (int i = 0; i < lfoArrayLengthTest2; i++) {
            auxValue = lfoSine->readSample();
            if (auxValue > maxValue)
                maxValue = auxValue;
        }
        EXPECT_NEAR(maxValue, fWidth, tolerance);
        

        delete[] pWavetableArray_1;
        delete[] pWavetableArray_2;
    }


    class RingBufferTest : public ::testing::Test{
    protected:
        // You can remove any or all of the following functions if their bodies would
        // be empty.

        RingBufferTest() {
            // You can do set-up work for each test here.
        }

        ~RingBufferTest() override {
            // You can do clean-up work that doesn't throw exceptions here.
        }

        // If the constructor and destructor are not enough for setting up
        // and cleaning up each test, you can define the following methods:

        void SetUp() override {
            // Code here will be called immediately after the constructor (right
            // before each test).
            ringBuffer = new CRingBuffer<float>(5);
            for(int i=0;i<ringBuffer->getLength();i++)
            {
                ringBuffer->putPostInc(i+1);
            }
            ringBuffer->setReadIdx(0);
        }

        void TearDown() override {
            // Code here will be called immediately after each test (right
            // before the destructor).
            delete ringBuffer;
        }

        // Class members declared here can be used by all tests in the test suite
        CRingBuffer<float> *ringBuffer;

    };

    TEST_F(RingBufferTest, InterpolationTest)
    {
        EXPECT_NEAR(ringBuffer->get(1.5f), 2.5f,1e-4f);
    }

    TEST_F(RingBufferTest, WrapAroundTest)
    {
        for(int i=0;i<ringBuffer->getLength();i++)
        {
            ringBuffer->getPostInc();
        }
        EXPECT_EQ(ringBuffer->getReadIdx(), 0);
        EXPECT_EQ(ringBuffer->getWriteIdx(), 0);
    }
}


#endif //WITH_TESTS
