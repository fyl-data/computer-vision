/*------------------------------------------------------------------------------------------*\
This file contains material supporting chapter 11 of the book:
OpenCV3 Computer Vision Application Programming Cookbook
Third Edition
by Robert Laganiere, Packt Publishing, 2016.

This program is free software; permission is hereby granted to use, copy, modify,
and distribute this source code, or portions thereof, for any purpose, without fee,
subject to the restriction that the copyright notice may not be removed
or altered from any source or altered source distribution.
The software is released on an as-is basis and without any warranties of any kind.
In particular, the software is not guaranteed to be fault-tolerant or free from failure.
The author disclaims all warranties with regard to this software, any use,
and any consequent failure, is purely the responsibility of the user.

Copyright (C) 2016 Robert Laganiere, www.laganiere.name
\*------------------------------------------------------------------------------------------*/

#include <iostream>
#include <iomanip>
#include <vector>
#include<string>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/xfeatures2d/nonfree.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <fstream>

#include "CameraCalibrator.h"

int main()
{
	//张正友棋盘格标定法 
	//换另一个相机标定时，程序要更改的内容：boardSize的参数；图片总数 i;图片路径

	cv::Mat image;
	std::vector<std::string> filelist;
	cv::Size boardSize(8, 5);//标定板上行，列的角点数     标定不同标定板时，该变量需要改

	// generate list of chessboard image filename
	// named chessboard01 to chessboard27 in chessboard sub-dir
	for (int i = 1; i <= 20; i++) {

		std::stringstream str;
		str << "chessboards/image" << std::setw(2) << std::setfill('0') << i << ".jpg";
		std::cout << str.str() << std::endl;

		filelist.push_back(str.str());
		image = cv::imread(str.str(), 0);

		// cv::imshow("Board Image",image);	
		// cv::waitKey(100);
	}

////指定路径下读取文件
//	cv::Mat image;
//	std::vector<std::string> filelist;
//	std::fstream files("str.txt");
//	std::string  line;
//	while (getline(files, line))//会自动把\n换行符去掉 
//	{
//		filelist.push_back(line);
//		image = cv::imread(line, 0);
//	}
	





	// Create calibrator object
	CameraCalibrator cameraCalibrator;
	// add the corners from the chessboard
	
	cameraCalibrator.addChessboardPoints(
		filelist,	// filenames of chessboard image
		boardSize, "Detected points");	// size of chessboard

	// calibrate the camera
	cameraCalibrator.setCalibrationFlag(true, true);
	cameraCalibrator.calibrate(image.size());

	// Exampple of Image Undistortion
	image = cv::imread(filelist[14], 0);
	cv::Size newSize(static_cast<int>(image.cols*1.5), static_cast<int>(image.rows*1.5));
	cv::Mat uImage = cameraCalibrator.remap(image, newSize);

	// display camera matrix
	cv::Mat cameraMatrix = cameraCalibrator.getCameraMatrix();
	std::cout << " Camera intrinsic: " << cameraMatrix.rows << "x" << cameraMatrix.cols << std::endl;
	std::cout << cameraMatrix.at<double>(0, 0) << " " << cameraMatrix.at<double>(0, 1) << " " << cameraMatrix.at<double>(0, 2) << std::endl;
	std::cout << cameraMatrix.at<double>(1, 0) << " " << cameraMatrix.at<double>(1, 1) << " " << cameraMatrix.at<double>(1, 2) << std::endl;
	std::cout << cameraMatrix.at<double>(2, 0) << " " << cameraMatrix.at<double>(2, 1) << " " << cameraMatrix.at<double>(2, 2) << std::endl;

	cv::namedWindow("Original Image");
	cv::imshow("Original Image", image);
	cv::namedWindow("Undistorted Image");
	cv::imshow("Undistorted Image", uImage);

	// Store everything in a xml file
	std::ofstream fs("calib1.txt");
	fs << "Intrinsic Mattrix:" << std::endl<<cameraMatrix << std::endl;
	fs << "Distortion:" << std::endl << cameraCalibrator.getDistCoeffs();
	fs.close();

	cv::waitKey();
	return 0;
}
