#ifndef FILTERS_H
#define FILTERS_H
#include "FilterBase.h"

class MedianFilter : public FilterBase<cv::Vec3b, cv::Vec3b>
{	
private:
	int size;
	std::vector<cv::Vec3b> points;
public:
	MedianFilter(int size) : size(size){ points.reserve(size * size); }
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

class ErosionFilter : public FilterBase < uchar, uchar >
{
private:
	int size;
public:
	ErosionFilter(int s) : size(s){}
	uchar CalculatePixel(cv::Mat_<uchar>& mat, int row, int column);
};

class DilationFilter : public FilterBase < uchar, uchar >
{
private:
	int size;
public:
	DilationFilter(int s) : size(s){}
	uchar CalculatePixel(cv::Mat_<uchar>& mat, int row, int column);
};


#endif