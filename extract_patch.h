#ifndef EXTRACT_PATCH_H
#define EXTRACT_PATCH_H

#include <string>
#include <vector>
#include "Rectangle.h"
#include "PatchExtractorConfiguration.h"

void extract_patches(const std::string &image_path,
                     const std::string &output_name,
                     const std::vector<Rectangle> &exclusions,
                     const PatchExtractorConfiguration &cfg);

#endif // EXTRACT_PATCH_H
