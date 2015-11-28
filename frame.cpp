#include "frame.h"
using namespace std;
using namespace cv;

void Frame::UpdateFrame(VideoCapture &capture){
	capture >> frame_;
	cvtColor(frame_, frame_, CV_BGR2BGRA);
}

void Frame::DetectFaces(CascadeClassifier &cascade, vector<Rect> &faces){
	Mat gray;
	if (frame_.channels() == 3) cvtColor(frame_, gray, CV_BGR2GRAY);
	if (frame_.channels() == 4) cvtColor(frame_, gray, CV_BGRA2GRAY);

	cascade.detectMultiScale(gray, faces, 1.2, 3, 0, Size(20, 20));

	return;
}

void Frame::PutSticker(const Mat &sticker, const Mat &mask, const Rect face){
	Point offset(face.width, face.height);
	Mat large_frame = Mat::zeros(Size(frame_.cols + offset.x * 2, frame_.rows + offset.y * 2), CV_8UC4);
	Mat region_of_frame = large_frame(Rect(offset.x, offset.y, frame_.cols, frame_.rows));
	frame_.copyTo(region_of_frame);

	Point face_center(offset.x + face.x + face.width / 2, offset.y + face.y + face.height / 2);
	Mat region_of_sticker = large_frame(Rect(face_center.x - sticker.cols / 2, face_center.y - sticker.rows / 2, sticker.cols, sticker.rows));
	sticker.copyTo(region_of_sticker, mask);

	region_of_frame.copyTo(frame_);
	return;
}

void Frame::ShowFrame(){
	imshow("detect face", frame_);
}

void Frame::SaveFrame(){
	imwrite("img.png", frame_);
}
