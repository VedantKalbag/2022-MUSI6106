//
//  CombFilter.cpp
//  CombFilter
//
//  Created by Vedant Kalbag on 09/02/22.
//
#include "ErrorDef.h"
#include "CombFilter.h"

class CCombIIRFilter : public CCombFilterBase
{
public:
    
private:
    Error_t process (float **ppfInputBuffer, float **ppfOutputBuffer, int iNumberOfFrames);
};

class CCombFIRFilter : public CCombFilterBase
{
public:

private:
    Error_t process (float **ppfInputBuffer, float **ppfOutputBuffer, int iNumberOfFrames);
};
