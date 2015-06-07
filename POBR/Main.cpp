#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "FilterRunner.h"
#include "FilterRunner.cpp"
#include "Filters.h"
#include "MatrixFilters.h"
#include "Segmentator.h"
#include <iostream>
#include <math.h>

using namespace std;

class BoundingBox
{
public:

	cv::Vec2i min;
	cv::Vec2i max;
	cv::Vec3b color;

	BoundingBox()
	{
		min = cv::Vec2i();
		max = cv::Vec2i();
		color = cv::Vec3b();
	}

	BoundingBox(int row, int column, cv::Vec3b c)
	{
		min = cv::Vec2i(row, column);
		max = cv::Vec2i(row, column);
		color = c;
	}

	void extend(int x, int y)
	{
		if (min[0] > x) min[0] = x;
		if (max[0] < x) max[0] = x;
		if (min[1] > y) min[1] = y;
		if (max[1] < y) max[1] = y;
	}
};

int get_binary_value(cv::Mat_<cv::Vec3b>& img, int x, int y, BoundingBox* box = NULL)
{
	if (box != NULL)
	{
		cv::Vec3b c = box->color;
		return img(x, y)[0] == c[0] && img(x, y)[1] == c[1] && img(x, y)[2] == c[2];
	}
	return img(x, y)[0] + img(x, y)[1] + img(x, y)[2] < 128 * 3;
}

int apply_matrix(cv::Mat_<cv::Vec3b>& img, cv::Mat_<cv::Vec3b>& target, const int matrix[9])
{
	int pixels = 0;
	for (int x = 1; x < img.rows - 1; ++x)
	{
		for (int y = 1; y < img.cols - 1; ++y)
		{
			bool all_in = true;
			bool nothing_in = true;
			for (int a = 0; a < 3; a++)
			{
				for (int b = 0; b < 3; b++)
				{
					int value = get_binary_value(img, x + a - 1, y + b - 1);
					all_in = all_in && value == 1;
					nothing_in = nothing_in && value == 0;
				}
			}

			bool is_edge = !all_in && !nothing_in && get_binary_value(img, x, y) == 1;
			target(x, y)[0] = 255 * !is_edge;
			target(x, y)[1] = 255 * !is_edge;
			target(x, y)[2] = 255 * !is_edge;

			pixels += is_edge;
		}
	}

	return pixels;
}

int calculate_circle(cv::Mat_<cv::Vec3b>& img, cv::Mat_<cv::Vec3b>& target)
{
	const int MATRIX[9] = { 1, 1, 1, 1, 1, 1, 1, 1, 1 };
	return apply_matrix(img, target, MATRIX);
}

long long calculate_geometric_moment(cv::Mat_<cv::Vec3b>& img, int p, int q, int i_sub = 0, int j_sub = 0, BoundingBox* box = NULL)
{
	long long result = 0;

	cv::Vec2i min = cv::Vec2i();
	cv::Vec2i max = cv::Vec2i(img.rows, img.cols);

	if (box != NULL)
	{
		min = box->min;
		max = box->max;
	}

	for (long long i = min[0]; i < max[0]; ++i)
	{
		for (long long j = min[1]; j < max[1]; ++j)
		{
			result += pow(i - i_sub, p) * pow(j - j_sub, q)  * get_binary_value(img, i, j, box);
		}
	}

	return result;
}

cv::Vec2i get_center_point(cv::Mat_<cv::Vec3b>& img, BoundingBox* box = NULL)
{
	long long m10 = calculate_geometric_moment(img, 1, 0, 0, 0, box);
	long long m01 = calculate_geometric_moment(img, 0, 1, 0, 0, box);
	long long m00 = calculate_geometric_moment(img, 0, 0, 0, 0, box);

	if (m00 == 0) return 0;
	return cv::Vec2i(m10 / m00, m01 / m00);
}

long long calculate_central_moment(cv::Mat_<cv::Vec3b>& img, int p, int q)
{
	cv::Vec2i center = get_center_point(img);
	return calculate_geometric_moment(img, p, q, center[1], center[0]);
}

double calculate_M3(cv::Mat_<cv::Vec3b>& img)
{
	double M30 = calculate_central_moment(img, 3, 0);
	double M12 = calculate_central_moment(img, 1, 2);
	double M03 = calculate_central_moment(img, 0, 3);
	double M21 = calculate_central_moment(img, 2, 1);
	double m00 = calculate_geometric_moment(img, 0, 0);

	return (pow(M30 - 3 * M12, 2) + (3 * M21 - M03, 2)) / pow(m00, 5);
}

double calculate_M7(cv::Mat_<cv::Vec3b>& img)
{
	double M20 = calculate_central_moment(img, 2, 0);
	double M02 = calculate_central_moment(img, 0, 2);
	double M11 = calculate_central_moment(img, 1, 1);
	double m00 = calculate_geometric_moment(img, 0, 0);

	return (M20 * M02 - pow(M11, 2)) / pow(m00, 4);
}

double calculate_W3(int S, int L)
{
	return (double(L) / (2 * sqrt(CV_PI * double(S)))) - 1;
}

void find_bounding_boxes(cv::Mat_<cv::Vec3b>& img, BoundingBox boxes[])
{
	int current = 0;
	for (int x = 0; x < img.rows; ++x)
	{
		for (int y = 0; y < img.cols; ++y)
		{
			if (!get_binary_value(img, x, y)) continue;

			for (int i = 0; i <= current; i++)
			{
				if (i == current)
				{
					boxes[i] = BoundingBox(x, y, img(x, y));
					current++;
					break;
				}
				if (boxes[i].color == img(x, y))
				{
					boxes[i].extend(x, y);
					break;
				}
			}
		}
	}
}

void calculate_directions(cv::Mat_<cv::Vec3b>& img)
{
	int count = 5;
	BoundingBox* boxes = new BoundingBox[5];
	for (int i = 0; i < count; i++)
	{
		boxes[i] = BoundingBox();
	}

	find_bounding_boxes(img, boxes);
	for (int i = 0; i < count; i++)
	{
		cv::Vec2i center = (boxes[i].max - boxes[i].min) / 2 + boxes[i].min;
		cv::Vec2i weight_center = get_center_point(img, &boxes[i]);
		cout << boxes[i].color << "  min: " << boxes[i].min << "  max: " << boxes[i].max << endl;
		cout << "Center: " << center << "  weight_center: " << weight_center << endl;
		cout << "Diff" << cv::Vec2i(weight_center[1] - center[1], weight_center[0] - center[0]) << endl;
		cout << "Direction: " << atan2(weight_center[1] - center[1], weight_center[0] - center[0]) * 180 / CV_PI << endl;
	}
}

cv::Mat calculate(cv::Mat& I)
{
	CV_Assert(I.depth() != sizeof(uchar));
	cv::Mat  copy(I.rows, I.cols, CV_8UC3);
	switch (I.channels())  {
	case 3:
		cv::Mat_<cv::Vec3b> _I = I;
		cv::Mat_<cv::Vec3b> _R = copy;

		long long surface = calculate_geometric_moment(_I, 0, 0);
		int circle = calculate_circle(_I, _R);
		cout << "-" << "S: " << surface << endl;
		cout << "-" << "Circle: " << circle << endl;
		cout << "-" << "Central point: " << get_center_point(_I) << endl;
		cout << "-" << "W3: " << calculate_W3(surface, circle) << endl;
		cout << "-" << "M3: " << calculate_M3(_I) << endl;
		cout << "-" << "M7: " << calculate_M7(_I) << endl;
		cout << endl << endl;
		calculate_directions(_I);
		copy = _R;
		break;
	}
	return copy;
}

cv::Mat_<uchar> convertToGrayscale(cv::Mat& img)
{
	cv::Mat_<cv::Vec3b> image = img;

	FilterRunner<cv::Vec3b> quality_filters({ new ContrastFilter(1.5f, -50), new MedianFilter(3)});
	image = quality_filters.Run(image);
	ThresholdFilter threshold_filter(cv::Vec3b(50, 50, 50), cv::Vec3b(50, 50, 50));
	cv::Mat_<uchar> grayscale = threshold_filter.Apply(image);
	return grayscale;
}

void findSegments(cv::Mat_<uchar>& img)
{
	Segmentator segmentator(200);
	vector<Segment> segments = segmentator.Segmentate(img);
	cout << segments.size() << endl;
}


int main(int, char *[])
{
	string file = "../Images/1_kupon.png";
	cv::Mat image = cv::imread(file);
	cout << "File " << file << endl;
	//cv::Mat max = calculate(image);

	cv::imshow("Image", image);
	cv::Mat_<uchar> grayscale = convertToGrayscale(image);
	findSegments(grayscale);
	cv::imshow("Modified", grayscale);
	cv::waitKey(-1);
	return 0;
}