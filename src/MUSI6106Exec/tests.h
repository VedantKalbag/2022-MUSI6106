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
using std::cout;
using std::endl;

Error_t run_filtering(std::string sInputFilePath, std::string sOutputFilePath, CCombFilterIf::CombFilterType_t filterType, float gain, float delayInSeconds, int blockSize)
{
//    cout << "Running test run_filtering"<< endl;
    static const int kBlockSize = blockSize;
    CCombFilterIf *filter = nullptr;
    CAudioFileIf *phAudioFile = nullptr;
    CAudioFileIf *phAudioFileOut = nullptr;
    float **ppfInputBuffer = nullptr;
    float **ppfOutputBuffer = nullptr;
    CAudioFileIf::FileSpec_t stFileSpec;
    //create
    CCombFilterIf::create(filter);
    CAudioFileIf::create(phAudioFile);
    phAudioFile->openFile(sInputFilePath, CAudioFileIf::kFileRead);
    phAudioFile->getFileSpec(stFileSpec);
    CAudioFileIf::create(phAudioFileOut);
    phAudioFileOut->openFile(sOutputFilePath,CAudioFileIf::FileIoType_t::kFileWrite,&stFileSpec);
    //allocate memory
    filter->init(filterType,delayInSeconds,stFileSpec.fSampleRateInHz,stFileSpec.iNumChannels);
    ppfInputBuffer = new float*[stFileSpec.iNumChannels];
    for (int i = 0; i < stFileSpec.iNumChannels; i++)
    {
        ppfInputBuffer[i] = new float[kBlockSize];
    }
    if (ppfInputBuffer == nullptr)
    {
        CAudioFileIf::destroy(phAudioFile);
        return Error_t::kFileOpenError;
    }
    if (ppfInputBuffer[0] == nullptr)
    {
        CAudioFileIf::destroy(phAudioFile);
        return Error_t::kFileOpenError;
    }
    ppfOutputBuffer = new float*[stFileSpec.iNumChannels];
    for (int i = 0; i < stFileSpec.iNumChannels; i++)
    {
        ppfOutputBuffer[i] = new float[kBlockSize];
    }
    //Process audio
    while (!phAudioFile->isEof())
    {
        long long int iNumFrames = kBlockSize;
        // read data (iNumOfFrames might be updated!)
        phAudioFile->readData(ppfInputBuffer, iNumFrames);
        filter->process(ppfInputBuffer,ppfOutputBuffer,iNumFrames);
        phAudioFileOut->writeData(ppfOutputBuffer,iNumFrames);
    }
    //destroy and delete to free up memory and avoid memory leaks
    CAudioFileIf::destroy(phAudioFile);
    CAudioFileIf::destroy(phAudioFileOut);
    CCombFilterIf::destroy(filter);

    for (int i = 0; i < stFileSpec.iNumChannels; i++)
    {
        delete[] ppfInputBuffer[i];
        delete[] ppfOutputBuffer[i];
    }

    delete[] ppfInputBuffer;
    delete[] ppfOutputBuffer;
    ppfInputBuffer = nullptr;
    ppfOutputBuffer = nullptr;

    cout << "Processing done, file written" << endl;
    return Error_t::kNoError;
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
    filter->init(filterType,0.01,44100,iNumChannels);
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
    filter->process(ppfInputBuffer,ppfOutputBuffer,iNumFrames);
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
    cout << "Test with silence successful!" << endl;
    return Error_t::kNoError;
}









Error_t runAllTests()
{
    cout << "Running tests now!" << endl;
    cout << "--------------------------------------------------------------------------------" << endl;
    cout << "Running test 3 - variable block size" << endl;
    test3(512, CCombFilterIf::kCombIIR);
    test3(2048, CCombFilterIf::kCombFIR);

    cout << "--------------------------------------------------------------------------------" << endl;
    cout << "Running test 4 - silence (all zeros)" << endl;
    test4(CCombFilterIf::kCombIIR);
    test4(CCombFilterIf::kCombFIR);
}
#endif //MUSI6106_TESTS_H
