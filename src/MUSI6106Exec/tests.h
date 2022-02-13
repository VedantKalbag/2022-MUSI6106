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
#include "time.h"
#include "CombFilterIf.h"
#include "AudioFileIf.h"
#include "ErrorDef.h"
using std::cout;
using std::endl;

Error_t run_filtering(std::string sInputFilePath, std::string sOutputFilePath, CCombFilterIf::CombFilterType_t filterType, float gain, float delayInSeconds, int blockSize)
{

    cout << "Running test run_filtering"<< endl;
    static const int kBlockSize = blockSize;
//    clock_t time = 0;

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
//        cout << "\r" << "reading and writing"<< endl;

    }
//    cout << "\nreading/writing done in: \t" << (clock() - time) * 1.F / CLOCKS_PER_SEC << " seconds." << endl;


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
}
//Error_t test1()
//{
//    std::string sInputFilePath, sOutputFilePath;
//    CCombFilterIf::CombFilterType_t filterType;
//    run_filtering(sInputFilePath,sOutputFilePath,filterType,1024);
//
//
//}

#endif //MUSI6106_TESTS_H
