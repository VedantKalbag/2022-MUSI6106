//
//  CombFilter.cpp
//  CombFilter
//
//  Created by Vedant Kalbag on 09/02/22.
//
#include "ErrorDef.h"
#include "CombFilter.h"


// =====================================================================================================
// CCombFilterBase method definitions
CCombFilterBase::CCombFilterBase(int delayLength, int iNumChannels, float gainValue)
{
    this->init(delayLength,iNumChannels,gainValue);
//    setDelay(delayLength);
//    setGain(gainValue);
//    m_iNumChannels = iNumChannels;
//    for (int i=0;i<m_iNumChannels;++i)
//    {
//        m_RingBuffer[i] = new CRingBuffer<float>(m_iDelayValueSamples);
//        m_RingBuffer[i]->reset();
//    }
}
CCombFilterBase::~CCombFilterBase()
{
    for (int i = 0; i < m_iNumChannels; i++)
    {
        delete[] m_RingBuffer[i];
    }
    delete m_RingBuffer;
}

Error_t CCombFilterBase::init(int delayLength, int iNumChannels, float gainValue)
{
    setDelay(delayLength);
    setGain(gainValue);
    m_iNumChannels = iNumChannels;
    m_RingBuffer = new CRingBuffer<float>*[m_iNumChannels];
    for (int i=0;i<m_iNumChannels;i++)
    {
        m_RingBuffer[i] = new CRingBuffer<float>(m_iDelayValueSamples);
//        m_RingBuffer[i]->reset();
    }
}

Error_t CCombFilterBase::setGain(float fGainValue)
{
    //Set gainValue to fGainValue
    m_fGainValue = fGainValue;
    return Error_t::kNoError;
}

float CCombFilterBase::getGain()
{
    return m_fGainValue;
}

Error_t CCombFilterBase::setDelay(int iDelayValueSamples)
{
    //Set delayValue to iDelayValue
    if(iDelayValueSamples > 0)
    {
        m_iDelayValueSamples = iDelayValueSamples;
        return Error_t::kNoError;
    }
    else
        return Error_t::kFunctionInvalidArgsError;
}

int CCombFilterBase::getDelay()
{
    return m_iDelayValueSamples;
}

Error_t CCombFilterBase::reset()
{
    // reset values
    m_iDelayValueSamples = 0;
    m_fGainValue = 0.f;

}

// =====================================================================================================
// CCombIIRFilter definition
class CCombIIRFilter : public CCombFilterBase
{
public:
    CCombIIRFilter(int delayLength, int iNumChannels, float gain);
    ~CCombIIRFilter();
protected:
//    int m_iNumChannels;
    Error_t process (float **ppfInputBuffer, float **ppfOutputBuffer, int iNumberOfFrames) override;
};
// =====================================================================================================
// CCombFIRFilter method definition
CCombIIRFilter::CCombIIRFilter(int delayLength, int iNumChannels, float gainValue): CCombFilterBase(delayLength, iNumChannels, gainValue)
{
    init(delayLength,iNumChannels,gainValue);
}
Error_t CCombIIRFilter::process (float **ppfInputBuffer, float **ppfOutputBuffer, int iNumberOfFrames)
{
    // Implement process method for IIR Filter
    // IIR LOGIC
    // 1. Read ring buffer at get pointer (and increment)
    // 2. Process signal => output = input + gain * read value
    // 3. Put current output into ring buffer and increment write pointer
    for(int channel=0;channel < m_iNumChannels;channel++)
    {
        for(int i=0;i<iNumberOfFrames;i++)
        {
            ppfOutputBuffer[channel][i] = ppfInputBuffer[channel][i] + m_fGainValue * m_RingBuffer[channel]->getPostInc();
            m_RingBuffer[channel]->putPostInc(ppfOutputBuffer[channel][i]);
        }
    }
    return Error_t::kNoError;
}

CCombIIRFilter::~CCombIIRFilter()
{
}

// =====================================================================================================
// CCombIIRFilter definition
class CCombFIRFilter : public CCombFilterBase
{
    
public:
    CCombFIRFilter(int delayLength, int iNumChannels, float gain);
    ~CCombFIRFilter();

protected:
//    int m_iNumChannels;
    Error_t process (float **ppfInputBuffer, float **ppfOutputBuffer, int iNumberOfFrames) override;
};

// =====================================================================================================
// CCombIIRFilter method definition

CCombFIRFilter::CCombFIRFilter(int delayLength, int iNumChannels, float gainValue): CCombFilterBase(delayLength, iNumChannels, gainValue)
{
    init(delayLength,iNumChannels,gainValue);
}
Error_t CCombFIRFilter::process (float **ppfInputBuffer, float **ppfOutputBuffer, int iNumberOfFrames)
{
    // Implement process method for FIR Filter
    // FIR LOGIC
    // 1. Read ring buffer at get pointer (and increment)
    // 2. Process signal => output = input + gain * read value
    // 3. Put current input into ring buffer and increment write pointer
    for(int channel=0;channel < m_iNumChannels;channel++)
    {
        for(int i=0;i<iNumberOfFrames;i++)
        {
            ppfOutputBuffer[channel][i] = ppfInputBuffer[channel][i] + m_fGainValue * m_RingBuffer[channel]->getPostInc();
            m_RingBuffer[channel]->putPostInc(ppfInputBuffer[channel][i]);
        }
    }
    return Error_t::kNoError;
}

CCombFIRFilter::~CCombFIRFilter()
{
}
