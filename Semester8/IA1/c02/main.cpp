#include <stdio.h>
#include <cmath>
#include <vector>

#include <opencv2/opencv.hpp>

struct ObjectStats {
    int area = 0;
    long long sum_x = 0;
    long long sum_y = 0;
    int circ = 0;
    double xt = 0.0;
    double yt = 0.0;
    double mu20 = 0.0;
    double mu02 = 0.0;
    double mu11 = 0.0;
};

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

int begin_flood_fill(cv::Mat &src)
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
                if (idx < 255) {
                    ++idx;
                }
            }
        }
    }
    return idx;
}

void compute_features(const cv::Mat& labeled, int num_objects, cv::Mat& color_image) {
    std::vector<ObjectStats> stats(num_objects);
    
    for (int y = 0; y < labeled.rows; y++) {
        for (int x = 0; x < labeled.cols; x++) {
            uchar val = labeled.at<uchar>(y, x);
            if (val > 0 && val < num_objects) {
                stats[val].area++;
                stats[val].sum_x += x;
                stats[val].sum_y += y;
                
                // Check circumference
                bool is_boundary = false;
                if (x == 0 || labeled.at<uchar>(y, x-1) != val) is_boundary = true;
                else if (x == labeled.cols - 1 || labeled.at<uchar>(y, x+1) != val) is_boundary = true;
                else if (y == 0 || labeled.at<uchar>(y-1, x) != val) is_boundary = true;
                else if (y == labeled.rows - 1 || labeled.at<uchar>(y+1, x) != val) is_boundary = true;
                
                if (is_boundary) {
                    stats[val].circ++;
                }
            }
        }
    }
    
    for (int i = 1; i < num_objects; i++) {
        if (stats[i].area > 0) {
            stats[i].xt = (double)stats[i].sum_x / stats[i].area;
            stats[i].yt = (double)stats[i].sum_y / stats[i].area;
        }
    }
    
    for (int y = 0; y < labeled.rows; y++) {
        for (int x = 0; x < labeled.cols; x++) {
            uchar val = labeled.at<uchar>(y, x);
            if (val > 0 && val < num_objects) {
                double dx = x - stats[val].xt;
                double dy = y - stats[val].yt;
                stats[val].mu20 += dx * dx;
                stats[val].mu02 += dy * dy;
                stats[val].mu11 += dx * dy;
            }
        }
    }
    
    // Compute F1, F2 and render text
    for (int i = 1; i < num_objects; i++) {
        if (stats[i].area > 0) {
            double mu20 = stats[i].mu20;
            double mu02 = stats[i].mu02;
            double mu11 = stats[i].mu11;
            
            double tmp1 = mu20 + mu02;
            double tmp2 = std::sqrt(4 * mu11 * mu11 + (mu20 - mu02) * (mu20 - mu02));
            double mu_max = 0.5 * (tmp1 + tmp2);
            double mu_min = 0.5 * (tmp1 - tmp2);
            
            double F1 = (double)(stats[i].circ * stats[i].circ) / (100.0 * stats[i].area);
            double F2 = (mu_max > 0.0) ? (mu_min / mu_max) : 0.0;
            
            printf("Object %d: Area=%d, Center=(%.1f, %.1f), Circ=%d, F1=%.4f, F2=%.4f\n",
                   i, stats[i].area, stats[i].xt, stats[i].yt, stats[i].circ, F1, F2);
                   
            char text1[64];
            char text2[64];
            snprintf(text1, sizeof(text1), "%d", stats[i].area);
            snprintf(text2, sizeof(text2), "%d", stats[i].circ);
            
            int fontFace = cv::FONT_HERSHEY_SIMPLEX;
            double fontScale = 0.5;
            int thickness = 1;
            
            // Draw text with black outline for better visibility
            cv::Point textOrg1(stats[i].xt - 15, stats[i].yt - 5);
            cv::putText(color_image, text1, textOrg1, fontFace, fontScale, cv::Scalar(0, 0, 0), thickness + 2);
            cv::putText(color_image, text1, textOrg1, fontFace, fontScale, cv::Scalar(255, 255, 255), thickness);
            
            cv::Point textOrg2(stats[i].xt - 15, stats[i].yt + 15);
            cv::putText(color_image, text2, textOrg2, fontFace, fontScale, cv::Scalar(0, 0, 0), thickness + 2);
            cv::putText(color_image, text2, textOrg2, fontFace, fontScale, cv::Scalar(255, 255, 255), thickness);
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

    int num_objects = begin_flood_fill(output);

    cv::imshow("Flood Fill", output);

    cv::Mat color_image = index_to_color(output);
    
    compute_features(output, num_objects, color_image);

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
