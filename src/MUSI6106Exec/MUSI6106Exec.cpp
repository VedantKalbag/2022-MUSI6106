
#include <iostream>
#include <ctime>

#include "MUSI6106Config.h"

#include "AudioFileIf.h"

using std::cout;
using std::endl;

// local function declarations
void    showClInfo ();

/////////////////////////////////////////////////////////////////////////////////
// main function
int main(int argc, char* argv[])
{
    std::string             sInputFilePath,                 //!< file paths
                            sOutputFilePath;

    static const int        kBlockSize = 1024;

    clock_t                 time = 0;

    float                   **ppfAudioData = 0;

    CAudioFileIf            *phAudioFile = 0;
    std::fstream            hOutputFile;
    CAudioFileIf::FileSpec_t stFileSpec;

    
//    phAudioFile = new CAudioFileIf();
    
    
    
//    stFileSpec.eFormat = CAudioFileIf::kFileFormatWav;
//    stFileSpec.eBitStreamType = CAudioFileIf::kFileBitStreamFloat32;
//    stFileSpec.iNumChannels = 2;
//    stFileSpec.fSampleRateInHz = 44100;
    
    Error_t fileSpecError = phAudioFile->getFileSpec(stFileSpec);
    
//    stFileSpec = phAudioFile->getFileSpec(stFileSpec)
//    phAudioFile->openFile(sInputFilePath, stFileSpec.eFormat);
    Error_t createError = CAudioFileIf::create(phAudioFile);
    //////////////////////////////////////////////////////////////////////////////
    // parse command line arguments
    if (argc == 2){
    sInputFilePath = argv[0];
    sOutputFilePath = argv[1];
    }
    sInputFilePath = "/Users/vedant/Desktop/Programming/2022-MUSI6106/1_funk_80_beat_4-4.wav";
    sOutputFilePath="/Users/vedant/Desktop/Programming/2022-MUSI6106/test.txt";
    //////////////////////////////////////////////////////////////////////////////
    // open the input wave file
    //Error_t openError = phAudioFile->openFile(sInputFilePath, CAudioFileIf::kFileRead, &stFileSpec);
    Error_t openError = phAudioFile->openFile(sInputFilePath, CAudioFileIf::FileIoType_t::kFileRead);
//    phAudioFile->getFileSpec(stFileSpec);
    
    //////////////////////////////////////////////////////////////////////////////
    // open the output text file
    hOutputFile.open(sOutputFilePath,std::ios::out);
    
    //////////////////////////////////////////////////////////////////////////////
    // allocate memory
    long long iLengthInFrames;
    Error_t frameError = phAudioFile->getLength(*&iLengthInFrames);
    
    const long long numFrames = iLengthInFrames;
    const int numChannels = stFileSpec.iNumChannels;
    
    ppfAudioData = new float* [numChannels];
    for (int i = 0; i < numChannels; ++i)
    {
            ppfAudioData[i] = new float[kBlockSize];
    }
    //////////////////////////////////////////////////////////////////////////////
    // get audio data and write it to the output text file (one column per channel)
    long long readAmount = kBlockSize; //should read 1024 samples every time
    while (readAmount == kBlockSize)
    {
        phAudioFile->readData(ppfAudioData, readAmount);
        for (int k = 0; k < readAmount; ++k)
        {
            for (int j = 0; j < numChannels; ++j)
            {
                std::string frameData = std::to_string(ppfAudioData[j][k]) + ",";
                hOutputFile << frameData;
            }
            hOutputFile << std::endl;
        }

    }
    //////////////////////////////////////////////////////////////////////////////
    // clean-up (close files and free memory)
    phAudioFile->closeFile();
    CAudioFileIf::destroy(phAudioFile);
    hOutputFile.close();
    // all done
    cout<<"DONE"<<endl;
    return 0;

}


void     showClInfo()
{
    cout << "MUSI6106 Assignment Executable" << endl;
    cout << "(c) 2014-2022 by Alexander Lerch" << endl;
    cout  << endl;

    return;
}

