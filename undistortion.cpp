#include <opencv2/opencv.hpp>

int main(const int argc, const char *const *const argv) {
    if (argc < 3) { return -1; }

    auto fs = cv::FileStorage(argv[1], cv::FileStorage::READ);

    cv::Mat camera_matrix, distortion_coefficients;
    int image_width, image_height;
    fs["camera_matrix"] >> camera_matrix;
    fs["distortion_coefficients"] >> distortion_coefficients;
    fs["image_width"] >> image_width;
    fs["image_height"] >> image_height;

    const auto image_size = cv::Size(image_width, image_height);
    cv::Mat map1, map2;
    cv::initUndistortRectifyMap(camera_matrix, distortion_coefficients,
                                cv::Mat(), camera_matrix, image_size,
                                CV_16SC2, map1, map2);

    const auto raw_image = cv::imread(argv[2], 0);
    cv::Mat undistorted_image;
    cv::remap(raw_image, undistorted_image, map1, map2, cv::INTER_LINEAR,
              cv::BORDER_CONSTANT, cv::Scalar());

    cv::imshow("raw", raw_image);
    cv::imshow("undistorted", undistorted_image);

    while (1) {
        const auto key = cv::waitKey(0);
        if (key == 27 || key == 'q') { break; }
    }

    return 0;
}