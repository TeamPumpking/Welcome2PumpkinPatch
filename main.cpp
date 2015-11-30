#include <opencv2/opencv.hpp>
#include "frame.h"
#include "sticker_image.h"

using namespace std;
using namespace cv;

int main()
{
	Frame frame;
    StickerImage sticker("share/pumpkin.png"); //stickerの読み込み

	VideoCapture cap;
	cap.open(0);
	if (!cap.isOpened()) {
		cerr << "cannot find camera" << endl;
		exit(-1);
	}

	CascadeClassifier cascade;
	string filename = "share/haarcascades/haarcascade_frontalface_alt.xml";
	cascade.load(filename);
	if (cascade.empty()) {
		cerr << "cannot load cascade file" << endl;
		exit(-1);
	}

	vector<Rect> faces;
	while (1) {

		frame.UpdateFrame(cap);

		frame.DetectFaces(cascade, faces);

		for (int i = 0; i < faces.size(); i++){
            sticker.ResizeSticker(faces[i].width, faces[i].height); //リサイズしたstickerを作成
            sticker.GenerateMask(); //マスク作成
			frame.PutSticker(sticker.resized_sticker_, sticker.mask_, faces[i]);
		}
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
