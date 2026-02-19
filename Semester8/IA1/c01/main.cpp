#include <stdio.h>

#include <opencv2/opencv.hpp>

void treshold(const cv::Mat &src, cv::Mat &dst, int threshold)
{
    dst = src.clone();
    for (int i = 0; i < src.rows; i++)
    {
        for (int j = 0; j < src.cols; j++)
        {
            if (src.at<uchar>(i, j) > threshold)
                dst.at<uchar>(i, j) = 255;
            else
                dst.at<uchar>(i, j) = 0;
        }
    }
}

void flood_fill(cv::Mat &src, int x, int y, uchar new_value)
{
    if (x < 0 || x >= src.cols || y < 0 || y >= src.rows)
        return; // Out of bounds

    uchar old_value = src.at<uchar>(y, x);

    if (old_value == 0)
        return; // Background, do not fill
    if (old_value == new_value)
        return; // Already filled

    src.at<uchar>(y, x) = new_value;

    flood_fill(src, x + 1, y, new_value);     // Right
    flood_fill(src, x - 1, y, new_value);     // Left
    flood_fill(src, x, y + 1, new_value);     // Down
    flood_fill(src, x, y - 1, new_value);     // Up
    flood_fill(src, x + 1, y + 1, new_value); // Down-Right
    flood_fill(src, x - 1, y + 1, new_value); // Down-Left
    flood_fill(src, x + 1, y - 1, new_value); // Up-Right
    flood_fill(src, x - 1, y - 1, new_value); // Up-Left
}

void begin_flood_fill(cv::Mat &src)
{
    uchar idx = 1;

    printf("rows: %d, cols: %d\n", src.rows, src.cols);

    for (int i = 0; i < src.rows; i++)
    {
        for (int j = 0; j < src.cols; j++)
        {
            if (src.at<uchar>(i, j) == 255)
            {
                flood_fill(src, j, i, idx);
                ++idx;
            }
        }
    }
}

cv::Mat index_to_color(const cv::Mat &src)
{
    cv::Mat color_image(src.size(), CV_8UC3);
    for (int i = 0; i < src.rows; i++)
    {
        for (int j = 0; j < src.cols; j++)
        {
            uchar idx = src.at<uchar>(i, j);
            if (idx == 0)
            {
                color_image.at<cv::Vec3b>(i, j) = cv::Vec3b(0, 0, 0); // Background
            }
            else
            {
                // Assign a random color based on the index
                color_image.at<cv::Vec3b>(i, j) = cv::Vec3b((idx * 37) % 256, (idx * 59) % 256, (idx * 83) % 256);
            }
        }
    }
    return color_image;
}

int main()
{
    cv::Mat image = cv::imread("../images/train.png", cv::IMREAD_GRAYSCALE); // load color image from file system to Mat variable, this will be loaded using 8 bits (uchar)

    if (image.empty())
    {
        printf("Unable to read input file (%s, %d).", __FILE__, __LINE__);
    }

    cv::Mat output;
    treshold(image, output, 128);

    cv::imshow("Output", output);

    begin_flood_fill(output);

    cv::imshow("Flood Fill", output);

    cv::Mat color_image = index_to_color(output);

    cv::imshow("Color Image", color_image);

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
