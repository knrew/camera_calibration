#include <opencv2/opencv.hpp>

int main(const int argc, const char *const *const argv) {
    static const auto kChessSize = cv::Size(7, 10);

    if (argc < 2) {
        std::cout << "no arguments." << std::endl;
        return -1;
    }

    auto image = cv::imread(argv[1], cv::IMREAD_GRAYSCALE);

    if (!image.data) {
        std::cout << "no image data." << std::endl;
        return -1;
    }

    cv::Mat corners;
    const bool has_found_corners = cv::findChessboardCorners(image, kChessSize, corners);

    std::cout << (has_found_corners ? "found." : "not found.") << std::endl;

    cv::drawChessboardCorners(image, kChessSize, corners, has_found_corners);

    cv::imshow("corners", image);

    while (1) {
        const auto key = cv::waitKey(0);
        if (key == 27 || key == 'q') { break; }
    }

    cv::destroyAllWindows();

    return 0;
}