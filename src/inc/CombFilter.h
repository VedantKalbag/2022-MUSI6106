//
//  CombFilter.hpp
//  CombFilter
//
//  Created by Vedant Kalbag on 09/02/22.
//

#ifndef CombFilter_h
#define CombFilter_h

#include <stdio.h>
#include "RingBuffer.h"



class CCombFilterBase
{
public:
//    CCombFilterBase(int delayLength, int iNumChannels, float gain=0.5);
    CCombFilterBase(int delayLength, int iNumChannels, float gainValue=0.5);
    ~CCombFilterBase();
    
    Error_t reset();
//    Error_t init(int iNumChannels, float delayValue, float gainValue);
    
    Error_t setGain(float fGainValue);
    float getGain();
    
    Error_t setDelay(int iDelayValueSamples);
    int getDelay();


    virtual Error_t process(float **ppfInputBuffer, float **ppfOutputBuffer, int iNumberOfFrames) = 0;

protected:
    CRingBuffer<float> **m_RingBuffer;
    float m_fGainValue;
    int m_iDelayValueSamples;
    int m_iNumChannels;
};

class CCombFIRFilter;
class CCombIIRFilter;



#endif /* CombFilter_h */
