#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "FilterRunner.h"
#include "FilterRunner.cpp"
#include "Filters.h"
#include "MatrixFilters.h"
#include "Segmentator.h"
#include <iostream>
#include <math.h>

using namespace std;

cv::Mat convertBackgroundToGreyscale(cv::Mat& img, cv::Mat& quality)
{
	cv::Mat_<cv::Vec3b> image = img;
	FilterRunner<cv::Vec3b> quality_filters({ new ContrastFilter(0.9f, -50) });
	FilterRunner<uchar> closeFilters({ new ErosionFilter(5), new DilationFilter(5) });
	ThresholdFilter threshold_filter(cv::Vec3b(30, 30, 30), cv::Vec3b(30, 30, 30));
	quality = quality_filters.Run(image);
	return closeFilters.Run(threshold_filter.Apply(cv::Mat_<cv::Vec3b>(quality)));
}

cv::Mat convertMugToGreyscale(cv::Mat& img, cv::Mat& quality)
{
	cv::Mat_<cv::Vec3b> image = img;
	FilterRunner<cv::Vec3b> quality_filters({ new LaplacianFilter(), new BoxFilter(3) });
	FilterRunner<uchar> closeFilters({ new DilationFilter(7), new ErosionFilter(7) });
	ThresholdFilter threshold_filter(cv::Vec3b(110, 140, 250), cv::Vec3b(70, 70, 70));
	quality = quality_filters.Run(image);
	return closeFilters.Run(threshold_filter.Apply(cv::Mat_<cv::Vec3b>(quality)));
}

void pairSegments(cv::Mat& img, vector<Segment*> mugSegments, vector<Segment*> backgroundSegments)
{
	for (int i = 0; i < mugSegments.size(); i++) 
	{
		for (int j = 0; j < backgroundSegments.size(); j++) 
		{
			Rect bgRect = Rect(backgroundSegments[j]->GetMin(), backgroundSegments[j]->GetMax());
			if (bgRect.contains(mugSegments[i]->GetMin()) && bgRect.contains(mugSegments[i]->GetMax()))
			{
				cout << "Found image at " << bgRect << "!" << endl;
				rectangle(img, bgRect, Scalar(255, 0, 0, 255), 3, LINE_4);
			}
		}
	}
}

vector<Segment*> findSegments(cv::Mat_<uchar>& img, float maxAspectRatio, Vec2d w3, Vec2d m3, Vec2d m7)
{
	Segmentator segmentator(400);
	vector<Segment*> segments = segmentator.Segmentate(img);
	cout << segments.size() << endl;
	for (int i = 0; i < segments.size(); i++)
	{
		cout << "C: " << segments[i]->GetCircuit() << "   S: " << segments[i]->GetSurface();
		cout << "   W3: " << segments[i]->GetW3() << "   M3: " << segments[i]->GetM3() << "     M7: " << segments[i]->GetM7() << endl;
	}

	vector<Segment*> filtered;
	for (int i = 0; i < segments.size(); i++)
	{
		float width = segments[i]->GetMax().x - segments[i]->GetMin().x;
		float height = segments[i]->GetMax().y - segments[i]->GetMin().y;
		float aspectRatio = width / height;
		if (abs(segments[i]->GetM3() - m3[0]) < m3[1] && abs(segments[i]->GetW3() - w3[0]) < w3[1] && abs(segments[i]->GetM7() - m7[0]) < m7[1] && aspectRatio < maxAspectRatio && 1 / aspectRatio < maxAspectRatio)
		{
			filtered.push_back(segments[i]);		
			rectangle(img, segments[i]->GetMin(), segments[i]->GetMax(), Scalar(255, 255, 255, 255));
			circle(img, segments[i]->GetCenterPoint(), 2, Scalar(200, 255, 255, 255), 3);
		}
		else 
		{			
			for (int j = 0; j < segments[i]->Points.size(); j++) {
				img(segments[i]->Points[j]) = 60;
			}			
		}
		
	}
	
	return filtered;
}


int main(int, char *[])
{
	string file = "../Images/7_kupon.png";
	cv::Mat image = cv::imread(file);
	cout << "File " << file << endl;
	cv::Mat qualityMug;
	cv::Mat qualityBackground;
	cv::Mat_<uchar> mug = convertMugToGreyscale(image, qualityMug);
	cv::Mat_<uchar> background = convertBackgroundToGreyscale(image, qualityBackground);
	cout << "Calculating mug segments..." << endl;
	vector<Segment*> mugSeg = findSegments(mug, 1.3f, Vec2d(2.4, 0.9), Vec2d(0.02, 0.02), Vec2d(0.01, 0.005));
	cout << endl << "Calculating background segments..." << endl;
	vector<Segment*> backgroundSeg = findSegments(background, 3.f, Vec2d(2.55, 0.6), Vec2d(0.02, 0.02), Vec2d(0.024, 0.01));
	pairSegments(image, mugSeg, backgroundSeg);
	cv::imshow("Image", image);
	//cv::imshow("Quality background", qualityBackground);
	//cv::imshow("Quality mug", qualityMug);
	cv::imshow("Background", background);
	cv::imshow("Mug", mug);
	cv::waitKey(-1);
	return 0;
}