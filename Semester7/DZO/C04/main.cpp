#include <stdio.h>
#include <numbers>
#include <format>

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
            output.at<double>(row, col) = std::atan2(values[IMAG], values[REAL]);
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

template <typename T>
cv::Mat flip_quadrants(cv::Mat realImag)
{
    cv::Mat output{realImag.size(), realImag.type()};

    for (int row = 0; row < realImag.rows; ++row)
    {
        for (int col = 0; col < realImag.cols; ++col)
        {
            auto value = realImag.at<T>(row, col);

            int newRow = (row + realImag.rows / 2) % realImag.rows;
            int newCol = (col + realImag.cols / 2) % realImag.cols;

            output.at<T>(newRow, newCol) = value;
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

cv::Mat generate_filter(cv::Size size, int radius, bool inverse = false)
{
    cv::Mat mask(size, CV_64F, 0.f);

    int centerX = size.width / 2;
    int centerY = size.height / 2;

    int white = 1.f;
    int black = 0.f;

    if (inverse)
    {
        std::swap(white, black);
    }

    for (int row = 0; row < size.height; ++row)
    {
        for (int col = 0; col < size.width; ++col)
        {
            int dx = std::abs(col - centerX);
            int dy = std::abs(row - centerY);

            mask.at<double>(row, col) = (dx * dx + dy * dy <= radius * radius) ? white : black;
        }
    }

    return mask;
}

cv::Mat generate_row_mask(cv::Size size, int percentageFromSides)
{
    cv::Mat mask(size, CV_64F, 0.f);

    int treshold = size.width * (percentageFromSides / 100);

    int centerY = size.height / 2;

    for (int col = 0; col < size.width; ++col)
    {
        if (col < treshold || (size))
    }

    return mask;
}

cv::Mat apply_filter(cv::Mat realImag, cv::Mat mask)
{
    cv::Mat output = realImag.clone();

    for (int row = 0; row < realImag.size().height; ++row)
    {
        for (int col = 0; col < realImag.size().width; ++col)
        {
            auto vec = output.at<cv::Vec2d>(row, col);
            if (mask.at<double>(row, col) < 1.f)
            {
                vec[REAL] = 0.f;
                vec[IMAG] = 0.f;
            }
            output.at<cv::Vec2d>(row, col) = vec;
        }
    }

    return output;
}

int main()
{
    cv::Mat img = cv::imread("../images/lena64_noise.png", cv::IMREAD_GRAYSCALE);

    if (img.empty())
    {
        printf("Unable to read input file (%s, %d).", __FILE__, __LINE__);
    }
    cv::imshow("LENA Noise", img);

    cv::Mat gray_scale_float;
    img.convertTo(gray_scale_float, CV_64F, 1.0 / 255.0);

    auto real_imag = furier(gray_scale_float);
    auto flipped = flip_quadrants<cv::Vec2d>(real_imag);

    auto power = power_spectrum(flipped);
    cv::imshow("power", power);

    for (int i = 10; i < 30; i += 5)
    {
        for (int l = 0; l <= 1; ++l)
        {
            auto mask = generate_filter(flipped.size(), i, l == 1);
            auto pass = apply_filter(flipped, mask);
            auto inverse = inverse_fourier(flip_quadrants<cv::Vec2d>(pass));

            // cv::imshow(std::format("MASK: {} - {}", i, l == 0 ? "high" : "low"), mask);
            cv::imshow(std::format("Lena {} - {}", i, l == 0 ? "high" : "low"), inverse);
        }
    }

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
