#include "Vibrato.h"

CVibrato::CVibrato(float fDelayInSec, float fWidthInSec, float fSampleRateInHz, float fFrequencyInHz, int numChannels)
{
    init(fDelayInSec, fWidthInSec, fSampleRateInHz, fFrequencyInHz, numChannels);
}

CVibrato::~CVibrato()
{
    reset();
}

Error_t CVibrato::create (CVibrato*& pCInstance, float fDelayInSec, float fWidthInSec, float fSampleRateInHz, float fFrequencyInHz, int numChannels )
{
    pCInstance = new CVibrato(fDelayInSec, fWidthInSec, fSampleRateInHz, fFrequencyInHz, numChannels );
    return Error_t::kNoError;
}

Error_t CVibrato::destroy (CVibrato*& pCInstance)
{
    delete pCInstance;
    pCInstance = nullptr;
    return Error_t::kNoError;
}

Error_t CVibrato::init(float fDelayInSec, float fWidthInSec, float fSampleRateInHz, float fFrequencyInHz, int numChannels )
{
    if(!m_isInitialised)
    {
        m_fSampleRateInHz = fSampleRateInHz;
        m_iNumChannels = numChannels;

        setParam(kDelay, fDelayInSec);
        setParam(kWidth, fWidthInSec);
        setParam(kFrequency, fFrequencyInHz);

        //Initialise and set memory for the vibrato ring buffer
        int bufferSize = 2+m_iDelayInSamples+m_iWidthInSamples*2;
        ringBuffer = new CRingBuffer<float> *[m_iNumChannels];
        for (int i=0;i<m_iNumChannels;i++)
        {
            ringBuffer[i] = new CRingBuffer<float>(bufferSize);
//        m_RingBuffer[i]->reset();
        }
        //TODO: change as required based on the LFO class
        // ALLOCATE MEMORY FOR LFO
        lfo = new LFO(m_fSampleRateInHz, LFO::Wavetable::Sine, fFrequencyInHz, static_cast<float>(m_iWidthInSamples));

        lfo->m_SampleRateInHz = m_fSampleRateInHz;
        lfo->setFreq(fFrequencyInHz);
        m_isInitialised = true;
    }
}

Error_t CVibrato::reset()
{
    setWidth(0.1);
    setDelay(0.1);
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

Error_t CVibrato::setFreq(float fFreqInHz)
{
    //m_fFreqInHz = fFreqInHz;
    lfo->setFreq(fFreqInHz);
    return Error_t::kNoError;
}
float CVibrato::getFreq() const
{
    return m_fFreqInHz;
}

Error_t CVibrato::process(float **ppfInputBuffer, float **ppfOutputBuffer, int iNumFrames) const
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
                ppfOutputBuffer[c][i] = ringBuffer[c]->getPostInc(fOffset);
            }
        }
    }
    else
        return Error_t::kNotInitializedError;
}