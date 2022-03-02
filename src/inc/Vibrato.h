#if !defined(__Vibrato_hdr__)
#define __Vibrato_hdr__
#include "RingBuffer.h"
#include "ErrorDef.h"
#include "lfo.h"

/***
 * TODO: ADD DESIGN CHOICES
 */
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
        kFrequency,
        kNumFilterTypes
    };
    /*!
     * \brief Creates an instance of CVibrato at the location of the pointer pCInstance
     * @param pCInstance - a pointer to an object of type CVibrato
     * @param fDelayInSec - delay value in seconds (float)
     * @param fWidthInSec - width value in seconds (float)
     * @param fSampleRateInHz - sample rate in Hz (float)
     * @param fFrequencyInHz - vibrato frequency in Hz (float)
     * @param numChannels - number of channels to process (int)
     * @return
     */
    static Error_t create (CVibrato*& pCInstance, float fDelayInSec, float fWidthInSec, float fSampleRateInHz, float fFrequencyInHz, int numChannels );

    /*!
     * \brief Destroys the instance of CVibrato at the location of the pointer pCInstance
     * @param pCInstance - a pointer to an object of type CVibrato
     * @return
     */
    static Error_t destroy (CVibrato*& pCInstance);

    /*!
     * \brief Resets the CVibrato instance
     * @return
     */
    Error_t reset();
    /*!
     * \brief Initializes the object of type CVibrato
     * @param fDelayInSec - delay value in seconds (float)
     * @param fWidthInSec - width value in seconds (float)
     * @param fSampleRateInHz - sample rate in Hz (float)
     * @param fFrequencyInHz - vibrato frequency in Hz (float)
     * @param numChannels - number of channels to process (int)
     * @return
     */
    Error_t init(float fDelayInSec, float fWidthInSec, float fSampleRateInHz, float fFrequencyInHz, int numChannels );

    /*!
     * \brief Sets the value of the specified parameter
     * @param paramName Parameter type to be set (type CVibratoParam)
     * @param paramValue Value of the parameter to be set (float)
     * @return
     */
    Error_t setParam(CVibratoParam paramName, float paramValue);

    /*!
     * \brief Returns the value of the specified parameter
     * @param paramName Parameter type to be returned (type CVibratoParam)
     * @return
     */
    float getParam(CVibratoParam paramName);

    /*!
     * \brief Processes the data in ppfInputBuffer and writes the data to ppfOutputBuffer
     * @param ppfInputBuffer buffer containing input data
     * @param ppfOutputBuffer buffer for output data to be written to
     * @param iNumFrames length of the buffers
     * @return
     */
    Error_t process(float **ppfInputBuffer, float **ppfOutputBuffer, long long iNumFrames) const;
protected:
private:
    // Private member variables
    LFO *lfo = nullptr;
    CRingBuffer<float> **ringBuffer = nullptr;
    float m_fSampleRateInHz, m_fFreqInHz;
    bool m_isInitialised;
//    float m_fDepthInSec, m_fDelayInSec = 0.f;
    int m_iWidthInSamples, m_iDelayInSamples, m_iNumChannels;

    // Private methods
    CVibrato(float fDelayInSec, float fWidthInSec, float fSampleRateInHz, float fFrequencyInHz, int numChannels );
    ~CVibrato();

    /*!
     * \brief Sets delay value, called by setParam
     * @param fDelayInSec - Delay value in seconds (float)
     * @return
     */
    Error_t setDelay(float fDelayInSec);

    /*!
     * \brief Sets depth value, called by setParam
     * @param fDepthInSec - depth value in seconds (float)
     * @return
     */
    Error_t setWidth(float fDepthInSec);

    /*!
     * \brief Sets Frequency value, called by setParam
     * @param fFreqInHz - vibrato frequency in Hz (float)
     * @return
     */
    Error_t setFreq(float fFreqInHz);

//    int getDelay() const;
//    int getWidth() const;
    /*!
     * \brief Returns the delay value
     * @return
     */
    float getDelay() const;

    /*!
     * \brief Returns width value
     * @return
     */
    float getWidth() const;

    /*!
     * \brief Returns frequency value
     * @return
     */
    float getFreq() const;
};
#endif // __Vibrato_hdr__
