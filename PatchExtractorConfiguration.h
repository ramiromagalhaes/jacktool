#ifndef PATCHEXTRACTORCONFIGURATION_H
#define PATCHEXTRACTORCONFIGURATION_H

#include <boost/filesystem.hpp>

class PatchExtractorConfiguration {
public:
    bool rotate90,rotate180,rotate270;
    int patchHeight, patchWidth;
    boost::filesystem::path destinationFolder;

    PatchExtractorConfiguration() {
        rotate90 = rotate180 = rotate270 = false;
        patchHeight = patchWidth = 20; //Viola, Jones; 2004 defaults.
                                       //Pavani et al; 2010 = 20 x 20
    }
};

#endif // PATCHEXTRACTORCONFIGURATION_H
