#ifndef MATRIX_FILTERS_H
#define MATRIX_FILTERS_H
#define M_PI 3.14159265358979323846
#include <iostream>
#include "opencv2/core/core.hpp"
#include "MatrixFilter.h"

class BoxFilter : public MatrixFilter
{
	public:
		BoxFilter(int size)
		{
			matrix = cv::Mat_<float>(size, size, 1);
		}
};

class GaussianFilter : public MatrixFilter {
	public:
		GaussianFilter(int size)
		{
			matrix = cv::Mat_<float>(size, size);
			double sigma = 1;
			double mean = size / 2;
			for (int x = 0; x < size; ++x)
			{
				for (int y = 0; y < size; ++y)
				{
					matrix(x, y) = exp(-0.5 * (pow((x - mean) / sigma, 2.0) + pow((y - mean) / sigma, 2.0)))
						/ (2 * M_PI * sigma * sigma);
					std::cout << matrix(x, y) << ", ";
				}
				std::cout << std::endl;
			}
		}
};

class SharpenFilter : public MatrixFilter
{
	public:
		SharpenFilter()
		{
			matrix = (cv::Mat_<float>(3, 3) << 0, -1, 0, -1, 5, -1, 0, -1, 0);
		}
};

#endif