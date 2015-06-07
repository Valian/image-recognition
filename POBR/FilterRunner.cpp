#include "FilterRunner.h"
#include "FilterBase.cpp"

template<typename Tinout> FilterRunner<Tinout>::FilterRunner(std::vector<FilterBase<Tinout, Tinout>*> filters) : filters(filters) {}

template<typename Tinout> FilterRunner<Tinout>::~FilterRunner()
{
	for (int i = 0; i < filters.size(); i++)
	{
		delete filters[i];
	}
}

template<typename Tinout> cv::Mat_<Tinout> FilterRunner<Tinout>::Run(cv::Mat_<Tinout>& img)
{
	for (int i = 0; i < filters.size(); i++) 
	{
		img = filters[i] -> Apply(img);
	}
	return img;
}