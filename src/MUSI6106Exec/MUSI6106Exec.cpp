
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
    showClInfo();
    for(int i =0; i<50;i++)
    {
        std::cout << i << std::endl;
        CVibrato *vibrato;
        float fDelayInSec = 0.1f;
        float fDepthInSec = 0.05f;
        float fSampleRateInHz = 44100.f;
        float fFrequencyInHz = 1.5f;
        int iNumChannels = 1;

        CVibrato::create(vibrato, fDelayInSec, fDepthInSec, fSampleRateInHz, fFrequencyInHz,iNumChannels);
        //Test 1
        assert(vibrato->setParam(CVibrato::kFrequency, -1.5) == Error_t::kFunctionInvalidArgsError);
        assert(vibrato->setParam(CVibrato::kDelay, -1.5) == Error_t::kFunctionInvalidArgsError);
        assert(vibrato->setParam(CVibrato::kWidth, -1.5) == Error_t::kFunctionInvalidArgsError);
        // Test 2
        vibrato->setParam(CVibrato::kDelay, 0.1);
        Error_t widthtest = vibrato->setParam(CVibrato::kWidth, 0.5);
//        assert(widthtest == Error_t::kFunctionInvalidArgsError);

        CVibrato::destroy(vibrato);
    }
    // all done
    return 0;

}


void     showClInfo()
{
    cout << "MUSI6106 Assignment Executable" << endl;
    cout << "(c) 2014-2022 by Alexander Lerch" << endl;
    cout  << endl;

    return;
}
