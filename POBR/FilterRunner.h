#ifndef RUNNER_H
#define RUNNER_H
#include "FilterBase.h"

template<typename Tinout> class FilterRunner
{
	private:
		std::vector<FilterBase<Tinout, Tinout>*> filters;
	public:
		FilterRunner(std::vector<FilterBase<Tinout, Tinout>*> filters);
		~FilterRunner();
		cv::Mat_<Tinout> Run(cv::Mat_<Tinout>& img);
};


#endif