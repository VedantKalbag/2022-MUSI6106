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

    TEST_F(FastConv, flushbuffer)
    {
    }

    TEST_F(FastConv, blocksize)
    {
    }

}

#endif //WITH_TESTS

