//
// Created by Vedant Kalbag on 27/02/22.
// Modified by Thiago Roque on 28/02/22.
//
#include <iostream>
#include <math.h>

#include "ErrorDef.h"
#include "Synthesis.h"

using std::cout;
using std::endl;

#ifndef MUSI6106_LFO_H
#define MUSI6106_LFO_H



class LFO
{
public:
    enum Wavetable
    {
        Sine,
        Saw,
        Rect,
        Dc,
        Noise
    };
    LFO(float SampleRate, Wavetable WaveType, float freq, float width) :
        m_SampleRateInHz(SampleRate),
        m_wavetableLength(0),
        m_isInitialised(false),
        m_phasor(0),
        m_phaseInc(0),
        m_Wave(Sine)
    {
        init(SampleRate, WaveType, freq, width);
    }

    ~LFO()
    {
        delete[] pfBuffer;
        pfBuffer = 0;
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

            m_phasor = static_cast<int>(PhasorInteger)% m_wavetableLength + PhasorDecimal;
            return Sample * m_Width;
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
            m_phaseInc = freq / (m_wavetableLength / m_SampleRateInHz);
        }
    }

protected:
private:
    // Private member variables
    CRingBuffer<float>** ringBuffer;
    double m_phasor;                      // phase accumulator
    double m_phaseInc;                    // phase increment
    int m_wavetableLength;
    float m_Width;
    float m_SampleRateInHz;
    float m_FreqInHz;

    float* pfBuffer;
    bool m_isInitialised;
    Wavetable m_Wave;


    // Private methods
    Error_t init(float fSampleRateInHz, Wavetable waveType, float freq, float width)
    {
        float MaxFreq = 10;
        m_wavetableLength = fSampleRateInHz / MaxFreq;
        m_SampleRateInHz = fSampleRateInHz;
        m_Width = width;
        pfBuffer = new float[m_wavetableLength];
        switch (waveType)
        {
        case Sine:
            CSynthesis::generateSine(pfBuffer, MaxFreq, m_SampleRateInHz, m_wavetableLength, 1);
            break;
        case Saw:
            CSynthesis::generateSaw(pfBuffer, MaxFreq, m_SampleRateInHz, m_wavetableLength, 1);
            break;
        case Rect:
            CSynthesis::generateRect(pfBuffer, MaxFreq, m_SampleRateInHz, m_wavetableLength, 1);
            break;
        case Dc:
            CSynthesis::generateDc(pfBuffer, m_wavetableLength, m_Width);
            break;
        case Noise:
            CSynthesis::generateNoise(pfBuffer, m_wavetableLength, m_Width);
            break;
        }

        m_phaseInc = freq / (m_wavetableLength / m_SampleRateInHz);

        m_isInitialised = true;
    }

    float interpol(int IntIdx, float DecIdx)
    {
        return((pfBuffer[IntIdx] * (1 - DecIdx)) + (pfBuffer[IntIdx] * (DecIdx)));
    }

};
#endif //MUSI6106_LFO_H
