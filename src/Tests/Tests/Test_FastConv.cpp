#include "MUSI6106Config.h"

#ifdef WITH_TESTS
#include "Synthesis.h"

#include "Vector.h"
#include "FastConv.h"

#include "gtest/gtest.h"


namespace fastconv_test {
    void CHECK_ARRAY_CLOSE(float* buffer1, float* buffer2, int iLength, float fTolerance)
    {
        for (int i = 0; i < iLength; i++)
        {
//            std::cout << "buffer1[i]  " << buffer1[i] << std::endl;
//            std::cout << "buffer2[i]  " << buffer2[i] << std::endl;
            EXPECT_NEAR(buffer1[i], buffer2[i], fTolerance);
        }
    }

    class FastConv: public testing::Test
    {
    protected:
        void SetUp() override
        {
            m_pCFastConv = new CFastConv();
        }

        virtual void TearDown()
        {
            m_pCFastConv->reset();
            delete m_pCFastConv;
        }
        CFastConv *m_pCFastConv = 0;
    };

    

    TEST_F(FastConv, timedomainidentity)
    {
        const int iBlockLength = 2;
        const int iDelayLength = 3;
        // generate a random IR of length 51 samples
        const int iIRLength = 51; //!!!!!!!! change it back
        float randIR[iIRLength];
        srand (static_cast <unsigned> (42));
        for(int i=0; i<iIRLength; i++)
           //randIR[i] = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
            randIR[i] = 0.5f; //change it back later
        
        const int iInputLength = 10;
        float input[iInputLength] = {0.f};
        input[iDelayLength] = 1.f;
        
        const int iOutputlength = iIRLength + iInputLength + iBlockLength;
        float delayedOutput[iOutputlength] = {0.f};
        
        //!!!!!!!! change it back
        for(int i=iDelayLength; i<iOutputlength-iInputLength; i++)
            delayedOutput[i] = randIR[i-iDelayLength]; // fill the delayedOutput with randIR
        
        //do the convolution
        m_pCFastConv->init(randIR, iIRLength, iBlockLength, CFastConv::kTimeDomain);
        float convOutput[iOutputlength] = {0.f};
        m_pCFastConv->process(convOutput, input, iInputLength);
        m_pCFastConv->flushBuffer(convOutput);
        m_pCFastConv->reset();
        
        //check if the convolution output matches the delayed output
        CHECK_ARRAY_CLOSE(delayedOutput, convOutput, iOutputlength, 0.0001f);
        
        
        
        
    }

    TEST_F(FastConv, timedomainflushbuffer)
    {
        float TestImpulse[51] = { 0 };
        float TestInput[10] = { 0 };
        float TestOutput[10] = { 0 };
        float TestFlush[50] = { 0 };
        float CheckOutput[60] = { 0 };
        TestInput[3] = 1;
        for (int i = 0; i < 51; i++)
        {
            TestImpulse[i] = static_cast<float>(std::rand()) / (static_cast <float> (RAND_MAX));
            TestImpulse[i] = TestImpulse[i] * 2.0 - 1.0;
            CheckOutput[i + 3] = TestImpulse[i];
        }


        m_pCFastConv->init(TestImpulse, 51, 0, CFastConv::kTimeDomain);
        m_pCFastConv->process(TestOutput, TestInput, 10);
        m_pCFastConv->flushBuffer(TestFlush);

        CHECK_ARRAY_CLOSE(TestOutput, CheckOutput, 10, 1e-3);
        CHECK_ARRAY_CLOSE(TestFlush, TestImpulse + 7, 51 - 7, 1e-3);
        m_pCFastConv->reset();
    }

    TEST_F(FastConv, timedomainblocksize)
    {
        float TestImpulse[51] = { 0 };
        float TestInput[10000] = { 0 };
        float TestOutput[10000] = { 0 };
        int BufferSize[8] = { 1, 13, 1023, 2048, 1, 17, 5000, 1897 };
        int InputStartIdx[8] = { 0 }; // All of the buffersizes add up to 10000, so we can just start the reading of the input at shifted positions

        // generate IR of length 51 samples
        for (int i = 0; i < 51; i++)
        {
            TestImpulse[i] = static_cast<float>(std::rand()) / (static_cast <float> (RAND_MAX));
            TestImpulse[i] = TestImpulse[i] * 2.0 - 1.0;
        }

        m_pCFastConv->init(TestImpulse, 51, 1024, CFastConv::kTimeDomain);
        for (int i = 0; i < 8; i++)
        {
            if (i == 0) {
                InputStartIdx[i] = 0;
            }
            else {
                InputStartIdx[i] = InputStartIdx[i-1] + BufferSize[i-1];
            }

            // set all beginnings of new block sizes to 1 for easy testing of convolution
            TestInput[InputStartIdx[i]] = 1;
            // reinitialize the convolution every time bc this is the easiest way to reset the pointers in the IR
            m_pCFastConv->init(TestImpulse, 51, 1024, CFastConv::kTimeDomain);

            m_pCFastConv->process(TestOutput + InputStartIdx[i], TestInput + InputStartIdx[i], BufferSize[i]);
            CHECK_ARRAY_CLOSE(TestOutput + InputStartIdx[i], TestImpulse, std::min(BufferSize[i], 51), 1e-3);

        }
        m_pCFastConv->reset();
    }


    TEST_F(FastConv, freqdomainidentity)
    {
    }

    TEST_F(FastConv, freqdomainflushbuffer)
    {
    }

    TEST_F(FastConv, freqdomainblocksize)
    {
    }
}

#endif //WITH_TESTS

