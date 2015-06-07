#include "Segmentator.h"
#include "Utils.h"
#include <stack>



vector<Segment> Segmentator::Segmentate(Mat_<uchar>& img)
{
	vector<Segment> segments;
	stack<Vec2b> stack;
	Mat_<bool> visited(img.rows, img.cols, false);
	for (int row = 0; row < img.rows; ++row)
	{
		for (int column = 0; column < img.cols; ++column)
		{
			if (visited(row, column)) continue;

			Segment newSeg;
			stack.push(Vec2b(row, column));
			while (!stack.empty())
			{
				Vec2b point = stack.top();
				stack.pop();
				if (visited(point[0], point[1])) continue;
				
				visited(point[0], point[1]) = true;
				newSeg.Points.push_back(point);				
				
				if (is_in_image(point[0] - 1, point[1], img) && img(point[0] - 1, point[1]) > 128)
					stack.push(Vec2b(point[0] - 1, point[1]));
				if (is_in_image(point[0] + 1, point[1], img) && img(point[0] + 1, point[1]) > 128)
					stack.push(Vec2b(point[0] + 1, point[1]));
				if (is_in_image(point[0], point[1] - 1, img) && img(point[0], point[1] - 1) > 128)
					stack.push(Vec2b(point[0], point[1] - 1));
				if (is_in_image(point[0], point[1] + 1, img) && img(point[0], point[1] + 1) > 128)
					stack.push(Vec2b(point[0], point[1] + 1));				
			}

			if (newSeg.Points.size() >= minPixelsInSegment) 
			{
				segments.push_back(newSeg);
			}
			else if (newSeg.Points.size() > 1)
			{
				for (int i = 0; i < newSeg.Points.size(); i++)
				{
					img(newSeg.Points[i][0], newSeg.Points[i][1]) = 128;
				}
			}
		}
	}

	return segments;
}
