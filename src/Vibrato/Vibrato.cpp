#include "Vibrato.h"

CVibrato::CVibrato(float fDelayInSec, float fDepthInSec, float fSampleRateInHz)
{
    init(fDelayInSec, fDepthInSec, fSampleRateInHz);
}

CVibrato::~CVibrato()
{
    reset();
}

Error_t CVibrato::create (CVibrato*& pCInstance, float fDelayInSec, float fDepthInSec, float fSampleRateInHz )
{
    pCInstance = new CVibrato(fDelayInSec, fDepthInSec, fSampleRateInHz );
    return Error_t::kNoError;
}

Error_t CVibrato::destroy (CVibrato*& pCInstance)
{
    delete pCInstance;
    pCInstance = nullptr;
    return Error_t::kNoError;
}

Error_t CVibrato::init(float fDelayInSec, float fDepthInSec, float fSampleRateInHz )
{
    m_fSampleRateInHz = fSampleRateInHz;
    setDepth(fDepthInSec);
    setDelay(fDelayInSec);
    m_isInitialised = true;
}

Error_t CVibrato::reset()
{
    //TODO: Write reset function
    setDepth(0.1);
    setDelay(0.1);
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
Error_t CVibrato::setDepth(float fDepthInSec)
{
    m_iDepthInSamples = static_cast<int>(fDepthInSec * m_fSampleRateInHz) ;
    return Error_t::kNoError;
}

int CVibrato::getDepth() const
{
    return m_iDepthInSamples;
}

Error_t CVibrato::process()
{
    if (m_isInitialised)
    {
        // process
        //TODO: write process logic
    }
    else
        return Error_t::kNotInitializedError;
}