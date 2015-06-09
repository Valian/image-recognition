#include "MatrixFilters.h"

cv::Vec3b LaplacianFilter::CalculatePixel(cv::Mat_<cv::Vec3b>& img, int row, int column) 
{
	cv::Vec3b pixel = MatrixFilter::CalculatePixel(img, row, column);
	return pixel;
}