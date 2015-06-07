#include "FilterBase.h"

template<typename Tin, typename Tout> cv::Mat_<Tout> FilterBase<Tin, Tout>::Apply(cv::Mat_<Tin>& mat)
{	
	cv::Mat_<Tout> newImg = cv::Mat_<Tout>(mat.rows, mat.cols);
	for (int row = 0; row < mat.rows; ++row)
	{
		for (int column = 0; column < mat.cols; ++column)
		{
			newImg(row, column) = this->CalculatePixel(mat, row, column);
		}
	}
	return newImg;
}

