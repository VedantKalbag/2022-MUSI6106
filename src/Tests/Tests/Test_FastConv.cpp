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
            EXPECT_NEAR(buffer1[i], buffer2[i], fTolerance);
        }
    }

    class FastConv: public testing::Test
    {
    protected:
        void SetUp() override
        {
            
        }

        virtual void TearDown()
        {
            m_pCFastConv->reset();
            delete m_pCFastConv;
        }

        CFastConv *m_pCFastConv = new CFastConv();
    };

    

    TEST_F(FastConv, identity)
    {
        // generate a random IR of length 51 samples
        const int iIRLength = 51;
        float randIR[iIRLength];
        for(int i=0; i<iIRLength; i++)
           randIR[i] = rand();
        
        // an impulse as input signal at sample index 3
        // make the input signal 10 samples long
        const int iInputLength = 10;
        float input[iInputLength] = {0.f};
        input[3] = 1.f;
        
        const int iOutputlength = iIRLength + iInputLength; // maybe -1?
        float delayedOutput[iOutputlength];
        for(int i=3; i<iOutputlength; i++)
            delayedOutput[i] = randIR[i-3]; // fill the delayedOutput with randIR
        
        //do the convolution
        m_pCFastConv->init(randIR, iIRLength, 2, CFastConv::kTimeDomain);
        float convOutput[iOutputlength];
        m_pCFastConv->process(convOutput, input, iInputLength);
        m_pCFastConv->flushBuffer(convOutput);
        m_pCFastConv->reset();
        
        //check if the convolution output matches the delayed output
        CHECK_ARRAY_CLOSE(delayedOutput, convOutput, iOutputlength, 0.001f);
        
        
        
    }

    TEST_F(FastConv, flushbuffer)
    {
    }

    TEST_F(FastConv, blocksize)
    {
    }

}

#endif //WITH_TESTS

