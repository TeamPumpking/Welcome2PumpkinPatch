#ifndef FRAME_H
#define FRAME_H

#include <opencv2/opencv.hpp>

class Frame{
private:
	cv::Mat frame_;
	cv::VideoCapture capture_;
public:
	Frame(cv::VideoCapture&);
	void UpdateFrame();
	void DetectFaces(cv::CascadeClassifier&, std::vector<cv::Rect>&);
	void PutSticker(const cv::Mat&, const cv::Mat&, const cv::Rect);
	void ShowFrame();
	void SaveFrame();
};

#endif