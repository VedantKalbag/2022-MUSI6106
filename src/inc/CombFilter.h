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
    CCombFilterBase(int delayLength, int iNumChannels, float gain=0.5);
    CCombFilterBase();
    ~CCombFilterBase();
    
    Error_t resetFilter();
    
    Error_t setGain(float fGainValue);
    float getGain();
    
    Error_t setDelay(float fDelayValue);
    float getDelay();
    
    
protected:
    float m_fGainValue = 0.5;
    float m_fDelayValue = 0;
    virtual Error_t process(float **ppfInputBuffer, float **ppfOutputBuffer, int iNumberOfFrames);
};

class CCombFIRFilter;
class CCombIIRFilter;



#endif /* CombFilter_h */
