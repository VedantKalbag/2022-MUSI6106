#include "Vibrato.h"

CVibrato::CVibrato(float fDelayInSec, float fWidthInSec, float fSampleRateInHz)
{
    init(fDelayInSec, fWidthInSec, fSampleRateInHz);
}

CVibrato::~CVibrato()
{
    reset();
}

Error_t CVibrato::create (CVibrato*& pCInstance, float fDelayInSec, float fWidthInSec, float fSampleRateInHz )
{
    pCInstance = new CVibrato(fDelayInSec, fWidthInSec, fSampleRateInHz );
    return Error_t::kNoError;
}

Error_t CVibrato::destroy (CVibrato*& pCInstance)
{
    delete pCInstance;
    pCInstance = nullptr;
    return Error_t::kNoError;
}

Error_t CVibrato::init(float fDelayInSec, float fWidthInSec, float fSampleRateInHz )
{
    m_isInitialised = true;
    m_fSampleRateInHz = fSampleRateInHz;
    setParam(kDelay, fDelayInSec);
    setParam(kWidth,fWidthInSec)
//    setWidth(fDepthInSec);
//    setDelay(fDelayInSec);
}

Error_t CVibrato::reset()
{
    setWidth(0.1);
    setDelay(0.1);
}
Error_t CVibrato::setParam(CVibratoParam paramName, float paramValue) {
    switch (paramName)
    {
        case kDelay:
            return setDelay(paramValue);
        case kWidth:
            return setWidth(paramValue);
    }
}

int CVibrato::getParam(CVibratoParam paramName)
{
    switch (paramName)
    {
        case kDelay:
            return getDelay();
        case kWidth:
            return getWidth();
    }
}

Error_t CVibrato::setDelay(float fDelayInSec)
{
    m_iDelayInSamples = static_cast<int>(fDelayInSec * m_fSampleRateInHz);
    return Error_t::kNoError;
}
int CVibrato::getDelay() const
{
    return m_iDelayInSamples;
}
Error_t CVibrato::setWidth(float fDepthInSec)
{
     m_iWidthInSamples = static_cast<int>(fDepthInSec * m_fSampleRateInHz) ;
    return Error_t::kNoError;
}

int CVibrato::getWidth() const
{
    return m_iWidthInSamples;
}

Error_t CVibrato::process(float **ppfInputBuffer, float **ppfOutputBuffer)
{
    if (m_isInitialised)
    {
        // process
        //TODO: write process logic
    }
    else
        return Error_t::kNotInitializedError;
}