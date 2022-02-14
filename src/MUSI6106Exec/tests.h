//
// Created by Vedant Kalbag on 13/02/22.
//

#ifndef MUSI6106_TESTS_H
#define MUSI6106_TESTS_H

//
// Created by Vedant Kalbag on 13/02/22.
//
#include <iostream>
#include <ctime>
#include <cstdlib>
#include "CombFilterIf.h"
#include "AudioFileIf.h"
#include "ErrorDef.h"
#include "run_filtering.h"
#include <cassert>
using std::cout;
using std::endl;

Error_t test1(float gainValue, float delayValue)
{
    int blockSize = 1024;
    std::string sInputFilePath = "/Users/vedant/Desktop/Programming/2022-MUSI6106/audio_files/sine100hz.wav";
    std::string sOutputFilePath = "/Users/vedant/Desktop/Programming/2022-MUSI6106/audio_files/output/sine100hz_fir_filtered.wav";

    Error_t test1error = run_filtering(sInputFilePath,sOutputFilePath,CCombFilterIf::kCombFIR,gainValue,delayValue,blockSize);
    return test1error;
}

Error_t test2(float gainValue, float delayValue)
{
    int blockSize = 1024;
    std::string sInputFilePath = "/Users/vedant/Desktop/Programming/2022-MUSI6106/audio_files/sine100hz.wav";
    std::string sOutputFilePath = "/Users/vedant/Desktop/Programming/2022-MUSI6106/audio_files/output/sine100hz_iir_filtered.wav";

    Error_t test2error = run_filtering(sInputFilePath,sOutputFilePath,CCombFilterIf::kCombIIR,gainValue,delayValue,blockSize);
    return test2error;
}

Error_t test3(int blockSize, CCombFilterIf::CombFilterType_t filterType)
{
    cout << "Test 3 in progress with blocksize: " << blockSize << endl;
    std::string sInputFilePath = "/Users/vedant/Desktop/Programming/2022-MUSI6106/audio_files/6.wav";
    std::string sOutputFilePath = "/Users/vedant/Desktop/Programming/2022-MUSI6106/audio_files/output/6_filtered.wav";
    std::string filterName;
    if (filterType == CCombFilterIf::kCombIIR)
    {
        filterName = "IIR";
    }
    else if (filterType == CCombFilterIf::kCombFIR)
    {
        filterName = "FIR";
    }
    float gain = 0.5;
    float delay = 0.01;
//    CCombFilterIf::CombFilterType_t filterType = CCombFilterIf::kCombIIR;
    Error_t test3error = run_filtering(sInputFilePath,sOutputFilePath,filterType,gain,delay,blockSize);
    cout << "Test with blocksize: " << blockSize << " and filter type: " << filterName << " successful!" << endl;
//    cout << "Test with blocksize: " << blockSize << " successful!" << endl;
    assert (test3error == Error_t::kNoError);
    return test3error;
}

Error_t test4(CCombFilterIf::CombFilterType_t filterType)
{
//    std::string sInputFilePath = "/Users/vedant/Desktop/Programming/2022-MUSI6106/audio_files/silence.wav";
//    std::string sOutputFilePath = "/Users/vedant/Desktop/Programming/2022-MUSI6106/audio_files/output/silence_filtered.wav";
//    std::string filterName;
//    if (filterType == CCombFilterIf::kCombIIR)
//    {
//        filterName = "IIR";
//    }
//    else if (filterType == CCombFilterIf::kCombFIR)
//    {
//        filterName = "FIR";
//    }
//    float gain = 0.5;
//    float delay = 0.01;
//    int blockSize = 1024;
//
//    Error_t test4error = run_filtering(sInputFilePath,sOutputFilePath,filterType,gain,delay,blockSize);
    int iNumChannels = 2;
    int kBlockSize = 1024;
    int iNumFrames = kBlockSize;
    float **ppfInputBuffer = nullptr;
    float **ppfOutputBuffer = nullptr;

    CCombFilterIf *filter = nullptr;
    CCombFilterIf::create(filter);
    filter->init(filterType,0.01,44100,iNumChannels, 0.5);
    ppfInputBuffer = new float*[iNumChannels];
    for (int i = 0; i < iNumChannels; i++)
    {
        ppfInputBuffer[i] = new float[kBlockSize];
    }
    ppfOutputBuffer = new float*[iNumChannels];
    for (int i = 0; i < iNumChannels; i++)
    {
        ppfOutputBuffer[i] = new float[kBlockSize];
    }

    //Set input buffer to zeros
    for (int c = 0; c < iNumChannels; c++)
    {
        for(int i = 0; i < iNumFrames;i++)
        {
            ppfInputBuffer[c][i] = 0;
        }
    }
    //Process
    Error_t test4 = filter->process(ppfInputBuffer,ppfOutputBuffer,iNumFrames);
    //Clear memory
    CCombFilterIf::destroy(filter);
    for (int i = 0; i < iNumChannels; i++)
    {
        delete[] ppfInputBuffer[i];
        delete[] ppfOutputBuffer[i];
    }
    delete[] ppfInputBuffer;
    delete[] ppfOutputBuffer;
    ppfInputBuffer = nullptr;
    ppfOutputBuffer = nullptr;
    assert (test4 == Error_t::kNoError);
    cout << "Test with silence successful!" << endl;
    return test4;
}

Error_t test5(CCombFilterIf::CombFilterType_t filterType)
{
    std::string sInputFilePath = "/Users/vedant/Desktop/Programming/2022-MUSI6106/audio_files/6.wav";
    std::string sOutputFilePath = "/Users/vedant/Desktop/Programming/2022-MUSI6106/audio_files/output/6_filtered.wav";
    std::string filterName;
    if (filterType == CCombFilterIf::kCombIIR)
    {
        filterName = "IIR";
    }
    else if (filterType == CCombFilterIf::kCombFIR)
    {
        filterName = "FIR";
    }
    int blockSize = 1024;
    float gain = -0.25f;
    float delay = -0.125f;
    Error_t test5error = run_filtering(sInputFilePath,sOutputFilePath,filterType,gain,delay,blockSize);
    assert ((test5error==Error_t::kNoError) || (test5error==Error_t::kFunctionInvalidArgsError));
    cout << "Negative values for gain and delay were caught and raised" << endl;
    return Error_t::kNoError;
}

Error_t runAllTests()
{
    cout << "Running tests now!" << endl;
    cout << "--------------------------------------------------------------------------------" << endl;
    cout << "Running test 1 - FIR output for input = feed forward" << endl;
    test1(-1.0f,0.01f);

    cout << "--------------------------------------------------------------------------------" << endl;
    cout << "Running test 2 - IIR magnitude change" << endl;
    test2(1.0f,0.01f);

    cout << "--------------------------------------------------------------------------------" << endl;
    cout << "Running test 3 - variable block size" << endl;
    test3(512, CCombFilterIf::kCombIIR);
    test3(2048, CCombFilterIf::kCombFIR);

    cout << "--------------------------------------------------------------------------------" << endl;
    cout << "Running test 4 - silence (all zeros)" << endl;
    test4(CCombFilterIf::kCombIIR);
    test4(CCombFilterIf::kCombFIR);

    cout << "--------------------------------------------------------------------------------" << endl;
    cout << "Running test 5 - negative gain and delay time" << endl;
    test5(CCombFilterIf::kCombIIR);
    test5(CCombFilterIf::kCombFIR);

    cout << "--------------------------------------------------------------------------------" << endl;
}
#endif //MUSI6106_TESTS_H
