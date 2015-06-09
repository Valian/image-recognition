#include "opencv2/core/core.hpp"

int return_in_range(int a)
{
	return a > 255 ? 255 : (a < 0 ? 0 : a);
}

int get_pixel_value(cv::Vec3b& pixel)
{
	return pixel[0] + pixel[1] + pixel[2];
}

bool is_in_image(cv::Point& p, cv::Mat& img) 
{
	return p.y >= 0 && p.x >= 0 && p.y < img.rows && p.x < img.cols;
}

bool is_in_image(int row, int column, cv::Mat& img)
{
	return row >= 0 && column >= 0 && row < img.rows && column < img.cols;
}