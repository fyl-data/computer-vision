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
	//��0������ͷ��������ͷ�л�ȡ��Ƶ
	VideoCapture capture(0);
	Mat frame;
	//����ͷ������
	if (!capture.isOpened())
	{
		cout << "ERROR! Unable to open camera\n";
		return 1;

	}

	//�������ڣ�����Ϊ��debug�����Զ�������С
	namedWindow("debug", WINDOW_AUTOSIZE);
	//�Ҷ�ͼ
	Mat grayFrame;
	//����zbarͼ��ɨ����
	ImageScanner scanner;
	//����zbarͼƬɨ����
	scanner.set_config(ZBAR_NONE, ZBAR_CFG_ENABLE, 1);
	while (1)
	{
		
		Mat grayFrame;
		//������ͷ��ץȡһ֡
		capture >> frame;
		//ͼ��Ϊ��
		if (!frame.empty())
		{

			//ԭͼת�Ҷ�ͼ
			cvtColor(frame, grayFrame, COLOR_BGR2GRAY);
			//��ʾ�Ҷ�ͼ
			imshow("debug", grayFrame);


			//����zbarͼ��
			Image image(frame.cols, frame.rows, "Y800", grayFrame.data, frame.cols*frame.rows);
			//ɨ��ͼ��ʶ���ά�룬��ȡ����
			int symbolCount = scanner.scan(image);
			//��ȡ��һ����ά��
			Image::SymbolIterator symbol = image.symbol_begin();
			//��������ʶ������Ķ�ά��
			while (symbolCount--)
			{
				//�����ά������
				cout <<"'" << symbol->get_data() << "'" << endl;
				//��ȡ��λ�����
				int pointCount = symbol->get_location_size();
				//�������ж�λ��
				for (int i = 0; i < pointCount; i++)
					cout << '(' << symbol->get_location_x(i) << ',' << symbol->get_location_y(i) << ")" << endl;
				//��һ����ά��
				++symbol;
			}
		}
		//��ʱ50ms���������ESC���˳�
		if (waitKey(50) == 27)
			break;
	}

	//���ٴ���
	destroyWindow("debug");

	waitKey(0);
	return 0;
}