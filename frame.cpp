#include "frame.h"
using namespace std;
using namespace cv;

void Frame::UpdateFrame(VideoCapture &capture){
	capture >> frame_;
	cvtColor(frame_, frame_, CV_RGB2RGBA);
}

void Frame::DetectFaces(CascadeClassifier &cascade, vector<Rect> &faces){
	Mat gray;
	cvtColor(frame_, gray, CV_BGRA2GRAY);

	cascade.detectMultiScale(gray, faces, 1.2, 3, 0, Size(20, 20));

	return;
}

void Frame::PutSticker(const Mat &sticker, const Mat &mask, const Rect face){
	int start_x = face.x + face.width / 2 - sticker.cols / 2;
	int start_y = face.y + face.height / 2 - sticker.rows / 2;
	int end_x = start_x + sticker.cols;
	int end_y = start_y + sticker.rows;

	if (start_x<0 || start_y<0 || end_x>frame_.cols || end_y>frame_.rows) return;

	Mat ROI = frame_(Rect(start_x, start_y, sticker.cols, sticker.rows));
	sticker.copyTo(ROI, mask);

	return;
}

void Frame::ShowFrame(){
	imshow("detect face", frame_);
}

void Frame::SaveFrame(){
	imwrite("img.png", frame_);
}
