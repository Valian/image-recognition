#ifndef UTILS_H
#define UTILS_H
#include "opencv2/core/core.hpp"

int return_in_range(int a);
int get_pixel_value(cv::Vec3b& pixel);
bool is_in_image(cv::Point& p, cv::Mat& img);
bool is_in_image(int row, int column, cv::Mat& img);

#endif