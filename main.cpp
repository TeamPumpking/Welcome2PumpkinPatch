#include "sticker_image.h"

#define RATE 1.7

using namespace cv;
using namespace std;

void setAlpha(Mat& srcMat);

int main()
{
	VideoCapture cap;
	Mat frame;
	Mat srcImg;
	Mat srcROI;
	Mat gray;
	Mat sealImg;
	Mat resizedSealImg;
	Mat mask;
    
    StickerImage sticker_image("share/pumpkin.png");
    

	cap.open(0);
	if (!cap.isOpened()) {
		cerr << "cannot find camera" << endl;
		return -1;
	}
    
    

	CascadeClassifier cascade;
	string filename = "share/haarcascades/haarcascade_frontalface_alt.xml";
	cascade.load(filename);

	while (1) {
		cap >> frame;
		cvtColor(frame, gray, CV_BGR2GRAY);
		srcImg = frame.clone();
		cvtColor(srcImg, srcImg, CV_RGB2RGBA);

		vector<Rect> faces;
		cascade.detectMultiScale(gray, faces, 1.2, 3, 0, Size(20, 20));
        

		for (int i = 0; i < faces.size(); i++){
            sticker_image.ResizeSticker(faces[i].width, faces[i].height);
            sticker_image.GenerateMask();
            
			int start_x = faces[i].x + faces[i].width / 2 -sticker_image.resized_sticker_.cols / 2;
			int start_y = faces[i].y + faces[i].height / 2 - sticker_image.resized_sticker_.rows / 2;
            
			int end_x = start_x + sticker_image.resized_sticker_.cols;
			int end_y = start_y + sticker_image.resized_sticker_.rows;
			if (start_x<0 || start_y<0 || end_x>srcImg.cols || end_y>srcImg.rows) continue;
			srcROI = srcImg(Rect(start_x, start_y, sticker_image.resized_sticker_.cols, sticker_image.resized_sticker_.rows));
			imshow("ROI", srcROI);
			imshow("mask", sticker_image.mask_);
			sticker_image.resized_sticker_.copyTo(srcROI, sticker_image.mask_);
		}
//		imshow("frame", frame);
		imshow("detect face", srcImg);

		switch (int key = waitKey(1))
		{
		case 'q':
			//終了する
			cv::destroyAllWindows();
			return 0;
		case 's':
			//フレーム画像を保存する．
			cv::imwrite("img.png", srcImg);
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