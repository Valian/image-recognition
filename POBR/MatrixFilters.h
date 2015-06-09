#ifndef MATRIX_FILTERS_H
#define MATRIX_FILTERS_H
#define M_PI 3.14159265358979323846
#include <iostream>
#include "opencv2/core/core.hpp"
#include "MatrixFilter.h"

class BoxFilter : public MatrixFilter
{
	public:
		BoxFilter(int s)
		{
			size = s;
			matrix = new float[size * size];
			for (int i = 0; i < size*size; i++) matrix[i] = 1;
		}
};

class SharpenFilter : public MatrixFilter
{
	public:
		SharpenFilter()
		{
			size = 3;
			matrix = new float[size * size] {0, -1, 0, -1, 5, -1, 0, -1, 0};
		}
};

class LaplacianFilter: public MatrixFilter
{
public:
	LaplacianFilter()
	{
		size = 3;
		matrix = new float[size * size] {-1, -1, -1, -1, 9, -1, -1, -1, -1};
	}
	cv::Vec3b CalculatePixel(cv::Mat_<cv::Vec3b>& img, int row, int column);
};


#endif