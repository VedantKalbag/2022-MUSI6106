
#include "FastConv.h"
#include "Util.h"

CFastConv::CFastConv( void )
{
}

CFastConv::~CFastConv( void )
{
    reset();
}

Error_t CFastConv::init(float* pfImpulseResponse, int iLengthOfIr, int iBlockLength /*= 8192*/, ConvCompMode_t eCompMode /*= kFreqDomain*/)
{
    m_iCirIdx = 0;
    
    m_iBlockLength = iBlockLength;
    m_pfInputBlock = new float [m_iBlockLength*2];
    for (int i=0; i<m_iBlockLength*2; i++)
        m_pfInputBlock[i] = 0.f;
    
    m_pfOutputBlock = new float [m_iBlockLength];
    for (int i=0; i<m_iBlockLength; i++)
        m_pfOutputBlock[i] = 0.f;
    
    m_pfBlockConvOuput = new float [m_iBlockLength*2];
    for (int i=0; i<m_iBlockLength*2; i++)
        m_pfBlockConvOuput[i] = 0.f;
    
    
    //slice pfImpulseResponse into m_pfIRMatrix (iBlockNum, iBlockLength)
    int iReminder = iLengthOfIr % iBlockLength;
    m_iBlockNum = (int) (1 + (iLengthOfIr - iReminder) / iBlockLength);
    
    m_pfIRMatrix = new float * [m_iBlockNum];
    for (int c=0; c<m_iBlockNum; c++)
    {
        m_pfIRMatrix[c] = new float [iBlockLength*2];
        for (int i=0; i<m_iBlockLength*2; i++)
            m_pfIRMatrix[c][i] = 0.f;
    }
    
    int c = -1;
    int reset = -iBlockLength;
    for (int i=0; i<iLengthOfIr; i++)
    {
        if (i % iBlockLength == 0)
        {
            c += 1;
            reset += iBlockLength;
        }

        m_pfIRMatrix[c][i-reset] = pfImpulseResponse[i];
    }

        
    m_pfConvMatrix = new float * [m_iBlockNum+1];
    for (int c=0; c<m_iBlockNum+1; c++)
    {
        m_pfConvMatrix[c] = new float [iBlockLength*(m_iBlockNum+1)];
        for (int i=0; i<iBlockLength*(m_iBlockNum+1); i++)
            m_pfConvMatrix[c][i] = 0.f;
    }
    
    m_pfConvOuput = new float [(m_iBlockNum+1)*m_iBlockLength];
    for (int i=0; i<(m_iBlockNum+1)*m_iBlockLength; i++)
        m_pfConvOuput[i] = 0.f;
    
    
    
    
    
    
    //if kFreqDomain, transform the IR to frequency domain
    if (eCompMode == kTimeDomain)
    {
        //do something
    }
    else if (eCompMode == kFreqDomain)
    {
        //do something
    }
    else
    {
        std::cout<< "ConvCompMode_t has to be either kTimeDomain or kFreqDomain";
        return Error_t::kFunctionInvalidArgsError;
    }

    
    m_bIsInitialized = true;

    return Error_t::kNoError;
}

Error_t CFastConv::reset()
{
    if(m_bIsInitialized)
    {
        for (int c=0; c<m_iBlockNum; c++)
            delete[] m_pfIRMatrix[c];
        delete m_pfIRMatrix;
        
        for (int c=0; c<m_iBlockNum+1; c++)
            delete[] m_pfConvMatrix[c];
        delete m_pfConvMatrix;
        
        delete[] m_pfInputBlock;
        delete[] m_pfOutputBlock;
        delete[] m_pfBlockConvOuput;
    }
    
    m_bIsInitialized = false;
    
    
    
    
    return Error_t::kNoError;
}


Error_t CFastConv::conv(float* pfInputBlock, float** m_pfIRMatrix, float** pfConvMatrix)
{
    
    for (int i = 0; i<m_iBlockNum; i++)
    {
        //switch to freqConv() after implemented
        timeConv(pfInputBlock, m_pfIRMatrix[i], m_pfBlockConvOuput);

        for (int n = 0; n < m_iBlockLength*2; n++)
        {
            pfConvMatrix[0][n+i*m_iBlockLength] += m_pfBlockConvOuput[n];
        }
        
        for (int m = 0; m < m_iBlockNum+1; m++)
        {
            for (int j = 0; j < m_iBlockLength; j++)
                m_pfOutputBlock[j] = m_pfOutputBlock[j] + pfConvMatrix[m][m*m_iBlockLength+j];
        }
        
        
    }
        
        
        
    
    return Error_t::kNoError;
}


Error_t CFastConv::timeConv(float* pfBuffer1, float* pfBuffer2, float* pfBlockConvOuput)
{
    
    int c = 0;
    int reset = -m_iBlockLength;
    for (int i=0; i<2*m_iBlockLength; i++)
    {
        float sum = 0.f;
        for (int m = 0; m < 2*m_iBlockLength-1; m++)
        {
            sum += pfBuffer1[m] * pfBuffer2[m_iBlockLength - m];
        }
        
        pfBlockConvOuput[i-m_iBlockLength] = sum;
            
        if (i>=m_iBlockLength){
            reset += m_iBlockLength;
        }
    }
    
    return Error_t::kNoError;
    
}

Error_t CFastConv::process (float* pfOutputBuffer, const float *pfInputBuffer, int iLengthOfBuffers )
{
    if (!m_bIsInitialized)
    {
        std::cout<< "not initialized";
        return Error_t::kNotInitializedError;
    }
    

    int reset = -m_iBlockLength;
    
    for (int i=0; i<iLengthOfBuffers; i++)
    {
        
        m_pfInputBlock[i-reset] = pfInputBuffer[i];
        
        if (i % m_iBlockLength == 0 and i != 0) //what is the best way counting numbers?
        {
            reset += m_iBlockLength;
            
            //output in pfBlockConvOuput
            conv(m_pfInputBlock, m_pfIRMatrix, m_pfConvMatrix);
            
            for (int n=0; n<m_iBlockLength; n++)
                pfOutputBuffer[n+reset+m_iBlockLength] = m_pfOutputBlock[n];
        
            //clear out m_pfOutputBlock
            
            if (++m_iCirIdx >= m_iBlockNum+1)
                m_iCirIdx = 0;

        }
        
    }
    
    
        
    
    return Error_t::kNoError;
}

Error_t CFastConv::flushBuffer(float* pfOutputBuffer)
{
    return Error_t::kNoError;
}
