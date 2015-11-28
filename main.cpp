#include <opencv2/opencv.hpp>
#include "frame.h"
#include "sticker_image.h"

using namespace std;
using namespace cv;

int main()
{
	Frame frame;
    StickerImage sticker_image("share/pumpkin.png");

	VideoCapture cap;
	cap.open(2);
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
            sticker_image.ResizeSticker(faces[i].width, faces[i].height);
            sticker_image.GenerateMask();
			frame.PutSticker(sticker_image.resized_sticker_, sticker_image.mask_, faces[i]);
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
