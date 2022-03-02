
#include <iostream>
#include <ctime>
#include "Vibrato.h"
#include "MUSI6106Config.h"
#include <cassert>
#include "AudioFileIf.h"
using std::cout;
using std::endl;

// local function declarations
void    showClInfo ();

/////////////////////////////////////////////////////////////////////////////////
// main function
int main(int argc, char* argv[])
{
    float fDelayInSec;//= 0.001f;
    float fDepthInSec;// = 0.001f;
    float fFrequencyInHz;// = 8.f;
    std::string sInputFilePath, sOutputFilePath;
    showClInfo();
    /* ARGS:
     * 1 - input file path
     * 2 - delay
     * 3 - depth
     * 4 - frequency
    */
    if (argc < 2) {
        // RUN TESTS REPEATEDLY TO MAKE SURE IT DOESN'T CRASH AFTER SEVERAL ITERATIONS
        for(int i =0; i<50; i++)
        {
            std::cout << i << std::endl;
            CVibrato *vibrato;
            //create
            CVibrato::create(vibrato, 0.001f, 0.001f, 44100.f, 8.4,1);
            //Test 1
            assert(vibrato->setParam(CVibrato::kFrequency, -1.5) == Error_t::kFunctionInvalidArgsError);
            assert(vibrato->setParam(CVibrato::kDelay, -1.5) == Error_t::kFunctionInvalidArgsError);
            assert(vibrato->setParam(CVibrato::kWidth, -1.5) == Error_t::kFunctionInvalidArgsError);
            // Test 2
            vibrato->setParam(CVibrato::kDelay, 0.1);
            assert(vibrato->setParam(CVibrato::kWidth, 0.5) == Error_t::kFunctionInvalidArgsError);

            //destroy
            CVibrato::destroy(vibrato);
        }
        return 0;
    }
    else if (argc < 5) {
        cout << "Missing arguments!";
        return -1;
    } else {
        sInputFilePath = argv[1];
        fDelayInSec = std::stof(argv[2]);
        fDepthInSec = std::stof(argv[3]);
        fFrequencyInHz = std::stof(argv[4]);
        sOutputFilePath = sInputFilePath.substr(0,sInputFilePath.size()-4) + "_vibrato.wav";
    }
    // ============================================
    // Allocate memory
    // ============================================
    static const int kBlockSize = 2048;
    CVibrato *vibrato = nullptr;
    CAudioFileIf *phAudioFile = nullptr;
    CAudioFileIf *phAudioFileOut = nullptr;
    float **ppfInputBuffer = nullptr;
    float **ppfOutputBuffer = nullptr;
    CAudioFileIf::FileSpec_t stFileSpec;
    Error_t process_error;
    Error_t read_error;
    Error_t write_error;

    CAudioFileIf::create(phAudioFile);
    phAudioFile->openFile(sInputFilePath, CAudioFileIf::kFileRead);
    phAudioFile->getFileSpec(stFileSpec);
    CAudioFileIf::create(phAudioFileOut);
    phAudioFileOut->openFile(sOutputFilePath,CAudioFileIf::FileIoType_t::kFileWrite,&stFileSpec);
    CVibrato::create(vibrato, fDelayInSec, fDepthInSec, stFileSpec.fSampleRateInHz, fFrequencyInHz,stFileSpec.iNumChannels);

    ppfInputBuffer = new float*[stFileSpec.iNumChannels];
    for (int i = 0; i < stFileSpec.iNumChannels; i++)
    {
        ppfInputBuffer[i] = new float[kBlockSize];
    }
    if (ppfInputBuffer == nullptr)
    {
        CAudioFileIf::destroy(phAudioFile);
        return 0;//Error_t::kFileOpenError;
    }
    if (ppfInputBuffer[0] == nullptr)
    {
        CAudioFileIf::destroy(phAudioFile);
        return 0;//Error_t::kFileOpenError;
    }
    ppfOutputBuffer = new float*[stFileSpec.iNumChannels];
    for (int i = 0; i < stFileSpec.iNumChannels; i++)
    {
        ppfOutputBuffer[i] = new float[kBlockSize];
    }
    std::cout << "Reading and processing file now" << std::endl;
    // ============================================
    // PROCESS
    // ============================================
    //FIXME: This does not work as intended
    while (!phAudioFile->isEof())
    {
        long long int iNumFrames = kBlockSize;
        // read data (iNumOfFrames might be updated!)
        read_error = phAudioFile->readData(ppfInputBuffer, iNumFrames);
        process_error = vibrato->process(ppfInputBuffer,ppfOutputBuffer, iNumFrames);
        write_error = phAudioFileOut->writeData(ppfOutputBuffer,iNumFrames);
//        write_error = phAudioFileOut->writeData(ppfInputBuffer, iNumFrames); //TEST WRITING FUNCTIONALITY TO ENSURE THAT OUTPUT = INPUT (WORKS)
    }
    std::cout << "File Written" << std::endl;
    // ============================================
    // DELETE MEMORY
    // ============================================
    CAudioFileIf::destroy(phAudioFile);
    CAudioFileIf::destroy(phAudioFileOut);
    CVibrato::destroy(vibrato);
    for (int i = 0; i < stFileSpec.iNumChannels; i++)
    {
        delete[] ppfInputBuffer[i];
        delete[] ppfOutputBuffer[i];
    }

    delete[] ppfInputBuffer;
    delete[] ppfOutputBuffer;
    ppfInputBuffer = nullptr;
    ppfOutputBuffer = nullptr;

    // all done
    return 0;
}


void     showClInfo()
{
    cout << "MUSI6106 Assignment Executable" << endl;
    cout << "(c) 2014-2022 by Alexander Lerch" << endl;
    cout  << endl;
//    return;
}
