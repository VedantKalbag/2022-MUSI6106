//
// Created by Vedant Kalbag on 13/02/22.
//

#ifndef MUSI6106_RUN_FILTERING_H
#define MUSI6106_RUN_FILTERING_H
using std::cout;
using std::endl;
Error_t run_filtering(std::string sInputFilePath, std::string sOutputFilePath, CCombFilterIf::CombFilterType_t filterType, float gain, float delayInSeconds, int blockSize)
{
//    cout << "Running test run_filtering"<< endl;

    if (delayInSeconds < 0)
        return Error_t::kFunctionInvalidArgsError;
    if (gain < 0)
        return Error_t::kFunctionInvalidArgsError;

    static const int kBlockSize = blockSize;
    CCombFilterIf *filter = nullptr;
    CAudioFileIf *phAudioFile = nullptr;
    CAudioFileIf *phAudioFileOut = nullptr;
    float **ppfInputBuffer = nullptr;
    float **ppfOutputBuffer = nullptr;
    CAudioFileIf::FileSpec_t stFileSpec;
    Error_t process_error;
    Error_t read_error;
    Error_t write_error;

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
        read_error = phAudioFile->readData(ppfInputBuffer, iNumFrames);
        process_error = filter->process(ppfInputBuffer,ppfOutputBuffer,iNumFrames);
        write_error = phAudioFileOut->writeData(ppfOutputBuffer,iNumFrames);
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
    assert(read_error == Error_t::kNoError);
    assert(process_error == Error_t::kNoError);
    assert(write_error == Error_t::kNoError);

    if (read_error != Error_t::kNoError)
        return read_error;
    if (process_error != Error_t::kNoError)
        return process_error;
    if (write_error != Error_t::kNoError)
        return write_error;
    return Error_t::kNoError;
}
#endif //MUSI6106_RUN_FILTERING_H
