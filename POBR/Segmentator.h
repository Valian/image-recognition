#ifndef SEG_H
#define SEG_H
#include <vector>
#include "opencv2/core/core.hpp"

using namespace std;
using namespace cv;

class Segment
{
public:
	vector<Vec2b> Points;
};

class Segmentator
{
private:
	uint minPixelsInSegment;
public:
	Segmentator(uint minPixelsInSegment) : minPixelsInSegment(minPixelsInSegment) {}
	vector<Segment> Segmentate(Mat_<uchar>& img);
};

#endif