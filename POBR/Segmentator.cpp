#include "Segmentator.h"
#include "Utils.h"
#include <iostream>
#include <queue>
vector<Segment*> Segmentator::Segmentate(Mat_<uchar>& img)
{
	vector<Segment*> segments;
	queue<Point> queue;
	vector<Point> points;
	Mat_<bool> visited(img.rows, img.cols, false);
	for (int row = 0; row < img.rows; ++row)
	{
		for (int column = 0; column < img.cols; ++column)
		{
			if (visited(row, column)) continue;

			int circuit = 0;
			points.clear();
			queue.push(Point(column, row));
			while (!queue.empty())
			{
				Point point = queue.front();
				queue.pop();
				
				points.push_back(point);	

				
				Point newPoint(point.x - 1, point.y);
				if (is_in_image(newPoint, img) && img(newPoint) > 128 && !visited(newPoint))
				{
					queue.push(newPoint);
					visited(newPoint) = true;
				}

				newPoint = Point(point.x + 1, point.y);
				if (is_in_image(newPoint, img) && img(newPoint) > 128 && !visited(newPoint))
				{
					queue.push(newPoint);
					visited(newPoint) = true;
				}

				newPoint = Point(point.x, point.y - 1);
				if (is_in_image(newPoint, img) && img(newPoint) > 128 && !visited(newPoint))
				{
					queue.push(newPoint);
					visited(newPoint) = true;
				}

				newPoint = Point(point.x, point.y + 1);
				if (is_in_image(newPoint, img) && img(newPoint) > 128 && !visited(newPoint))
				{
					queue.push(newPoint);
					visited(newPoint) = true;
				}

				bool isEdge = false;
				for (int i = -1; i <= 1; i++)
				{
					for (int j = -1; j <= 1; j++) 
					{
						if (is_in_image(i + point.y, j + point.x, img) && img(i + point.y, j + point.x) < 128)
						{
							isEdge = true;
							break;
						}
					}
				}
				if (isEdge) circuit++;
			}

			if (points.size() >= minPixelsInSegment) 
			{
				segments.push_back(new Segment(points, circuit));				
			}
			else if (points.size() > 1)
			{
				for (int i = 0; i < points.size(); i++)
				{
					img(points[i]) = 100;
				}
			}
		}
	}

	return segments;
}


Segment::Segment(vector<Point> points, int circuit) : Points(points), circuit(circuit)
{
	FindMaxAndMin();
	center = CalculateCenterPoint();
	W3 = CalculateW3();
	M3 = CalculateM3();
	M7 = CalculateM7();
}

void Segment::FindMaxAndMin()
{
	min = max = Points[0];
	for (int i = 0; i < Points.size(); i++)
	{
		if (Points[i].x < min.x) min.x = Points[i].x;
		if (Points[i].y < min.y) min.y = Points[i].y;
		if (Points[i].x > max.x) max.x = Points[i].x;
		if (Points[i].y > max.y) max.y = Points[i].y;
	}
}

long long Segment::CalculateGeometricMoment(int p, int q, int i_sub, int j_sub)
{
	long long result = 0;
	for (int i = 0; i < Points.size(); i++)
	{
		result += pow(Points[i].x - i_sub, p) * pow(Points[i].y - j_sub, q);
	}

	return result;
}

Point Segment::CalculateCenterPoint()
{
	long long m10 = CalculateGeometricMoment(1, 0, 0, 0);
	long long m01 = CalculateGeometricMoment(0, 1, 0, 0);
	long long m00 = CalculateGeometricMoment(0, 0, 0, 0);

	if (m00 == 0) return 0;
	return Point(m10 / m00, m01 / m00);
}

long long Segment::CalculateCentralMoment(int p, int q)
{
	Point center = GetCenterPoint();
	return CalculateGeometricMoment(p, q, center.x, center.y);
}

double Segment::CalculateM3()
{
	double M30 = CalculateCentralMoment(3, 0);
	double M12 = CalculateCentralMoment(1, 2);
	double M03 = CalculateCentralMoment(0, 3);
	double M21 = CalculateCentralMoment(2, 1);
	double m00 = CalculateGeometricMoment(0, 0);

	return (pow(M30 - 3 * M12, 2) + (3 * M21 - M03, 2)) / pow(m00, 5);
}

double Segment::CalculateM7()
{
	double M20 = CalculateCentralMoment(2, 0);
	double M02 = CalculateCentralMoment(0, 2);
	double M11 = CalculateCentralMoment(1, 1);
	double m00 = CalculateGeometricMoment(0, 0);

	return (M20 * M02 - pow(M11, 2)) / pow(m00, 4);
}

double Segment::CalculateW3()
{
	return (double(GetCircuit()) / (2 * sqrt(CV_PI * double(GetSurface())))) - 1;
}

