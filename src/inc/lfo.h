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
    LFO(float SampleRate, Wavetable WaveType, float freq, int widthInSamples) :
        m_SampleRateInHz(SampleRate),
        m_wavetableLength(0),
        m_isInitialised(false),
        m_phasor(0),
        m_phaseInc(0),
        m_Wave(Wavetable::Sine)
    {
        init(SampleRate, WaveType, freq, widthInSamples);
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
            return -1;
        }
        else
        {
            float PhasorDecimal, PhasorInteger;

            float Sample;
            PhasorDecimal = modf(m_phasor, &PhasorInteger);

            Sample = interpol(static_cast<int>(PhasorInteger), PhasorDecimal);
            m_phasor = m_phasor + m_phaseInc;
            PhasorDecimal = modf(m_phasor, &PhasorInteger);
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

    Error_t setFreq(float freq)
    {
        if (!m_isInitialised)
        {
            cout << "LFO uninitiallized" << endl << "Initialize before use" << endl;
            return Error_t::kNotInitializedError;
        }
        else if (freq <= 0)
        {
            cout << "LFO frequency cannot be negative" << endl;
            return Error_t::kFunctionInvalidArgsError;
        }
        else
        {
            m_FreqInHz = freq;
            m_phaseInc = freq * (static_cast<float>(m_wavetableLength) / m_SampleRateInHz);
            return Error_t::kNoError;
        }
    }

    Error_t setWidth(int width)
    {
        if (!m_isInitialised)
        {
            cout << "LFO uninitiallized" << endl << "Initialize before use" << endl;
            return Error_t::kNotInitializedError;
        }
        else
        {
            m_Width = width;
            return Error_t::kNoError;
        }
    }

    int getLength()
    {
        return m_wavetableLength;
    }

    float getPhaseInc()
    {
        return m_phaseInc;
    }

    float getPhasor()
    {
        return m_phasor;
    }

    int getAmplitude()
    {
        return m_Width;
    }
protected:
private:
    // Private member variables
    float m_phasor;                      // phase accumulator
    float m_phaseInc;                    // phase increment
    int m_wavetableLength;
    int m_Width;
    float m_SampleRateInHz;
    float m_FreqInHz;

    float* pfBuffer;
    bool m_isInitialised;
    Wavetable m_Wave;


    // Private methods
    Error_t init(float fSampleRateInHz, Wavetable waveType, float freq, int widthInSamples)
    {
        m_isInitialised = true;
        float FreqResolution = 10.f;
        m_wavetableLength = static_cast<int>(fSampleRateInHz / FreqResolution);
        m_SampleRateInHz = fSampleRateInHz;
//        m_Width = width;
        setWidth(widthInSamples);
        pfBuffer = new float[m_wavetableLength];

        switch (waveType)
        {
        case Wavetable::Sine:
            CSynthesis::generateSine(pfBuffer, FreqResolution, m_SampleRateInHz, m_wavetableLength, 1);
            break;
        case Wavetable::Saw:
            CSynthesis::generateSaw(pfBuffer, FreqResolution, m_SampleRateInHz, m_wavetableLength, 1);
            break;
        case Wavetable::Rect:
            CSynthesis::generateRect(pfBuffer, FreqResolution, m_SampleRateInHz, m_wavetableLength, 1);
            break;
        case Wavetable::Dc:
            CSynthesis::generateDc(pfBuffer, m_wavetableLength, 1);
            break;
        case Wavetable::Noise:
            CSynthesis::generateNoise(pfBuffer, m_wavetableLength, 1);
            break;
        }
        setFreq(freq);
        m_phaseInc = freq * (static_cast<float>(m_wavetableLength) / m_SampleRateInHz);
        
        return Error_t::kNoError;

    }

    float interpol(int IntIdx, float DecIdx)
    {
        return((pfBuffer[IntIdx] * (1 - DecIdx)) + (pfBuffer[IntIdx] * (DecIdx)));
    }

};
#endif //MUSI6106_LFO_H
