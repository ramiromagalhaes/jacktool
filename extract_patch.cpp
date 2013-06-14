#include "extract_patch.h"

void extract_patches(const std::string &image_path, const std::vector<Exclusion> &exclusions, const PatchExtractorConfiguration &cfg)
{
    //cfg.patchHeight
    //cfg.patchWidth

    const int image_width = 0;
    const int image_heigth = 0;

    for (int h = 0; h < image_heigth; h += cfg.patchHeight)
    {
        for (int w = 0; w < image_width; w += cfg.patchWidth)
        {
            const int h_big = h + cfg.patchHeight;
            const int w_big = w + cfg.patchWidth;

            bool intersects = false;

            for (std::vector<Exclusion>::const_iterator it = exclusions.begin(); it != exclusions.end(); ++it)
            {
                //teste
                const Exclusion e = *it;
                const int x = e.x;
                const int y = e.y;
                const int x_big = e.x + e.width;
                const int y_big = e.y + e.height;

                if ( ((x <= w && w <= x_big) && (y <= h && h <= y_big)) ||
                     ((w <= x && x <= w_big) && (h <= y && y <= h_big)) )
                {
                    intersects = true;
                    break;
                }
            }

            if (intersects)
            {
                continue;
            }

            //copia a imagem e grava em arquivo
        }
    }

}
