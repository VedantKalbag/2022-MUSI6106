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

CCombFilterBase::CCombFilterBase()
{
    m_RingBuffer = new CRingBuffer<float>* [m_iNumChannels];
    for (int i=0;i<m_iNumChannels;i++)
    {
        m_RingBuffer[i] = new CRingBuffer<float>(m_iDelayValue);
        m_RingBuffer[i]->reset();
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

Error_t CCombFilterBase::setDelay(int iDelayValue)
{
    //Set delayValue to fDelayValue
    m_iDelayValue = iDelayValue;
    return Error_t::kNoError;
}

int CCombFilterBase::getDelay()
{
    return m_iDelayValue;
}

// =====================================================================================================
// CCombIIRFilter definition
class CCombIIRFilter : public CCombFilterBase
{
public:
//    CCombIIRFilter(int delayLength, int iNumChannels, float gain);
protected:
    int m_iNumChannels = 0;
    Error_t process (float **ppfInputBuffer, float **ppfOutputBuffer, int iNumberOfFrames) override;
};
// =====================================================================================================
// CCombFIRFilter method definition
//CCombIIRFilter::CCombIIRFilter(int delayLength, int iNumChannels, float gain=0.5): CCombFilterBase(delayLength, gain, iNumChannels)
//{
//
//}
Error_t CCombIIRFilter::process (float **ppfInputBuffer, float **ppfOutputBuffer, int iNumberOfFrames)
{
    return Error_t::kNoError;
}

// =====================================================================================================
// CCombIIRFilter definition
class CCombFIRFilter : public CCombFilterBase
{
    
public:
//    CCombFIRFilter(int delayLength, int iNumChannels, float gain);
protected:
    int m_iNumChannels = 0;
    Error_t process (float **ppfInputBuffer, float **ppfOutputBuffer, int iNumberOfFrames) override;
};

// =====================================================================================================
// CCombIIRFilter method definition

//CCombFIRFilter::CCombFIRFilter(int delayLength, int iNumChannels, float gain=0.5): CCombFilterBase(delayLength, iNumChannels, gain)
//{
//
//}

