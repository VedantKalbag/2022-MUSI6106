
#include <iostream>
#include <ctime>

#include "MUSI6106Config.h"
#include "Fft.h"
#include "RingBuffer.h"
#include "AudioFileIf.h"

using std::cout;
using std::endl;

// local function declarations
void    showClInfo();

/////////////////////////////////////////////////////////////////////////////////
// main function
int main(int argc, char* argv[])
{
    std::string             sInputFilePath,                 //!< file paths
        sOutputFilePath;

    static const int        kBlockSize = 1024;

    clock_t                 time = 0;

    float** ppfAudioData = 0;
    int fftBlockSize = 0;
    int fftHopSize = 0;
    CAudioFileIf* phAudioFile = 0;

    float *ppfInputBuffer = nullptr;
//    float *ppfMagnitudeBuffer = nullptr;
    CFft* cFFT = nullptr;
    CRingBuffer<float> *ringBuffer = nullptr;
    CFft::complex_t *pfSpectrum=nullptr;
    float *pfMagnitude = nullptr;
    float *pfPhase = nullptr;

    std::fstream            hOutputFile;
    CAudioFileIf::FileSpec_t stFileSpec;

    showClInfo();

    //////////////////////////////////////////////////////////////////////////////
    // parse command line arguments
    if (argc < 3)
    {
        cout << "Missing audio input path!";
        return -1;
    }
    else
    {
        sInputFilePath = argv[1];
        fftBlockSize = std::stoi(argv[2]);
        fftHopSize = std::stoi(argv[3]);
        sOutputFilePath = sInputFilePath + ".txt";
    }

    //////////////////////////////////////////////////////////////////////////////
    // open the input wave file
    CAudioFileIf::create(phAudioFile);
    CFft::createInstance(cFFT);

    phAudioFile->openFile(sInputFilePath, CAudioFileIf::kFileRead);
    if (!phAudioFile->isOpen())
    {
        cout << "Wave file open error!";
        CAudioFileIf::destroy(phAudioFile);
        return -1;
    }
    phAudioFile->getFileSpec(stFileSpec);
    // FIXME: does ringBuffer need to be block+hop, or is just block enough?
    ringBuffer = new CRingBuffer<float>(fftBlockSize+fftHopSize);
    //////////////////////////////////////////////////////////////////////////////
    // open the output text file
    hOutputFile.open(sOutputFilePath.c_str(), std::ios::out);
    if (!hOutputFile.is_open())
    {
        cout << "Text file open error!";
        CAudioFileIf::destroy(phAudioFile);
        return -1;
    }

    //////////////////////////////////////////////////////////////////////////////
    // allocate memory

    ppfAudioData = new float* [stFileSpec.iNumChannels];
    for (int i = 0; i < stFileSpec.iNumChannels; i++)
        ppfAudioData[i] = new float[kBlockSize];

    if (ppfAudioData == 0)
    {
        CAudioFileIf::destroy(phAudioFile);
        hOutputFile.close();
        return -1;
    }
    if (ppfAudioData[0] == 0)
    {
        CAudioFileIf::destroy(phAudioFile);
        hOutputFile.close();
        return -1;
    }
    //TODO: ALLOCATE MEMORY FOR pfSpectrum, pfMagnitude, pfPhase
    ppfInputBuffer = new float[kBlockSize];
//    ppfMagnitudeBuffer = new float[kBlockSize];
    time = clock();

    //////////////////////////////////////////////////////////////////////////////
    // get audio data and write it to the output text file (one column per channel)
    while (!phAudioFile->isEof())
    {
        // set block length variable
        long long iNumFrames = kBlockSize;

        // read data (iNumOfFrames might be updated!)
        phAudioFile->readData(ppfAudioData, iNumFrames);

        cout << "\r" << "reading and writing";

        // write
        for (int i = 0; i < iNumFrames; i++)
        {
            for (int c = 0; c < stFileSpec.iNumChannels; c++)
            {
                ringBuffer->putPostInc(ppfAudioData[c][i]);

                if (ringBuffer->getNumValuesInBuffer() == fftBlockSize)
                {
                    // Copy data from ringBuffer to ppfInputBuffer (getpostinc?)
                    for(i=0;i<fftBlockSize;i++)
                    {
                        ppfInputBuffer[i] = ringBuffer->getPostInc();
                    }
                    // Run fft
                    cFFT->doFft(pfSpectrum,ppfInputBuffer);
                    cFFT->getMagnitude(pfMagnitude,pfSpectrum);
                    cFFT->getPhase(pfPhase,pfSpectrum);

                    // TODO: Write magnitude and phase to hOutputFile
                }
                //hOutputFile << ppfAudioData[c][i] << "\t";
            }
            hOutputFile << endl;
        }
    }

    cout << "\nreading/writing done in: \t" << (clock() - time) * 1.F / CLOCKS_PER_SEC << " seconds." << endl;

    //////////////////////////////////////////////////////////////////////////////
    // clean-up (close files and free memory)
    CAudioFileIf::destroy(phAudioFile);
    CFft::destroyInstance(cFFT);
    hOutputFile.close();

    for (int i = 0; i < stFileSpec.iNumChannels; i++)
        delete[] ppfAudioData[i];
    delete[] ppfAudioData;
    ppfAudioData = 0;

    // all done
    return 0;

}


void     showClInfo()
{
    cout << "MUSI6106 Assignment Executable" << endl;
    cout << "(c) 2014-2022 by Alexander Lerch" << endl;
    cout << endl;

    return;
}

