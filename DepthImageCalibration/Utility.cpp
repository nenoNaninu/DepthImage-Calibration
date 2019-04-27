#include "pch.h"
#include "Utility.h"
#include "MouseParameter.h"

std::vector<std::string> get_image_file_name(const std::string& dir_name) {

    HANDLE hFind;
    WIN32_FIND_DATA win32fd;
    std::vector<std::string> file_names;

    // png,jpg,bmpの拡張子のファイルのみを読み込む
    std::string extension[3] = { "png" ,"jpg", "bmp" };

    for (int i = 0; i < 3; i++) {
        std::string search_name = dir_name + "\\*." + extension[i];
        hFind = FindFirstFile(search_name.c_str(), &win32fd);

        if (hFind == INVALID_HANDLE_VALUE) {
            continue;
        }
        do {
            if (win32fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
            }
            else {
                std::cout << win32fd.cFileName << std::endl;
                file_names.push_back(win32fd.cFileName);
            }
        } while (FindNextFile(hFind, &win32fd));

        FindClose(hFind);
    }
    return file_names;
}

bool plot_corner_sub(cv::Mat& img, cv::Size& size, std::vector<cv::Point2f>& img_corner, std::string window_name)
{
    MouseParameter prevMouseEvent = {};
    MouseParameter mouseEvent = {};
    cv::namedWindow(window_name);

    cv::setMouseCallback(window_name, mFunc, &mouseEvent);
    cv::Scalar scalar1(0, 255, 0);
    cv::Scalar scalar2(0, 0, 255);
	cv::resize(img, img, cv::Size(img.rows * 2, img.cols * 2));

    for (int i = 0; i < size.height; i++)
    {
        std::cout << "横方向に進んでいく" << std::endl;
        for (int j = 0; j < size.width; j++)
        {
            bool clicked = false;
            while (!clicked)
            {
                cv::imshow(window_name, img);

                char key = cv::waitKey(100);

                if (mouseEvent.event == cv::MouseEventFlags::EVENT_FLAG_LBUTTON && prevMouseEvent.event != cv::MouseEventFlags::EVENT_FLAG_LBUTTON)
                {
                    cv::Point point = cv::Point(mouseEvent.x, mouseEvent.y);
                    cv::circle(img, point, 10, scalar1);
                    if (0 < img_corner.size())
                    {
                        cv::Point last = img_corner[img_corner.size() - 1] * 2;
                        cv::line(img, point, last, scalar2);
                    }
					cv::Point true_point = point / 2;
                    img_corner.push_back(true_point);
                    clicked = true;
                }

                if (key == 'c')
                {
                    img_corner.clear();
                    return false;
                }
                prevMouseEvent = mouseEvent;
            }
        }
    }
}

void plot_corner(cv::Mat& img, cv::Size size, std::vector<cv::Point2f>& img_corner)
{

    std::string plot_window_name = "plot";

    bool success = false;

    while(!success)
    {
        cv::Mat clone_img = img.clone();
        success = plot_corner_sub(clone_img, size, img_corner, plot_window_name);
    }
}

void save_corner(std::vector<cv::Point2f>& img_corner, const std::string& img_file_name, std::ofstream& ofs)
{
    ofs << "[" << img_file_name << "]" << std::endl;

    for (cv::Point2f element : img_corner)
    {
        ofs << element.x << "," << element.y << std::endl;
    }
}

void write_camera_matrix(const cv::Mat& camera_matrix, const std::string& file_name)
{
    double* camera_mat_ptr = (double*)camera_matrix.data;

    std::ofstream ofs(file_name);

    ofs << camera_mat_ptr[0] << "," << camera_mat_ptr[1] << "," << camera_mat_ptr[2] << std::endl;
    ofs << camera_mat_ptr[3] << "," << camera_mat_ptr[4] << "," << camera_mat_ptr[5] << std::endl;
    ofs << camera_mat_ptr[6] << "," << camera_mat_ptr[7] << "," << camera_mat_ptr[8] << std::endl;

    ofs.close();
}

void write_dist_coeffs_matrix(const cv::Mat& dist_coeffs_matrix, const std::string& file_name)
{
    double* mat_ptr = (double*)dist_coeffs_matrix.data;

    std::ofstream ofs(file_name);

    ofs << mat_ptr[0] << "," << mat_ptr[1] << "," << mat_ptr[2] << mat_ptr[3] << "," << mat_ptr[4] << std::endl;

    ofs.close();
}
