#if !defined(__Vibrato_hdr__)
#define __Vibrato_hdr__
#include "RingBuffer.h"
#include "ErrorDef.h"
#include "lfo.h"

/*
 * use this as an interface for the user, call lfo and ringbuffer here directly
 * */
class CVibrato
{
public:
    enum CVibratoParam
    {
        kWidth,
        kDelay,
        kNumFilterTypes
    };
    static Error_t create (CVibrato*& pCInstance, float fDelayInSec, float fDepthInSec, float fSampleRateInHz );
    static Error_t destroy (CVibrato*& pCInstance);
    Error_t setParam(CVibratoParam paramName, float paramValue);
    int getParam(CVibratoParam paramName);
    Error_t process(float **ppfInputBuffer, float **ppfOutputBuffer);
protected:
private:
    // Private member variables
    LFO *lfo;
    CRingBuffer<float> *ringBuffer;
    float m_fSampleRateInHz;
    bool m_isInitialised = false;
//    float m_fDepthInSec, m_fDelayInSec = 0.f;
    int m_iWidthInSamples, m_iDelayInSamples;

    // Private methods
    CVibrato(float fDelayInSec, float fWidthInSec, float fSampleRateInHz );
    ~CVibrato();
    Error_t reset();
    Error_t init(float fDelayInSec, float fWidthInSec, float fSampleRateInHz );
    Error_t setDelay(float fDelayInSec);
    int getDelay() const;
    Error_t setWidth(float fDepthInSec);
    int getWidth() const;
};

//parameters - kdelayinsec, kdepth - similar to combfilter enum

// create, destroy, init(LFO, ringbuffer init, converts params in seconds to samples),setparam, getparam,
// process (use the lfo to get values from wavetable, and pass to ringbuffer)

//set up lfo freq (mod freq), sr, width in samples, delay in samples, numChannels
#endif // __Vibrato_hdr__
