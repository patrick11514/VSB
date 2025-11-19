#include <opencv2/opencv.hpp>

struct RLDUserData
{
    cv::Mat &src_8uc3_img;
    cv::Mat &undistorted_8uc3_img;
    int K1;
    int K2;
    int bili = 0;

    RLDUserData(const int K1, const int K2, cv::Mat &src_8uc3_img, cv::Mat &undistorted_8uc3_img) : K1(K1), K2(K2), src_8uc3_img(src_8uc3_img), undistorted_8uc3_img(undistorted_8uc3_img)
    {
    }
};

void geom_dist(cv::Mat &src_8uc3_img, cv::Mat &dst_8uc3_img, bool bili, double K1 = 1.0, double K2 = 1.0)
{
    double c_u = static_cast<double>(dst_8uc3_img.size().width) / 2;
    double c_v = static_cast<double>(dst_8uc3_img.size().height) / 2;
    double R = std::sqrt(std::pow(c_u, 2) + std::pow(c_v, 2));

#pragma omp parallel for
    for (int x_n = 0; x_n < dst_8uc3_img.size().width; ++x_n)
    {
#pragma omp parallel for
        for (int y_n = 0; y_n < dst_8uc3_img.size().height; ++y_n)
        {
            double x = (x_n - c_u) / R;
            double y = (y_n - c_v) / R;

            double r2 = std::pow(x, 2) + std::pow(y, 2);
            double scaling = 1.f / (1 + (K1)*r2 + (K2)*std::pow(r2, 2));

            double x_dd = (x_n - c_u) * scaling + c_u;
            int x_d = static_cast<int>(std::floor(x_dd));
            double y_dd = (y_n - c_v) * scaling + c_v;
            int y_d = static_cast<int>(std::floor(y_dd));

            if (x_d < 0 || x_d >= src_8uc3_img.cols ||
                y_d < 0 || y_d >= src_8uc3_img.rows)
            {
                dst_8uc3_img.at<cv::Vec3b>(y_n, x_n) = cv::Vec3b(0, 0, 0);
                continue;
            }

            if (!bili)
            {

                dst_8uc3_img.at<cv::Vec3b>(y_n, x_n) = src_8uc3_img.at<cv::Vec3b>(y_d, x_d);
                continue;
            }

            // BILI
            cv::Vec3b pixels[4]{};
            for (int y = 0; y < 2; ++y)
            {
                for (int x = 0; x < 2; ++x)
                {
                    if (x_d + x >= src_8uc3_img.cols ||
                        y_d + y >= src_8uc3_img.rows)
                    {
                        pixels[x + y * 2] = cv::Vec3b(0, 0, 0);
                        continue;
                    }

                    pixels[x + y * 2] = src_8uc3_img.at<cv::Vec3b>(y_d + y, x_d + x);
                }
            }

            cv::Vec3b colors{};

            for (int c = 0; c < 3; ++c)
            {
                double c11 = static_cast<double>(pixels[0][c]);
                double c21 = static_cast<double>(pixels[1][c]);
                double c12 = static_cast<double>(pixels[2][c]);
                double c22 = static_cast<double>(pixels[3][c]);

                double dx = x_dd - static_cast<double>(x_d);
                double dy = y_dd - static_cast<double>(y_d);

                double final = c11 * (1 - dx) * (1 - dy) +
                               c21 * dx * (1 - dy) +
                               c12 * (1 - dx) * dy +
                               c22 * dx * dy;

                colors[c] = static_cast<uchar>(std::clamp(final, 0.0, 255.0));
            }

            dst_8uc3_img.at<cv::Vec3b>(y_n, x_n) = colors;
        }
    }
}

void apply_rld(int id, void *user_data)
{
    RLDUserData *rld_user_data = (RLDUserData *)user_data;

    geom_dist(rld_user_data->src_8uc3_img, rld_user_data->undistorted_8uc3_img, static_cast<bool>(rld_user_data->bili), rld_user_data->K1 / 100.0, rld_user_data->K2 / 100.0);
    cv::imshow("Geom Dist", rld_user_data->undistorted_8uc3_img);
}

int ex_rld()
{
    cv::Mat src_8uc3_img, geom_8uc3_img;
    RLDUserData rld_user_data(3.0, 1.0, src_8uc3_img, geom_8uc3_img);

    src_8uc3_img = cv::imread("../images/distorted_window.jpg", cv::IMREAD_COLOR);
    if (src_8uc3_img.empty())
    {
        printf("Unable to load image!\n");
        exit(-1);
    }

    cv::namedWindow("Original Image");
    cv::imshow("Original Image", src_8uc3_img);

    geom_8uc3_img = src_8uc3_img.clone();
    apply_rld(0, (void *)&rld_user_data);

    cv::namedWindow("Geom Dist");
    cv::imshow("Geom Dist", geom_8uc3_img);

    cv::createTrackbar("K1", "Geom Dist", &rld_user_data.K1, 100, apply_rld, &rld_user_data);
    cv::createTrackbar("K2", "Geom Dist", &rld_user_data.K2, 100, apply_rld, &rld_user_data);
    // checkbox
    cv::createTrackbar("Bilinear Interp", "Geom Dist", &rld_user_data.bili, 1, apply_rld, &rld_user_data);

    // Bacause of hyprland, I need to filter keys :)
    int key;
    while (true)
    {
        key = cv::waitKey(0);
        if (key == 27 || key == '\r')
            break; // ESC or Enter
    }

    return 0;
}

int main()
{
    return ex_rld();
}