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
    CCombFilterBase();
    ~CCombFilterBase();
    
    Error_t reset();
    Error_t init();
    
    Error_t setGain(float fGainValue);
    float getGain();
    
    Error_t setDelay(int iDelayValueSamples);
    int getDelay();


    virtual Error_t process(float **ppfInputBuffer, float **ppfOutputBuffer, int iNumberOfFrames) = 0;

protected:
    CRingBuffer<float> **m_RingBuffer;
    float m_fGainValue = 0.5;
    int m_iDelayValueSamples = 0;
    int m_iNumChannels = 0;
};

class CCombFIRFilter;
class CCombIIRFilter;



#endif /* CombFilter_h */
