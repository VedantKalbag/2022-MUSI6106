
#if !defined(__FastConv_HEADER_INCLUDED__)
#define __FastConv_HEADER_INCLUDED__

#pragma once

#include "ErrorDef.h"
#include <iostream>

/*! \brief interface for fast convolution
*/
class CFastConv
{
public:
    enum ConvCompMode_t
    {
        kTimeDomain,
        kFreqDomain,

        kNumConvCompModes
    };

    CFastConv(void);
    virtual ~CFastConv(void);

    /*! initializes the class with the impulse response and the block length
    \param pfImpulseResponse impulse response samples (mono only)
    \param iLengthOfIr length of impulse response
    \param iBlockLength processing block size
    \return Error_t
    */
    Error_t init(float* pfImpulseResponse, int iLengthOfIr, int iBlockLength = 8192, ConvCompMode_t eCompMode = kFreqDomain);

    /*! resets all internal class members
    \return Error_t
    */
    Error_t reset ();

    /*! computes the output with reverb
    \param pfOutputBuffer (mono)
    \param pfInputBuffer (mono)
    \param iLengthOfBuffers can be anything from 1 sample to 10000000 samples
    \return Error_t
    */
    Error_t process(float* pfOutputBuffer, const float* pfInputBuffer, int iLengthOfBuffers);

    /*! return the 'tail' after processing has finished (identical to feeding in zeros
    \param pfOutputBuffer (mono)
    \return Error_t
    */
    Error_t flushBuffer(float* pfOutputBuffer);

private:
    bool m_bIsInitialized;
    int m_iBlockLength;
    int m_iBlockNum;
    int m_iCirIdx;
    float** m_pfIRMatrix;
    float* m_pfInputBlock;
    float* m_pfOutputBlock;
    float* m_pfBlockConvOuput;
    float* m_pfConvOuput;
    float** m_pfConvMatrix;
    Error_t timeConv(float* pfBuffer1, float* pfBuffer2, float* m_pfBlockConvOuput);
    Error_t conv(float* pfBuffer1, float** m_pfIRMatrix, float** m_pfConvMatrix);

};


#endif
