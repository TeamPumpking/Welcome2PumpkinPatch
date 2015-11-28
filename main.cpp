#include <opencv2/opencv.hpp>

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

	cap.open(2);
	if (!cap.isOpened()) {
		cerr << "cannot find camera" << endl;
		return -1;
	}

	sealImg = imread("pumpkin.png", -1);
	if (sealImg.empty()) {
		cerr << "cannot find sealImg" << endl;
		return -1;
	}
	resizedSealImg = sealImg.clone();

	CascadeClassifier cascade;
	string filename = "haarcascades/haarcascade_frontalface_alt.xml";
	cascade.load(filename);

	while (1) {
		cap >> frame;
		cvtColor(frame, gray, CV_BGR2GRAY);
		srcImg = frame.clone();
		cvtColor(srcImg, srcImg, CV_RGB2RGBA);

		vector<Rect> faces;
		cascade.detectMultiScale(gray, faces, 1.2, 3, 0, Size(20, 20));

		for (int i = 0; i < faces.size(); i++){
			//rectangle(srcImg, Point(faces[i].x, faces[i].y), Point(faces[i].x + faces[i].width, faces[i].y + faces[i].height), Scalar(0, 200, 0), 3, CV_AA);
			double fx = (double)faces[i].width / sealImg.cols*RATE;
			double fy = (double)faces[i].height / sealImg.rows*RATE;
			if (fx>fy) resize(sealImg, resizedSealImg, Size(), fx, fx, INTER_AREA);
			else resize(sealImg, resizedSealImg, Size(), fy, fy, INTER_AREA);

			mask = resizedSealImg.clone();
			setAlpha(mask);
			int start_x = faces[i].x + faces[i].width / 2 - resizedSealImg.cols / 2;
			int start_y = faces[i].y + faces[i].height / 2 - resizedSealImg.rows / 2;
			int end_x = start_x + resizedSealImg.cols;
			int end_y = start_y + resizedSealImg.rows;
			if (start_x<0 || start_y<0 || end_x>srcImg.cols || end_y>srcImg.rows) continue;
			srcROI = srcImg(Rect(start_x, start_y, resizedSealImg.cols, resizedSealImg.rows));
			imshow("ROI", srcROI);
			imshow("mask", mask);
			resizedSealImg.copyTo(srcROI, mask);
		}
		imshow("frame", frame);
		imshow("detect face", srcImg);

		switch (int key = waitKey(1))
		{
		case 'q':
			//�I������
			cv::destroyAllWindows();
			return 0;
		case 's':
			//�t���[���摜��ۑ�����D
			cv::imwrite("img.png", srcImg);
			break;
		}
	}

	//�O�̂���
	cv::destroyAllWindows();
	return 0;
}

void setAlpha(Mat& srcMat){

	int width = srcMat.cols;
	int height = srcMat.rows;

	for (int i = 0; i < height; i++){
		for (int j = 0; j < width; j++){
			Vec4b &p = srcMat.at<Vec4b>(i, j);

			// a�l = 0�̂Ƃ� �F���]
			if (p[3] == 255){
				p[0] = 255 - p[0];
				p[1] = 255 - p[1];
				p[2] = 255 - p[2];
				p[3] = 255 - p[3];
			}
			// ����ȊO�͂��ׂ�0(�M�U�M�U������)
			else{
				p[0] = p[1] = p[2] = p[3] = 0;
			}
		}
	}
}