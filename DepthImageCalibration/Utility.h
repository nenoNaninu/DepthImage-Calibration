#pragma once

std::vector<std::string> get_image_file_name(const std::string& dir_name);

void plot_corner(cv::Mat& img, cv::Size size, std::vector<cv::Point2f>& img_corner);

void save_corner(std::vector<cv::Point2f>& img_corner, const std::string& img_file_name, std::ofstream& ofs);

void write_camera_matrix(const cv::Mat& camera_matrix, const std::string& file_name);

void write_dist_coeffs_matrix(const cv::Mat& dist_coeffs_matrix, const std::string& file_name);