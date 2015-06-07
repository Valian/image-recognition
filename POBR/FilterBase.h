#ifndef FILTER_H
#define FILTER_H
#include "opencv2/core/core.hpp"

template<typename Tin, typename Tout> class FilterBase
{
	public:
		cv::Mat_<Tout> Apply(cv::Mat_<Tin>& mat);
		virtual Tout CalculatePixel(cv::Mat_<Tin>& mat, int row, int column) = 0;
};

#endif