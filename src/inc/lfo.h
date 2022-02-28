//
// Created by Vedant Kalbag on 27/02/22.
// Modified by Thiago Roque on 27/02/22.
//
#include <iostream>
#include <math.h>

#include "ErrorDef.h"
#include "Synthesis.h"

using std::cout;
using std::endl;

#ifndef MUSI6106_LFO_H
#define MUSI6106_LFO_H

enum Wavetable
{
    Sine,
    Saw,
    Rect,
    Dc,
    Noise
};

class LFO
{
public:

    LFO(float SampleRate, Wavetable WaveType) :
        m_SampleRateInHz(SampleRate),
        m_WidthInSamples(0),
        m_isInitialised(false),
        m_phasor(0),
        m_phaseInc(0),
        m_Wave(Sine)
    {
        init(m_SampleRateInHz, m_Wave);
    }

    float readSample()
    {
        if (!m_isInitialised)
        {
            cout << "LFO uninitiallized" << endl << "Initialize before use" << endl;
        }
        else
        {
            float PhasorDecimal, PhasorInteger, Sample;
            PhasorDecimal = modf(m_phasor, &PhasorInteger);
            Sample = interpol(static_cast<int>(PhasorInteger), PhasorDecimal);

            m_phasor = static_cast<int>(PhasorInteger)%m_WidthInSamples + PhasorDecimal;
            return Sample;
        }
    }

    void reset()
    {
        m_phasor = 0;
        m_phaseInc = 0;
        m_FreqInHz = 0;
        m_isInitialised = false;
    }

    void setFreq(float freq)
    {
        if (!m_isInitialised)
        {
            cout << "LFO uninitiallized" << endl << "Initialize before use" << endl;
        }
        else
        {
            m_FreqInHz = freq;
            m_phaseInc = freq / (m_WidthInSamples / m_SampleRateInHz);
        }
    }




protected:
private:
    // Private member variables
    CRingBuffer<float>** ringBuffer;
    double m_phasor;                      // phase accumulator
    double m_phaseInc;                    // phase increment
    int m_WidthInSamples;
    float m_SampleRateInHz;
    float m_FreqInHz;

    float* pfBuffer;
    bool m_isInitialised;
    Wavetable m_Wave;


    // Private methods
    Error_t init(float fSampleRateInHz, Wavetable waveType)
    {
        float MaxFreq = 10;
        m_WidthInSamples = fSampleRateInHz / MaxFreq;

        switch (waveType)
        {
        case Sine:
            CSynthesis::generateSine(pfBuffer, MaxFreq, m_SampleRateInHz, m_WidthInSamples);
            break;
        case Saw:
            CSynthesis::generateSaw(pfBuffer, MaxFreq, m_SampleRateInHz, m_WidthInSamples);
            break;
        case Rect:
            CSynthesis::generateRect(pfBuffer, MaxFreq, m_SampleRateInHz, m_WidthInSamples);
            break;
        case Dc:
            CSynthesis::generateDc(pfBuffer, m_WidthInSamples);
            break;
        case Noise:
            CSynthesis::generateNoise(pfBuffer, m_WidthInSamples);
            break;
        }

        m_phaseInc = 

        m_isInitialised = true;
    }

    float interpol(int IntIdx, float DecIdx)
    {
        return((pfBuffer[IntIdx] * (1 - DecIdx)) + (pfBuffer[IntIdx] * (DecIdx)));
    }

};
#endif //MUSI6106_LFO_H
