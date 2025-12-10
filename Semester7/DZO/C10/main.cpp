#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>

using namespace cv;
using namespace std;

int main()
{

    int size = 400;
    Mat img = Mat::zeros(size, size, CV_8UC1);
    circle(img, Point(size / 2 - 50, size / 2), 80, Scalar(255), 1, 16);
    rectangle(img, Point(size / 2, size / 2), Point(size / 2 + 60, size / 2 + 150), Scalar(128), -1);

    Mat sinogram = Mat::zeros(180, size, CV_32F);
    Mat reconstruction = Mat::zeros(size, size, CV_32F);
    Point2f center(size / 2.0f, size / 2.0f);

    for (int angle = 0; angle < 180; angle++)
    {
        Mat rotated_img;

        Mat rot_mat = getRotationMatrix2D(center, angle, 1.0);
        warpAffine(img, rotated_img, rot_mat, img.size(), INTER_LINEAR, BORDER_CONSTANT, Scalar(0));

        Mat projection;
        reduce(rotated_img, projection, 0, 1, CV_32F);

        projection.copyTo(sinogram.row(angle));
    }

    for (int angle = 0; angle < 180; angle++)
    {

        Mat projection = sinogram.row(angle);

        Mat back_proj_layer;
        repeat(projection, size, 1, back_proj_layer);

        Mat rot_mat = getRotationMatrix2D(center, -angle, 1.0);
        Mat rotated_layer;
        warpAffine(back_proj_layer, rotated_layer, rot_mat, reconstruction.size(), INTER_LINEAR, BORDER_CONSTANT, Scalar(0));

        add(reconstruction, rotated_layer, reconstruction);
    }

    Mat sinogram_display, recon_display;
    normalize(sinogram, sinogram_display, 0, 255, NORM_MINMAX, CV_8U);
    normalize(reconstruction, recon_display, 0, 255, NORM_MINMAX, CV_8U);

    imshow("Input", img);
    imshow("Sinogram", sinogram_display);
    imshow("Backprojection", recon_display);

    while (true)
    {
        int key = cv::waitKey(0);
        if (key == 27 || key == '\r')
            break;
    }
    return 0;
}