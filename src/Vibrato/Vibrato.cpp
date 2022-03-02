#include "Vibrato.h"

CVibrato::CVibrato(float fDelayInSec, float fWidthInSec, float fSampleRateInHz, float fFrequencyInHz, int numChannels): m_isInitialised(false)
{
    init(fDelayInSec, fWidthInSec, fSampleRateInHz, fFrequencyInHz, numChannels);
}

CVibrato::~CVibrato()
{
    this->reset();
}

Error_t CVibrato::create (CVibrato*& pCInstance, float fDelayInSec, float fWidthInSec, float fSampleRateInHz, float fFrequencyInHz, int numChannels )
{
    pCInstance = new CVibrato(fDelayInSec, fWidthInSec, fSampleRateInHz, fFrequencyInHz, numChannels );
    return Error_t::kNoError;
}

Error_t CVibrato::destroy (CVibrato*& pCInstance)
{
    if (!pCInstance)
        return Error_t::kUnknownError;
    delete pCInstance;
    pCInstance = nullptr;
    return Error_t::kNoError;
}

Error_t CVibrato::init(float fDelayInSec, float fWidthInSec, float fSampleRateInHz, float fFrequencyInHz, int numChannels )
{
        m_fSampleRateInHz = fSampleRateInHz;
        m_iNumChannels = numChannels;

        setParam(kDelay, fDelayInSec);
        setParam(kWidth, fWidthInSec);

//        this->reset();
        //Initialise and set memory for the vibrato ring buffer
        if(ringBuffer != nullptr)
        {
            for(int i=0;i<m_iNumChannels;i++)
            {
                delete ringBuffer[i];
            }
            delete[] ringBuffer;
            ringBuffer = nullptr;
        }
        int bufferSize = 2 + m_iDelayInSamples + m_iWidthInSamples * 2;
        ringBuffer = new CRingBuffer<float> *[m_iNumChannels];
        for (int i = 0; i < m_iNumChannels; i++)
        {
            ringBuffer[i] = new CRingBuffer<float>(bufferSize);
        }
        if (lfo != nullptr)
        {
            delete lfo;
            lfo = nullptr;
        }
        lfo = new LFO(m_fSampleRateInHz, LFO::Wavetable::Sine, fFrequencyInHz, m_iWidthInSamples);
//        setParam(kFrequency, fFrequencyInHz);

//        lfo->setFreq(fFrequencyInHz);
        m_isInitialised = true;
}

Error_t CVibrato::reset()
{
    for(int i=0;i<m_iNumChannels;i++)
    {
        delete ringBuffer[i];
    }
    delete[] ringBuffer;
    ringBuffer = nullptr;
    delete lfo;
    lfo = nullptr;
    m_isInitialised = false;
}
Error_t CVibrato::setParam(CVibratoParam paramName, float paramValue)
{
    switch (paramName)
    {
        case kDelay:
            return setDelay(paramValue);
        case kWidth:
            return setWidth(paramValue);
        case kFrequency:
            return setFreq(paramValue);
        case  kNumFilterTypes:
            return Error_t::kFunctionInvalidArgsError;
    }
}

float CVibrato::getParam(CVibratoParam paramName)
{
    switch (paramName)
    {
        case kDelay:
            return static_cast<float>(getDelay());
        case kWidth:
            return static_cast<float>(getWidth());
        case kFrequency:
            return getFreq();
        case  kNumFilterTypes:
            return 0;
    }
}

Error_t CVibrato::setDelay(float fDelayInSec)
{
    if(fDelayInSec > 0)
    {
        m_iDelayInSamples = static_cast<int>(fDelayInSec * m_fSampleRateInHz);
        return Error_t::kNoError;
    }
    else
        return Error_t::kFunctionInvalidArgsError;

}
float CVibrato::getDelay() const
{
//    return m_iDelayInSamples;
//    return m_iDelayInSamples;
    return (static_cast<float>(m_iDelayInSamples) / m_fSampleRateInHz);
}
//float CVibrato::getDelay() const
//{
//    return static_cast<float>(m_iDelayInSamples) / m_fSampleRateInHz;
//}

Error_t CVibrato::setWidth(float fDepthInSec)
{
    if(fDepthInSec > 0)
    {
        if(fDepthInSec < (m_iDelayInSamples/m_fSampleRateInHz))
        {
            m_iWidthInSamples = static_cast<int>(fDepthInSec * m_fSampleRateInHz) ;
            return Error_t::kNoError;
        }
        else
            return Error_t::kFunctionInvalidArgsError;
    }
    else
        return Error_t::kFunctionInvalidArgsError;

}
float CVibrato::getWidth() const
{
//    return m_iWidthInSamples;
    return (static_cast<float>(m_iWidthInSamples) / m_fSampleRateInHz);
}
//float CVibrato::getWidth() const
//{
//    return static_cast<float>(m_iWidthInSamples) / m_fSampleRateInHz;
//}

Error_t CVibrato::setFreq(float fFreqInHz)
{
    //m_fFreqInHz = fFreqInHz;
    if(fFreqInHz > 0)
    {
        lfo->setFreq(fFreqInHz);
        m_fFreqInHz = fFreqInHz;
        return Error_t::kNoError;
    }
    else
        return Error_t::kFunctionInvalidArgsError;
}
float CVibrato::getFreq() const
{
    return m_fFreqInHz;
}

Error_t CVibrato::process(float **ppfInputBuffer, float **ppfOutputBuffer, long long iNumFrames) const
{
    if (m_isInitialised)
    {
        // process
        for (int c = 0; c < m_iNumChannels; c++)
        {
            for (int i = 0; i < iNumFrames; i++)
            {
                float lfoOffset = lfo->readSample();
                float fOffset = 1.f + static_cast<float>(m_iDelayInSamples) + lfoOffset;
                ringBuffer[c]->putPostInc(ppfInputBuffer[c][i]);
                ppfOutputBuffer[c][i] = ringBuffer[c]->get(fOffset);
                ringBuffer[c]->getPostInc();
            }
        }
        return Error_t::kNoError;
    }
    else
        return Error_t::kNotInitializedError;
}