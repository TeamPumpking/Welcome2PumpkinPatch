<<<<<<< HEAD
#include <opencv2/opencv.hpp>
#include "frame.h"
=======
#include "sticker_image.h"
>>>>>>> origin/sticker_class

using namespace std;
using namespace cv;

#define RATE 1.7

void setAlpha(Mat& srcMat);

int main()
{
	VideoCapture cap;

	Mat srcROI;
	Mat sealImg;
	Mat resizedSealImg;
	Mat mask;
    
<<<<<<< HEAD
	Frame frame;
=======
    StickerImage sticker_image("share/pumpkin.png");
    
>>>>>>> origin/sticker_class

	cap.open(2);
	if (!cap.isOpened()) {
		cerr << "cannot find camera" << endl;
		exit(-1);
	}
<<<<<<< HEAD

	sealImg = imread("share/pumpkin.png", -1);
	if (sealImg.empty()) {
		cerr << "cannot find sealImg" << endl;
		exit(-1);
	}
=======
    
    
>>>>>>> origin/sticker_class

	CascadeClassifier cascade;
	string filename = "share/haarcascades/haarcascade_frontalface_alt.xml";
	cascade.load(filename);
	if (cascade.empty()) {
		cerr << "cannot load cascade file" << endl;
		exit(-1);
	}

	while (1) {
		frame.UpdateFrame(cap);

		vector<Rect> faces;
<<<<<<< HEAD
		frame.DetectFaces(cascade, faces);

		for (int i = 0; i < faces.size(); i++){
			//rectangle(frame, Point(faces[i].x, faces[i].y), Point(faces[i].x + faces[i].width, faces[i].y + faces[i].height), Scalar(0, 200, 0), 3, CV_AA);
			double fx = (double)faces[i].width / sealImg.cols*RATE;
			double fy = (double)faces[i].height / sealImg.rows*RATE;
			resizedSealImg = sealImg.clone();
			if (fx>fy) resize(sealImg, resizedSealImg, Size(), fx, fx, INTER_AREA);
			else resize(sealImg, resizedSealImg, Size(), fy, fy, INTER_AREA);

			mask = resizedSealImg.clone();
			setAlpha(mask);
			frame.PutSticker(resizedSealImg, mask, faces[i]);
=======
        

            sticker_image.ResizeSticker(faces[i].width, faces[i].height);
            sticker_image.GenerateMask();
            
			int start_y = faces[i].y + faces[i].height / 2 - sticker_image.resized_sticker_.rows / 2;
            
			int end_x = start_x + sticker_image.resized_sticker_.cols;
			int end_y = start_y + sticker_image.resized_sticker_.rows;
			if (start_x<0 || start_y<0 || end_x>srcImg.cols || end_y>srcImg.rows) continue;
			srcROI = srcImg(Rect(start_x, start_y, sticker_image.resized_sticker_.cols, sticker_image.resized_sticker_.rows));
			imshow("ROI", srcROI);
			imshow("mask", sticker_image.mask_);
			sticker_image.resized_sticker_.copyTo(srcROI, sticker_image.mask_);
>>>>>>> origin/sticker_class
		}
//		imshow("frame", frame);
		frame.ShowFrame();

		switch (int key = waitKey(1))
		{
		case 'q':
			//終了する
			destroyAllWindows();
			return 0;
		case 's':
			//フレーム画像を保存する．
			frame.SaveFrame();
			break;
		}
	}

	//念のため
	cv::destroyAllWindows();
	return 0;
}

void setAlpha(Mat& srcMat){

	int width = srcMat.cols;
	int height = srcMat.rows;

	for (int i = 0; i < height; i++){
		for (int j = 0; j < width; j++){
			Vec4b &p = srcMat.at<Vec4b>(i, j);

			// a値 = 0のとき 色反転
			if (p[3] == 255){
				p[0] = 255 - p[0];
				p[1] = 255 - p[1];
				p[2] = 255 - p[2];
				p[3] = 255 - p[3];
			}
			// それ以外はすべて0(ギザギザを解消)
			else{
				p[0] = p[1] = p[2] = p[3] = 0;
			}
		}
	}
}