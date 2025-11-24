#include <stdio.h>
#include <numbers>
#include <format>
#include <opencv2/opencv.hpp>
#include <string>
#include <vector>
#include <iostream>

struct PointPair
{
    double src_x, src_y;
    double dst_x, dst_y;
};

cv::Mat computePerspectiveMatrix()
{

    std::vector<PointPair> points = {
        {0, 0, 69, 107},
        {323, 0, 227, 76},
        {323, 215, 228, 122},
        {0, 215, 66, 134}};

    cv::Mat A = cv::Mat::zeros(8, 8, CV_64F);
    cv::Mat B = cv::Mat::zeros(8, 1, CV_64F);

    for (int i = 0; i < 4; ++i)
    {
        double tx = points[i].src_x;
        double ty = points[i].src_y;
        double x = points[i].dst_x;
        double y = points[i].dst_y;

        int row = i * 2;

        A.at<double>(row, 0) = y;
        A.at<double>(row, 1) = 1;
        A.at<double>(row, 2) = 0;
        A.at<double>(row, 3) = 0;
        A.at<double>(row, 4) = 0;
        A.at<double>(row, 5) = -tx * x;
        A.at<double>(row, 6) = -tx * y;
        A.at<double>(row, 7) = -tx;

        B.at<double>(row, 0) = -x;

        A.at<double>(row + 1, 0) = 0;
        A.at<double>(row + 1, 1) = 0;
        A.at<double>(row + 1, 2) = x;
        A.at<double>(row + 1, 3) = y;
        A.at<double>(row + 1, 4) = 1;
        A.at<double>(row + 1, 5) = -ty * x;
        A.at<double>(row + 1, 6) = -ty * y;
        A.at<double>(row + 1, 7) = -ty;

        B.at<double>(row + 1, 0) = 0;
    }

    cv::Mat X;
    cv::solve(A, B, X, cv::DECOMP_SVD);

    cv::Mat P = cv::Mat::zeros(3, 3, CV_64F);
    P.at<double>(0, 0) = 1.0;

    P.at<double>(0, 1) = X.at<double>(0);
    P.at<double>(0, 2) = X.at<double>(1);
    P.at<double>(1, 0) = X.at<double>(2);
    P.at<double>(1, 1) = X.at<double>(3);
    P.at<double>(1, 2) = X.at<double>(4);
    P.at<double>(2, 0) = X.at<double>(5);
    P.at<double>(2, 1) = X.at<double>(6);
    P.at<double>(2, 2) = X.at<double>(7);

    return P;
}

cv::Mat warpPerspective(cv::Mat &img, const cv::Mat &flag, const cv::Mat &P)
{
    cv::Mat out = img.clone();

    int flagWidth = flag.cols;
    int flagHeight = flag.rows;
    int imgWidth = img.cols;
    int imgHeight = img.rows;

    double p11 = P.at<double>(0, 0);
    double p12 = P.at<double>(0, 1);
    double p13 = P.at<double>(0, 2);
    double p21 = P.at<double>(1, 0);
    double p22 = P.at<double>(1, 1);
    double p23 = P.at<double>(1, 2);
    double p31 = P.at<double>(2, 0);
    double p32 = P.at<double>(2, 1);
    double p33 = P.at<double>(2, 2);

    for (int y = 0; y < imgHeight; y++)
    {
        for (int x = 0; x < imgWidth; x++)
        {

            double w = p31 * x + p32 * y + p33;

            if (std::abs(w) < 1e-9)
                continue;

            double u = (p11 * x + p12 * y + p13) / w;
            double v = (p21 * x + p22 * y + p23) / w;

            if (u >= 0 && u < flagWidth && v >= 0 && v < flagHeight)
            {

                int srcX = static_cast<int>(u);
                int srcY = static_cast<int>(v);

                out.at<cv::Vec3b>(y, x) = flag.at<cv::Vec3b>(srcY, srcX);
            }
        }
    }

    return out;
}

int main()
{
    cv::Mat img = cv::imread("../images/vsb.jpg", cv::IMREAD_COLOR);

    if (img.empty())
    {
        printf("Unable to read input file (%s, %d).", __FILE__, __LINE__);
    }

    cv::imshow("VSB", img);

    cv::Mat flag = cv::imread("../images/flag.png", cv::IMREAD_COLOR);

    if (flag.empty())
    {
        printf("Unable to read input file (%s, %d).", __FILE__, __LINE__);
    }

    cv::imshow("Flag", flag);

    auto mat = computePerspectiveMatrix();
    auto res = warpPerspective(img, flag, mat);

    cv::imshow("Result", res);

    int key;
    while (true)
    {
        key = cv::waitKey(0);
        if (key == 27 || key == '\r')
            break;
    }

    return 0;
}
