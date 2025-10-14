#include <stdio.h>
#include <numbers>

#include <opencv2/opencv.hpp>

#define REAL 0
#define IMAG 1

#include <string>

cv::Mat furier(cv::Mat input)
{
    cv::Mat normalized;
    input.convertTo(normalized, CV_64F, 1.f / (std::sqrt(input.rows * input.cols)));
    // REAL | IMAG
    cv::Mat output(normalized.size(), CV_64FC2, cv::Scalar(0, 0));

    auto base = [&](int k, int l, int m, int n)
    {
        // REAL | IMAG
        cv::Vec2d res;

        auto bracket = (static_cast<double>(m * k) / input.rows) + (static_cast<double>(n * l) / input.cols);
        auto x = 2 * std::numbers::pi * bracket;

        res[REAL] = std::cos(x);
        res[IMAG] = -std::sin(x);

        return res;
    };

    for (int k = 0; k < normalized.rows; ++k)
    {
        for (int l = 0; l < normalized.cols; ++l)
        {
            cv::Vec2d sum{0.f, 0.f};

            for (int row = 0; row < normalized.rows; ++row)
            {
                for (int col = 0; col < normalized.cols; ++col)
                {
                    auto pixel = normalized.at<double>(row, col);
                    auto _base = base(k, l, row, col);
                    sum[REAL] += _base[REAL] * pixel;
                    sum[IMAG] += _base[IMAG] * pixel;
                }
            }

            output.at<cv::Vec2d>(k, l) = sum;
        }
    }

    return output;
}

cv::Mat phase_shift(cv::Mat realImag)
{
    cv::Mat output{realImag.size(), CV_64F};

    for (int row = 0; row < realImag.rows; ++row)
    {
        for (int col = 0; col < realImag.cols; ++col)
        {
            auto values = realImag.at<cv::Vec2d>(row, col);
            output.at<double>(row, col) = std::atan2(values[REAL], values[IMAG]);
        }
    }

    return output;
}

double amplitude(cv::Vec2d &realImag)
{
    auto r = realImag[REAL];
    auto i = realImag[IMAG];

    return std::sqrt((r * r) + (i * i));
}

cv::Mat power_spectrum(cv::Mat realImag)
{
    cv::Mat output{realImag.size(), CV_64F};

    for (int row = 0; row < realImag.rows; ++row)
    {
        for (int col = 0; col < realImag.cols; ++col)
        {
            auto vec = realImag.at<cv::Vec2d>(row, col);
            auto ampl = amplitude(vec);
            output.at<double>(row, col) = ampl * ampl;
        }
    }

    return output;
}

cv::Mat spectrum_normalize(cv::Mat spectrum)
{
    cv::Mat normalized;
    cv::log(spectrum, normalized);
    cv::normalize(normalized, normalized, 0, 1, cv::NORM_MINMAX);

    return normalized;
}

cv::Mat flip_quadrants(cv::Mat realImag)
{
    cv::Mat output{realImag.size(), CV_64F};

    for (int row = 0; row < realImag.rows; ++row)
    {
        for (int col = 0; col < realImag.cols; ++col)
        {
            auto value = realImag.at<double>(row, col);

            int newRow = (row + realImag.rows / 2) % realImag.rows;
            int newCol = (col + realImag.cols / 2) % realImag.cols;

            output.at<double>(newRow, newCol) = value;
        }
    }

    return output;
}

cv::Mat inverse_fourier(cv::Mat realImag)
{
    cv::Mat normalized(realImag.size(), CV_64F, 0.f);

    auto base = [&](int k, int l, int m, int n)
    {
        // REAL | IMAG
        cv::Vec2d res;

        auto bracket = (static_cast<double>(m * k) / realImag.rows) + (static_cast<double>(n * l) / realImag.cols);
        auto x = 2 * std::numbers::pi * bracket;

        res[REAL] = std::cos(x);
        res[IMAG] = std::sin(x);

        return res;
    };

    for (int k = 0; k < realImag.rows; ++k)
    {
        for (int l = 0; l < realImag.cols; ++l)
        {
            double sum = 0.f;

            for (int row = 0; row < realImag.rows; ++row)
            {
                for (int col = 0; col < realImag.cols; ++col)
                {
                    auto vec = realImag.at<cv::Vec2d>(row, col);
                    auto _base = base(k, l, row, col);

                    sum += vec[REAL] * _base[REAL] - vec[IMAG] * _base[IMAG];
                }
            }

            normalized.at<double>(k, l) = sum;
        }
    }

    cv::Mat output;
    normalized.convertTo(output, CV_64F, 1.f / (std::sqrt(normalized.rows * normalized.cols)));

    return output;
}

int main()
{
    cv::Mat img = cv::imread("../images/lena64.png", cv::IMREAD_GRAYSCALE);

    if (img.empty())
    {
        printf("Unable to read input file (%s, %d).", __FILE__, __LINE__);
    }
    cv::imshow("LENA", img);

    cv::Mat gray_scale_float;
    img.convertTo(gray_scale_float, CV_64F, 1.0 / 255.0);

    auto real_imag = furier(gray_scale_float);
    auto phase = phase_shift(real_imag);
    auto spectrum = power_spectrum(real_imag);
    auto norm_spectrum = flip_quadrants(spectrum_normalize(spectrum));

    cv::imshow("Lena Phase", phase);
    cv::imshow("Lena Spectrum", spectrum);
    cv::imshow("Lena norm Spectrum", norm_spectrum);

    auto inverse = inverse_fourier(real_imag);
    cv::imshow("Lena inverse", inverse);

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
