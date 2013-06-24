#ifndef PATCHEXTRACTORCONFIGURATION_H
#define PATCHEXTRACTORCONFIGURATION_H

#include <string>

class PatchExtractorConfiguration {
public:
    bool rotate90,rotate180,rotate270;
    int patchHeight, patchWidth;
    std::string destinationFolder;

    PatchExtractorConfiguration() {
        rotate90 = rotate180 = rotate270 = true;
        patchHeight = patchWidth = 24; //Viola, Jones; 2004 defaults.
                                       //Pavani et al; 2010 = 20 x 20
    }
};

#endif // PATCHEXTRACTORCONFIGURATION_H
