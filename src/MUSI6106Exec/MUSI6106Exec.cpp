
#include <iostream>
#include <ctime>
#include <cstdlib>
#include "time.h"
#include "tests.h"

#include "MUSI6106Config.h"

#include "AudioFileIf.h"
#include "CombFilterIf.h"

using std::cout;
using std::endl;

// local function declarations
void    showClInfo ();

/////////////////////////////////////////////////////////////////////////////////
// main function
int main(int argc, char* argv[]) {
    std::string sInputFilePath,                 //!< file paths
    sOutputFilePath, filterName;
    CCombFilterIf::CombFilterType_t filterType;
    float gain, delayInSeconds;
    static const int kBlockSize = 1024;

    clock_t time = 0;

//    float **ppfAudioData = 0;
    float **ppfInputBuffer = nullptr;
    float **ppfOutputBuffer = nullptr;

    //Initialize object of CCombFilterIf type
    CCombFilterIf *filter = nullptr;

    CAudioFileIf *phAudioFile = nullptr;
    CAudioFileIf *phAudioFileOut = nullptr;
//    std::fstream hOutputFile;
    CAudioFileIf::FileSpec_t stFileSpec;

    showClInfo();

    //////////////////////////////////////////////////////////////////////////////
    // parse command line arguments
    // ADD FILENAME, Filtertype , GAIN, DELAY as inputs
    // eg input 'path_to_file' iir 0.5 0.25
    if (argc < 2) {
        cout << "Running tests now!" << endl;
//        test1();
        return -1;
    } else if (argc < 3) {
        cout << "Missing Filter Type!";
        return -1;
    } else if (argc < 4) {
        cout << "Missing gain!";
        return -1;
    } else if (argc < 5) {
        cout << "Missing delay!";
        return -1;
    } else {
        sInputFilePath = argv[1];
        filterName = argv[2];
        if (filterName == "iir") {
            filterType = CCombFilterIf::kCombIIR;
        } else if (filterName == "fir") {
            filterType = CCombFilterIf::kCombFIR;
        }
//        filterType = static_cast<CCombFilterIf::CombFilterType_t>(argv[2]);
        gain = atof(argv[3]);
        delayInSeconds = atof(argv[4]);

        sOutputFilePath = sInputFilePath + "_filtered.wav";

    }
//    run_filtering(sInputFilePath,sOutputFilePath,filterType, gain, delayInSeconds, kBlockSize);
    run_filtering(sInputFilePath, sOutputFilePath, filterType, gain, 0.01, kBlockSize);
    return 0;
}
//
//    CCombFilterIf::create(filter);
////    for (int channel=0;channel<stFileSpec.iNumChannels;channel++)
////    {
////         CCombFilterIf::create(filter[channel]);
////    }
//    //////////////////////////////////////////////////////////////////////////////
//    // open the input wave file
//    CAudioFileIf::create(phAudioFile);
//    phAudioFile->openFile(sInputFilePath, CAudioFileIf::kFileRead);
//    phAudioFile->getFileSpec(stFileSpec);
//
//    CAudioFileIf::create(phAudioFileOut);
//    phAudioFileOut->openFile(sOutputFilePath,CAudioFileIf::FileIoType_t::kFileWrite,&stFileSpec);
//    if (!phAudioFile->isOpen())
//    {
//        cout << "Wave file open error!";
//        CAudioFileIf::destroy(phAudioFile);
//        return -1;
//    }
//    //////////////////////////////////////////////////////////////////////////////
//    // allocate memory
//    filter->init(filterType,delayInSeconds,stFileSpec.fSampleRateInHz,stFileSpec.iNumChannels);
//
//    //INPUT BUFFER INITIALIZATION
//    ppfInputBuffer = new float*[stFileSpec.iNumChannels];
//    for (int i = 0; i < stFileSpec.iNumChannels; i++)
//    {
//        ppfInputBuffer[i] = new float[kBlockSize];
//    }
//    if (ppfInputBuffer == nullptr)
//    {
//        CAudioFileIf::destroy(phAudioFile);
//        return -1;
//    }
//    if (ppfInputBuffer[0] == nullptr)
//    {
//        CAudioFileIf::destroy(phAudioFile);
//        return -1;
//    }
//
//    //OUTPUT BUFFER INITIALIZATION
//    ppfOutputBuffer = new float*[stFileSpec.iNumChannels];
//    for (int i = 0; i < stFileSpec.iNumChannels; i++)
//    {
//        ppfOutputBuffer[i] = new float[kBlockSize];
//    }
//    time = clock();
//    //////////////////////////////////////////////////////////////////////////////
//    // get audio data and write it to the output text file (one column per channel)
//    while (!phAudioFile->isEof())
//    {
//        long long int iNumFrames = kBlockSize;
//
//        // read data (iNumOfFrames might be updated!)
//        phAudioFile->readData(ppfInputBuffer, iNumFrames);
//        //Process data
//        filter->process(ppfInputBuffer,ppfOutputBuffer,iNumFrames);
//
//        phAudioFileOut->writeData(ppfOutputBuffer,iNumFrames);
//        cout << "\r" << "reading and writing";
//
//    }
//
//    cout << "\nreading/writing done in: \t" << (clock() - time) * 1.F / CLOCKS_PER_SEC << " seconds." << endl;
//
//    //////////////////////////////////////////////////////////////////////////////
//    // clean-up (close files and free memory)
//    // Free memory for the buffers and filters to avoid memory leaks
//    CAudioFileIf::destroy(phAudioFile);
//    CAudioFileIf::destroy(phAudioFileOut);
//
//    CCombFilterIf::destroy(filter);
////    hOutputFile.close();
//
//    for (int i = 0; i < stFileSpec.iNumChannels; i++)
//    {
//        delete[] ppfInputBuffer[i];
//        delete[] ppfOutputBuffer[i];
//    }
//
//    delete[] ppfInputBuffer;
//    delete[] ppfOutputBuffer;
//    ppfInputBuffer = 0;
//    ppfOutputBuffer = 0;
//
//    // all done
//    return 0;
//
//}


void     showClInfo()
{
    cout << "MUSI6106 Assignment Executable" << endl;
    cout << "(c) 2014-2022 by Alexander Lerch" << endl;
    cout  << endl;

    return;
}

