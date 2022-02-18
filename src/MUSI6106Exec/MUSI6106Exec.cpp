
#include <iostream>
#include <ctime>
#include <cstdlib>
#include "tests.h"

#include "MUSI6106Config.h"
#include "run_filtering.h"
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
    showClInfo();

    //////////////////////////////////////////////////////////////////////////////
    // parse command line arguments
    // eg input "path_to_file" "iir" 0.5 0.01
    if (argc < 2) {
        runAllTests();
        return 0;
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
        gain = atof(argv[3]);
        delayInSeconds = atof(argv[4]);
        sOutputFilePath = sInputFilePath + "_filtered.wav";
    }

//    run_filtering(sInputFilePath,sOutputFilePath,filterType, gain, delayInSeconds, kBlockSize);
    run_filtering(sInputFilePath, sOutputFilePath, filterType, gain, 0.01, kBlockSize);

    return 0;
}

void     showClInfo()
{
    cout << "MUSI6106 Assignment Executable" << endl;
    cout << "(c) 2014-2022 by Alexander Lerch" << endl;
    cout  << endl;

    return;
}

