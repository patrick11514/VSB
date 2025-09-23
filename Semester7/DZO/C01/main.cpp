#include <stdio.h>

#include <opencv2/opencv.hpp>

template <typename MatType, int Width, int Height>
cv::Mat applyConvolution(cv::Mat &image, const cv::Matx<float, Width, Height> &matrix, float maskFrac)
{
    cv::Mat output(image.size(), image.type());

    for (int row = 0; row < image.size().height; ++row)
    {
        for (int col = 0; col < image.size().width; ++col)
        {
            cv::Vec3f sum{0, 0, 0};

            // Loop matrix + points around
            int startRow = std::max(row - (Height / 2), 0);
            int endRow = std::min(row + (Height / 2), image.size().height - 1);
            int rowPixels = endRow - startRow;

            int startCol = std::max(col - (Width / 2), 0);
            int endCol = std::min(col + (Width / 2), image.size().width - 1);
            int colPixels = endCol - startCol;

            for (int _row = startRow; _row <= endRow; ++_row)
            {
                for (int _col = startCol; _col <= endCol; ++_col)
                {
                    auto pixel = image.at<MatType>(_col, _row);
                    int matValue = matrix(endRow - _row, endCol - _col);
                    sum[0] += pixel[0] * matValue;
                    sum[1] += pixel[1] * matValue;
                    sum[2] += pixel[2] * matValue;
                }
            }

            output.at<MatType>(col, row) = MatType{
                cv::saturate_cast<uchar>(sum[0] * maskFrac),
                cv::saturate_cast<uchar>(sum[1] * maskFrac),
                cv::saturate_cast<uchar>(sum[2] * maskFrac)};
        }
    }

    return output;
}

int main()
{
    cv::Mat img = cv::imread("../images/lena.png", cv::IMREAD_COLOR); // load color image from file system to Mat variable, this will be loaded using 8 bits (uchar)

    if (img.empty())
    {
        printf("Unable to read input file (%s, %d).", __FILE__, __LINE__);
    }
    cv::imshow("LENA", img);

    auto boxBlur = applyConvolution<cv::Vec3b>(img, cv::Matx<float, 3, 3>::ones(), 1.f / 9.f);
    cv::imshow("LENA box blur", boxBlur);

    auto gausseanBlur = applyConvolution<cv::Vec3b>(img, cv::Matx<float, 3, 3>{1, 2, 1, 2, 3, 2, 1, 2, 1}, 1.f / 16.f);
    cv::imshow("LENA gausean blur", gausseanBlur);

    float matrix[25] = {
        1, 4, 6, 4, 1,
        4, 16, 24, 16, 4,
        6, 24, 36, 24, 6,
        4, 16, 24, 16, 4,
        1, 4, 6, 4, 1};
    auto gausseanBlur5x5 = applyConvolution<cv::Vec3b>(img, cv::Matx<float, 5, 5>{matrix}, 1.f / 256.f);
    cv::imshow("LENA gausean blur 5x5", gausseanBlur5x5);

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
