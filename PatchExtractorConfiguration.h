#ifndef PATCHEXTRACTORCONFIGURATION_H
#define PATCHEXTRACTORCONFIGURATION_H

#include <string>

class PatchExtractorConfiguration {
public:
    bool rotate90,rotate180,rotate270;
    int patchHeight, patchWidth;
    std::string destinationFolder;

    PatchExtractorConfiguration() {
        rotate90 = rotate180 = rotate270 = false;
        patchHeight = patchWidth = 24; //Viola, Jones; 2004 defaults.
    }
};

#endif // PATCHEXTRACTORCONFIGURATION_H
