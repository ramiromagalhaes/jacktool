#ifndef EXTRACT_PATCH_H
#define EXTRACT_PATCH_H

#include <string>
#include <vector>
#include "Exclusion.h"
#include "PatchExtractorConfiguration.h"

void extract_patches(const std::string &image_path, const std::vector<Exclusion> &exclusions, const PatchExtractorConfiguration &cfg);

#endif // EXTRACT_PATCH_H
