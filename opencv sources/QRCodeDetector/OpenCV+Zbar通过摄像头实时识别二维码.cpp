#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <zbar.h>
#include <iostream>
#include<zbar/Image.h>

using namespace std;
using namespace zbar;
using namespace cv;

int main()
{
	//打开0号摄像头，从摄像头中获取视频
	VideoCapture capture(0);
	Mat frame;
	//摄像头不存在
	if (!capture.isOpened())
	{
		cout << "ERROR! Unable to open camera\n";
		return 1;

	}

	//创建窗口，名称为“debug”，自动调整大小
	namedWindow("debug", WINDOW_AUTOSIZE);
	//灰度图
	Mat grayFrame;
	//创建zbar图像扫描器
	ImageScanner scanner;
	//配置zbar图片扫描器
	scanner.set_config(ZBAR_NONE, ZBAR_CFG_ENABLE, 1);
	while (1)
	{
		
		Mat grayFrame;
		//从摄像头中抓取一帧
		capture >> frame;
		//图像不为空
		if (!frame.empty())
		{

			//原图转灰度图
			cvtColor(frame, grayFrame, COLOR_BGR2GRAY);
			//显示灰度图
			imshow("debug", grayFrame);


			//创建zbar图像
			Image image(frame.cols, frame.rows, "Y800", grayFrame.data, frame.cols*frame.rows);
			//扫描图像，识别二维码，获取个数
			int symbolCount = scanner.scan(image);
			//获取第一个二维码
			Image::SymbolIterator symbol = image.symbol_begin();
			//遍历所有识别出来的二维码
			while (symbolCount--)
			{
				//输出二维码内容
				cout <<"'" << symbol->get_data() << "'" << endl;
				//获取定位点个数
				int pointCount = symbol->get_location_size();
				//遍历所有定位点
				for (int i = 0; i < pointCount; i++)
					cout << '(' << symbol->get_location_x(i) << ',' << symbol->get_location_y(i) << ")" << endl;
				//下一个二维码
				++symbol;
			}
		}
		//延时50ms，如果按了ESC就退出
		if (waitKey(50) == 27)
			break;
	}

	//销毁窗口
	destroyWindow("debug");

	waitKey(0);
	return 0;
}