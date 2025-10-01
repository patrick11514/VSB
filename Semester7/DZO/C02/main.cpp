#include <stdio.h>

#include <opencv2/opencv.hpp>

// https://upload.patrick115.eu/screenshot/31029888c6.png

cv::Mat anisotropic_filter(cv::Mat &img, int times)
{
    double sigma = 0.015f;
    double lambda = 0.1f;

    cv::Mat prev = img.clone();
    cv::Mat next = img.clone();

    auto g = [&](double d)
    { return std::exp(-(d * d) * (1.f / (sigma * sigma))); };

    for (int t = 0; t < times; ++t)
    {
        for (int row = 1; row < img.size().height - 1; ++row)
        {
            for (int col = 1; col < img.size().width - 1; ++col)
            {
                auto c = prev.at<double>(row, col);
                auto n = prev.at<double>(row - 1, col);
                auto s = prev.at<double>(row + 1, col);
                auto w = prev.at<double>(row, col - 1);
                auto e = prev.at<double>(row, col + 1);

                auto dn = n - c, ds = s - c, dw = w - c, de = e - c;
                auto gN = g(dn), gS = g(ds), gW = g(dw), gE = g(de);

                auto sumC = gN + gS + gW + gE;
                auto lnewI = c * (1.0f - (lambda * sumC));
                auto rnewI = lambda * (n * gN + s * gS + w * gW + e * gE);
                auto newI = lnewI + rnewI;
                next.at<double>(row, col) = newI;
            }
        }

        prev = next;
    }

    return prev;
}

int main()
{
    cv::Mat img = cv::imread("../images/lena.png", cv::IMREAD_COLOR); // load color image from file system to Mat variable, this will be loaded using 8 bits (uchar)

    if (img.empty())
    {
        printf("Unable to read input file (%s, %d).", __FILE__, __LINE__);
    }
    cv::imshow("LENA", img);

    cv::Mat gray_scale_u8;
    cv::cvtColor(img, gray_scale_u8, cv::COLOR_BGR2GRAY);

    cv::Mat gray_scale_float;
    gray_scale_u8.convertTo(gray_scale_float, CV_64FC1, 1.0 / 255.0);

    cv::imshow("LENA Gray", gray_scale_u8);

    auto new_image = anisotropic_filter(gray_scale_float, 1000);

    cv::imshow("Lena A", new_image);

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
