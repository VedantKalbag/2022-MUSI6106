
#include "FastConv.h"

CFastConv::CFastConv( void )
{
}

CFastConv::~CFastConv( void )
{
    delete[] m_pfImpulseResponse;
    delete[] m_pfTail;

    delete[] m_pfBlockBuffer;

    delete[] m_pfTimeInput;
    delete[] m_pfTimeIr;
    delete[] m_pfFreqInput;
    delete[] m_pfFreqIr;
    delete[] m_pfFreqConv;

    delete[] m_pfRealInput;
    delete[] m_pfImagInput;
    delete[] m_pfRealIr;
    delete[] m_pfImagIr;
    delete[] m_pfRealConv;
    delete[] m_pfImagConv;
    delete[] m_pfTmpConv;
    reset();
}

Error_t CFastConv::init(float *pfImpulseResponse, int iIRLength, int iBlockLength /*= 8192*/, ConvCompMode_t eCompMode /*= kFreqDomain*/)
{
    m_ConvType = eCompMode;
    m_pfImpulseResponse = new float [iIRLength];
    m_pfTail = new float [iIRLength-1];
    m_pfBlockBuffer = new float[iBlockLength];

    m_pfTimeInput = new float[2 * iBlockLength];
    m_pfTimeIr = new float[2 * iBlockLength];

    m_pfFreqInput = new CFft::complex_t[2 * iBlockLength];
    m_pfFreqIr = new CFft::complex_t[2 * iBlockLength];
    m_pfFreqConv = new CFft::complex_t[2 * iBlockLength];


    m_pfRealInput = new float[2 * iBlockLength];
    m_pfImagInput = new float[2 * iBlockLength];
    m_pfRealIr = new float[2 * iBlockLength];
    m_pfImagIr = new float[2 * iBlockLength];
    m_pfRealConv = new float[2 * iBlockLength];
    m_pfImagConv = new float[2 * iBlockLength];
    m_pfTmpConv = new float[2 * iBlockLength];


    m_iIRLength = iIRLength;
    m_iblockSize = iBlockLength;


    if (m_ConvType == ConvCompMode_t::kFreqDomain)
    {
        m_pCFftInstance->initInstance(iBlockLength, 1, CFft::kWindowHann, CFft::kNoWindow);
    }

    for (int i = 0; i < iIRLength; i++)
    {
        m_pfImpulseResponse[i] = pfImpulseResponse[i];
    }
    return Error_t::kNoError;
}

Error_t CFastConv::reset()
{

    return Error_t::kNoError;
}

Error_t CFastConv::process (float* pfOutputBuffer, const float *pfInputBuffer, int iLengthOfBuffers )
{
    if (m_ConvType == ConvCompMode_t::kTimeDomain)
    {
        // DO SOMETHING
    }

    else if (m_ConvType == ConvCompMode_t::kFreqDomain)
    {
        int nBlockInput = 0;
        int nBlockIr = 0;
        while (nBlockInput < (iLengthOfBuffers / m_iblockSize))
        {
            for (int i = m_iblockSize * nBlockInput; i < m_iblockSize * (nBlockInput + 2); i++)
            {
                if (i < m_iblockSize * (nBlockInput + 1))
                {
                    m_pfTimeInput[i] = pfInputBuffer[i];
                }
                else
                {
                    m_pfTimeInput[i] = 0;
                }
            }
            // Divide IR into chunks
            while (nBlockIr < (m_iIRLength / m_iblockSize))
            {
                for (int i = m_iblockSize * nBlockIr; i < m_iblockSize * (nBlockIr + 2); i++)
                {
                    if (i < m_iblockSize * (nBlockInput + 1))
                    {
                        m_pfTimeIr[i] = m_pfImpulseResponse[i];
                    }
                    else
                    {
                        m_pfTimeIr[i] = 0;
                    }
                }
                // PERFORM FFT AND SPLIT INTO REAL AND IMAG
                m_pCFftInstance->doFft(m_pfFreqInput, m_pfTimeInput);
                m_pCFftInstance->doFft(m_pfFreqIr, m_pfTimeIr);

                m_pCFftInstance->splitRealImag(m_pfRealInput, m_pfImagInput, m_pfFreqInput);
                m_pCFftInstance->splitRealImag(m_pfRealIr, m_pfImagIr, m_pfFreqIr);

                // Freq domain multiplication == time domain convolution
                for (int i = 0; i < (2 * m_iblockSize); i++)
                {
                    m_pfRealConv[i] = m_pfRealInput[i] * m_pfRealIr[i];
                    m_pfImagConv[i] = m_pfImagInput[i] * m_pfImagIr[i];
                }
                m_pCFftInstance->mergeRealImag(m_pfFreqConv, m_pfRealConv, m_pfImagConv);

                // Convert back to time domain
                m_pCFftInstance->doInvFft(m_pfTmpConv, m_pfFreqConv);

                for (int i = m_iblockSize * nBlockIr; i < m_iblockSize * (nBlockIr + 2); i++)
                {
                    pfOutputBuffer[i] = m_pfTmpConv[i];
                }
                nBlockIr++;
            }
            nBlockInput++;
        }
        return Error_t::kNoError;
    }
    else return Error_t::kFunctionInvalidArgsError;

}

Error_t CFastConv::flushBuffer(float* pfOutputBuffer)
{
    for (int i = 0; i < m_iIRLength-1; i++)
    {
        pfOutputBuffer[i] = m_pfTail[i];
    }
    return Error_t::kNoError;
}
