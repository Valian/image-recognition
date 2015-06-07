#ifndef FILTERS_H
#define FILTERS_H
#include "FilterBase.h"

class MedianFilter : public FilterBase<cv::Vec3b, cv::Vec3b>
{	
private:
	int size;
public:
	MedianFilter(int size) : size(size){}
	cv::Vec3b CalculatePixel(cv::Mat_<cv::Vec3b>& mat, int row, int column);
};

class ContrastFilter : public FilterBase<cv::Vec3b, cv::Vec3b>
{
private:
	float alpha;
	int brightness;
public:
	ContrastFilter(float alpha, int brightness) : alpha(alpha), brightness(brightness){}
	cv::Vec3b CalculatePixel(cv::Mat_<cv::Vec3b>& mat, int row, int column);
};

class ThresholdFilter : public FilterBase<cv::Vec3b, uchar>
{
private:
	cv::Vec3b color;
	cv::Vec3b diff;
public:
	ThresholdFilter(cv::Vec3b color, cv::Vec3b diff) : color(color), diff(diff){}
	uchar CalculatePixel(cv::Mat_<cv::Vec3b>& mat, int row, int column);
};

#endif