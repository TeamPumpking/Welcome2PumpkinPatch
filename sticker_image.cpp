#include "sticker_image.h"

#define RATE 1.7

StickerImage::StickerImage(string image){
    sticker_ = imread(image, -1);

    if(sticker_.empty()){
        cerr << "cannot find camera" << endl;
        exit(0);
    }
}

void StickerImage::ResizeSticker(double face_w, double face_h){
    resized_sticker_ = sticker_.clone();
    double fx = face_w / sticker_.cols * RATE;
    double fy = face_h / sticker_.rows * RATE;

    if(fx > fy) resize(sticker_, resized_sticker_, Size(), fy, fy, INTER_AREA);
    else resize(sticker_, resized_sticker_, Size(), fx, fx, INTER_AREA);
}

void StickerImage::GenerateMask(){
    mask_ = resized_sticker_.clone();
    int width = mask_.cols;
    int height = mask_.rows;
 

    for(int i = 0; i < height; i++){
        for(int j = 0; j < width; j++){
            Vec4b &p = mask_.at<Vec4b>(i,j);

            if(p[3] == 255){
                p[0] = 255 - p[0];
                p[1] = 255 - p[1];
                p[2] = 255 - p[2];
                p[3] = 255 - p[3];
            }
            else{
                p[0] = p[1] = p[2] = p[3] = 0;
            }
        }
    }
}

//Mat GetMask(){
//    Mat output = mask_;
//    return output;
//}


