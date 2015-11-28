#include <opencv2/opencv.hpp>
#include "frame.h"

using namespace std;
using namespace cv;

#define RATE 1.7

void setAlpha(Mat& srcMat);

int main()
{
	VideoCapture cap;
	vector<Rect> faces;
	Mat srcROI;
	Mat sealImg;
	Mat resizedSealImg;
	Mat mask;
    
	Frame frame;

	cap.open(0);
	if (!cap.isOpened()) {
		cerr << "cannot find camera" << endl;
		return -1;
	}

	sealImg = imread("share/pumpkin.png", -1);
	if (sealImg.empty()) {
		cerr << "cannot find sealImg" << endl;
		return -1;
	}

	CascadeClassifier cascade;
	string filename = "share/haarcascades/haarcascade_frontalface_alt.xml";
	cascade.load(filename);

	while (1) {

		frame.UpdateFrame(cap);

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
		}
//		imshow("frame", frame);
		frame.ShowFrame();

		switch (int key = waitKey(1))
		{
		case 'q':
			//終了する
			cv::destroyAllWindows();
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