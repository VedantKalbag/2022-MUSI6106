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

Error_t CCombFilterBase::setDelay(float fDelayValue)
{
    //Set delayValue to fDelayValue
    m_fDelayValue = fDelayValue;
    return Error_t::kNoError;
}

float CCombFilterBase::getDelay()
{
    return m_fDelayValue;
}

// =====================================================================================================
// CCombIIRFilter definition
class CCombIIRFilter : public CCombFilterBase
{
public:
    CCombIIRFilter(int delayLength, int iNumChannels, float gain);
protected:
    
    Error_t process (float **ppfInputBuffer, float **ppfOutputBuffer, int iNumberOfFrames) override;
};
// =====================================================================================================
// CCombFIRFilter method definition
CCombIIRFilter::CCombIIRFilter(int delayLength, int iNumChannels, float gain=0.5): CCombFilterBase(delayLength, gain, iNumChannels)
{
    
}
Error_t CCombIIRFilter::process (float **ppfInputBuffer, float **ppfOutputBuffer, int iNumberOfFrames)
{
    return Error_t::kNoError;
}

// =====================================================================================================
// CCombIIRFilter definition
class CCombFIRFilter : public CCombFilterBase
{
    
public:
    CCombFIRFilter(int delayLength, int iNumChannels, float gain);
protected:
    Error_t process (float **ppfInputBuffer, float **ppfOutputBuffer, int iNumberOfFrames) override;
};

// =====================================================================================================
// CCombIIRFilter method definition

CCombFIRFilter::CCombFIRFilter(int delayLength, int iNumChannels, float gain=0.5): CCombFilterBase(delayLength, iNumChannels, gain)
{
    
}

