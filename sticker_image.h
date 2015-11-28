#ifndef STICKER_IMAGE_H
#define STICKER_IMAGE_H

#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

class StickerImage {

public:
    StickerImage(string image);
    
    void ResizeSticker(double face_w, double face_h);
    void GenerateMask();
    
    Mat GetMask;
    
    Mat mask_;
    Mat sticker_;
    Mat resized_sticker_;


  private:
};

#endif

