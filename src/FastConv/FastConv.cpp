
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
    //set processing mode
    if (eCompMode!= kTimeDomain and eCompMode!= kFreqDomain)
    {
        std::cout<< "ConvCompMode_t has to be either kTimeDomain or kFreqDomain";
        return Error_t::kFunctionInvalidArgsError;
    }
    m_eCompMode = eCompMode;
    
    this->reset();

    
    m_iCirIdx = 0;
    m_iLengthOfIr = iLengthOfIr;
    
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
    m_iBlockNum = (int) (iLengthOfIr - iReminder) / iBlockLength;
    
    m_pfIRMatrix = new float * [m_iBlockNum];
    for (int c=0; c<m_iBlockNum; c++)
    {
        m_pfIRMatrix[c] = new float [iBlockLength*2];
        for (int i=0; i<m_iBlockLength*2; i++)
            m_pfIRMatrix[c][i] = 0.f;
    }
    
    int c = 0;
    int reset = 0;
    for (int i=0; i<iLengthOfIr; i++)
    {
        m_pfIRMatrix[c][i-reset] = pfImpulseResponse[i];
        
        if ((i+1) % iBlockLength == 0)
        {
            c += 1;
            reset += iBlockLength;
        }
    }

        
    m_pfConvMatrix = new float * [m_iBlockNum+1];
    for (int c=0; c<m_iBlockNum+1; c++)
    {
        m_pfConvMatrix[c] = new float [iBlockLength*(m_iBlockNum+1)];
        for (int i=0; i<iBlockLength*(m_iBlockNum+1); i++)
            m_pfConvMatrix[c][i] = 0.f;
    }
    
    m_pfConvOuput = new float [m_iBlockNum*m_iBlockLength];
    for (int i=0; i<m_iBlockNum*m_iBlockLength; i++)
        m_pfConvOuput[i] = 0.f;
    
    
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
        delete[] m_pfConvOuput;
        
        m_iBlockNum = 0;
        m_iCirIdx = 0;
        m_iBlockLength = 0;
        m_iLengthOfIr = 0;
    }
    
    m_bIsInitialized = false;
    
    return Error_t::kNoError;
}


Error_t CFastConv::conv()
{
    //clear previous values in m_pfConvMatrix
    for (int i=0; i<m_iBlockLength*(m_iBlockNum+1); i++)
    {
        std::cout << "clear m_pfConvMatrix[m_iCirIdx][i]  " << m_pfConvMatrix[m_iCirIdx][i] << std::endl;
        m_pfConvMatrix[m_iCirIdx][i] = 0.f;
    }
    
    for (int i=0; i<m_iBlockLength; i++)
        m_pfOutputBlock[i] = 0.f;
    
    for (int i = 0; i<m_iBlockNum; i++)
    {
        //switch to freqConv() after implemented
        timeConv(i);
        
        std::cout << "i  " << i << std::endl;
        
        for (int n = 0; n < m_iBlockLength*2 - 1; n++)
            m_pfConvMatrix[m_iCirIdx][n+i*m_iBlockLength] += m_pfBlockConvOuput[n];
    }
    
    
    for (int i = 0; i < m_iBlockNum+1; i++)
    {
        
        std::cout << "output i " << i << std::endl;
        
        int iMatrixIdx = (m_iCirIdx+m_iBlockNum+1-i) % (m_iBlockNum+1);
        for (int j = 0; j < m_iBlockLength; j++)
        {
            m_pfOutputBlock[j] += m_pfConvMatrix[iMatrixIdx][i*m_iBlockLength+j];
//            std::cout << "iMatrixIdx " << iMatrixIdx << std::endl;
//            std::cout << "i*m_iBlockLength+j " << i*m_iBlockLength+j << std::endl;
//            std::cout << "m_pfConvMatrix[iMatrixIdx][i*m_iBlockLength+j] " << m_pfConvMatrix[iMatrixIdx][i*m_iBlockLength+j] << std::endl;
        }
        
        
        
    }

    
    return Error_t::kNoError;
}


Error_t CFastConv::timeConv(int idx)
{
    for (int i=0; i<2*m_iBlockLength-1; i++)
    {
        float sum = 0.f;
        for (int m = 0; m < 2*m_iBlockLength; m++)
        {
            sum += m_pfInputBlock[m] * m_pfIRMatrix[idx][i - m];
//            std::cout << "pfBuffer1[m]" << pfBuffer1[m] << std::endl;
//            std::cout << "pfBuffer2[i - m]" << pfBuffer2[i - m] << std::endl;
        }
        m_pfBlockConvOuput[i] = sum;
    }
    
    return Error_t::kNoError;
    
}

Error_t CFastConv::freqConv(int idx)
{
    m_pfTimeIr = m_pfIRMatrix[idx];
    // Take FFT of x and h, store in m_pfFreqInput, m_pfFreqIr
    m_pCFftInstance->doFft(m_pfFreqInput, m_pfTimeInput);
    m_pCFftInstance->doFft(m_pfFreqIr, m_pfIRMatrix[idx]);
    // Split the real and imaginary parts for multiplication
    m_pCFftInstance->splitRealImag(m_pfRealInput, m_pfImagInput, m_pfFreqInput);
    m_pCFftInstance->splitRealImag(m_pfRealIr, m_pfImagIr, m_pfFreqIr);

    // frequency domain multiplication == time domain convolution
    for (int i = 0; i < (2 * m_iBlockLength); i++)
    {
        m_pfRealConv[i] = m_pfRealInput[i] * m_pfRealIr[i];
        m_pfImagConv[i] = m_pfImagInput[i] * m_pfImagIr[i];
    }
    m_pCFftInstance->mergeRealImag(m_pfFreqConv, m_pfRealConv, m_pfImagConv);
    // Convert back to time domain
    m_pCFftInstance->doInvFft(m_pfConvOuput, m_pfFreqConv);
}

Error_t CFastConv::process (float* pfOutputBuffer, const float *pfInputBuffer, int iLengthOfBuffers)
{
    if (!m_bIsInitialized)
    {
        std::cout<< "not initialized";
        return Error_t::kNotInitializedError;
    }
    
    
    int iTotalBlockNum = (int) ((iLengthOfBuffers + m_iBlockLength + m_iLengthOfIr) / m_iBlockLength)+1;

    int reset = 0;
    
    for (int i=0; i<iTotalBlockNum; i++)
    {
        
        m_pfInputBlock[i-reset] = pfInputBuffer[i];
        
        if ((i+1) % m_iBlockLength == 0)
        {
            //output in pfBlockConvOuput
            conv();
            
            for (int n=0; n<m_iBlockLength; n++)
                pfOutputBuffer[n+reset] = m_pfOutputBlock[n];
            
            
            reset += m_iBlockLength;
            
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
