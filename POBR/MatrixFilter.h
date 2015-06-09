#ifndef MATRIX_FILTER_H
#define MATRIX_FILTER_H
#include "opencv2/core/core.hpp"
#include "FilterBase.h"

class MatrixFilter : public FilterBase<cv::Vec3b, cv::Vec3b>
{	
	protected:
		float* matrix;
		int size;
	public:
		cv::Vec3b CalculatePixel(cv::Mat_<cv::Vec3b>& img, int row, int column); 
		~MatrixFilter() { delete matrix; }
};

#endif