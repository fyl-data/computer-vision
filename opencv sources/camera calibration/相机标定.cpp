#include<opencv2/opencv.hpp>
#include<opencv2/core/core.hpp>
#include<opencv2/imgproc/imgproc.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<calib3d/calib3d.hpp>
#include<iostream>
#include<fstream>
#include<string>

using namespace cv;
using namespace std;

int main()
{
	//���������̸�궨�� 
	//����һ������궨ʱ������Ҫ���ĵ����ݣ�boardSize�Ĳ�����squareSize�Ĳ�����ͼƬ���� i;ͼƬ·��

	ifstream inputFileName("list.txt");//���ļ�������·��
	string fileName;//�����ļ���
	vector<string> fileNameCount;
	
	Mat image,grayImage;
	Size imageSize;//ͼ��ĳ���

	Size boardSize = Size(8, 5);//�궨�����У��еĽǵ���     �궨��ͬ�궨��ʱ���ñ�����Ҫ��
	Size squareSize = Size(23.5, 23.5);//ÿһ��С����ĳ���     �ɸ�

	int imageCount = 0;//ͼ������
	vector<Point2f> imageCorners;//�ǵ�
	vector<vector<Point2f>> imageCornersNumbers;//ͼ���нǵ������
	vector<vector<Point3f>> objectPoints;//Ŀ������������ϵ�����꣨��ͼ�����ʵ����ߴ磩

	//��ȡ�궨��ǵ�����
	for (int i = 0; i < 20; i++)
	{ 
		imageCount++;
		cout << "ImageCount=" << imageCount << endl;

		getline(inputFileName, fileName);
		image = imread(fileName);
				
		if (imageCount == 1)
		{
			imageSize.width = image.cols;
			imageSize.height = image.rows;
			cout << "ͼ��Ŀ��Ϊ��" << imageSize.width << endl;
			cout << "ͼ��ĸ߶�Ϊ��" << imageSize.height << endl;
		}

		bool finds = findChessboardCorners(image, boardSize, imageCorners);//��ȡ�ǵ�
		
		if (finds)
		{
			cvtColor(image, grayImage, COLOR_BGR2GRAY);      //��ɫͼתΪ�Ҷ�ͼ
			//cornerSubPix()���������ͼƬ�����ǻҶ�ͼ���ú�����߶�λ����
			cornerSubPix(grayImage, imageCorners, Size(5, 5), Size(-1, -1),
				TermCriteria(TermCriteria::EPS + TermCriteria::MAX_ITER, 30, 0.1));
			drawChessboardCorners(image, boardSize, imageCorners, finds);//����ͼƬ�еĽǵ�
			imageCornersNumbers.push_back(imageCorners);//����ǵ�

			namedWindow("Image");
			imshow("Image", image);
			cv::waitKey(100);
		}
				
	}

	int count = imageCornersNumbers.size();//��ȡͼƬ����
	cout << endl << "�궨ͼƬ����Ϊ��" << count << endl;

	//��ʼ���궨���Ͻǵ����ά����
	for (int t = 0; t < count; t++)
	{
		vector<Point3f> realCorner;
		for (int i = 0; i < boardSize.height; i++)
		{
			for (int j = 0; j < boardSize.width; j++)
			{
				realCorner.push_back(Point3f(i*squareSize.width, j*squareSize.height, 0.0f));
			}
		}
		objectPoints.push_back(realCorner);
	}


	//����궨
	Mat cameraMatrix;//������ڲ���
	Mat distCoeffs;//����ϵ��
	vector<Mat> rvecs;//ÿ��ͼ�����ת����
	vector<Mat> tvecs;//ÿ��ͼ���ƽ������
	int flags=0;
	double err = calibrateCamera(objectPoints, imageCornersNumbers, imageSize, cameraMatrix,distCoeffs, rvecs, tvecs, flags,
		TermCriteria(TermCriteria::COUNT|TermCriteria::EPS,30, DBL_EPSILON));

	cout << "error" << err << endl << endl;
	
	std::cout << " Camera intrinsic: " << cameraMatrix.rows << "x" << cameraMatrix.cols << std::endl;
	std::cout << cameraMatrix.at<double>(0, 0) << " " << cameraMatrix.at<double>(0, 1) << " " << cameraMatrix.at<double>(0, 2) << std::endl;
	std::cout << cameraMatrix.at<double>(1, 0) << " " << cameraMatrix.at<double>(1, 1) << " " << cameraMatrix.at<double>(1, 2) << std::endl;
	std::cout << cameraMatrix.at<double>(2, 0) << " " << cameraMatrix.at<double>(2, 1) << " " << cameraMatrix.at<double>(2, 2) << std::endl;


	//�궨����д���ļ�
	ofstream fs("calib.txt", ios_base::out);
	fs << "����ڲΣ�" << endl;
	fs << cameraMatrix << endl;
	//for (int i = 0; i < 3; i++)
	//{
	//	fs<< cameraMatrix.at<double>(i, 0)<< "\t" << cameraMatrix.at<double>(i, 1) << "\t" << cameraMatrix.at<double>(i, 2) << std::endl;
	//}
	
	fs << "����ϵ����" << endl;
	fs << distCoeffs << endl;
	fs.close();
	
	//����
	Mat img = imread("F:/Image/����궨/compressjpeg/image18.jpg");
	Mat outImage = img.clone();
	Mat map1, map2;
	initUndistortRectifyMap(cameraMatrix, distCoeffs, noArray(), noArray(), imageSize, CV_32FC1, map1, map2);//�������ӳ��Ĳ���
	remap(img, outImage, map1, map2, INTER_LINEAR);
	namedWindow("ԭͼ");
	imshow("ԭͼ", img);
	namedWindow("����ͼ");
	imshow("����ͼ", outImage);

		
	cv::waitKey(0);
	return 0;
}