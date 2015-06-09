#include "Filters.h"
#include "Utils.h"
#include <vector>
#include <algorithm>

bool cmp(cv::Vec3b& i, cv::Vec3b& j) 
{ 
	return get_pixel_value(i) < get_pixel_value(j); 
}

cv::Vec3b MedianFilter::CalculatePixel(cv::Mat_<cv::Vec3b>& img, int imageRow, int imageColumn)
{
	points.clear();
	int number_of_pixels = 0;
	for (int row = 0; row < size; ++row)
	{
		for (int column = 0; column < size; ++column)
		{
			int currRow = row - size / 2 + imageRow;
			int currCol = column - size / 2 + imageColumn;
			if (currRow >= 0 && currRow < img.rows && currCol >= 0 && currCol < img.cols)
			{
				number_of_pixels++;
				points.push_back(img(currRow, currCol));
			}
		}
	}	
	
	std::nth_element(points.begin(), points.begin() + points.size() / 2, points.end(), cmp);
	return points[points.size() / 2];
}

cv::Vec3b ContrastFilter::CalculatePixel(cv::Mat_<cv::Vec3b>& img, int imageRow, int imageColumn)
{
	cv::Vec3b pixel;
	for (int c = 0; c < 3; c++) 
	{
		pixel[c] = return_in_range(int((alpha * img(imageRow, imageColumn)[c]) + brightness));
	}
	return pixel;
}

uchar ThresholdFilter::CalculatePixel(cv::Mat_<cv::Vec3b>& img, int imageRow, int imageColumn)
{
	cv::Vec3b pixel = img(imageRow, imageColumn);
	
	for (int c = 0; c < 3; c++)
	{
		if (abs(pixel[c] - color[c]) > diff[c]) return 0;
	}

	return 255;
}

uchar ErosionFilter::CalculatePixel(cv::Mat_<uchar>& img, int imageRow, int imageColumn)
{
	for (int row = imageRow - size / 2; row <= imageRow + size / 2; row++)
	{
		for (int column = imageColumn - size / 2; column <= imageColumn + size / 2; column++)
		{
			if (is_in_image(row, column, img) && img(row, column) < 128) {
				return 0;
			}
		}
	}
	return 255;
}

uchar DilationFilter::CalculatePixel(cv::Mat_<uchar>& img, int imageRow, int imageColumn)
{
	for (int row = imageRow - size / 2; row <= imageRow + size / 2; row++)
	{
		for (int column = imageColumn - size / 2; column <= imageColumn + size / 2; column++)
		{
			if (is_in_image(row, column, img) && img(row, column) > 128) {
				return 255;
			}
		}
	}
	return 0;
}