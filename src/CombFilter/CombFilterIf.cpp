
// standard headers

// project headers
#include "MUSI6106Config.h"

#include "ErrorDef.h"
#include "Util.h"

#include "CombFilterIf.h"
#include "CombFilter.cpp"

static const char*  kCMyProjectBuildDate = __DATE__;


CCombFilterIf::CCombFilterIf ()://(CombFilterType_t eFilterType, float fMaxDelayLengthInS, float fSampleRateInHz, int iNumChannels) :
    m_bIsInitialized(false),
    m_pCCombFilter(0),
    m_fSampleRate(0)
{
    this->reset();
//    this->init(eFilterType, fMaxDelayLengthInS, fSampleRateInHz, iNumChannels);
}


CCombFilterIf::~CCombFilterIf ()
{
    this->reset();
}

const int  CCombFilterIf::getVersion (const Version_t eVersionIdx)
{
    int iVersion = 0;

    switch (eVersionIdx)
    {
    case kMajor:
        iVersion    = MUSI6106_VERSION_MAJOR; 
        break;
    case kMinor:
        iVersion    = MUSI6106_VERSION_MINOR; 
        break;
    case kPatch:
        iVersion    = MUSI6106_VERSION_PATCH; 
        break;
    case kNumVersionInts:
        iVersion    = -1;
        break;
    }

    return iVersion;
}
const char*  CCombFilterIf::getBuildDate ()
{
    return kCMyProjectBuildDate;
}

Error_t CCombFilterIf::create (CCombFilterIf*& pCCombFilter)
{
    pCCombFilter = new CCombFilterIf();
    return Error_t::kNoError;
}

Error_t CCombFilterIf::destroy (CCombFilterIf*& pCCombFilter)
{
    delete pCCombFilter;
    pCCombFilter = nullptr;
    return Error_t::kNoError;
}

Error_t CCombFilterIf::init (CombFilterType_t eFilterType, float fMaxDelayLengthInS, float fSampleRateInHz, int iNumChannels)
{
    
    m_fSampleRate = fSampleRateInHz;
    int delayLength = static_cast<int>(fMaxDelayLengthInS*fSampleRateInHz);
    if (eFilterType == kCombFIR)
    {
        m_pCCombFilter = new CCombFIRFilter(delayLength, iNumChannels);
    }
    else if (eFilterType == kCombIIR)
    {
        m_pCCombFilter = new CCombIIRFilter(delayLength, iNumChannels);
    }
    m_pCCombFilter->reset();
    
    m_bIsInitialized = true;
    return Error_t::kNoError;
}

Error_t CCombFilterIf::reset ()
{
    delete m_pCCombFilter;
    m_pCCombFilter = 0;
    m_fSampleRate = 0;
    m_bIsInitialized = false;
    
    return Error_t::kNoError;
}

Error_t CCombFilterIf::process (float **ppfInputBuffer, float **ppfOutputBuffer, long long iNumberOfFrames)
{
    m_pCCombFilter->process(ppfInputBuffer,ppfOutputBuffer,iNumberOfFrames);
    return Error_t::kNoError;
}

Error_t CCombFilterIf::setParam (FilterParam_t eParam, float fParamValue)
{
    if (eParam == kParamGain)
    {
        m_pCCombFilter->setGain(fParamValue);
    }
    else if (eParam == kParamDelay)
    {
        m_pCCombFilter->setDelay(fParamValue * m_fSampleRate);
    }

    return Error_t::kNoError;
}

float CCombFilterIf::getParam (FilterParam_t eParam) const
{
    if(eParam == kParamGain)
    {
        return m_pCCombFilter->getGain();
    }
    else if(eParam == kParamDelay)
    {
        return static_cast<float>(m_pCCombFilter->getDelay());
    }
    else
    {
        return 0;
    }
}
