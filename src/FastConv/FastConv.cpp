
#include "FastConv.h"

CFastConv::CFastConv( void )
{
}

CFastConv::~CFastConv( void )
{
    reset();
}

Error_t CFastConv::init(float *pfImpulseResponse, int iLengthOfIr, int iBlockLength /*= 8192*/, ConvCompMode_t eCompMode /*= kFreqDomain*/)
{
    
    if (kTimeDomain)
    {
        //do something
    }
    else if (kFreqDomain)
    {
        //do something
    }
    else
    {
        cout<< "ConvCompMode_t has to be either kTimeDomain or kFreqDomain";
        assert("ConvCompMode_t has to be either kTimeDomain or kFreqDomain");
        return Error_t::kFunctionInvalidArgsError;
    }
    
    
    isInitialized = true;

    return Error_t::kNoError;
}

Error_t CFastConv::reset()
{
    isInitialized = false;
    
    return Error_t::kNoError;
}

Error_t CFastConv::process (float* pfOutputBuffer, const float *pfInputBuffer, int iLengthOfBuffers )
{
    if (not isInitialized)
    {
        cout<< "not initialized";
        assert("not initialized");
        return Error_t::kNotInitializedError;
    }
        
    
    return Error_t::kNoError;
}

Error_t CFastConv::flushBuffer(float* pfOutputBuffer)
{
    return Error_t::kNoError;
}
