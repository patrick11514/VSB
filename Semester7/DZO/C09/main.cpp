#include <iostream>
#include <vector>
#include <tuple>
#include <opencv2/opencv.hpp>

void follow_edges(int x, int y, cv::Mat &nms_img, cv::Mat &output_img, int t_low)
{
    output_img.at<uchar>(y, x) = 255;

    for (int i = -1; i <= 1; i++)
    {
        for (int j = -1; j <= 1; j++)
        {
            if (i == 0 && j == 0)
                continue;

            int nx = x + j;
            int ny = y + i;

            if (nx >= 0 && nx < nms_img.cols && ny >= 0 && ny < nms_img.rows)
            {
                uchar mag = nms_img.at<uchar>(ny, nx);
                if (mag > t_low && output_img.at<uchar>(ny, nx) == 0)
                {
                    follow_edges(nx, ny, nms_img, output_img, t_low);
                }
            }
        }
    }
}

struct UserData
{
    cv::Mat nms;
    int t1;
    int t2;
};

void solve(int, void *d)
{
    UserData *data = static_cast<UserData *>(d);

    cv::Mat &nms = data->nms;

    int t_low = std::min(data->t1, data->t2);
    int t_high = std::max(data->t1, data->t2);

    cv::Mat final_edges = cv::Mat::zeros(nms.size(), CV_8U);

    for (int y = 0; y < nms.rows; y++)
    {
        for (int x = 0; x < nms.cols; x++)
        {
            uchar magnitude = nms.at<uchar>(y, x);

            if (magnitude >= t_high && final_edges.at<uchar>(y, x) == 0)
            {
                follow_edges(x, y, nms, final_edges, t_low);
            }
        }
    }

    cv::imshow("Double threshold", final_edges);
}

std::tuple<cv::Mat, cv::Mat, cv::Mat> sobel(cv::Mat &orig)
{
    cv::Mat gray, grad_x, grad_y, magnitude;

    cv::cvtColor(orig, gray, cv::COLOR_BGR2GRAY);

    cv::Sobel(gray, grad_x, CV_32F, 1, 0, 3);
    cv::Sobel(gray, grad_y, CV_32F, 0, 1, 3);

    cv::magnitude(grad_x, grad_y, magnitude);
    cv::normalize(magnitude, magnitude, 0, 255, cv::NORM_MINMAX);

    return {magnitude, grad_x, grad_y};
}

cv::Mat nms(cv::Mat &mag, cv::Mat &gx, cv::Mat &gy)
{
    cv::Mat nms_img = cv::Mat::zeros(mag.size(), CV_8U);
    int rows = mag.rows;
    int cols = mag.cols;

    for (int y = 1; y < rows - 1; y++)
    {
        for (int x = 1; x < cols - 1; x++)
        {
            float gX = gx.at<float>(y, x);
            float gY = gy.at<float>(y, x);
            float magCurr = mag.at<float>(y, x);

            if (magCurr == 0)
                continue;

            float magPos = 0.0f, magNeg = 0.0f;
            float absX = std::abs(gX);
            float absY = std::abs(gY);

            if (absX >= absY)
            {
                float alpha = absY / absX;
                if (gX * gY > 0)
                {
                    magPos = (1 - alpha) * mag.at<float>(y, x + 1) + alpha * mag.at<float>(y + 1, x + 1);
                    magNeg = (1 - alpha) * mag.at<float>(y, x - 1) + alpha * mag.at<float>(y - 1, x - 1);
                }
                else
                {
                    magPos = (1 - alpha) * mag.at<float>(y, x + 1) + alpha * mag.at<float>(y - 1, x + 1);
                    magNeg = (1 - alpha) * mag.at<float>(y, x - 1) + alpha * mag.at<float>(y + 1, x - 1);
                }
            }
            else
            {
                float alpha = absX / absY;
                if (gX * gY > 0)
                {
                    magPos = (1 - alpha) * mag.at<float>(y + 1, x) + alpha * mag.at<float>(y + 1, x + 1);
                    magNeg = (1 - alpha) * mag.at<float>(y - 1, x) + alpha * mag.at<float>(y - 1, x - 1);
                }
                else
                {
                    magPos = (1 - alpha) * mag.at<float>(y + 1, x) + alpha * mag.at<float>(y + 1, x - 1);
                    magNeg = (1 - alpha) * mag.at<float>(y - 1, x) + alpha * mag.at<float>(y - 1, x + 1);
                }
            }

            if (magCurr >= magPos && magCurr >= magNeg)
                nms_img.at<uchar>(y, x) = (uchar)magCurr;
        }
    }
    return nms_img;
}

int main()
{
    cv::Mat img = cv::imread("../images/valve.png", cv::IMREAD_COLOR);
    if (img.empty())
    {
        printf("Error reading file.\n");
        return -1;
    }

    auto [sobel_img, gx, gy] = sobel(img);

    cv::Mat sobel_vis;
    sobel_img.convertTo(sobel_vis, CV_8U);
    cv::imshow("Sobel", sobel_vis);

    UserData data;
    data.nms = nms(sobel_img, gx, gy);
    data.t1 = 0;
    data.t2 = 100;

    cv::namedWindow("Double threshold");
    cv::createTrackbar("Low", "Double threshold", &data.t1, 255, solve, &data);
    cv::createTrackbar("High", "Double threshold", &data.t2, 255, solve, &data);

    solve(0, &data);

    while (true)
    {
        int key = cv::waitKey(0);
        if (key == 27 || key == '\r')
            break;
    }

    return 0;
}