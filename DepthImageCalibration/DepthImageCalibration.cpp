// DepthImageCalibration.cpp : このファイルには 'main' 関数が含まれています。プログラム実行の開始と終了がそこで行われます。
//

#include "pch.h"
#include "Utility.h"

int main(int argc, char* argv[])
{
    using namespace cv;
    std::cout << argc;

    if (argc < 4)
    {
        std::cout << "画像の入っているパスを引数で与えてください" << std::endl;
        std::cout << "例: hoge.exe file_path corner_width corner_height square_scale" << std::endl;
        return 0;
    }

    std::string dir_path = argv[1];

    int corner_width   = std::atoi(argv[2]);
    int corner_height  = std::atoi(argv[3]);
    float square_scale = std::atof(argv[4]);

    std::vector<std::string> img_file_names = get_image_file_name(dir_path);
    
    std::vector<Point2f> img_corner;
    std::vector<std::vector<Point2f>> img_corner_vec;
    
    Size chess_borad_size = Size(corner_width, corner_height);
    Size img_size;
    std::ofstream ofs("corner_save.csv");

    try
    {
        for (int i = 0, n = img_file_names.size(); i < n; i++)
        {
			img_corner = std::vector<Point2f>();
            Mat mat = imread(dir_path + "\\" + img_file_names[i]);
            if (i == 0)
            {
                img_size = mat.size();;
            }
            plot_corner(mat, chess_borad_size, img_corner);
            save_corner(img_corner, img_file_names[i], ofs);
            img_corner_vec.push_back(img_corner);
        }
    }
    catch(Exception e)
    {
        ofs.close();
    }

    ofs.close();

    std::vector<Point3f> corner_3d;

    for (int j = 0; j < corner_height; j++)
    {
        for (int i = 0; i < corner_width; i++)
        {
            corner_3d.push_back(Point3d(i * square_scale, j * square_scale, 0.0f));
        }
    }
 
    std::vector<std::vector<Point3f>> corner_3d_vec;
    for (int i = 0, n = img_file_names.size(); i < n; i++)
    {
        corner_3d_vec.push_back(corner_3d);
    }

    Mat camera_matrix;
    Mat dist_coeffs;
    std::vector<Mat> rvecs;
    std::vector<Mat> tvecs;
    double rms = cv::calibrateCamera(corner_3d_vec, img_corner_vec, img_size, camera_matrix, dist_coeffs, rvecs, tvecs);

    write_camera_matrix(camera_matrix, "camera_matrix.csv");
    write_dist_coeffs_matrix(dist_coeffs, "dist_coeffs.csv");

	for (int i = 0, n = img_file_names.size(); i < n; i++)
	{
		Mat mat = imread(dir_path + "\\" + img_file_names[i]);
		Mat dst;
		undistort(mat, dst, camera_matrix, dist_coeffs);

		imshow("補正画像", dst);
		waitKey(0);
	}
	
    return 0;
}

// プログラムの実行: Ctrl + F5 または [デバッグ] > [デバッグなしで開始] メニュー
// プログラムのデバッグ: F5 または [デバッグ] > [デバッグの開始] メニュー

// 作業を開始するためのヒント: 
//    1. ソリューション エクスプローラー ウィンドウを使用してファイルを追加/管理します 
//   2. チーム エクスプローラー ウィンドウを使用してソース管理に接続します
//   3. 出力ウィンドウを使用して、ビルド出力とその他のメッセージを表示します
//   4. エラー一覧ウィンドウを使用してエラーを表示します
//   5. [プロジェクト] > [新しい項目の追加] と移動して新しいコード ファイルを作成するか、[プロジェクト] > [既存の項目の追加] と移動して既存のコード ファイルをプロジェクトに追加します
//   6. 後ほどこのプロジェクトを再び開く場合、[ファイル] > [開く] > [プロジェクト] と移動して .sln ファイルを選択します
