#include "opencv2/core/core.hpp"

int return_in_range(int a)
{
	return a > 255 ? 255 : (a < 0 ? 0 : a);
}

int get_pixel_value(cv::Vec3b pixel)
{
	return pixel[0] + pixel[1] + pixel[2];
}