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
	//张正友棋盘格标定法 
	//换另一个相机标定时，程序要更改的内容：boardSize的参数；squareSize的参数；图片总数 i;图片路径

	ifstream inputFileName("list.txt");//从文件中输入路径
	string fileName;//单个文件名
	vector<string> fileNameCount;
	
	Mat image,grayImage;
	Size imageSize;//图像的长宽

	Size boardSize = Size(8, 5);//标定板上行，列的角点数     标定不同标定板时，该变量需要改
	Size squareSize = Size(23.5, 23.5);//每一个小方格的长宽     可改

	int imageCount = 0;//图像数量
	vector<Point2f> imageCorners;//角点
	vector<vector<Point2f>> imageCornersNumbers;//图像中角点的坐标
	vector<vector<Point3f>> objectPoints;//目标在世界坐标系的坐标（即图像的真实坐标尺寸）

	//提取标定板角点数据
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
			cout << "图像的宽度为：" << imageSize.width << endl;
			cout << "图像的高度为：" << imageSize.height << endl;
		}

		bool finds = findChessboardCorners(image, boardSize, imageCorners);//提取角点
		
		if (finds)
		{
			cvtColor(image, grayImage, COLOR_BGR2GRAY);      //彩色图转为灰度图
			//cornerSubPix()函数输入的图片必须是灰度图，该函数提高定位精度
			cornerSubPix(grayImage, imageCorners, Size(5, 5), Size(-1, -1),
				TermCriteria(TermCriteria::EPS + TermCriteria::MAX_ITER, 30, 0.1));
			drawChessboardCorners(image, boardSize, imageCorners, finds);//画出图片中的角点
			imageCornersNumbers.push_back(imageCorners);//保存角点

			namedWindow("Image");
			imshow("Image", image);
			cv::waitKey(100);
		}
				
	}

	int count = imageCornersNumbers.size();//读取图片总数
	cout << endl << "标定图片总数为：" << count << endl;

	//初始化标定板上角点的三维坐标
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


	//相机标定
	Mat cameraMatrix;//相机内在参数
	Mat distCoeffs;//畸变系数
	vector<Mat> rvecs;//每幅图像的旋转向量
	vector<Mat> tvecs;//每幅图像的平移向量
	int flags=0;
	double err = calibrateCamera(objectPoints, imageCornersNumbers, imageSize, cameraMatrix,distCoeffs, rvecs, tvecs, flags,
		TermCriteria(TermCriteria::COUNT|TermCriteria::EPS,30, DBL_EPSILON));

	cout << "error" << err << endl << endl;
	
	std::cout << " Camera intrinsic: " << cameraMatrix.rows << "x" << cameraMatrix.cols << std::endl;
	std::cout << cameraMatrix.at<double>(0, 0) << " " << cameraMatrix.at<double>(0, 1) << " " << cameraMatrix.at<double>(0, 2) << std::endl;
	std::cout << cameraMatrix.at<double>(1, 0) << " " << cameraMatrix.at<double>(1, 1) << " " << cameraMatrix.at<double>(1, 2) << std::endl;
	std::cout << cameraMatrix.at<double>(2, 0) << " " << cameraMatrix.at<double>(2, 1) << " " << cameraMatrix.at<double>(2, 2) << std::endl;


	//标定参数写入文件
	ofstream fs("calib.txt", ios_base::out);
	fs << "相机内参：" << endl;
	fs << cameraMatrix << endl;
	//for (int i = 0; i < 3; i++)
	//{
	//	fs<< cameraMatrix.at<double>(i, 0)<< "\t" << cameraMatrix.at<double>(i, 1) << "\t" << cameraMatrix.at<double>(i, 2) << std::endl;
	//}
	
	fs << "畸变系数：" << endl;
	fs << distCoeffs << endl;
	fs.close();
	
	//矫正
	Mat img = imread("F:/Image/相机标定/compressjpeg/image18.jpg");
	Mat outImage = img.clone();
	Mat map1, map2;
	initUndistortRectifyMap(cameraMatrix, distCoeffs, noArray(), noArray(), imageSize, CV_32FC1, map1, map2);//计算矫正映射的参数
	remap(img, outImage, map1, map2, INTER_LINEAR);
	namedWindow("原图");
	imshow("原图", img);
	namedWindow("矫正图");
	imshow("矫正图", outImage);

		
	cv::waitKey(0);
	return 0;
}