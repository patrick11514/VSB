#include <stdio.h>

#include <opencv2/opencv.hpp>

void anisotropic_filter(cv::Mat &img)
{
}

int main()
{
    cv::Mat img = cv::imread("../images/lena.png", cv::IMREAD_COLOR); // load color image from file system to Mat variable, this will be loaded using 8 bits (uchar)

    if (img.empty())
    {
        printf("Unable to read input file (%s, %d).", __FILE__, __LINE__);
    }
    cv::imshow("LENA", img);

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
