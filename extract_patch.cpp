#include "extract_patch.h"

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <sstream>

bool extract_patches(const boost::filesystem::path &image_path,
                     const std::vector<Rectangle> &exclusions,
                     const PatchExtractorConfiguration &cfg)
{
    cv::Mat image;
    image = cv::imread(image_path.native());
    if (!image.data)
    {
        return false;
    }

    int patch_counter = 0;

    for (int h = 0; h < image.size().height - cfg.patchHeight; h += cfg.patchHeight)
    {
        for (int w = 0; w < image.size().width - cfg.patchHeight; w += cfg.patchWidth)
        {
            const int h_big = h + cfg.patchHeight;
            const int w_big = w + cfg.patchWidth;

            bool intersects = false;

            for (std::vector<Rectangle>::const_iterator it = exclusions.begin(); it != exclusions.end(); ++it)
            {
                const Rectangle e = *it;
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



            //builds the patch path...
            boost::filesystem::path filePatchPath = cfg.destinationFolder / image_path.parent_path().filename();
            //...creates a folder where patches will be saved, if necessary...
            if ( !boost::filesystem::exists(filePatchPath) )
            {
                boost::filesystem::create_directory(filePatchPath);
            }
            //..sets the basic patch filename
            filePatchPath /= image_path.filename();
            std::stringstream ss;
            ss << filePatchPath.native().c_str() << "-" << patch_counter++;

            //...extract the patch...
            cv::Rect roi(w, h, cfg.patchWidth, cfg.patchHeight);
            cv::Mat patch(image, roi);
            cv::cvtColor(patch, patch, CV_BGR2GRAY);

            //...and writes it on file.
            std::string filename = ss.str() + ".bmp";
            cv::imwrite(filename, patch);

            if (cfg.rotate90)
            {
                filename = ss.str() + "-90.bmp";
                cv::Mat transp = patch.clone();
                cv::transpose(patch, transp);
                cv::imwrite(filename, transp);
            }
            if (cfg.rotate180)
            {
                filename = ss.str() + "-180.bmp";
                cv::Mat transp = patch.clone();
                cv::transpose(patch, transp);
                cv::transpose(transp, transp);
                cv::imwrite(filename, transp);
            }
            if (cfg.rotate270)
            {
                filename = ss.str() + "-270.bmp";
                cv::Mat transp = patch.clone();
                cv::transpose(patch, transp);
                cv::transpose(transp, transp);
                cv::transpose(transp, transp);
                cv::imwrite(filename, transp);
            }
        }
    }

    return true;
}
