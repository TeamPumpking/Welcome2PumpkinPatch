#include <opencv2/opencv.hpp>
#include "frame.h"
#include "sticker_image.h"

using namespace std;
using namespace cv;

VideoCapture OpenCamera(int);
CascadeClassifier LoadClassifier(string);

int main()
{
	Frame frame;
    StickerImage sticker("share/pumpkin.png"); //stickerの読み込み

	//カメラをオープン
	VideoCapture capture = OpenCamera(2);
	//検出器を設定
	CascadeClassifier cascade = LoadClassifier("share/haarcascades/haarcascade_frontalface_alt.xml");

	vector<Rect> faces;
	while (1) {

		//フレームの更新
		frame.UpdateFrame(capture);
		//顔の検出
		frame.DetectFaces(cascade, faces);

		for (int i = 0; i < faces.size(); i++){
			//ステッカーのサイズ調節
            sticker.ResizeSticker(faces[i].width, faces[i].height);
			//ステッカーのマスク画像生成
            sticker.GenerateMask();
			//ステッカーをフレームの顔の位置に貼り付け
			frame.PutSticker(sticker.resized_sticker_, sticker.mask_, faces[i]);
		}
		//フレームの出力
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

VideoCapture OpenCamera(int camera_id){
	VideoCapture cap;
	cap.open(camera_id);
	if (!cap.isOpened()) {
		cerr << "cannot find camera" << endl;
		exit(-1);
	}
	return cap;
}

CascadeClassifier LoadClassifier(string file_name){
	CascadeClassifier cascade;
	cascade.load(file_name);
	if (cascade.empty()) {
		cerr << "cannot load cascade file" << endl;
		exit(-1);
	}
	return cascade;
}
