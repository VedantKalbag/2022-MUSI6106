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

    static Error_t create (CVibrato*& pCInstance, float fDelayInSec, float fDepthInSec, float fSampleRateInHz );
    static Error_t destroy (CVibrato*& pCInstance);
    Error_t setDelay(float fDelayInSec);
    int getDelay() const;
    Error_t setDepth(float fDepthInSec);
    int getDepth() const;
    Error_t process();
protected:
    CVibrato(float fDelayInSec, float fDepthInSec, float fSampleRateInHz );
    ~CVibrato();
    Error_t reset();
    Error_t init(float fDelayInSec, float fDepthInSec, float fSampleRateInHz );
private:
    // set up pointer to lfo and ring buffer classes
    LFO *lfo;
    CRingBuffer<float> *ringBuffer;
    float m_fSampleRateInHz;
    bool m_isInitialised = false;
//    float m_fDepthInSec, m_fDelayInSec = 0.f;
    int m_iDepthInSamples, m_iDelayInSamples;
};

//parameters - kdelayinsec, kdepth - similar to combfilter enum

// create, destroy, init(LFO, ringbuffer init, converts params in seconds to samples),setparam, getparam,
// process (use the lfo to get values from wavetable, and pass to ringbuffer)

//set up lfo freq (mod freq), sr, width in samples, delay in samples, numChannels
#endif // __Vibrato_hdr__
