#include "MatrixFilter.h"
#include "Utils.h"

cv::Vec3b MatrixFilter::CalculatePixel(cv::Mat_<cv::Vec3b>& img, int imageRow, int imageColumn)
{	
	cv::Vec3b pixel;
	for (int color = 0; color < 3; color++)
	{
		float sum = 0;
		float filterSum = 0;
		for (int row = 0; row < matrix.rows; ++row)
		{
			for (int column = 0; column < matrix.cols; ++column)
			{
				int currRow = row - matrix.rows / 2 + imageRow;
				int currCol = column - matrix.cols / 2 + imageColumn;
				if (currRow >= 0 && currRow < img.rows && currCol >= 0 && currCol < img.cols) 
				{
					filterSum += matrix(row, column);
					sum += matrix(row, column) * img(currRow, currCol)[color];
				}
			}
		}
		pixel[color] = return_in_range(sum / (filterSum == 0 ? 1 : filterSum));
	}	
	return pixel;
}