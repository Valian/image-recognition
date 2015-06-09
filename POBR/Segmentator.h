#ifndef SEG_H
#define SEG_H
#include <vector>
#include <list>
#include "opencv2/core/core.hpp"

using namespace std;
using namespace cv;

class Segment
{
private:
	Point min, max, center;
	int circuit;
	double M3, W3, M7;

	void FindMaxAndMin();
	Point CalculateCenterPoint();
	double CalculateM3();
	double CalculateW3();
	double CalculateM7();

	long long CalculateGeometricMoment(int p, int q, int i_sub = 0, int j_sub = 0);
	long long CalculateCentralMoment(int p, int q);

public:
	vector<Point> Points;
	Segment(vector<Point> points, int circuit);	
	int GetCircuit() { return circuit; }
	int GetSurface() { return Points.size(); }
	Point GetCenterPoint() { return center; }
	Point GetMin() { return min; }
	Point GetMax() { return max; }
	double GetM3() { return M3; }
	double GetW3() { return W3; }
	double GetM7() { return M7; }
};

class Segmentator
{
private:
	uint minPixelsInSegment;
public:
	Segmentator(uint minPixelsInSegment) : minPixelsInSegment(minPixelsInSegment) {}
	vector<Segment*> Segmentate(Mat_<uchar>& img);
};

#endif