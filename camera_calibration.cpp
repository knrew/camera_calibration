#include <opencv2/opencv.hpp>
#include <fstream>

int main(const int argc, const char *const *const argv) {
    constexpr float kLengthOfSideOfSquare = 0.025f;
    constexpr int kBoardWidth = 7;
    constexpr int kBoardHeight = 10;
    const auto kBoardSize = cv::Size(kBoardWidth, kBoardHeight);

    if (argc < 3) {
        std::cout << "invalid arguments." << std::endl;
        return -1;
    }

    const auto camera_matrix_file = std::string(argv[2]);

    std::vector<std::string> chess_board_image_list;

    {
        auto ifs = std::ifstream(argv[1]);
        std::string line;
        while (getline(ifs, line)) {
            chess_board_image_list.emplace_back(line);
        }
    }

    const int num_chess_board = chess_board_image_list.size();

    std::vector<cv::Point3f> object_point(kBoardWidth * kBoardHeight);
    for (int w = 0; w < kBoardWidth; ++w) {
        for (int h = 0; h < kBoardHeight; ++h) {
            object_point[w + h * kBoardWidth]
                    = cv::Point3f(static_cast<float>(w) * kLengthOfSideOfSquare,
                                  static_cast<float>(h) * kLengthOfSideOfSquare, 0.f);
        }
    }

    std::vector<std::vector<cv::Point3f>> object_points(num_chess_board, object_point);
    std::vector<std::vector<cv::Point2f>> image_points;

    cv::Mat image, image_with_corners;

    cv::Mat corners;
    for (const auto &image_name : chess_board_image_list) {
        std::cout << image_name << std::endl;

        image = cv::imread(image_name, cv::IMREAD_GRAYSCALE);

        if (!image.data) {
            std::cout << "no image data." << std::endl;
            return -1;
        }

        const bool has_found_corners = cv::findChessboardCorners(image, kBoardSize, corners);

        std::cout << (has_found_corners ? "found." : "not found.") << std::endl;

//        image.copyTo(image_with_corners);
//        cv::drawChessboardCorners(image_with_corners, kBoardSize, corners, has_found_corners);

        if (has_found_corners) {
//            cv::imshow("image", image_with_corners);
            image_points.emplace_back(corners);
        }
    }

    cv::Mat camera_matrix, distortion_coefficients;
    cv::calibrateCamera(object_points, image_points, kBoardSize,
                        camera_matrix, distortion_coefficients,
                        cv::noArray(), cv::noArray());

    std::cout << "save camera_matrix..." << std::endl;

    auto fs = cv::FileStorage(camera_matrix_file, cv::FileStorage::WRITE);
    fs << "image_width" << image.size().width
       << "image_height" << image.size().height
       << "camera_matrix" << camera_matrix
       << "distortion_coefficients" << distortion_coefficients;
    fs.release();

//    while (1) {
//        const auto key = cv::waitKey(0);
//        if (key == 27 || key == 'q') { break; }
//    }

    cv::destroyAllWindows();

    std::cout << "finish." << std::endl;

    return 0;
}