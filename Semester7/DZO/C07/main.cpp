#include <stdio.h>
#include <numbers>
#include <format>
#include <opencv2/opencv.hpp>
#include <string>

using Histogram = std::array<int, 256>;

auto get_histogram(cv::Mat &mat)
{
    Histogram histogram{};

    for (int x = 0; x < mat.size().width; ++x)
    {
        for (int y = 0; y < mat.size().height; ++y)
        {
            auto value = mat.at<uchar>(y, x);
            histogram[value] += 1;
        }
    }

    return histogram;
}

int cdf(Histogram &hist, int idx)
{
    int sum = 0;

    for (int i = 0; i < idx; ++i)
    {
        sum += hist[i];
    }

    return sum;
}

void render_hist(const Histogram &histogram, const std::string &name)
{
    int hist_w = 512, hist_h = 400;
    int bin_w = cvRound((double)hist_w / histogram.size());

    cv::Mat histImage(hist_h, hist_w, CV_8UC3, cv::Scalar(0, 0, 0));

    int max_val = *std::max_element(histogram.begin(), histogram.end());

    for (int i = 0; i < histogram.size(); i++)
    {
        int normalized_h = cvRound((histogram[i] / (double)max_val) * hist_h);

        cv::line(histImage,
                 cv::Point(i * bin_w, hist_h),
                 cv::Point(i * bin_w, hist_h - normalized_h),
                 cv::Scalar(255, 255, 255), // White color
                 2, 8, 0);
    }

    cv::imshow(name, histImage);
}

cv::Mat equalize_image(const cv::Mat &img, const Histogram &cumulated_histogram)
{
    cv::Mat equalized_img{img.size(), img.type()};

    auto min_cdf = 256;
    for (const auto &value : cumulated_histogram)
    {
        if (value != 0 && value < min_cdf)
        {
            min_cdf = value;
        }
    }

    for (int x = 0; x < img.size().width; ++x)
    {
        for (int y = 0; y < img.size().height; ++y)
        {
            auto old_value = img.at<uchar>(y, x);

            auto value = std::round(cumulated_histogram[old_value] - min_cdf) / (img.size().width * img.size().height - min_cdf) * 255.0;

            equalized_img.at<uchar>(y, x) = value;
        }
    }

    return equalized_img;
}

int main()
{
    cv::Mat img = cv::imread("../images/uneq.jpg", cv::IMREAD_GRAYSCALE);

    if (img.empty())
    {
        printf("Unable to read input file (%s, %d).", __FILE__, __LINE__);
    }
    cv::imshow("Uneq", img);

    auto histogram = get_histogram(img);
    render_hist(histogram, "Input Histogram");

    Histogram cumulated_histogram{};
    for (int i = 0; i < histogram.size(); ++i)
    {
        cumulated_histogram[i] = cdf(histogram, i);
    }

    render_hist(cumulated_histogram, "Input Cumulated Histogram");

    auto equalized_img = equalize_image(img, cumulated_histogram);
    cv::imshow("Equalized Image", equalized_img);

    auto equalized_histogram = get_histogram(equalized_img);
    render_hist(equalized_histogram, "Equalized Histogram");

    auto equalized_cumulated_histogram = Histogram{};
    for (int i = 0; i < equalized_histogram.size(); ++i)
    {
        equalized_cumulated_histogram[i] = cdf(equalized_histogram, i);
    }

    render_hist(equalized_cumulated_histogram, "Equalized Cumulated Histogram");

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
