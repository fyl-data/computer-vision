#include <opencv2/objdetect.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>

using namespace cv;
using namespace std;

void display(Mat &im, Mat &bbox)
{
	int n = bbox.rows;
	for (int i = 0; i < n; i++)
	{
		line(im, Point2i(bbox.at<float>(i, 0), bbox.at<float>(i, 1)), Point2i(bbox.at<float>((i + 1) % n, 0), bbox.at<float>((i + 1) % n, 1)), Scalar(255, 0, 0), 3);
	}
	namedWindow("Result", WINDOW_NORMAL);
	imshow("Result", im);
}

int main(int argc, char* argv[])
{
	// Read image
	Mat inputImage;
	if (argc > 1)
		inputImage = imread(argv[1]);
	else
		inputImage = imread("F:/Image/ЖўЮЌТы.jpg");
	//cvtColor(inputImage, inputImage, COLOR_BGR2GRAY);

	QRCodeDetector qrDecoder = QRCodeDetector::QRCodeDetector();

	Mat bbox, rectifiedImage;

	std::string data = qrDecoder.detectAndDecode(inputImage, bbox, rectifiedImage);
	if (data.length() > 0)
	{
		cout << "Decoded Data : " << data << endl;

		display(inputImage, bbox);
		rectifiedImage.convertTo(rectifiedImage, CV_8UC3);
		namedWindow("Rectified QRCode", WINDOW_NORMAL);
		imshow("Rectified QRCode", rectifiedImage);

		waitKey(0);
	}
	else
		cout << "QR Code not detected" << endl;

	waitKey(0);
	return 0;
}